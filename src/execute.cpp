#include <execute.hpp>
#include <types.hpp>
#include <commands.hpp>
#include <utils.hpp>
#include <iostream>
#include <filesystem>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace shell {

static void exec_external(Command& cmd) {
    std::vector<char*> argv;
    argv.push_back(const_cast<char*>(cmd.program.c_str()));
    for (auto& arg : cmd.args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid == 0) {
        if (cmd.redirect != std::nullopt) {
            auto& rd = cmd.redirect.value();
            int flags;
            mode_t mode = 0644;
            switch (rd.op) {
                case RedirectOp::Replace:
                    flags = O_WRONLY | O_CREAT | O_TRUNC;
                    break;
            }
            int fd = open(rd.target.c_str(), flags, mode);
            if (fd < 0) {
                std::cerr << "Failed to open " << rd.target << '\n';
                exit(1);
            }

            if (dup2(fd, rd.fd) < 0) {
                std::cerr << "dup2 failed\n";
                exit(1);
            }
            close(fd);
        }
        execvp(argv[0], argv.data());
        std::cerr << "execvp failed\n";
        exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        std::cerr << "fork failed\n";
    }
}

void execute(Pipeline pipe) {
    for (auto& cmd : pipe) {
        auto it = Commands::cmds.find(cmd.program);

        if (it != Commands::cmds.end()) {
            if (cmd.redirect != std::nullopt) {
                auto& rd = cmd.redirect.value();

                int saved_fd = dup(rd.fd);
                if (saved_fd < 0) {
                    std::cerr << "Failed to save fd\n";
                    close(saved_fd);
                    continue;
                }

                int flags;
                mode_t mode = 0644;
                switch (rd.op) {
                    case RedirectOp::Replace:
                        flags = O_WRONLY | O_CREAT | O_TRUNC;
                        break;
                }

                int fd = open(rd.target.c_str(), flags, mode);
                if (fd < 0) {
                    std::cerr << "Failed to open " << rd.target << '\n';
                    close(fd);
                    close(saved_fd);
                    continue;
                }

                if (dup2(fd, rd.fd) < 0) {
                    std::cerr << "dup2 failed\n";
                    close(fd);
                    close(saved_fd);
                    continue;
                }
                close(fd);

                it->second(cmd);

                dup2(saved_fd, rd.fd);
                if (dup2(saved_fd, rd.fd) < 0) {
                    std::cerr << "Failed to restore fd\n";
                }
                close(saved_fd);
            } else {
                it->second(cmd);
            }
        } else if (search_path(cmd.program)) {
            exec_external(cmd);
        } else {
            std::cerr << cmd.program << ": command not found\n";
        }
    }
}

} // namespace shell
