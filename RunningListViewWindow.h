// RunningListViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>
#include <wtsapi32.h>

#include "Ascii.h"
#include "Common.h"

#define RUNNING_LIST_VIEW_WINDOW_CLASS_NAME										WC_LISTVIEW

#define RUNNING_LIST_VIEW_WINDOW_EXTENDED_STYLE									LVS_EX_FULLROWSELECT
#define RUNNING_LIST_VIEW_WINDOW_STYLE											( WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | LVS_REPORT )
#define RUNNING_LIST_VIEW_WINDOW_TEXT											NULL

#define RUNNING_LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH							100
#define RUNNING_LIST_VIEW_WINDOW_COLUMN_TITLES									{ "Name", "ID" }

#define RUNNING_LIST_VIEW_WINDOW_SYSTEM_IDLE_PROCESS_ID							0
#define RUNNING_LIST_VIEW_WINDOW_SYSTEM_IDLE_PROCESS_NAME						"System Idle Process"

#define RUNNING_LIST_VIEW_WINDOW_PROCESS_ID_FORMAT_STRING						"%08lu"

#define RUNNING_LIST_VIEW_WINDOW_UNABBLE_TO_ENUMERATE_PROCESSES_ERROR_MESSAGE	"Unable to enumerate processes"

typedef enum RunningListViewWindowColumns
{
	RUNNING_LIST_VIEW_WINDOW_NAME_COLUMN_ID = 0,
	RUNNING_LIST_VIEW_WINDOW_ID_COLUMN_ID,

	RUNNING_LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS

} RUNNING_LIST_VIEW_WINDOW_COLUMNS;

BOOL IsRunningListViewWindow( HWND hWnd );

int RunningListViewWindowAddItem( LPCTSTR lpszItemText );

int RunningListViewWindowAutoSizeAllColumns();

int CALLBACK RunningListViewWindowCompareProcedure( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort );

BOOL RunningListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

int RunningListViewWindowGetCurrentSelection();

BOOL RunningListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText );

BOOL RunningListViewWindowGetRect( LPRECT lpRect );

BOOL RunningListViewWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpItemSelectedFunction )( LPCTSTR lpszItemText ) );

BOOL RunningListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int RunningListViewWindowPopulate();

HWND RunningListViewWindowSetFocus();

void RunningListViewWindowSetFont( HFONT hFont );

BOOL RunningListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText );
