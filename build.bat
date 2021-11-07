@echo off
if not exist build mkdir build
pushd build

del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp

REM compile engine
cl -Zi ../src/*.cpp /c /I ../include /EHsc 
lib *.obj /OUT:d3d11_motor.lib
link *.obj /DLL /OUT:d3d11_motor.dll

set OUT=demo.exe
REM compile app
cl -Zi /Fe%OUT% -nologo -O2 -FC -WX ../app/main.cpp /I ../include /link d3d11_motor.lib -incremental:no -opt:ref /SUBSYSTEM:CONSOLE

del lock.tmp
del *.obj

popd