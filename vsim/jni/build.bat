@echo off
call %cd%\clean_obj.bat
call %cd%\clean_log.bat
call %cd%\clean.bat
call %cd%\compile.bat
pause