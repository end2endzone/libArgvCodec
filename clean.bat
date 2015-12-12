@echo off
rmdir /q/s .\Debug
rmdir /q/s .\Release
del .\ArgumentLister\*.user
del .\ArgumentManager\*.user
del .\ArgumentManagerTest\*.user
rmdir /q/s .\ArgumentLister\Debug
rmdir /q/s .\ArgumentLister\Release
rmdir /q/s .\ArgumentManager\Debug
rmdir /q/s .\ArgumentManager\Release
rmdir /q/s .\ArgumentManagerTest\Debug
rmdir /q/s .\ArgumentManagerTest\Release
del *.ncb
del /ah *.suo
