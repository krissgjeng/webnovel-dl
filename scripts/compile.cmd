SETLOCAL ENABLEDELAYEDEXPANSION
::need to be 64bit
SET Commandc=gcc.exe
SET Commandcpp=clang++.exe
SET MYLIBF=-Llib/libtidy -Llib/libcurl
SET MYLIBS=-ltidy -lcurl
SET MYINCLUDES=-Ilib\libtidy\include -Ilib\libcurl\include

SET "files="

::mkdir build

for %%f in (*.cpp) do (
	SET files=!files! %%f
)
::echo %files%
%Commandcpp% -std=c++17 %files% %MYINCLUDES% %MYLIBF% %MYLIBS% -o main.exe
move main.exe build\
copy lib\libtidy\tidy.dll build\
copy lib\libcurl\libcurl.dll build\
copy lib\libcurl\libcrypto-1_1-x64.dll build\
copy lib\libcurl\libssl-1_1-x64.dll build\
copy lib\libcurl\cacert.pem build\