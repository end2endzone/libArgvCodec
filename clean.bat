@echo off
rmdir /q/s .\Debug
rmdir /q/s .\Release
del .\libArgvCodec\*.user
del .\llibArgvCodecTest\*.user
rmdir /q/s .\libArgvCodec\Debug
rmdir /q/s .\libArgvCodec\Release
rmdir /q/s .\libArgvCodecTest\Debug
rmdir /q/s .\libArgvCodecTest\Release
del *.ncb
del *.sdf
del /ah *.suo
pause
