set FEDRA_ROOT=%~dp0%
path %FEDRA_ROOT%\bin;%FEDRA_ROOT%\lib;%PATH%
win32\tools\setenv.exe  -u FEDRA_ROOT %~dp0%
win32\tools\pathman.exe /au %%FEDRA_ROOT%%\bin
win32\tools\pathman.exe /au %%FEDRA_ROOT%%\lib

if DEFINED FEDRA win32\tools\pathman /ru %%FEDRA%%\bin
if DEFINED FEDRA win32\tools\pathman /ru %%FEDRA%%\lib
if DEFINED FEDRA win32\tools\setenv.exe  -u FEDRA -delete

if NOT DEFINED HOME copy src\appl\macros\fedra.rootrc %%HOMEDRIVE%%%%HOMEPATH%%\.rootrc
if NOT DEFINED HOME win32\tools\setenv.exe  -u HOME %%HOMEDRIVE%%%%HOMEPATH%%

