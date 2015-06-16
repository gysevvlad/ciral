cls
echo off
cd obj
cl /W4 /nologo /c /D _CRT_SECURE_NO_WARNINGS ..\src\test.c ..\src\matrix.c ..\src\liner_alg.c
cd ..\bin
link ..\obj\matrix.obj ..\obj\test.obj ..\obj\liner_alg.obj /nologo /OUT:test.exe
cd ..
.\bin\test < in2
