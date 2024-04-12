// DetailsListViewWindow.cpp

#include "DetailsListViewWindow.h"

// Global variables
static HWND g_hWndDetailsListView;

BOOL IsDetailsListViewWindow( HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndDetailsListView );

} // End of function IsDetailsListViewWindow

int DetailsListViewWindowAddItem( LPCTSTR lpszItemText )
{
	int nResult;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= SendMessage( g_hWndDetailsListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );
	lvItem.iSubItem		= DETAILS_LIST_VIEW_WINDOW_NAME_COLUMN_ID;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Add item to list view window
	nResult = SendMessage( g_hWndDetailsListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	return nResult;

} // End of function DetailsListViewWindowAddItem

int DetailsListViewWindowAutoSizeAllColumns()
{
	int nResult = 0;

	int nWhichColumn;

	// Loop through columns
	for( nWhichColumn = 0; nWhichColumn < DETAILS_LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
	{
		// Auto-size current column
		if( SendMessage( g_hWndDetailsListView, LVM_SETCOLUMNWIDTH,( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER ) )
		{
			// Successfully auto-sized current column

			// Update return value
			nResult ++;

		} // End of successfully auto-sized current column
		else
		{
			// Unable to auto-size current column

			// Force exit from loop
			nWhichColumn = DETAILS_LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS;

		} // End of unable to auto-size current column

	} // End of loop through columns

	return nResult;

} // End of function DetailsListViewWindowAutoSizeAllColumns

int CALLBACK DetailsListViewWindowCompareProcedure( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
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
	SendMessage( g_hWndDetailsListView, LVM_GETITEMTEXT, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	// Update list view item structure for second item
	lvItem.iItem	= lParam2;
	lvItem.pszText	= lpszBuffer2;

	// Get second item text
	SendMessage( g_hWndDetailsListView, LVM_GETITEMTEXT, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	// Update return value
	nResult = lstrcmp( lpszBuffer1, lpszBuffer2 );

	// Free string memory
	delete [] lpszBuffer1;
	delete [] lpszBuffer2;

	return nResult;

} // End of function DetailsListViewWindowCompareProcedure

BOOL DetailsListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndDetailsListView = ::CreateWindowEx( DETAILS_LIST_VIEW_WINDOW_EXTENDED_STYLE, DETAILS_LIST_VIEW_WINDOW_CLASS_NAME, DETAILS_LIST_VIEW_WINDOW_TEXT, DETAILS_LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndDetailsListView )
	{
		// Successfully created list view window
		LVCOLUMN lvColumn;
		LPCTSTR lpszColumnTitles[] = DETAILS_LIST_VIEW_WINDOW_COLUMN_TITLES;

		// Clear list view column structure
		ZeroMemory( &lvColumn, sizeof( lvColumn ) );

		// Initialise list view column structure
		lvColumn.mask	= ( LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM );
		lvColumn.cx		= DETAILS_LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH;

		// Set extended list view window style
		SendMessage( g_hWndDetailsListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )0, ( LPARAM )DETAILS_LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Insert columns
		for( lvColumn.iSubItem = 0; lvColumn.iSubItem < DETAILS_LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; lvColumn.iSubItem ++ )
		{
			// Update list view column structure for column
			lvColumn.pszText = ( LPTSTR )lpszColumnTitles[ lvColumn.iSubItem ];

			// Insert column
			SendMessage( g_hWndDetailsListView, LVM_INSERTCOLUMN, ( WPARAM )lvColumn.iSubItem, ( LPARAM )&lvColumn );

		}; // End of loop to insert columns

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window
	return bResult;

} // End of function DetailsListViewWindowCreate

int DetailsListViewWindowGetCurrentSelection()
{
	// Get current selection item
	return ::SendMessage( g_hWndDetailsListView, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

} // End of function DetailsListViewWindowGetCurrentSelection

BOOL DetailsListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText )
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
	bResult = SendMessage( g_hWndDetailsListView, LVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function DetailsListViewWindowGetItemText

BOOL DetailsListViewWindowGetRect( LPRECT lpRect )
{
	// Get list view window rect
	return ::GetWindowRect( g_hWndDetailsListView, lpRect );

} // End of function DetailsListViewWindowGetRect

BOOL DetailsListViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpItemSelectedFunction )( LPCTSTR lpszItemText ) )
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
			::SendMessage( g_hWndDetailsListView, LVM_SORTITEMSEX, ( WPARAM )( lpNmListView->iSubItem ), ( LPARAM )&DetailsListViewWindowCompareProcedure );

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
			nSelectedItem = SendMessage( g_hWndDetailsListView, LVM_GETNEXTITEM, ( WPARAM )-1, ( LPARAM )LVNI_FOCUSED );

			// Get selected item text
			if( DetailsListViewWindowGetItemText( nSelectedItem, DETAILS_LIST_VIEW_WINDOW_NAME_COLUMN_ID, lpszItemText ) )
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
				if( DetailsListViewWindowGetItemText( lpNmListView->iItem, DETAILS_LIST_VIEW_WINDOW_NAME_COLUMN_ID, lpszItemText ) )
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

} // End of function DetailsListViewWindowHandleNotifyMessage

BOOL DetailsListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list view window
	return ::MoveWindow( g_hWndDetailsListView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function DetailsListViewWindowMove

HWND DetailsListViewWindowSetFocus()
{
	// Focus on list view window
	return ::SetFocus( g_hWndDetailsListView );

} // End of function DetailsListViewWindowSetFocus

void DetailsListViewWindowSetFont( HFONT hFont )
{
	// Set list view window font
	::SendMessage( g_hWndDetailsListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function DetailsListViewWindowSetFont

BOOL DetailsListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText )
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
	bResult = SendMessage( g_hWndDetailsListView, LVM_SETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function DetailsListViewWindowSetItemText