SETLOCAL ENABLEDELAYEDEXPANSION
::need to be 64bit
SET Commandc=gcc.exe
SET Commandcpp=clang++.exe
SET MYLIBF=lib/libtidy
SET MYLIBS=tidy
SET MYINCLUDES=%cd%\lib\libtidy\include

SET "files="

::mkdir build

for %%f in (*.cpp) do (
	SET files=!files! %%f
)
::echo %files%
%Commandcpp% -std=c++17 %files% -I%MYINCLUDES% -L%MYLIBF% -l%MYLIBS% -o main.exe
move main.exe build\
copy lib\libtidy\tidy.dll build\