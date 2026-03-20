#include <utils.hpp>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <unistd.h>
#include <fcntl.h>

namespace shell {

FdRedirect::FdRedirect(int target_fd, const std::string& filename, int open_flags, mode_t mode) {
    init(target_fd, filename, open_flags, mode);
}

FdRedirect::~FdRedirect() {
    release();
}

FdRedirect::FdRedirect(FdRedirect&& other) noexcept {
    steal_from(other);
}

FdRedirect& FdRedirect::operator=(FdRedirect&& other) noexcept {
    if (this != &other) {
        release();
        steal_from(other);
    }
    return *this;
}


void FdRedirect::init(int target_fd, const std::string& filename, int open_flags, mode_t mode) {
    target_fd_ = target_fd;
    saved_fd_ = dup(target_fd);
    if (saved_fd_ < 0) {
        return;
    }

    int fd = open(filename.c_str(), open_flags, mode);
    if (fd < 0) return;

    if (dup2(fd, target_fd) < 0) {
        close(fd);
        close(saved_fd_);
        return;
    }

    close(fd);
    ok_ = true;
}

void FdRedirect::release() noexcept {
    if (!ok_) return;

    if (dup2(saved_fd_, target_fd_) < 0) {
        std::cerr << "Failed to restore fd\n";
        std::exit(1);
    }
    close(saved_fd_);

    ok_ = false;
}

void FdRedirect::steal_from(FdRedirect& other) noexcept {
    target_fd_ = other.target_fd_;
    saved_fd_ = other.saved_fd_;
    ok_ = other.ok_;

    other.ok_ = false;
}

CoutRedirect::CoutRedirect(const std::string& filename, RedirectOp op) {
    int flags = (op == RedirectOp::Append)
        ? (O_WRONLY | O_CREAT | O_APPEND)
        : (O_WRONLY | O_CREAT | O_TRUNC);
    init(STDOUT_FILENO, filename, flags, 0644);
}

CerrRedirect::CerrRedirect(const std::string& filename, RedirectOp op) {
    int flags = (op == RedirectOp::Append)
        ? (O_WRONLY | O_CREAT | O_APPEND)
        : (O_WRONLY | O_CREAT | O_TRUNC);
    init(STDERR_FILENO, filename, flags, 0644);
}

std::optional<std::string> search_path(const std::string& program) {
    static const char* cpath = getenv("PATH");
    if (cpath == nullptr) {
        std::cerr << "Failed to get path\n";
        return std::nullopt;
    }
    std::stringstream ss(cpath);
    std::string dir;
    while (std::getline(ss, dir, ':')) {
        std::filesystem::path full = std::filesystem::path(dir) / program;
        if (std::filesystem::exists(full) && access(full.c_str(), X_OK) == 0) {
            return full.string();
        }
    }
    return std::nullopt;
}

} // namespace shell
