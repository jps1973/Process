cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra Process.cpp DetailsListViewWindow.cpp RunningListViewWindow.cpp StatusBarWindow.cpp Resource.o -o Process.exe -lwtsapi32
