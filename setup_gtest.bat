@echo off
echo Downloading Google Test...
powershell -Command "& {Invoke-WebRequest -Uri 'https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip' -OutFile 'googletest.zip'}"

echo Extracting Google Test...
powershell -Command "& {Expand-Archive -Path 'googletest.zip' -DestinationPath '.' -Force}"

echo Building Google Test...
cd googletest-1.14.0
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release

echo Copying files...
mkdir ..\..\googletest
mkdir ..\..\googletest\include
mkdir ..\..\googletest\lib
xcopy /E /I /Y include\gtest ..\..\googletest\include\gtest
copy /Y lib\Release\*.lib ..\..\googletest\lib\

echo Cleaning up...
cd ..\..
rmdir /S /Q googletest-1.14.0
del googletest.zip

echo Done! 