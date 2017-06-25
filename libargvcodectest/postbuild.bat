@echo off
:: %1 is $(ProjectDir)
:: %2 is $(OutDir)
::

copy /v "%~1TestEncodeCommandLine*" "%~2"
