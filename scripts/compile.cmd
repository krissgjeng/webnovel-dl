SETLOCAL ENABLEDELAYEDEXPANSION
::need to be 64bit
SET Commandc=gcc.exe
SET Commandcpp=clang++.exe
SET MYLIBF=-Llib/libtidy -Llib/libcurl -Llib/libzip
SET MYLIBS=-ltidy -lcurl -llibzip
SET MYINCLUDES=-Ilib\libtidy\include -Ilib\libcurl\include -Ilib\libzip\lib

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
copy lib\libzip\libzip.dll build\