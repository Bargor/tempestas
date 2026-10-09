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

## Mandatory Clang-Tidy Rule

- After every change to any C++ source or header file (`.c`, `.cc`, `.cpp`, `.h`, `.hh`, `.hpp`), run clang-tidy on each changed file after formatting and before finishing:
  - `./clang-tidy <changed-file> -p <build-directory>`
- Use the repository's `.clang-tidy` configuration and a build directory containing `compile_commands.json`. For headers, supply the compilation flags from a source file that includes the header if the compilation database has no header entry.
- Fix diagnostics reported for changed code and rerun formatting and clang-tidy after any fixes.
- This rule is mandatory for all code changes, including tests.
