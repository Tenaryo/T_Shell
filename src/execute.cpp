#include <execute.hpp>
#include <types.hpp>
#include <commands.hpp>
#include <utils.hpp>
#include <iostream>
#include <filesystem>
#include <sys/wait.h>
#include <sys/stat.h>

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
            switch (rd.fd) {
                case 1: {
                    CoutRedirect r(rd.target);
                    if (!r) {
                        std::cerr << "Failed to redirect\n";
                        exit(1);
                    }
                    execvp(argv[0], argv.data());
                    std::cerr << "execvp failed\n";
                    exit(1);
                }
                case 2: {
                    CerrRedirect r(rd.target);
                    if (!r) {
                        std::cerr << "Failed to redirect\n";
                        exit(1);
                    }
                    execvp(argv[0], argv.data());
                    std::cerr << "execvp failed\n";
                    exit(1);
                }
            }
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
                switch (rd.fd) {
                    case 1: {
                        CoutRedirect r(rd.target);
                        if (!r) {
                            std::cerr << "Failed to redirect\n";
                            continue;
                        }
                        it->second(cmd);
                        break;
                    }
                    case 2: {
                        CerrRedirect r(rd.target);
                        if (!r) {
                            std::cerr << "Failed to redirect\n";
                            continue;
                        }
                        it->second(cmd);
                        break;
                    }
                }
            } else {
                it->second(cmd);
            }
        } else if (search_path(cmd.program)) {
            exec_external(cmd);
        } else {
            if (cmd.redirect != std::nullopt) {
                auto& rd = cmd.redirect.value();
                if (rd.fd == 2) {
                    CerrRedirect r(rd.target);
                    if (!r) {
                        std::cerr << "Failed to redirect\n";
                        continue;
                    }
                    std::cerr << cmd.program << ": command not found\n";
                }
            } else {
                std::cerr << cmd.program << ": command not found\n";
            }
        }
    }
}

} // namespace shell
