@echo off
rem #### Run quick test of NomJSON.

if exist err del err
if exist __out*.out del __out*.out

echo Running tests.  This may take a minute.

bin\nomjsontest.exe ref\epsg_io_json_output.txt >> err
if errorlevel 1 goto fail

if exist err type err
echo Done.
exit /b 0

:fail
if exist type err
echo Completed with error(s).
exit /b 1

