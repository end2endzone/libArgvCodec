@echo off
rmdir /q/s .\Debug
rmdir /q/s .\Release
del .\libargvcodec\*.user
del .\libargvcodectest\*.user
rmdir /q/s .\libargvcodec\Debug
rmdir /q/s .\libargvcodec\Release
rmdir /q/s .\libargvcodectest\Debug
rmdir /q/s .\libargvcodectest\Release
del *.ncb
del *.sdf
del /ah *.suo
