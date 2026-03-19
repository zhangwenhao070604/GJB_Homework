@echo off
chcp 65001 >nul
cd /d C:\Users\congm\Desktop\GJB_Homework
set PATH=%PATH%;D:\llvm-project\llvm-project\build\Release\bin

echo Generating A-1-1-1...
clang --analyze -Xanalyzer "-analyzer-checker=GJB8114.A_1_1_1" -o NUL test_A_1_1_1_violation.c > output_A_1_1_1.txt 2>&1

echo Generating A-1-6-4...
clang --analyze -Xanalyzer "-analyzer-checker=GJB8114.A_1_6_4" -o NUL test_A_1_6_4_violation.c > output_A_1_6_4.txt 2>&1

echo Generating A-1-6-5...
clang --analyze -Xanalyzer "-analyzer-checker=GJB8114.A_1_6_5" -o NUL test_A_1_6_5_violation.c > output_A_1_6_5.txt 2>&1

echo Generating A-1-7-2...
clang --analyze -Xanalyzer "-analyzer-checker=GJB8114.A_1_7_2" -o NUL test_A_1_7_2_violation.c > output_A_1_7_2.txt 2>&1

echo Generating A-1-8-1...
clang --analyze -Xanalyzer "-analyzer-checker=GJB8114.A_1_8_1" -o NUL test_A_1_8_1_violation.c > output_A_1_8_1.txt 2>&1

echo Generating A-1-8-2...
clang --analyze -Xanalyzer "-analyzer-checker=GJB8114.A_1_8_2" -o NUL test_A_1_8_2_violation.c > output_A_1_8_2.txt 2>&1

echo Generating A-1-10-3...
clang --analyze -Xanalyzer "-analyzer-checker=GJB8114.A_1_10_3" -o NUL test_A_1_10_3_violation.c > output_A_1_10_3.txt 2>&1

echo Generating A-1-11-1...
clang --analyze -Xanalyzer "-analyzer-checker=GJB8114.A_1_11_1" -o NUL test_A_1_11_1_violation.c > output_A_1_11_1.txt 2>&1

echo Generating A-1-12-1...
clang --analyze -Xanalyzer "-analyzer-checker=GJB8114.A_1_12_1" -o NUL test_A_1_12_1_violation.c > output_A_1_12_1.txt 2>&1

echo Generating A-2-7-1...
clang --analyze -Xanalyzer "-analyzer-checker=GJB8114.A_2_7_1" -o NUL -D_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH test_A_2_7_1_violation.cpp > output_A_2_7_1.txt 2>&1

echo Generating A-2-7-2...
clang --analyze -Xanalyzer "-analyzer-checker=GJB8114.A_2_7_2" -o NUL -D_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH test_A_2_7_2_violation.cpp > output_A_2_7_2.txt 2>&1

echo All done! Press any key to open the first file with default program...
pause >nul
start output_A_1_1_1.txt