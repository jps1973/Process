// DetailsListViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define DETAILS_LIST_VIEW_WINDOW_CLASS_NAME										WC_LISTVIEW

#define DETAILS_LIST_VIEW_WINDOW_EXTENDED_STYLE									LVS_EX_FULLROWSELECT
#define DETAILS_LIST_VIEW_WINDOW_STYLE											( WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | LVS_REPORT )
#define DETAILS_LIST_VIEW_WINDOW_TEXT											NULL

#define DETAILS_LIST_VIEW_WINDOW_HEIGHT											100

#define DETAILS_LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH							100
#define DETAILS_LIST_VIEW_WINDOW_COLUMN_TITLES									{ "Name", "Details" }

typedef enum DetailsListViewWindowColumns
{
	DETAILS_LIST_VIEW_WINDOW_NAME_COLUMN_ID = 0,
	DETAILS_LIST_VIEW_WINDOW_DETAILS_COLUMN_ID,

	DETAILS_LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS

} DETAILS_LIST_VIEW_WINDOW_COLUMNS;

BOOL IsDetailsListViewWindow( HWND hWnd );

int DetailsListViewWindowAddItem( LPCTSTR lpszItemText );

int DetailsListViewWindowAutoSizeAllColumns();

int CALLBACK DetailsListViewWindowCompareProcedure( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort );

BOOL DetailsListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

int DetailsListViewWindowGetCurrentSelection();

BOOL DetailsListViewWindowGetItemPath( int nWhichItem, LPTSTR lpszItemPath );

BOOL DetailsListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText );

BOOL DetailsListViewWindowGetRect( LPRECT lpRect );

BOOL DetailsListViewWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpItemSelectedFunction )( LPCTSTR lpszItemText ) );

BOOL DetailsListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

HWND DetailsListViewWindowSetFocus();

void DetailsListViewWindowSetFont( HFONT hFont );

BOOL DetailsListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText );
