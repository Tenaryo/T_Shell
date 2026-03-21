#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

FILES=$(find src include -name '*.cpp' -o -name '*.hpp' 2>/dev/null)

if [ -z "$FILES" ]; then
    echo "No C++ files found"
    exit 1
fi

echo "=== Running clang-format ==="
echo "$FILES" | xargs clang-format -i
echo "clang-format: done"
