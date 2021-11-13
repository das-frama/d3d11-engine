@echo off
if not exist build mkdir build
pushd build

del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp
del *.ilk

set sources=..\src\*.c ..\src\utils\*.c ..\src\platform\*.c ..\src\renderer\*.c ..\src\math\*.c ..\src\assets\*.c
set warnings_to_ignore=-wd4201 -wd4116 -wd4101 -wd4103 -wd4081 -wd4204 -wd4255 -wd4668 -wd4820 -wd4100 -wd4189 -wd4711 -wd4710 -wd4101 -wd4296 -wd4311 -wd4115 -wd4702 -wd4456 -wd4555
set flags=-nologo -Zi -FC -W1 %warnings_to_ignore%
set libs=user32.lib dxgi.lib d3d11.lib D3DCompiler.lib dxguid.lib

REM Compile shaders.
rem fxc /E vsmain /Fh ..\include\shaders\vs_shader.h /T vs_5_0 ..\assets\vertex_shader.hlsl

REM Compile engine.
cl %sources% %flags% /c /I ../include /D "_CRT_SECURE_NO_DEPRECATE=1" 
lib *.obj /OUT:d3d11_motor.lib /NOLOGO
rem link *.obj %libs% /DLL /OUT:d3d11_motor.dll /DEBUG /NOLOGO

REM Compile app.
cl -Zi /Fedemo.exe -nologo -O2 -FC -W1 ../app/main.c /I ../include /link d3d11_motor.lib %libs% /SUBSYSTEM:CONSOLE

REM create symlink to assets.
rem mklink /D assets ..\app\assets

del lock.tmp
del *.obj

popd