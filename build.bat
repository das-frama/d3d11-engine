@echo off
if not exist build mkdir build
pushd build

del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp
del *.ilk

REM compile engine
cl -Zi ../src/*.c ../src/utils/*.c /c /I ../include /EHsc -nologo /Wall /D "_CRT_SECURE_NO_DEPRECATE=1" /wd4081 /wd4820
lib *.obj /OUT:d3d11_motor.lib /NOLOGO
link *.obj /DLL /OUT:d3d11_motor.dll /DEBUG /NOLOGO

REM compile app
set OUT=demo.exe
cl -Zi /Fe%OUT% -nologo -O2 -FC -W0 ../app/main.c /I ../include /link d3d11_motor.lib /SUBSYSTEM:CONSOLE

del lock.tmp
del *.obj

popd