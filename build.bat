@echo off
if not exist build mkdir build
pushd build

del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp
del *.ilk

REM compile engine
cl -Zi ../src/*.c /c /I ../include /EHsc 
lib *.obj /OUT:d3d11_motor.lib
link *.obj /DLL /OUT:d3d11_motor.dll /DEBUG

set OUT=demo.exe
REM compile app
cl -Zi /Fe%OUT% -nologo -O2 -FC -WX ../app/main.c /I ../include /link d3d11_motor.lib /SUBSYSTEM:CONSOLE

del lock.tmp
del *.obj

popd