// ListBoxWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>
#include <tlhelp32.h>

#include "Ascii.h"
#include "Common.h"

#define LIST_BOX_WINDOW_CLASS_NAME												WC_LISTBOX

#define LIST_BOX_WINDOW_EXTENDED_STYLE											0
#define LIST_BOX_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | LBS_SORT )
#define LIST_BOX_WINDOW_TEXT													NULL

#define LIST_BOX_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING					"%d processes"

#define LIST_BOX_WINDOW_UNABLE_TO_GET_FIRST_PROCESS_ENTRY_ERROR_MESSAGE			"Unable to get first process entry"
#define LIST_BOX_WINDOW_UNABLE_TO_TAKE_SNAPSHOT_OF_ALL_PROCESSES_ERROR_MESSAGE	"Unable to take snapshot of all processes"

BOOL IsListBoxWindow( HWND hWnd );

int ListBoxWindowAddString( LPCTSTR lpszString );

BOOL ListBoxWindowCreate( HWND hWndParent, HINSTANCE hInstance );

int ListBoxWindowGetCurrentSelection();

int ListBoxWindowGetItemText( int nWhichItem, LPTSTR lpszItemText );

BOOL ListBoxWindowGetRect( LPRECT lpRect );

BOOL ListBoxWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) );

BOOL ListBoxWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int ListBoxWindowPopulate( HWND hWndMain );

HWND ListBoxWindowSetFocus();

void ListBoxWindowSetFont( HFONT hFont );

