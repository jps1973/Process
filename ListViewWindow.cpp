// ListViewWindow.cpp

#include "ListViewWindow.h"

// Global variables
HWND g_hWndListView;
LPTSTR g_lpszParentFolderPath;

BOOL IsListViewWindow( HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndListView );

} // End of function IsListViewWindow

int ListViewWindowAddFiles( LPCTSTR lpszFileFilter )
{
	int nResult = 0;

	// Allocate string memory
	LPTSTR lpszFolderPath = new char[ STRING_LENGTH ];

	// Get folder path
	if( GetCurrentDirectory( STRING_LENGTH, lpszFolderPath ) )
	{
		// Successfully got folder path

		// Add files
		nResult = ListViewWindowAddFiles( lpszFolderPath, lpszFileFilter );

	} // End of successfully got folder path

	// Free string memory
	delete [] lpszFolderPath;

	return nResult;

} // End of function ListViewWindowAddFiles

int ListViewWindowAddFiles( LPCTSTR lpszFolderPath, LPCTSTR lpszFileFilter )
{
	int nResult = 0;

	WIN32_FIND_DATA  wfd;
	HANDLE hFind;

	// Allocate string memory
	LPTSTR lpszFullSearchPattern = new char[ STRING_LENGTH ];

	// Copy folder path into global parent folder path
	lstrcpy( g_lpszParentFolderPath, lpszFolderPath );

	// Ensure that global parent folder path ends with a back-slash
	if( g_lpszParentFolderPath[ lstrlen( g_lpszParentFolderPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
	{
		// Global parent folder path does not end with a back-slash

		// Append a back-slash onto global parent folder path
		lstrcat( g_lpszParentFolderPath, ASCII_BACK_SLASH_STRING );

	} // End of global parent folder path does not end with a back-slash

	// Copy global parent folder path into full search pattern
	lstrcpy( lpszFullSearchPattern, g_lpszParentFolderPath );

	// Append file filter onto full search pattern
	lstrcat( lpszFullSearchPattern, lpszFileFilter );

	// Find first item
	hFind = FindFirstFile( lpszFullSearchPattern, &wfd );

	// Ensure that first item was found
	if( hFind != INVALID_HANDLE_VALUE ) 
	{
		// Successfully found first item
		LVITEM lvItem;

		// Allocate string memory
		LPTSTR lpszFoundFilePath	= new char[ STRING_LENGTH ];
		LPTSTR lpszModifiedTime		= new char[ STRING_LENGTH ];

		// Clear list view item structure
		ZeroMemory( &lvItem, sizeof( lvItem ) );

		// Initialise list view item structure
		lvItem.mask			= ( LVIF_TEXT | LVIF_IMAGE );
		lvItem.cchTextMax	= STRING_LENGTH;
		lvItem.iItem		= 0;

		// Delete all items from list view window
		SendMessage( g_hWndListView, LVM_DELETEALLITEMS, ( WPARAM )NULL, ( LPARAM )NULL );

		// Loop through all items
		do
		{
			// See if current item is a file
			if( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				// Current item is a file

				// Copy parent folder path into found file path
				lstrcpy( lpszFoundFilePath, g_lpszParentFolderPath );

				// Append found file name onto found file path
				lstrcat( lpszFoundFilePath, wfd.cFileName );

				// Update list view item structure for current file name
				lvItem.iSubItem		= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
				lvItem.pszText		= wfd.cFileName;

				// Add current file name to list view window
				lvItem.iItem = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

				// Ensure that current file name was added to list view window
				if( lvItem.iItem >= 0 )
				{
					// Successfully added current file name to list view window

					// Update list view item structure for next item
					lvItem.iItem ++;

					// Update return value
					nResult ++;

				} // End of successfully added current file name to list view window

			} // End of current item is a file

		} while( FindNextFile( hFind, &wfd ) != 0 ); // End of loop through all items

		// Auto-size all list view window columns
		ListViewWindowAutoSizeAllColumns();

		// Close file find
		FindClose( hFind );

		// Free string memory
		delete [] lpszFoundFilePath;
		delete [] lpszModifiedTime;

	} // End of successfully found first item

	// Free string memory
	delete [] lpszFullSearchPattern;

	return nResult;

} // End of function ListViewWindowAddFiles

int ListViewWindowAddItem( LPCTSTR lpszItemText )
{
	int nResult;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );
	lvItem.iSubItem		= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Add item to list view window
	nResult = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	return nResult;

} // End of function ListViewWindowAddItem

int ListViewWindowAutoSizeAllColumns()
{
	int nResult = 0;

	int nWhichColumn;

	// Loop through columns
	for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
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
			nWhichColumn = LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS;

		} // End of unable to auto-size current column

	} // End of loop through columns

	return nResult;

} // End of function ListViewWindowAutoSizeAllColumns

int CALLBACK ListViewWindowCompareProcedure( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
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

} // End of function ListViewWindowCompareProcedure

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndListView = ::CreateWindowEx( LIST_VIEW_WINDOW_EXTENDED_STYLE, LIST_VIEW_WINDOW_CLASS_NAME, LIST_VIEW_WINDOW_TEXT, LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndListView )
	{
		// Successfully created list view window
		LVCOLUMN lvColumn;
		LPCTSTR lpszColumnTitles[] = LIST_VIEW_WINDOW_COLUMN_TITLES;

		// Clear list view column structure
		ZeroMemory( &lvColumn, sizeof( lvColumn ) );

		// Initialise list view column structure
		lvColumn.mask	= ( LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM );
		lvColumn.cx		= LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH;

		// Set extended list view window style
		SendMessage( g_hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )0, ( LPARAM )LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Insert columns
		for( lvColumn.iSubItem = 0; lvColumn.iSubItem < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; lvColumn.iSubItem ++ )
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

} // End of function ListViewWindowCreate

int ListViewWindowGetCurrentSelection()
{
	// Get current selection item
	return ::SendMessage( g_hWndListView, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

} // End of function ListViewWindowGetCurrentSelection

BOOL ListViewWindowGetItemPath( int nWhichItem, LPTSTR lpszItemPath )
{
	BOOL bResult;

	LVITEM lvItem;

	// Copy global parent folder path into item path
	lstrcpy( lpszItemPath, g_lpszParentFolderPath );

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
	lvItem.pszText		= ( lpszItemPath + lstrlen( lpszItemPath ) );

	// Get item text
	bResult = SendMessage( g_hWndListView, LVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function ListViewWindowGetItemText

BOOL ListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText )
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

} // End of function ListViewWindowGetItemText

BOOL ListViewWindowGetRect( LPRECT lpRect )
{
	// Get list view window rect
	return ::GetWindowRect( g_hWndListView, lpRect );

} // End of function ListViewWindowGetRect

BOOL ListViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpItemSelectedFunction )( LPCTSTR lpszItemText ) )
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
			::SendMessage( g_hWndListView, LVM_SORTITEMSEX, ( WPARAM )( lpNmListView->iSubItem ), ( LPARAM )&ListViewWindowCompareProcedure );

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
			LPTSTR lpszItemPath = new char[ STRING_LENGTH ];

			// Get selected item
			nSelectedItem = SendMessage( g_hWndListView, LVM_GETNEXTITEM, ( WPARAM )-1, ( LPARAM )LVNI_FOCUSED );

			// Get selected item path
			if( ListViewWindowGetItemPath( nSelectedItem, lpszItemPath ) )
			{
				// Successfully got selected item path

				// Call double click function
				( *lpDoubleClickFunction )( lpszItemPath );

				// Update return value
				bResult = TRUE;

			} // End of successfully got selected item path

			// Free string memory
			delete [] lpszItemPath;

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
				LPTSTR lpszItemPath = new char[ STRING_LENGTH ];

				// Get selected item path
				if( ListViewWindowGetItemPath( lpNmListView->iItem, lpszItemPath ) )
				{
					// Successfully got selected item path

					// Call item selected function
					( *lpItemSelectedFunction )( lpszItemPath );

					// Update return value
					bResult = TRUE;

				} // End of successfully got selected item path

				// Free string memory
				delete [] lpszItemPath;

			} // End of item is now selected

		} // End of a list view item changed notify message

	}; // End of selection for notify message

	return bResult;

} // End of function ListViewWindowHandleNotifyMessage

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list view window
	return ::MoveWindow( g_hWndListView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListViewWindowMove

HWND ListViewWindowSetFocus()
{
	// Focus on list view window
	return ::SetFocus( g_hWndListView );

} // End of function ListViewWindowSetFocus

void ListViewWindowSetFont( HFONT hFont )
{
	// Set list view window font
	::SendMessage( g_hWndListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListViewWindowSetFont

BOOL ListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText )
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

} // End of function ListViewWindowSetItemText