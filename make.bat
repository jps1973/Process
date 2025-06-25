cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra ^
 ^
 Process.cpp ^
 ^
 ListViewWindow.cpp ^
 StatusBarWindow.cpp ^
 ^
 Resource.o ^
 ^
 -o Process.exe
