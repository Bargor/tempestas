#!/usr/bin/env bash

set -euo pipefail

if ! command -v git >/dev/null 2>&1; then
  echo "Error: git is not available in PATH." >&2
  exit 1
fi

if ! command -v clang-format >/dev/null 2>&1; then
  echo "Error: clang-format is not available in PATH." >&2
  exit 1
fi

if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
  echo "Error: this script must be run inside a git repository." >&2
  exit 1
fi

mapfile -d '' staged_files < <(
  git diff --cached --name-only --diff-filter=ACMR -z -- \
    '*.c' '*.cc' '*.cpp' '*.cxx' '*.h' '*.hh' '*.hpp' '*.hxx' '*.ipp' '*.inl'
)

if [[ ${#staged_files[@]} -eq 0 ]]; then
  echo "No staged C/C++ files to format."
  exit 0
fi

echo "Formatting staged files with clang-format..."
for file in "${staged_files[@]}"; do
  if [[ -f "$file" ]]; then
    clang-format -i "$file"
    git add -- "$file"
    echo "  formatted: $file"
  fi
done

echo "Done."
