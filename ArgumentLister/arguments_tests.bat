@echo off

set CONFIGURATION=Debug
set ARGLISTER_LOCATION=..\%CONFIGURATION%\ArgumentLister.exe
prompt $G 

@echo on

@echo Test #1
%ARGLISTER_LOCATION% a b c
@echo.

@echo Test #2
%ARGLISTER_LOCATION% a b c 
@echo.

@echo Test #3
%ARGLISTER_LOCATION% "a b" c
@echo.

@echo Test #4
%ARGLISTER_LOCATION% "a b c"
@echo.

@echo Test #5
%ARGLISTER_LOCATION% "a b c
@echo.

@echo Test #6
%ARGLISTER_LOCATION% a b c"
@echo.

@echo Test #7
%ARGLISTER_LOCATION% "a b" c"
@echo.

@echo Test #8
%ARGLISTER_LOCATION% "a b\" c" d
@echo.

@echo Test #9
%ARGLISTER_LOCATION% "a b\" c d
@echo.

@echo Test #10
%ARGLISTER_LOCATION% "a b\\" c d
@echo.

@echo Test #11
%ARGLISTER_LOCATION% a b\" c d
@echo.

@echo Test #12
%ARGLISTER_LOCATION% a \t c
@echo.

@echo Test #13
%ARGLISTER_LOCATION% a \\b c
@echo.

pause
