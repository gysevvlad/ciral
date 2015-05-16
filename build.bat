echo off
cd obj
cl /W4 /nologo /c /D _CRT_SECURE_NO_WARNINGS ..\src\*.c
cd ..\bin
link ..\obj\*.obj /nologo /OUT:ciral.exe
cd ..
