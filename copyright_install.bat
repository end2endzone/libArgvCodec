@echo off

call :INSERT %~dp0ArgEncoder\main.cpp
call :INSERT %~dp0ArgEncoder\targetver.h

call :INSERT %~dp0libargvcodec\ArgumentList.cpp
call :INSERT %~dp0libargvcodec\ArgumentList.h
call :INSERT %~dp0libargvcodec\CmdPromptArgumentCodec.cpp
call :INSERT %~dp0libargvcodec\CmdPromptArgumentCodec.h
call :INSERT %~dp0libargvcodec\CreateProcessArgumentCodec.cpp
call :INSERT %~dp0libargvcodec\CreateProcessArgumentCodec.h
call :INSERT %~dp0libargvcodec\dllmain.cpp
call :INSERT %~dp0libargvcodec\IArgumentDecoder.h
call :INSERT %~dp0libargvcodec\IArgumentEncoder.h
call :INSERT %~dp0libargvcodec\libargvcodec.h
call :INSERT %~dp0libargvcodec\macros.h
call :INSERT %~dp0libargvcodec\os.cpp
call :INSERT %~dp0libargvcodec\os.h
call :INSERT %~dp0libargvcodec\targetver.h
call :INSERT %~dp0libargvcodec\utils.cpp
call :INSERT %~dp0libargvcodec\utils.h

call :INSERT %~dp0libargvcodectest\ArgumentLister.cpp
call :INSERT %~dp0libargvcodectest\ArgumentLister.h
call :INSERT %~dp0libargvcodectest\gtesthelper.cpp
call :INSERT %~dp0libargvcodectest\gtesthelper.h
call :INSERT %~dp0libargvcodectest\main.cpp
call :INSERT %~dp0libargvcodectest\targetver.h
call :INSERT %~dp0libargvcodectest\TestArgumentList.cpp
call :INSERT %~dp0libargvcodectest\TestArgumentList.h
call :INSERT %~dp0libargvcodectest\TestCmdPromptArgumentCodec.cpp
call :INSERT %~dp0libargvcodectest\TestCmdPromptArgumentCodec.h
call :INSERT %~dp0libargvcodectest\TestCreateProcessArgumentCodec.cpp
call :INSERT %~dp0libargvcodectest\TestCreateProcessArgumentCodec.h
call :INSERT %~dp0libargvcodectest\TestUtils.cpp
call :INSERT %~dp0libargvcodectest\TestUtils.h
call :INSERT %~dp0libargvcodectest\TestValidator.cpp
call :INSERT %~dp0libargvcodectest\TestValidator.h

call :INSERT %~dp0libPromptConsole\libPromptConsole.cpp
call :INSERT %~dp0libPromptConsole\libPromptConsole.h

call :INSERT %~dp0ShowArgs\ShowArgs.cpp
call :INSERT %~dp0ShowArgs\targetver.h

pause


GOTO :EOF


:INSERT
REM %1 is the file path to add the header
echo Processing file %1...
move /y "%~1" "%~1.tmp" >NUL
type %~dp0copyright.txt>"%~1"
type "%~1.tmp">>"%~1"
del "%~1.tmp" >NUL

:EOF
