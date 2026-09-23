# Tempestas Agent Instructions

## Canonical Source

- This file is the canonical instruction source for coding assistants in this repository.

## Naming Convention

- Use `snake_case` as the default naming convention for project-owned code.
- Apply `snake_case` to functions, methods, variables, parameters, and new types unless an external API or framework requires a different form.

## C++ Exceptions

- Project-owned C++ code does not use exceptions.
- Do not use `try`, `catch`, `throw`, or APIs that require exception-based error handling in project-owned code.
- Preserve the no-exceptions compiler configuration (`/EHs-c-` with `_HAS_EXCEPTIONS=0` on MSVC and `-fno-exceptions` on GCC/Clang).
- Third-party dependencies may retain exception support when required by their implementation.

## Mandatory Formatting Rule

- After every change to any C++ source or header file (`.c`, `.cc`, `.cpp`, `.h`, `.hh`, `.hpp`), run formatting on each changed file before finishing:
  - `./clang-format -i <changed-file>`
- This rule is mandatory for all code changes, including tests.
