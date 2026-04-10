@echo off
setlocal EnableExtensions EnableDelayedExpansion

where git >nul 2>nul
if errorlevel 1 (
    echo Error: git is not available in PATH.
    exit /b 1
)

where clang-format >nul 2>nul
if errorlevel 1 (
    echo Error: clang-format is not available in PATH.
    exit /b 1
)

git rev-parse --is-inside-work-tree >nul 2>nul
if errorlevel 1 (
    echo Error: this script must be run inside a git repository.
    exit /b 1
)

set "HAS_FILES=0"
for /f "usebackq delims=" %%F in (`git diff --cached --name-only --diff-filter=ACMR -- *.c *.cc *.cpp *.cxx *.h *.hh *.hpp *.hxx *.ipp *.inl`) do (
    if exist "%%F" (
        if !HAS_FILES! equ 0 (
            echo Formatting staged files with clang-format...
            set "HAS_FILES=1"
        )
        clang-format -i "%%F"
        if errorlevel 1 (
            echo Error: clang-format failed for %%F
            exit /b 1
        )
        git add -- "%%F"
        if errorlevel 1 (
            echo Error: git add failed for %%F
            exit /b 1
        )
        echo   formatted: %%F
    )
)

if "%HAS_FILES%"=="0" (
    echo No staged C/C++ files to format.
    exit /b 0
)

echo Done.
exit /b 0
