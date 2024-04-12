// RunningListViewWindow.cpp

#include "RunningListViewWindow.h"

// Global variables
HWND g_hWndListView;
LPTSTR g_lpszParentFolderPath;

BOOL IsRunningListViewWindow( HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndListView );

} // End of function IsRunningListViewWindow

int RunningListViewWindowAddItem( LPCTSTR lpszItemText )
{
	int nResult;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );
	lvItem.iSubItem		= RUNNING_LIST_VIEW_WINDOW_NAME_COLUMN_ID;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Add item to list view window
	nResult = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	return nResult;

} // End of function RunningListViewWindowAddItem

int RunningListViewWindowAutoSizeAllColumns()
{
	int nResult = 0;

	int nWhichColumn;

	// Loop through columns
	for( nWhichColumn = 0; nWhichColumn < RUNNING_LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
	{
		// Auto-size current column
		if( SendMessage( g_hWndListView, LVM_SETCOLUMNWIDTH,( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER ) )
		{
			// Successfully auto-sized current column

			// Update return value
			nResult ++;

		} // End of successfully auto-sized current column
		else
		{
			// Unable to auto-size current column

			// Force exit from loop
			nWhichColumn = RUNNING_LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS;

		} // End of unable to auto-size current column

	} // End of loop through columns

	return nResult;

} // End of function RunningListViewWindowAutoSizeAllColumns

int CALLBACK RunningListViewWindowCompareProcedure( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	int nResult;

	LVITEM lvItem;

	// Allocate string memory
	LPTSTR lpszBuffer1 = new char[ STRING_LENGTH ];
	LPTSTR lpszBuffer2 = new char[ STRING_LENGTH ];

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.iSubItem		= lParamSort;
	lvItem.cchTextMax	= STRING_LENGTH;

	// Update list view item structure for first item
	lvItem.iItem	= lParam1;
	lvItem.pszText	= lpszBuffer1;

	// Get first item text
	SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	// Update list view item structure for second item
	lvItem.iItem	= lParam2;
	lvItem.pszText	= lpszBuffer2;

	// Get second item text
	SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	// Update return value
	nResult = lstrcmp( lpszBuffer1, lpszBuffer2 );

	// Free string memory
	delete [] lpszBuffer1;
	delete [] lpszBuffer2;

	return nResult;

} // End of function RunningListViewWindowCompareProcedure

BOOL RunningListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndListView = ::CreateWindowEx( RUNNING_LIST_VIEW_WINDOW_EXTENDED_STYLE, RUNNING_LIST_VIEW_WINDOW_CLASS_NAME, RUNNING_LIST_VIEW_WINDOW_TEXT, RUNNING_LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndListView )
	{
		// Successfully created list view window
		LVCOLUMN lvColumn;
		LPCTSTR lpszColumnTitles[] = RUNNING_LIST_VIEW_WINDOW_COLUMN_TITLES;

		// Clear list view column structure
		ZeroMemory( &lvColumn, sizeof( lvColumn ) );

		// Initialise list view column structure
		lvColumn.mask	= ( LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM );
		lvColumn.cx		= RUNNING_LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH;

		// Set extended list view window style
		SendMessage( g_hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )0, ( LPARAM )RUNNING_LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Insert columns
		for( lvColumn.iSubItem = 0; lvColumn.iSubItem < RUNNING_LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; lvColumn.iSubItem ++ )
		{
			// Update list view column structure for column
			lvColumn.pszText = ( LPTSTR )lpszColumnTitles[ lvColumn.iSubItem ];

			// Insert column
			SendMessage( g_hWndListView, LVM_INSERTCOLUMN, ( WPARAM )lvColumn.iSubItem, ( LPARAM )&lvColumn );

		}; // End of loop to insert columns

		// Allocate global string memory
		g_lpszParentFolderPath = new char[ STRING_LENGTH ];

		// Clear global parent folder path
		g_lpszParentFolderPath[ 0 ] = ( char )NULL;

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window
	return bResult;

} // End of function RunningListViewWindowCreate

int RunningListViewWindowGetCurrentSelection()
{
	// Get current selection item
	return ::SendMessage( g_hWndListView, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

} // End of function RunningListViewWindowGetCurrentSelection

BOOL RunningListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText )
{
	BOOL bResult;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= lpszItemText;

	// Get item text
	bResult = SendMessage( g_hWndListView, LVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function RunningListViewWindowGetItemText

BOOL RunningListViewWindowGetRect( LPRECT lpRect )
{
	// Get list view window rect
	return ::GetWindowRect( g_hWndListView, lpRect );

} // End of function RunningListViewWindowGetRect

BOOL RunningListViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpItemSelectedFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMHDR lpNmHdr;

	// Get notify message handler
	lpNmHdr = ( LPNMHDR )lParam;

	// Select notify message
	switch( lpNmHdr->code )
	{
		case LVN_COLUMNCLICK:
		{
			// A list view window column click notification code
			LPNMLISTVIEW lpNmListView;

			// Get list view notify message handler
			lpNmListView = ( LPNMLISTVIEW )lpNmHdr;

			// Sort the list view
			::SendMessage( g_hWndListView, LVM_SORTITEMSEX, ( WPARAM )( lpNmListView->iSubItem ), ( LPARAM )&RunningListViewWindowCompareProcedure );

			// Update return value
			bResult = TRUE;

			// Break out of switch
			break;

		} // End of a list view window column click notification code
		case NM_DBLCLK:
		{
			// A double-click notify message
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszItemText = new char[ STRING_LENGTH ];

			// Get selected item
			nSelectedItem = SendMessage( g_hWndListView, LVM_GETNEXTITEM, ( WPARAM )-1, ( LPARAM )LVNI_FOCUSED );

			// Get selected item text
			if( RunningListViewWindowGetItemText( nSelectedItem, RUNNING_LIST_VIEW_WINDOW_NAME_COLUMN_ID, lpszItemText ) )
			{
				// Successfully got selected item text

				// Call double click function
				( *lpDoubleClickFunction )( lpszItemText );

				// Update return value
				bResult = TRUE;

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszItemText;

			// Break out of switch
			break;

		} // End of a double-click notify message
		case LVN_ITEMCHANGED:
		{
			// A list view item changed notify message
			LPNMLISTVIEW lpNmListView;

			// Get list view notify message handler
			lpNmListView = ( LPNMLISTVIEW )lParam;

			// See if item is now selected
			if( ( lpNmListView->uNewState ^ lpNmListView->uOldState ) & LVIS_SELECTED )
			{
				// Item is now selected

				// Allocate string memory
				LPTSTR lpszItemText = new char[ STRING_LENGTH ];

				// Get selected item text
				if( RunningListViewWindowGetItemText( lpNmListView->iItem, RUNNING_LIST_VIEW_WINDOW_NAME_COLUMN_ID, lpszItemText ) )
				{
					// Successfully got selected item text

					// Call item selected function
					( *lpItemSelectedFunction )( lpszItemText );

					// Update return value
					bResult = TRUE;

				} // End of successfully got selected item text

				// Free string memory
				delete [] lpszItemText;

			} // End of item is now selected

		} // End of a list view item changed notify message

	}; // End of selection for notify message

	return bResult;

} // End of function RunningListViewWindowHandleNotifyMessage

BOOL RunningListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list view window
	return ::MoveWindow( g_hWndListView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function RunningListViewWindowMove

HWND RunningListViewWindowSetFocus()
{
	// Focus on list view window
	return ::SetFocus( g_hWndListView );

} // End of function RunningListViewWindowSetFocus

void RunningListViewWindowSetFont( HFONT hFont )
{
	// Set list view window font
	::SendMessage( g_hWndListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function RunningListViewWindowSetFont

BOOL RunningListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText )
{
	BOOL bResult;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Set item text
	bResult = SendMessage( g_hWndListView, LVM_SETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function RunningListViewWindowSetItemText