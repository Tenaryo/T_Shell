# T Shell - A Modern C++ Shell

A lightweight POSIX-compliant shell implementation for practice in modern C++.

---

## Features

### Core Shell Features

- **Command Execution**: Run external programs and builtin commands
- **Pipeline Support**: Chain commands with `|` operator
- **I/O Redirection**: Support for `>`, `>>`, `1>`, `2>`, `1>>`, `2>>`
- **Quote Handling**: Single quotes, double quotes, and escape sequences
- **Command History**: Full history management with readline integration

### Builtin Commands

| Command | Description |
|---------|-------------|
| `cd [dir]` | Change directory (supports `~` expansion) |
| `pwd` | Print working directory |
| `echo [args...]` | Print arguments |
| `type [cmd]` | Show command type (builtin or executable path) |
| `history [N]` | Show last N commands |
| `history -r <file>` | Read history from file |
| `history -w <file>` | Write history to file |
| `history -a <file>` | Append new history to file |
| `exit` | Exit the shell |

### Interactive Features

- **Tab Completion**: Auto-complete commands (builtins + PATH executables)
- **HISTFILE Support**: Automatic history persistence via `HISTFILE` environment variable

---

## Build

### Prerequisites

- CMake 3.13+
- C++23 compiler (GCC 13+, Clang 16+)
- readline library

### Compile

```bash
cmake -B build
cmake --build build
```

### Run

```bash
./build/shell
```

---

## Code Quality

### Format & Lint

```bash
./scripts/lint.sh
```

This runs:
- `clang-format` - Code formatting (LLVM style)
- `clang-tidy` - Static analysis

---

## Project Structure

```
.
├── include/           # Header files
│   ├── commands.hpp   # Builtin command declarations
│   ├── execute.hpp    # Execution interface
│   ├── executable_cache.hpp  # PATH executable cache
│   ├── lexer.hpp      # Tokenizer
│   ├── parse.hpp      # Parser
│   ├── reader.hpp     # Line reader with readline
│   ├── redirect.hpp   # I/O redirection RAII
│   └── types.hpp      # Core types (Token, Command, Pipeline)
├── src/               # Implementation files
├── scripts/
│   └── lint.sh        # Format & lint script
├── .clang-format      # clang-format config
└── .clang-tidy        # clang-tidy config
```

---

## Technical Highlights

### C++ Techniques

- **C++23 Standard**: Uses latest C++ features
- **RAII**: `FdRedirect` class for automatic file descriptor restoration
- **Modern Memory Management**: Minimal raw pointers, extensive use of RAII
- **Namespace Organization**: Clean separation with `shell::` namespace
- **Constexpr & Templates**: Compile-time computation where applicable

### Architecture

```
Input Line
    │
    ▼
┌─────────┐
│  Lexer  │  Tokenization (words, redirects, pipes)
└────┬────┘
     │
     ▼
┌─────────┐
│  Parser │  Build Command/Pipeline AST
└────┬────┘
     │
     ▼
┌──────────┐
│ Executor │  Fork/exec for external, direct call for builtins
└──────────┘
```

### Key Design Decisions

1. **Pipeline Execution**: Each command in a pipeline runs in a separate child process via `fork()`, connected with `pipe()` and `dup2()`

2. **Builtin in Pipeline**: Builtin commands also run in child processes within pipelines, ensuring `cd | cat` doesn't affect the main shell's working directory

3. **Redirect RAII**: For single builtin commands, `FdRedirect` RAII class saves and restores file descriptors; for external commands and pipelines, direct `dup2()` is used since `execvp()` replaces the process

4. **Executable Cache**: `ExecutableCache` scans `$PATH` once at startup and caches all available executables for fast tab completion

5. **History Management**: Uses readline's built-in history API with `HISTFILE` environment variable support for automatic persistence

---

## License

MIT
