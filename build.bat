@echo off
if not exist build mkdir build
pushd build

REM Process flags.
set /A flag_s=0
set /A flag_e=0
set /A flag_g=0
set /A flag_c=0
FOR %%A IN (%*) DO (
    IF "%%A"=="/s" SET /A flag_s=1
    IF "%%A"=="/e" SET /A flag_e=1
    IF "%%A"=="/g" SET /A flag_g=1
    IF "%%A"=="/c" SET /A flag_c=1
)

REM Compile shaders.
IF %flag_s%==1 (
    echo Compiling shaders...
    REM Mesh shader.
    fxc /E vsmain /Fh ..\include\shaders\mesh_vs.h /Vn g_mesh_vs /T vs_5_0 ..\assets\mesh_shader.hlsl /nologo
    fxc /E psmain /Fh ..\include\shaders\mesh_ps.h /Vn g_mesh_ps /T ps_5_0 ..\assets\mesh_shader.hlsl /nologo

    REM Grid shader.
    fxc /E vsmain /Fh ..\include\shaders\grid_vs.h /Vn g_grid_vs /T vs_5_0 ..\assets\grid_shader.hlsl /nologo
    fxc /E psmain /Fh ..\include\shaders\grid_ps.h /Vn g_grid_ps /T ps_5_0 ..\assets\grid_shader.hlsl /nologo

    REM Skybox shader.
    fxc /E vsmain /Fh ..\include\shaders\skybox_vs.h /Vn g_skybox_vs /T vs_5_0 ..\assets\skybox_shader.hlsl /nologo
    fxc /E psmain /Fh ..\include\shaders\skybox_ps.h /Vn g_skybox_ps /T ps_5_0 ..\assets\skybox_shader.hlsl /nologo
)


REM Compile engine.
set engine_sources=..\src\*.c ..\src\utils\*.c ..\src\platform\*.c ..\src\renderer\*.c ..\src\math\*.c ..\src\assets\*.c ..\src\entities\*.c
set engine_warnings_to_ignore=/wd4201 /wd4116 /wd4101 /wd4103 /wd4081 /wd4204 /wd4255 /wd4668 /wd4820 /wd4100 /wd4189 /wd4711 /wd4710 /wd4101 /wd4296 /wd4311 /wd4115 /wd4702 /wd4456 /wd4555
set engine_flags=/c /EHsc /nologo /Zi /FC /W1 %engine_warnings_to_ignore% 
set engine_libs=user32.lib dxgi.lib d3d11.lib D3DCompiler.lib dxguid.lib
set engine_out=d3d11_motor
IF %flag_e%==1 (
    echo Building the engine...
    cl %engine_sources% %engine_flags% /I ../include /D "_CRT_SECURE_NO_DEPRECATE=1" 
    lib *.obj %engine_libs% /OUT:%engine_out%.lib /NOLOGO /IGNORE:4006
    rem link *.obj %engine_libs% /DLL /OUT:%engine_out%.dll /DEBUG /NOLOGO
)

REM Compile game.
set game_sources=..\demo\solar\main.c
set game_flags=/Zi /nologo /O2 /FC /W1
set game_libs=d3d11_motor.lib
set out=solar
IF %flag_g%==1 (
    echo Building the game...
    cl %game_sources% %game_flags% /Fe%out%.exe /I ../include /link %game_libs% /SUBSYSTEM:CONSOLE

    REM create symlink to assets.
    IF NOT EXIST assets  (
        mklink /D assets ..\demo\solar\assets
    )
)


REM Clean up.
IF %flag_c%==1 (
    echo Clean up build folder...
    del *.exe > NUL 2> NUL
    del *.lib > NUL 2> NUL
    del *.dll > NUL 2> NUL
    del *.pdb > NUL 2> NUL
    del *.obj > NUL 2> NUL
    del *.ilk > NUL 2> NUL
    rmdir assets > NUL 2> NUL
)

popd