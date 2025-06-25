cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra ^
 ^
 ListView.cpp ^
 ^
 ListViewWindow.cpp ^
 StatusBarWindow.cpp ^
 ^
 Resource.o ^
 ^
 -o ListView.exe
