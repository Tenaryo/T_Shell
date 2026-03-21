#pragma once
#include <types.hpp>
#include <string>
#include <optional>
#include <fcntl.h>

namespace shell {

class FdRedirect {
public:
    FdRedirect(int target_fd, const std::string& filename, int open_flags, mode_t mode = 0644);
    ~FdRedirect();

    FdRedirect(const FdRedirect&) = delete;
    FdRedirect& operator=(const FdRedirect&) = delete;

    FdRedirect(FdRedirect&& other) noexcept;
    FdRedirect& operator=(FdRedirect&& other) noexcept;

    explicit operator bool() const { return ok_; }
    bool ok() const { return ok_; }

protected:
    FdRedirect() = default;
    void init(int target_fd, const std::string& filename, int open_flags, mode_t mode = 0644);

private:
    void release() noexcept;
    void steal_from(FdRedirect& other) noexcept;

    int target_fd_{-1};
    int saved_fd_{-1};
    bool ok_{false};
};

class CoutRedirect : public FdRedirect {
public:
    explicit CoutRedirect(const std::string& filename, RedirectOp op);
};

class CerrRedirect : public FdRedirect {
public:
    explicit CerrRedirect(const std::string& filename, RedirectOp op);
};

} // namespace shell
