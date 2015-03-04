@echo off

set ROOT_DIR=%CD%
set BISON_DIR=%ROOT_DIR%\Tools\bison
set FLEX_DIR=%ROOT_DIR%\Tools\flex
set GCC_DIR=%ROOT_DIR%\Tools\MinGW

set PATH=%PATH%;%BISON_DIR%\bin;%FLEX_DIR%\bin;%GCC_DIR%\bin

bison -d -v hcc.y
IF %ERRORLEVEL% GEQ 1 goto :error

flex  hcc.l
IF %ERRORLEVEL% GEQ 1 goto :error

gcc -g -c hcc.tab.c -o hcc_y.o
IF %ERRORLEVEL% GEQ 1 goto :error

gcc -g -Wall -c lex.yy.c
IF %ERRORLEVEL% GEQ 1 goto :error

gcc -g -c hcc.tab.c -o hcc_y.o
IF %ERRORLEVEL% GEQ 1 goto :error

gcc -g -Wall -c hcc.c 
IF %ERRORLEVEL% GEQ 1 goto :error

gcc -g -Wall -c queue.c 
IF %ERRORLEVEL% GEQ 1 goto :error

gcc -g -Wall -c node.c 
IF %ERRORLEVEL% GEQ 1 goto :error

gcc -g -Wall -c variableQueueLogic.c 
IF %ERRORLEVEL% GEQ 1 goto :error

gcc -g -Wall -c codeGeneration.c 
IF %ERRORLEVEL% GEQ 1 goto :error

gcc -g -Wall -c symTab.c 
IF %ERRORLEVEL% GEQ 1 goto :error

gcc -g -o hcc.exe  codeGeneration.o symTab.o hcc.o queue.o node.o variableQueueLogic.o hcc_y.o lex.yy.o -lm -lfl
IF %ERRORLEVEL% GEQ 1 goto :error

pause
goto :eof

:error
echo -----
echo Failed to compile
echo -----
pause