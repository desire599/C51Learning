@echo off
set KEIL_BIN_FOLDER=C:\Program Files (x86)\Keil_v5\C51\BIN
cls
echo author:kcqnly
echo compiling....
echo %1
echo %2
echo %3
cd /D %2
for %%i in (*.c) do ( 
    echo %%i
    call "%KEIL_BIN_FOLDER%\C51.EXE" %%i 
)
for %%i in (*.LST) do ( move %%i "%1\bin" > NUL )
for %%i in (*.OBJ) do ( move %%i "%1\bin" > NUL )
call "%KEIL_BIN_FOLDER%\A51.EXE" %3\STARTUP.A51 >NUL

move "%3\STARTUP.OBJ" "%1\bin" > NUL
move "%3\STARTUP.LST" "%1\bin" > NUL
cd %1/bin
@setlocal enableextensions enabledelayedexpansion
set result=%3\C51S.LIB
for %%i in (*.OBJ) do (
    set result=!result!,%%i
)
echo %result%

call "%KEIL_BIN_FOLDER%\BL51.EXE" %result% TO %4 
call "%KEIL_BIN_FOLDER%\OH51.EXE" "%1\bin\%4" 


echo Compiled successfully: bin\%4.hex
exit 0
