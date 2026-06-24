-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.
-- The CXX compiler identification is MSVC 19.44.35228.0
-- The C compiler identification is MSVC 19.44.35228.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Looking for fork
-- Looking for fork - not found
-- Looking for wait
-- Looking for wait - not found
-- Looking for C++ include bit
-- Looking for C++ include bit - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - not found
-- Found Threads: TRUE
-- CPM: Adding package google_benchmarks@1.9.5 (1.9.5)
-- Found Git: C:/Program Files/Git/cmd/git.exe (found version "2.51.2.windows.1")
-- Google Benchmark version: v4.6.1-64-gad4b3121, normalized to 4.6.1.64
-- Looking for shm_open in rt
-- Looking for shm_open in rt - not found
-- Performing Test STD_REGEX -- success
-- Performing Test STEADY_CLOCK -- success
-- Performing Test PTHREAD_AFFINITY -- failed
-- CPM: Adding package counters@3.1.0 (3.1.0)
-- CPM: Adding package simdjson-data@ ()
-- Performing Test SIMDJSON_FOUND_STRING_VIEW
-- Performing Test SIMDJSON_FOUND_STRING_VIEW - Success
-- CPM: Adding package cjson@ ()
-- Populating cjson
-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.
-- Configuring done (0.3s)
-- Generating done (0.0s)
-- Build files have been written to: C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/cjson-subbuild
MSBuild version 17.14.40+3e7442088 for .NET Framework

  1>Checking Build System
  1>Creating directories for 'cjson-populate'
  Performing download step (download, verify and extract) for 'cjson-populate'
  -- Downloading...
     dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/cjson-subbuild/cjson-populate-prefix/src/c69134d01746dcf551dd7724b4edb12f922eb0d1.zip'
     timeout='none'
     inactivity timeout='none'
  -- Using src='https://github.com/DaveGamble/cJSON/archive/c69134d01746dcf551dd7724b4edb12f922eb0d1.zip'
  -- [download 2% complete]
  -- [download 4% complete]
  -- [download 5% complete]
  -- [download 6% complete]
  -- [download 8% complete]
  -- [download 9% complete]
  -- [download 11% complete]
  -- [download 12% complete]
  -- [download 13% complete]
  -- [download 15% complete]
  -- [download 16% complete]
  -- [download 17% complete]
  -- [download 19% complete]
  -- [download 21% complete]
  -- [download 22% complete]
  -- [download 24% complete]
  -- [download 25% complete]
  -- [download 28% complete]
  -- [download 30% complete]
  -- [download 31% complete]
  -- [download 32% complete]
  -- [download 34% complete]
  -- [download 35% complete]
  -- [download 36% complete]
  -- [download 37% complete]
  -- [download 38% complete]
  -- [download 39% complete]
  -- [download 41% complete]
  -- [download 42% complete]
  -- [download 43% complete]
  -- [download 44% complete]
  -- [download 45% complete]
  -- [download 46% complete]
  -- [download 48% complete]
  -- [download 49% complete]
  -- [download 50% complete]
  -- [download 51% complete]
  -- [download 52% complete]
  -- [download 54% complete]
  -- [download 55% complete]
  -- [download 56% complete]
  -- [download 57% complete]
  -- [download 58% complete]
  -- [download 59% complete]
  -- [download 61% complete]
  -- [download 62% complete]
  -- [download 64% complete]
  -- [download 66% complete]
  -- [download 68% complete]
  -- [download 69% complete]
  -- [download 70% complete]
  -- [download 71% complete]
  -- [download 72% complete]
  -- [download 73% complete]
  -- [download 74% complete]
  -- [download 75% complete]
  -- [download 78% complete]
  -- [download 82% complete]
  -- [download 85% complete]
  -- [download 88% complete]
  -- [download 90% complete]
  -- [download 91% complete]
  -- [download 92% complete]
  -- [download 93% complete]
  -- [download 94% complete]
  -- [download 95% complete]
  -- [download 96% complete]
  -- [download 97% complete]
  -- [download 98% complete]
  -- [download 99% complete]
  -- [download 100% complete]
  -- Downloading... done
  -- extracting...
       src='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/cjson-subbuild/cjson-populate-prefix/src/c69134d01746dcf551dd7724b4edb12f922eb0d1.zip'
       dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/cjson-src'
  -- extracting... [tar xf]
  -- extracting... [analysis]
  -- extracting... [rename]
  -- extracting... [clean up]
  -- extracting... done
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/cjson-subbuild/CMakeLists.txt
  No update step for 'cjson-populate'
  No patch step for 'cjson-populate'
  No configure step for 'cjson-populate'
  No build step for 'cjson-populate'
  No install step for 'cjson-populate'
  No test step for 'cjson-populate'
  Completed 'cjson-populate'
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/cjson-subbuild/CMakeLists.txt
-- CPM: Adding package fastjson@ ()
-- Populating fastjson
-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.
-- Configuring done (0.3s)
-- Generating done (0.0s)
-- Build files have been written to: C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/fastjson-subbuild
MSBuild version 17.14.40+3e7442088 for .NET Framework

  1>Checking Build System
  1>Creating directories for 'fastjson-populate'
  Performing download step (download, verify and extract) for 'fastjson-populate'
  -- Downloading...
     dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/fastjson-subbuild/fastjson-populate-prefix/src/485f994a61a64ac73fa6a40d4d639b99b463563b.zip'
     timeout='none'
     inactivity timeout='none'
  -- Using src='https://github.com/mikeando/fastjson/archive/485f994a61a64ac73fa6a40d4d639b99b463563b.zip'
  -- [download 2% complete]
  -- [download 17% complete]
  -- [download 30% complete]
  -- [download 32% complete]
  -- [download 37% complete]
  -- [download 42% complete]
  -- [download 45% complete]
  -- [download 52% complete]
  -- [download 57% complete]
  -- [download 68% complete]
  -- [download 70% complete]
  -- [download 73% complete]
  -- [download 83% complete]
  -- [download 86% complete]
  -- [download 88% complete]
  -- [download 96% complete]
  -- [download 100% complete]
  -- Downloading... done
  -- extracting...
       src='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/fastjson-subbuild/fastjson-populate-prefix/src/485f994a61a64ac73fa6a40d4d639b99b463563b.zip'
       dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/fastjson-src'
  -- extracting... [tar xf]
  -- extracting... [analysis]
  -- extracting... [rename]
  -- extracting... [clean up]
  -- extracting... done
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/fastjson-subbuild/CMakeLists.txt
  No update step for 'fastjson-populate'
  No patch step for 'fastjson-populate'
  No configure step for 'fastjson-populate'
  No build step for 'fastjson-populate'
  No install step for 'fastjson-populate'
  No test step for 'fastjson-populate'
  Completed 'fastjson-populate'
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/fastjson-subbuild/CMakeLists.txt
-- CPM: Adding package gason@ ()
-- Populating gason
-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.
-- Configuring done (0.3s)
-- Generating done (0.0s)
-- Build files have been written to: C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/gason-subbuild
MSBuild version 17.14.40+3e7442088 for .NET Framework

  1>Checking Build System
  1>Creating directories for 'gason-populate'
  Performing download step (download, verify and extract) for 'gason-populate'
  -- Downloading...
     dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/gason-subbuild/gason-populate-prefix/src/7aee524189da1c1ecd19f67981e3d903dae25470.zip'
     timeout='none'
     inactivity timeout='none'
  -- Using src='https://github.com/vivkin/gason/archive/7aee524189da1c1ecd19f67981e3d903dae25470.zip'
  -- [download 45% complete]
  -- [download 52% complete]
  -- [download 84% complete]
  -- [download 92% complete]
  -- [download 100% complete]
  -- Downloading... done
  -- extracting...
       src='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/gason-subbuild/gason-populate-prefix/src/7aee524189da1c1ecd19f67981e3d903dae25470.zip'
       dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/gason-src'
  -- extracting... [tar xf]
  -- extracting... [analysis]
  -- extracting... [rename]
  -- extracting... [clean up]
  -- extracting... done
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/gason-subbuild/CMakeLists.txt
  No update step for 'gason-populate'
  No patch step for 'gason-populate'
  No configure step for 'gason-populate'
  No build step for 'gason-populate'
  No install step for 'gason-populate'
  No test step for 'gason-populate'
  Completed 'gason-populate'
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/gason-subbuild/CMakeLists.txt
-- CPM: Adding package jsmn@ ()
-- Populating jsmn
-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.
-- Configuring done (0.3s)
-- Generating done (0.0s)
-- Build files have been written to: C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/jsmn-subbuild
MSBuild version 17.14.40+3e7442088 for .NET Framework

  1>Checking Build System
  1>Creating directories for 'jsmn-populate'
  Performing download step (download, verify and extract) for 'jsmn-populate'
  -- Downloading...
     dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/jsmn-subbuild/jsmn-populate-prefix/src/18e9fe42cbfe21d65076f5c77ae2be379ad1270f.zip'
     timeout='none'
     inactivity timeout='none'
  -- Using src='https://github.com/zserge/jsmn/archive/18e9fe42cbfe21d65076f5c77ae2be379ad1270f.zip'
  -- [download 41% complete]
  -- [download 59% complete]
  -- [download 95% complete]
  -- [download 100% complete]
  -- Downloading... done
  -- extracting...
       src='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/jsmn-subbuild/jsmn-populate-prefix/src/18e9fe42cbfe21d65076f5c77ae2be379ad1270f.zip'
       dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/jsmn-src'
  -- extracting... [tar xf]
  -- extracting... [analysis]
  -- extracting... [rename]
  -- extracting... [clean up]
  -- extracting... done
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/jsmn-subbuild/CMakeLists.txt
  No update step for 'jsmn-populate'
  No patch step for 'jsmn-populate'
  No configure step for 'jsmn-populate'
  No build step for 'jsmn-populate'
  No install step for 'jsmn-populate'
  No test step for 'jsmn-populate'
  Completed 'jsmn-populate'
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/jsmn-subbuild/CMakeLists.txt
-- CPM: Adding package nlohmann_json@3.12.0 (3.12.0)
-- Using the multi-header code from C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/nlohmann_json-src/include/
-- CPM: Adding package rapidjson@ ()
-- Populating rapidjson
-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.
-- Configuring done (0.3s)
-- Generating done (0.0s)
-- Build files have been written to: C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/rapidjson-subbuild
MSBuild version 17.14.40+3e7442088 for .NET Framework

  1>Checking Build System
  1>Creating directories for 'rapidjson-populate'
  Performing download step (download, verify and extract) for 'rapidjson-populate'
  -- Downloading...
     dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/rapidjson-subbuild/rapidjson-populate-prefix/src/805d7ed5dfe97a39b8b0816fd5eeed8731dc4936.zip'
     timeout='none'
     inactivity timeout='none'
  -- Using src='https://github.com/Tencent/rapidjson/archive/805d7ed5dfe97a39b8b0816fd5eeed8731dc4936.zip'
  -- Downloading... done
  -- extracting...
       src='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/rapidjson-subbuild/rapidjson-populate-prefix/src/805d7ed5dfe97a39b8b0816fd5eeed8731dc4936.zip'
       dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/rapidjson-src'
  -- extracting... [tar xf]
  -- extracting... [analysis]
  -- extracting... [rename]
  -- extracting... [clean up]
  -- extracting... done
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/rapidjson-subbuild/CMakeLists.txt
  No update step for 'rapidjson-populate'
  No patch step for 'rapidjson-populate'
  No configure step for 'rapidjson-populate'
  No build step for 'rapidjson-populate'
  No install step for 'rapidjson-populate'
  No test step for 'rapidjson-populate'
  Completed 'rapidjson-populate'
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/rapidjson-subbuild/CMakeLists.txt
-- CPM: Adding package sajson@ ()
-- Populating sajson
-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.
-- Configuring done (0.3s)
-- Generating done (0.0s)
-- Build files have been written to: C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/sajson-subbuild
MSBuild version 17.14.40+3e7442088 for .NET Framework

  1>Checking Build System
  1>Creating directories for 'sajson-populate'
  Performing download step (download, verify and extract) for 'sajson-populate'
  -- Downloading...
     dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/sajson-subbuild/sajson-populate-prefix/src/2dcfd350586375f9910f74821d4f07d67ae455ba.zip'
     timeout='none'
     inactivity timeout='none'
  -- Using src='https://github.com/chadaustin/sajson/archive/2dcfd350586375f9910f74821d4f07d67ae455ba.zip'
  -- [download 0% complete]
  -- [download 1% complete]
  -- [download 2% complete]
  -- [download 3% complete]
  -- [download 4% complete]
  -- [download 5% complete]
  -- [download 6% complete]
  -- [download 7% complete]
  -- [download 8% complete]
  -- [download 9% complete]
  -- [download 10% complete]
  -- [download 11% complete]
  -- [download 12% complete]
  -- [download 13% complete]
  -- [download 14% complete]
  -- [download 15% complete]
  -- [download 16% complete]
  -- [download 17% complete]
  -- [download 18% complete]
  -- [download 19% complete]
  -- [download 20% complete]
  -- [download 21% complete]
  -- [download 22% complete]
  -- [download 23% complete]
  -- [download 24% complete]
  -- [download 25% complete]
  -- [download 26% complete]
  -- [download 27% complete]
  -- [download 28% complete]
  -- [download 29% complete]
  -- [download 30% complete]
  -- [download 31% complete]
  -- [download 32% complete]
  -- [download 33% complete]
  -- [download 34% complete]
  -- [download 35% complete]
  -- [download 36% complete]
  -- [download 37% complete]
  -- [download 38% complete]
  -- [download 39% complete]
  -- [download 40% complete]
  -- [download 41% complete]
  -- [download 42% complete]
  -- [download 43% complete]
  -- [download 44% complete]
  -- [download 45% complete]
  -- [download 46% complete]
  -- [download 47% complete]
  -- [download 49% complete]
  -- [download 51% complete]
  -- [download 53% complete]
  -- [download 54% complete]
  -- [download 55% complete]
  -- [download 56% complete]
  -- [download 57% complete]
  -- [download 58% complete]
  -- [download 59% complete]
  -- [download 60% complete]
  -- [download 61% complete]
  -- [download 62% complete]
  -- [download 63% complete]
  -- [download 64% complete]
  -- [download 65% complete]
  -- [download 66% complete]
  -- [download 67% complete]
  -- [download 68% complete]
  -- [download 69% complete]
  -- [download 70% complete]
  -- [download 71% complete]
  -- [download 72% complete]
  -- [download 73% complete]
  -- [download 74% complete]
  -- [download 75% complete]
  -- [download 76% complete]
  -- [download 78% complete]
  -- [download 81% complete]
  -- [download 83% complete]
  -- [download 85% complete]
  -- [download 86% complete]
  -- [download 87% complete]
  -- [download 88% complete]
  -- [download 89% complete]
  -- [download 90% complete]
  -- [download 91% complete]
  -- [download 92% complete]
  -- [download 93% complete]
  -- [download 94% complete]
  -- [download 95% complete]
  -- [download 96% complete]
  -- [download 97% complete]
  -- [download 98% complete]
  -- [download 99% complete]
  -- [download 100% complete]
  -- Downloading... done
  -- extracting...
       src='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/sajson-subbuild/sajson-populate-prefix/src/2dcfd350586375f9910f74821d4f07d67ae455ba.zip'
       dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/sajson-src'
  -- extracting... [tar xf]
  -- extracting... [analysis]
  -- extracting... [rename]
  -- extracting... [clean up]
  -- extracting... done
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/sajson-subbuild/CMakeLists.txt
  No update step for 'sajson-populate'
  No patch step for 'sajson-populate'
  No configure step for 'sajson-populate'
  No build step for 'sajson-populate'
  No install step for 'sajson-populate'
  No test step for 'sajson-populate'
  Completed 'sajson-populate'
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/sajson-subbuild/CMakeLists.txt

C:\Users\Chris\source\repos\simdjson\build-sweep-msvc\msvc-max8-step\_deps\jsonifier-src\cmake>"C:/Program Files/CMake/bin/cmake.exe" -S ./ -B ./Build -DCMAKE_BUILD_TYPE=Release 
-- Building for: Visual Studio 17 2022
-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.
-- The C compiler identification is MSVC 19.44.35228.0
-- The CXX compiler identification is MSVC 19.44.35228.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (2.6s)
-- Generating done (0.0s)
CMake Warning:
  Manually-specified variables were not used by the project:

    CMAKE_BUILD_TYPE


-- Build files have been written to: C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/jsonifier-src/cmake/Build

C:\Users\Chris\source\repos\simdjson\build-sweep-msvc\msvc-max8-step\_deps\jsonifier-src\cmake>"C:/Program Files/CMake/bin/cmake.exe" --build ./Build --config=Release 
MSBuild version 17.14.40+3e7442088 for .NET Framework

  1>Checking Build System
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/jsonifier-src/cmake/CMakeLists.txt
  main.cpp
  feature_detector.vcxproj -> C:\Users\Chris\source\repos\simdjson\build-sweep-msvc\msvc-max8-step\_deps\jsonifier-src\cmake\Build\Release\feature_detector.exe
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/jsonifier-src/cmake/CMakeLists.txt
CPU Brand: Intel(R) Core(TM) i9-14900KF
-- Instruction Set Found: PopCnt
-- Instruction Set Found: Bmi
-- Instruction Set Found: Avx2
-- CPM: Adding package ujson4c@ ()
-- Populating ujson4c
-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.
-- Configuring done (0.3s)
-- Generating done (0.0s)
-- Build files have been written to: C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/ujson4c-subbuild
MSBuild version 17.14.40+3e7442088 for .NET Framework

  1>Checking Build System
  1>Creating directories for 'ujson4c-populate'
  Performing download step (download, verify and extract) for 'ujson4c-populate'
  -- Downloading...
     dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/ujson4c-subbuild/ujson4c-populate-prefix/src/e14f3fd5207fe30d1bdea723f260609e69d1abfa.zip'
     timeout='none'
     inactivity timeout='none'
  -- Using src='https://github.com/esnme/ujson4c/archive/e14f3fd5207fe30d1bdea723f260609e69d1abfa.zip'
  -- [download 7% complete]
  -- [download 10% complete]
  -- [download 11% complete]
  -- [download 13% complete]
  -- [download 17% complete]
  -- [download 18% complete]
  -- [download 21% complete]
  -- [download 22% complete]
  -- [download 27% complete]
  -- [download 29% complete]
  -- [download 33% complete]
  -- [download 39% complete]
  -- [download 44% complete]
  -- [download 46% complete]
  -- [download 47% complete]
  -- [download 49% complete]
  -- [download 51% complete]
  -- [download 54% complete]
  -- [download 56% complete]
  -- [download 57% complete]
  -- [download 59% complete]
  -- [download 60% complete]
  -- [download 63% complete]
  -- [download 68% complete]
  -- [download 69% complete]
  -- [download 73% complete]
  -- [download 75% complete]
  -- [download 77% complete]
  -- [download 78% complete]
  -- [download 84% complete]
  -- [download 85% complete]
  -- [download 88% complete]
  -- [download 89% complete]
  -- [download 93% complete]
  -- [download 94% complete]
  -- [download 100% complete]
  -- Downloading... done
  -- extracting...
       src='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/ujson4c-subbuild/ujson4c-populate-prefix/src/e14f3fd5207fe30d1bdea723f260609e69d1abfa.zip'
       dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/ujson4c-src'
  -- extracting... [tar xf]
  -- extracting... [analysis]
  -- extracting... [rename]
  -- extracting... [clean up]
  -- extracting... done
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/ujson4c-subbuild/CMakeLists.txt
  No update step for 'ujson4c-populate'
  No patch step for 'ujson4c-populate'
  No configure step for 'ujson4c-populate'
  No build step for 'ujson4c-populate'
  No install step for 'ujson4c-populate'
  No test step for 'ujson4c-populate'
  Completed 'ujson4c-populate'
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/ujson4c-subbuild/CMakeLists.txt
-- CPM: Adding package yyjson@ ()
-- Populating yyjson
-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.
-- Configuring done (0.3s)
-- Generating done (0.0s)
-- Build files have been written to: C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/yyjson-subbuild
MSBuild version 17.14.40+3e7442088 for .NET Framework

  1>Checking Build System
  1>Creating directories for 'yyjson-populate'
  Performing download step (download, verify and extract) for 'yyjson-populate'
  -- Downloading...
     dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/yyjson-subbuild/yyjson-populate-prefix/src/c3856514de0a67d7b66939bf3ed491a2d6e61277.zip'
     timeout='none'
     inactivity timeout='none'
  -- Using src='https://github.com/ibireme/yyjson/archive/c3856514de0a67d7b66939bf3ed491a2d6e61277.zip'
  -- [download 0% complete]
  -- [download 1% complete]
  -- [download 2% complete]
  -- [download 3% complete]
  -- [download 4% complete]
  -- [download 5% complete]
  -- [download 6% complete]
  -- [download 7% complete]
  -- [download 8% complete]
  -- [download 9% complete]
  -- [download 10% complete]
  -- [download 11% complete]
  -- [download 12% complete]
  -- [download 13% complete]
  -- [download 14% complete]
  -- [download 15% complete]
  -- [download 16% complete]
  -- [download 17% complete]
  -- [download 18% complete]
  -- [download 19% complete]
  -- [download 20% complete]
  -- [download 21% complete]
  -- [download 22% complete]
  -- [download 23% complete]
  -- [download 24% complete]
  -- [download 25% complete]
  -- [download 26% complete]
  -- [download 27% complete]
  -- [download 28% complete]
  -- [download 29% complete]
  -- [download 30% complete]
  -- [download 31% complete]
  -- [download 32% complete]
  -- [download 33% complete]
  -- [download 34% complete]
  -- [download 35% complete]
  -- [download 36% complete]
  -- [download 37% complete]
  -- [download 38% complete]
  -- [download 39% complete]
  -- [download 40% complete]
  -- [download 41% complete]
  -- [download 42% complete]
  -- [download 43% complete]
  -- [download 44% complete]
  -- [download 45% complete]
  -- [download 46% complete]
  -- [download 47% complete]
  -- [download 48% complete]
  -- [download 49% complete]
  -- [download 50% complete]
  -- [download 51% complete]
  -- [download 52% complete]
  -- [download 53% complete]
  -- [download 54% complete]
  -- [download 55% complete]
  -- [download 56% complete]
  -- [download 57% complete]
  -- [download 58% complete]
  -- [download 59% complete]
  -- [download 60% complete]
  -- [download 61% complete]
  -- [download 62% complete]
  -- [download 63% complete]
  -- [download 64% complete]
  -- [download 65% complete]
  -- [download 66% complete]
  -- [download 67% complete]
  -- [download 68% complete]
  -- [download 69% complete]
  -- [download 70% complete]
  -- [download 71% complete]
  -- [download 72% complete]
  -- [download 73% complete]
  -- [download 74% complete]
  -- [download 75% complete]
  -- [download 76% complete]
  -- [download 77% complete]
  -- [download 78% complete]
  -- [download 79% complete]
  -- [download 80% complete]
  -- [download 81% complete]
  -- [download 82% complete]
  -- [download 83% complete]
  -- [download 84% complete]
  -- [download 85% complete]
  -- [download 86% complete]
  -- [download 87% complete]
  -- [download 88% complete]
  -- [download 89% complete]
  -- [download 90% complete]
  -- [download 91% complete]
  -- [download 92% complete]
  -- [download 93% complete]
  -- [download 94% complete]
  -- [download 95% complete]
  -- [download 96% complete]
  -- [download 97% complete]
  -- [download 98% complete]
  -- [download 99% complete]
  -- [download 100% complete]
  -- Downloading... done
  -- extracting...
       src='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/yyjson-subbuild/yyjson-populate-prefix/src/c3856514de0a67d7b66939bf3ed491a2d6e61277.zip'
       dst='C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/yyjson-src'
  -- extracting... [tar xf]
  -- extracting... [analysis]
  -- extracting... [rename]
  -- extracting... [clean up]
  -- extracting... done
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/yyjson-subbuild/CMakeLists.txt
  No update step for 'yyjson-populate'
  No patch step for 'yyjson-populate'
  No configure step for 'yyjson-populate'
  No build step for 'yyjson-populate'
  No install step for 'yyjson-populate'
  No test step for 'yyjson-populate'
  Completed 'yyjson-populate'
  Building Custom Rule C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step/_deps/yyjson-subbuild/CMakeLists.txt
-- CPM: Adding package cxxopts@ ()
-- We have cxxopts as a dependency and we are building the tools (e.g., json2json).
-- Found Python3: C:/Users/Chris/AppData/Local/Programs/Python/Python311/python.exe (found version "3.11.9") found components: Interpreter
-- Including amalgamate_demo test. 
-- Including simdjson-singleheader test.
-- Including amalgamate_demo_direct_from_repository test.
-- compiler id: MSVC version: 19.44.35228.0
-- compiler id: MSVC version: 19.44.35228.0
-- Either git is unavailable or else it is too old. We are disabling checkperf targets.
-- Performing Test SIMDJSON_COMPILER_SUPPORTS_CXX20
-- Performing Test SIMDJSON_COMPILER_SUPPORTS_CXX20 - Failed
CMake Warning at CMakeLists.txt:473 (message):
  just_ascii test disabled because required tools were not found:
  find='C:/Windows/System32/find.exe', file='FILE_CMD-NOTFOUND',
  grep='GREP_CMD-NOTFOUND'


grep and nm are unavailable on this system.
-- Configuring done (142.8s)
-- Generating done (4.7s)
-- Build files have been written to: C:/Users/Chris/source/repos/simdjson/build-sweep-msvc/msvc-max8-step
