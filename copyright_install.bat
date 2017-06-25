@echo off

call :INSERT %~dp0ArgEncoder\main.cpp
call :INSERT %~dp0ArgEncoder\targetver.h

call :INSERT %~dp0libArgvCodec\ArgumentList.cpp
call :INSERT %~dp0libArgvCodec\ArgumentList.h
call :INSERT %~dp0libArgvCodec\CmdPromptArgumentCodec.cpp
call :INSERT %~dp0libArgvCodec\CmdPromptArgumentCodec.h
call :INSERT %~dp0libArgvCodec\CreateProcessArgumentCodec.cpp
call :INSERT %~dp0libArgvCodec\CreateProcessArgumentCodec.h
call :INSERT %~dp0libArgvCodec\dllmain.cpp
call :INSERT %~dp0libArgvCodec\IArgumentDecoder.h
call :INSERT %~dp0libArgvCodec\IArgumentEncoder.h
call :INSERT %~dp0libArgvCodec\libArgvCodec.h
call :INSERT %~dp0libArgvCodec\macros.h
call :INSERT %~dp0libArgvCodec\os.cpp
call :INSERT %~dp0libArgvCodec\os.h
call :INSERT %~dp0libArgvCodec\targetver.h
call :INSERT %~dp0libArgvCodec\utils.cpp
call :INSERT %~dp0libArgvCodec\utils.h

call :INSERT %~dp0libArgvCodecTest\ArgumentLister.cpp
call :INSERT %~dp0libArgvCodecTest\ArgumentLister.h
call :INSERT %~dp0libArgvCodecTest\gtesthelper.cpp
call :INSERT %~dp0libArgvCodecTest\gtesthelper.h
call :INSERT %~dp0libArgvCodecTest\main.cpp
call :INSERT %~dp0libArgvCodecTest\targetver.h
call :INSERT %~dp0libArgvCodecTest\TestArgumentList.cpp
call :INSERT %~dp0libArgvCodecTest\TestArgumentList.h
call :INSERT %~dp0libArgvCodecTest\TestCmdPromptArgumentCodec.cpp
call :INSERT %~dp0libArgvCodecTest\TestCmdPromptArgumentCodec.h
call :INSERT %~dp0libArgvCodecTest\TestCreateProcessArgumentCodec.cpp
call :INSERT %~dp0libArgvCodecTest\TestCreateProcessArgumentCodec.h
call :INSERT %~dp0libArgvCodecTest\TestUtils.cpp
call :INSERT %~dp0libArgvCodecTest\TestUtils.h
call :INSERT %~dp0libArgvCodecTest\TestValidator.cpp
call :INSERT %~dp0libArgvCodecTest\TestValidator.h

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
