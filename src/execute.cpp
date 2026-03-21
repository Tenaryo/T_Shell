#include <commands.hpp>
#include <executable_cache.hpp>
#include <execute.hpp>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <redirect.hpp>
#include <sys/stat.h>
#include <sys/wait.h>
#include <types.hpp>
#include <unistd.h>

namespace shell {

static std::vector<char*> build_argv(const Command& cmd) {
    std::vector<char*> argv;
    argv.push_back(const_cast<char*>(cmd.program.c_str()));
    for (auto& arg : cmd.args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);
    return argv;
}

static void apply_redirect(const Command& cmd) {
    if (!cmd.redirect)
        return;
    auto& rd = cmd.redirect.value();

    int flags = (rd.op == RedirectOp::Append) ? (O_WRONLY | O_CREAT | O_APPEND)
                                              : (O_WRONLY | O_CREAT | O_TRUNC);
    int fd = open(rd.target.c_str(), flags, 0644);
    if (fd < 0) {
        std::cerr << "Failed to redirect\n";
        exit(1);
    }
    dup2(fd, rd.fd);
    close(fd);
}

static void exec_single(const Command& cmd) {
    auto it = Commands::cmds.find(cmd.program);

    if (it != Commands::cmds.end()) {
        if (cmd.redirect) {
            auto& rd = cmd.redirect.value();
            switch (rd.fd) {
            case 1: {
                CoutRedirect r(rd.target, rd.op);
                if (!r) {
                    std::cerr << "Failed to redirect\n";
                    return;
                }
                it->second(cmd);
                break;
            }
            case 2: {
                CerrRedirect r(rd.target, rd.op);
                if (!r) {
                    std::cerr << "Failed to redirect\n";
                    return;
                }
                it->second(cmd);
                break;
            }
            }
        } else {
            it->second(cmd);
        }
    } else if (ExecutableCache::instance().contains(cmd.program)) {
        pid_t pid = fork();
        if (pid == 0) {
            apply_redirect(cmd);
            auto argv = build_argv(cmd);
            execvp(argv[0], argv.data());
            std::cerr << "execvp failed\n";
            exit(1);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        } else {
            std::cerr << "fork failed\n";
        }
    } else {
        if (cmd.redirect && cmd.redirect->fd == 2) {
            CerrRedirect r(cmd.redirect->target, cmd.redirect->op);
            std::cerr << cmd.program << ": command not found\n";
        } else {
            std::cerr << cmd.program << ": command not found\n";
        }
    }
}

static void exec_pipeline(const Pipeline& pipeline) {
    std::vector<pid_t> pids;
    int prev_read = -1;

    for (size_t i = 0; i < pipeline.size(); i++) {
        const auto& cmd = pipeline[i];
        bool is_last = (i == pipeline.size() - 1);

        int pipefd[2];
        if (!is_last) {
            if (pipe(pipefd) < 0) {
                std::cerr << "pipe failed\n";
                return;
            }
        }

        pid_t pid = fork();
        if (pid < 0) {
            std::cerr << "fork failed\n";
            return;
        }

        if (pid == 0) {
            if (prev_read != -1) {
                dup2(prev_read, STDIN_FILENO);
                close(prev_read);
            }
            if (!is_last) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            apply_redirect(cmd);

            auto it = Commands::cmds.find(cmd.program);
            if (it != Commands::cmds.end()) {
                it->second(cmd);
                exit(0);
            }

            auto argv = build_argv(cmd);
            execvp(argv[0], argv.data());
            std::cerr << cmd.program << ": command not found\n";
            exit(127);
        }

        if (prev_read != -1) {
            close(prev_read);
        }
        if (!is_last) {
            close(pipefd[1]);
            prev_read = pipefd[0];
        }
        pids.push_back(pid);
    }

    for (auto pid : pids) {
        int status;
        waitpid(pid, &status, 0);
    }
}

void execute(const Pipeline& pipeline) {
    if (pipeline.empty())
        return;

    if (pipeline.size() == 1) {
        exec_single(pipeline[0]);
    } else {
        exec_pipeline(pipeline);
    }
}

} // namespace shell
