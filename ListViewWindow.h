// ListViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define LIST_VIEW_WINDOW_CLASS_NAME												WC_LISTVIEW

#define LIST_VIEW_WINDOW_EXTENDED_STYLE											LVS_EX_FULLROWSELECT
#define LIST_VIEW_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT )
#define LIST_VIEW_WINDOW_TEXT													NULL

#define LIST_VIEW_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING					"%s (%d items )"

#define LIST_VIEW_WINDOW_UNABLE_TO_SAVE_WARNING_MESSAGE							"Unable to save.\r\n\r\nDo you want to close?"

#define LIST_VIEW_WINDOW_COLUMN_TITLES											{ "First" }

typedef enum
{
	LIST_VIEW_WINDOW_FIRST_COLUMN_ID = 0,

	LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS

} LIST_VIEW_WINDOW_COLUMNS;

BOOL IsListViewWindow( HWND hWnd );

int ListViewWindowAddItem( LPCTSTR lpszItem, DWORD dwMaximumTextLength = STRING_LENGTH );

int ListViewWindowAutoSizeAllColumns();

int CALLBACK ListViewWindowCompare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamColumn );

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL ListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText, DWORD dwMaximumTextLength = STRING_LENGTH );

BOOL ListViewWindowGetRect( LPRECT lpRect );

BOOL ListViewWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

BOOL ListViewWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int ListViewWindowLoad( LPCTSTR lpszFileName );

int ListViewWindowPopulate( LPCTSTR lpszFileName );

int ListViewWindowSave( LPCTSTR lpszFileName );

HWND ListViewWindowSetFocus();

BOOL ListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText, DWORD dwMaximumTextLength = STRING_LENGTH );

void ListViewWindowSetFont( HFONT hFont );
