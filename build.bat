@echo off
if not exist build mkdir build
pushd build

del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp
del *.ilk

REM compile engine
set sources=..\src\*.c ..\src\platform\*.c ..\src\utils\*.c ..\src\renderer\*.c
set warnings_to_ignore=-wd4201 -wd4116 -wd4101 -wd4103 -wd4081 -wd4204 -wd4255 -wd4668 -wd4820 -wd4100 -wd4189 -wd4711 -wd4710 -wd4101 -wd4296 -wd4311 -wd4115 -wd4702 -wd4456 -wd4555
set flags=-nologo  -Zi -FC -W0 %warnings_to_ignore%

REM cl -nologo -Zi %warnings_to_ignore% /Tc %sources% /I ../include /link ws2_32.lib 

set libs=user32.lib dxgi.lib d3d11.lib dxguid.lib

cl %sources% %flags% /c /I ../include /D "_CRT_SECURE_NO_DEPRECATE=1" 
lib *.obj /OUT:d3d11_motor.lib /NOLOGO
link *.obj %libs% /DLL /OUT:d3d11_motor.dll /DEBUG /NOLOGO

REM compile app
set out=demo.exe
cl -Zi /Fe%out% -nologo -O2 -FC -W0 ../app/main.c /I ../include /link d3d11_motor.lib %libs% /SUBSYSTEM:CONSOLE

del lock.tmp
del *.obj

popd