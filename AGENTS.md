# Tempestas Agent Instructions

## Canonical Source

- This file is the canonical instruction source for coding assistants in this repository.

## Naming Convention

- Use `snake_case` as the default naming convention for project-owned code.
- Apply `snake_case` to functions, methods, variables, parameters, and new types unless an external API or framework requires a different form.

## Mandatory Formatting Rule

- After every change to any C++ source or header file (`.c`, `.cc`, `.cpp`, `.h`, `.hh`, `.hpp`), run formatting on each changed file before finishing:
  - `./clang-format -i <changed-file>`
- This rule is mandatory for all code changes, including tests.
