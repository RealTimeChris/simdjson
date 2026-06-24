@echo off
setlocal EnableDelayedExpansion

REM Jsonifier tape sweep for MSVC
REM Run from a Visual Studio Developer Command Prompt (so cl.exe / cmake are on PATH)
REM Or run from a vcvars64-initialized cmd shell.

set "COMPILER_KEY=msvc"

if not defined REPO_ROOT (
    set "REPO_ROOT=%CD%"
)
set "RESULTS_DIR=%REPO_ROOT%\sweep_results_%COMPILER_KEY%"
set "BUILD_ROOT=%REPO_ROOT%\build-sweep-%COMPILER_KEY%"
if not defined BENCH_FILTER set "BENCH_FILTER=twitter_full^|kostya^|partial_tweets^|large_random^|distinct_user_id^|find_tweet^|top_tweet"
if not defined BENCH_TARGET set "BENCH_TARGET=benchmark\Release\bench_ondemand.exe"
if not defined JOBS set "JOBS=%NUMBER_OF_PROCESSORS%"

if not exist "%RESULTS_DIR%" mkdir "%RESULTS_DIR%"

echo ========================================== > "%RESULTS_DIR%\sweep.log"
echo Jsonifier Tape Sweep [%COMPILER_KEY%] >> "%RESULTS_DIR%\sweep.log"
echo Repo:    %REPO_ROOT% >> "%RESULTS_DIR%\sweep.log"
echo Filter:  %BENCH_FILTER% >> "%RESULTS_DIR%\sweep.log"
echo Target:  %BENCH_TARGET% >> "%RESULTS_DIR%\sweep.log"
echo Jobs:    %JOBS% >> "%RESULTS_DIR%\sweep.log"
echo ========================================== >> "%RESULTS_DIR%\sweep.log"

type "%RESULTS_DIR%\sweep.log"

set "CONFIGS=16,2 16,4 16,8 20,2 20,4 24,2 24,4 24,6 24,8 28,2 28,4 32,2 32,4 32,8"

for %%C in (%CONFIGS%) do (
    for /f "tokens=1,2 delims=," %%A in ("%%C") do (
        set "tape_max=%%A"
        set "tape_step=%%B"
        call :run_one_config !tape_max! !tape_step!
    )
)

echo. >> "%RESULTS_DIR%\sweep.log"
echo ========================================== >> "%RESULTS_DIR%\sweep.log"
echo [%COMPILER_KEY%] Sweep complete. Results in: %RESULTS_DIR% >> "%RESULTS_DIR%\sweep.log"
echo ========================================== >> "%RESULTS_DIR%\sweep.log"

echo.
echo Sweep complete. Results in: %RESULTS_DIR%
goto :eof

:run_one_config
set "tape_max=%~1"
set "tape_step=%~2"
set "tag=%COMPILER_KEY%-max%tape_max%-step%tape_step%"
set "build_dir=%BUILD_ROOT%\%tag%"
set "csv_out=%RESULTS_DIR%\%tag%.csv"
set "bench_log=%RESULTS_DIR%\%tag%.log"

echo. >> "%RESULTS_DIR%\sweep.log"
echo --- %tag% --- >> "%RESULTS_DIR%\sweep.log"
echo --- %tag% ---

if exist "%build_dir%" rmdir /s /q "%build_dir%"
mkdir "%build_dir%"

cmake -S "%REPO_ROOT%" -B "%build_dir%" ^
    -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DSIMDJSON_DEVELOPER_MODE=ON ^
    -DJSONIFIER_TAPE_MAX=%tape_max% ^
    -DJSONIFIER_TAPE_STEP=%tape_step% ^
    > "%bench_log%.cmake" 2>&1

if errorlevel 1 (
    echo   CMAKE FAILED ^(see %bench_log%.cmake^) >> "%RESULTS_DIR%\sweep.log"
    echo   CMAKE FAILED ^(see %bench_log%.cmake^)
    goto :eof
)

cmake --build "%build_dir%" --config Release -j %JOBS% --target bench_ondemand ^
    > "%bench_log%.build" 2>&1

if errorlevel 1 (
    echo   BUILD FAILED ^(see %bench_log%.build^) >> "%RESULTS_DIR%\sweep.log"
    echo   BUILD FAILED ^(see %bench_log%.build^)
    goto :eof
)

set "bench_bin=%build_dir%\%BENCH_TARGET%"
if not exist "%bench_bin%" (
    echo   BINARY MISSING: %bench_bin% >> "%RESULTS_DIR%\sweep.log"
    echo   BINARY MISSING: %bench_bin%
    goto :eof
)

"%bench_bin%" ^
    --benchmark_filter="%BENCH_FILTER%" ^
    --benchmark_format=csv ^
    --benchmark_out="%csv_out%" ^
    --benchmark_out_format=csv ^
    1> "%bench_log%" 2>&1

if errorlevel 1 (
    echo   BENCHMARK FAILED ^(see %bench_log%^) >> "%RESULTS_DIR%\sweep.log"
    echo   BENCHMARK FAILED ^(see %bench_log%^)
    goto :eof
)

echo   OK -^> %csv_out% >> "%RESULTS_DIR%\sweep.log"
echo   OK -^> %csv_out%
goto :eof