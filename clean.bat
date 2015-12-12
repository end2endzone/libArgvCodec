@echo off
rmdir /q/s .\Debug
rmdir /q/s .\Release
rmdir /q/s .\ArgumentManager\Debug
rmdir /q/s .\ArgumentManager\Release
rmdir /q/s .\ArgumentManagerTest\Debug
rmdir /q/s .\ArgumentManagerTest\Release
del *.ncb
del *.suo
