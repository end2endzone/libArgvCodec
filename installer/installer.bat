@echo off
REM %1 is OutDir
REM %2 is SolutionDir
REM %3 is ConfigurationName
REM %4 is PlatformName
REM %5 is PlatformToolset

REM building runtime_library...
set RUNTIME_LIBRARY=MD
if (%3) == (Debug) set RUNTIME_LIBRARY=%RUNTIME_LIBRARY%d

set _PLATFORMNAME=%4
if (%4) == () set _PLATFORMNAME=x86

set _TOOLSET=%5
if (%5) == () set _TOOLSET=v90

echo =============================================
echo Installer package properties:
echo =============================================
echo.   outdir:      %1
echo.   solutiondir: %2
echo.   config:      %3
echo.   platform:    %_PLATFORMNAME%
echo.   toolset:     %_TOOLSET%
echo.   runtime:     %RUNTIME_LIBRARY%

REM Building version information...
set MAIN_VERSION=1.0
svn info %2 | findstr /C:"Last Changed Rev:">%TEMP%\installer.subversion.revision.txt
set /p SVN_REVISION=<%TEMP%\installer.subversion.revision.txt
del %TEMP%\installer.subversion.revision.txt
set SVN_REVISION=%SVN_REVISION:Last Changed Rev: =%
echo.   version:     %MAIN_VERSION%.%SVN_REVISION%

REM building output file...
set OUTPUT_FILE=%1\libargvcodec %_PLATFORMNAME% v%MAIN_VERSION%.%SVN_REVISION% portable.zip
echo.   output file: %OUTPUT_FILE%
echo done
echo.

echo =============================================
echo Building output structure...
echo =============================================
rmdir /Q /S ""%1\package" >NUL 2>NUL
mkdir "%1\package" >NUL 2>NUL

mkdir "%1\package\tests" >NUL 2>NUL
del /Q "%1\package\tests\*.*" >NUL 2>NUL
copy /v "%1\libargvcodectest.exe"                      "%1\package\tests"
copy /v "%1\libargvcodec.dll"                          "%1\package\tests"
copy /v "%1\shellCommandLines.txt"                     "%1\package\tests"
copy /v "%1\TestEncodeCommandLine001.txt"              "%1\package\tests"
copy /v "%1\TestEncodeCommandLine002.txt"              "%1\package\tests"

mkdir "%1\package\include" >NUL 2>NUL
del /Q "%1\package\include\*.*" >NUL 2>NUL
copy /v "%2\libargvcodec\ArgumentList.h"               "%1\package\include"
copy /v "%2\libargvcodec\CmdPromptArgumentCodec.h"     "%1\package\include"
copy /v "%2\libargvcodec\CreateProcessArgumentCodec.h" "%1\package\include"
copy /v "%2\libargvcodec\IArgumentDecoder.h"           "%1\package\include"
copy /v "%2\libargvcodec\IArgumentEncoder.h"           "%1\package\include"
copy /v "%2\libargvcodec\libargvcodec.h"               "%1\package\include"
copy /v "%2\libargvcodec\targetver.h"                  "%1\package\include"

mkdir "%1\package\lib" >NUL 2>NUL
del /Q "%1\package\lib\*.*" >NUL 2>NUL
copy /v "%1\libargvcodec.lib"                           "%1\package\lib\libargvcodec_%_TOOLSET%_%_PLATFORMNAME%_%RUNTIME_LIBRARY%.lib"

mkdir "%1\package\bin" >NUL 2>NUL
del /Q "%1\package\bin\*.*" >NUL 2>NUL
copy /v "%1\argencoder.exe"                             "%1\package\bin"
copy /v "%1\showargs.exe"                               "%1\package\bin"
copy /v "%1\libargvcodec.dll"                           "%1\package\bin"
echo done
echo.

echo =============================================
echo Building installer package...
echo =============================================
del /Q "%OUTPUT_FILE%" >NUL 2>NUL
"C:\Program Files\7-Zip\7z.exe" a -tzip "%OUTPUT_FILE%" "%1\package\tests" "%1\package\include" "%1\package\lib" "%1\package\bin"
echo done
echo.

rmdir /Q /S "%1\package" >NUL 2>NUL
