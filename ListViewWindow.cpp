// ListViewWindow.cpp

#include "ListViewWindow.h"

// Global variables
static HWND g_hWndListView;

BOOL IsListViewWindow( HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndListView );

} // End of function IsListViewWindow

int ListViewWindowAddItem( LPCTSTR lpszItem, DWORD dwMaximumTextLength )
{
	int nResult = 0;

	LVITEM lvItem;
	int nItemCount;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Count items on list view window
	nItemCount = SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= dwMaximumTextLength;
	lvItem.iItem		= nItemCount;
	lvItem.iSubItem		= 0;
	lvItem.pszText		= ( LPTSTR )lpszItem;

	// Add item to list view window
	SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )nItemCount, ( LPARAM )&lvItem );

	return nResult;

} // End of function ListViewWindowAddItem

int ListViewWindowAutoSizeAllColumns()
{
	int nResult = 0;

	int nWhichColumn;

	// Loop through all columns
	for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
	{
		// Auto-size current column
		if( SendMessage( g_hWndListView, LVM_SETCOLUMNWIDTH, ( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER ) )
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

	}; // End of loop through all columns

	return nResult;

} // End of function ListViewWindowAutoSizeAllColumns

int CALLBACK ListViewWindowCompare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamColumn )
{
	int nResult = 0;

	LVITEM lvItem;

	// Allocate string memory
	LPTSTR lpszItem1 = new char[ STRING_LENGTH + sizeof( char ) ];
	LPTSTR lpszItem2 = new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.iSubItem		= lParamColumn;
	lvItem.cchTextMax	= STRING_LENGTH;

	// Update list view item structure for first item
	lvItem.iItem	= lParam1;
	lvItem.pszText	= lpszItem1;

	// Get first item text
	if( SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )lParam1, ( LPARAM )&lvItem ) )
	{
		// Successfully got first item text

		// Update list view item structure for second item
		lvItem.iItem	= lParam2;
		lvItem.pszText	= lpszItem2;

		// Get second item text
		if( SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )lParam2, ( LPARAM )&lvItem ) )
		{
			// Successfully got second item text

			// Compare item texts
			nResult = lstrcmp( lpszItem1, lpszItem2 );

		} // End of successfully got second item text

	} // End of successfully got first item text

	// Free string memory
	delete [] lpszItem1;
	delete [] lpszItem2;

	return nResult;

} // End of function ListViewWindowCompare

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndListView = CreateWindowEx( LIST_VIEW_WINDOW_EXTENDED_STYLE, LIST_VIEW_WINDOW_CLASS_NAME, LIST_VIEW_WINDOW_TEXT, LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndListView )
	{
		// Successfully created list view window
		int nWhichColumn;
		LVCOLUMN lvColumn;
		LPCTSTR lpszColumnTitles [] = LIST_VIEW_WINDOW_COLUMN_TITLES;

		// Clear list view column structure
		ZeroMemory( &lvColumn, sizeof( lvColumn ) );

		// Initialise list view column structure
		lvColumn.mask = LVCF_TEXT;

		// Set extended list view window style
		SendMessage( g_hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )NULL, ( LPARAM )LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Loop through all list view window columns
		for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
		{
			// Update list view column structure for current column
			lvColumn.pszText = ( LPTSTR )lpszColumnTitles[ nWhichColumn ];

			// Insert current column
			SendMessage( g_hWndListView, LVM_INSERTCOLUMN, ( WPARAM )nWhichColumn, ( LPARAM )&lvColumn );

		}; // End of loop through all list view window columns

		// Auto-size all list view window columns
		ListViewWindowAutoSizeAllColumns();

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window
	return bResult;

} // End of function ListViewWindowCreate

BOOL ListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText, DWORD dwMaximumTextLength )
{
	BOOL bResult = FALSE;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= dwMaximumTextLength;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= lpszItemText;

	// Get item from list view window
	bResult = SendMessage( g_hWndListView, LVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function ListViewWindowGetItemText

BOOL ListViewWindowGetRect( LPRECT lpRect )
{
	// Get list view window rect
	return GetWindowRect( g_hWndListView, lpRect );

} // End of function ListViewWindowGetRect

BOOL ListViewWindowHandleCommandMessage( WPARAM wParam, LPARAM, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select list view window notification code
	switch( HIWORD( wParam ) )
	{
		case LBN_DBLCLK:
		{
			// A list view window double click notification code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelected = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get selected item
			nSelectedItem = SendMessage( g_hWndListView, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Get selected item text
			if( SendMessage( g_hWndListView, LB_GETTEXT, ( WPARAM )nSelectedItem, ( LPARAM )lpszSelected ) )
			{
				// Successfully got selected item text

				// Display selected item text
				MessageBox( NULL, lpszSelected, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelected;

			// Break out of switch
			break;

		} // End of a list view window double click notification code
		case LBN_SELCHANGE:
		{
			// A list view window selection change notification code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelected = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get selected item
			nSelectedItem = SendMessage( g_hWndListView, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Get selected item text
			if( SendMessage( g_hWndListView, LB_GETTEXT, ( WPARAM )nSelectedItem, ( LPARAM )lpszSelected ) )
			{
				// Successfully got selected item text

				// Show selected item text on status bar window
				( *lpStatusFunction )( lpszSelected );

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelected;

			// Break out of switch
			break;

		} // End of a list view window selection change notification code
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for list view window notification code

	return bResult;

} // End of function ListViewWindowHandleCommandMessage

BOOL ListViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMLISTVIEW lpNmListView;

	// Get list view notify message handler
	lpNmListView = ( LPNMLISTVIEW )lParam;

	// Select list view window notification code
	switch( lpNmListView->hdr.code )
	{
		case NM_DBLCLK:
		{
			// A double click notify message

			// Allocate string memory
			LPTSTR lpszFilePath = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get file path
			if( ListViewWindowGetItemText( lpNmListView->iItem, lpNmListView->iSubItem, lpszFilePath ) )
			{
				// Successfully got file path

				// Display file path
				MessageBox( NULL, lpszFilePath, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

			} // End of successfully got file path

			// Free string memory
			delete [] lpszFilePath;

			// Break out of switch
			break;

		} // End of a double click notify message
		case LVN_COLUMNCLICK:
		{
			// A column click notify message

			// Sort the list view
			ListView_SortItemsEx( g_hWndListView, &ListViewWindowCompare, lpNmListView->iSubItem );

			// Break out of switch
			break;

		} // End of a column click notify message
		case LVN_ITEMCHANGED:
		{
			// A list view item changed notify message

			// See if item state has changed to selected
			if( ( lpNmListView->uNewState ^ lpNmListView->uOldState ) & LVIS_SELECTED )
			{
				// Item state has changed to selected

				// Allocate string memory
				LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get item text
				if( ListViewWindowGetItemText( lpNmListView->iItem, lpNmListView->iSubItem, lpszItemText ) )
				{
					// Successfully got item text

					// Call status function
					bResult = ( *lpStatusFunction )( lpszItemText );

				} // End of successfully got item text

				// Free string memory
				delete [] lpszItemText;

			} // End of item state has changed to selected

			// Break out of switch
			break;

		} // End of a list view item changed notify message
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for list view window notification code

	return bResult;

} // End of function ListViewWindowHandleNotifyMessage

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list view window
	return MoveWindow( g_hWndListView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListViewWindowMove

int ListViewWindowLoad( LPCTSTR lpszFileName )
{
	int nResult = 0;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

	// Ensure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file
		DWORD dwFileSize;

		// Get file size
		dwFileSize = GetFileSize( hFile, NULL );

		// Ensure that file size was got
		if( dwFileSize != INVALID_FILE_SIZE )
		{
			// Successfully got file size

			// Allocate string memory
			LPTSTR lpszFileText = new char[ dwFileSize + sizeof( char ) ];

			// Read file text
			if( ReadFile( hFile, lpszFileText, dwFileSize, NULL, NULL ) )
			{
				// Successfully read file text
				LPTSTR lpszLine;
				LVITEM lvItem;
				int nItemCount;
				int nInserted;

				// Clear list view item structure
				ZeroMemory( &lvItem, sizeof( lvItem ) );

				// Count items on list view window
				nItemCount = SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

				// Initialise list view item structure
				lvItem.mask			= LVIF_TEXT;
				lvItem.cchTextMax	= STRING_LENGTH;
				lvItem.iItem		= nItemCount;

				// Terminate file text
				lpszFileText[ dwFileSize ] = ( char )NULL;

				// Get first line in file text
				lpszLine = strtok( lpszFileText, NEW_LINE_TEXT );

				// Loop through all lines in file text
				while( lpszLine )
				{
					// Update list view item structure for line
					lvItem.iSubItem	= LIST_VIEW_WINDOW_FIRST_COLUMN_ID;
					lvItem.pszText	= lpszLine;

					// Add line to list view window
					nInserted = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )nItemCount, ( LPARAM )&lvItem );

					// Ensure that line was added to list view window
					if( nInserted >= 0 )
					{
						// Successfully added line to list view window

						// Update list view item structure
						lvItem.iItem = nInserted;

						// Update return value
						nResult ++;

						// Advance to next item
						lvItem.iItem ++;

						// Get next line in file text
						lpszLine = strtok( NULL, NEW_LINE_TEXT );

					} // End of successfully added line to list view window
					else
					{
						// Unable to add line to list view window

						// Force exit from loop
						lpszLine = NULL;

					} // End of unable to add line to list view window

				}; // End of loop through all lines in file text

			} // End of successfully read file text

			// Free string memory
			delete [] lpszFileText;

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return nResult;

} // End of function ListViewWindowLoad

int ListViewWindowPopulate( LPCTSTR lpszFileName )
{
	int nResult = 0;

	// Clear list view window
	SendMessage( g_hWndListView, LB_RESETCONTENT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Load file
	nResult = ListViewWindowLoad( lpszFileName );

	// Auto-size all list view window columns
	ListViewWindowAutoSizeAllColumns();

	return nResult;

} // End of function ListViewWindowPopulate

int ListViewWindowSave( LPCTSTR lpszFileName )
{
	int nResult = 0;

	HANDLE hFile;

	// Create file
	hFile = CreateFile( lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	// Ensure that file was created
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully created file
		int nItemCount;
		LVITEM lvItem;

		// Allocate string memory
		LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

		// Clear list view item structure
		ZeroMemory( &lvItem, sizeof( lvItem ) );

		// Initialise list view item structure
		lvItem.mask			= LVIF_TEXT;
		lvItem.cchTextMax	= STRING_LENGTH;
		lvItem.pszText		= lpszItemText;

		// Count items on list view window
		nItemCount = SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Loop through items on list view window
		for( lvItem.iItem = 0; lvItem.iItem < nItemCount; lvItem.iItem ++ )
		{
			// Update list view item structure for item
			lvItem.iSubItem		= LIST_VIEW_WINDOW_FIRST_COLUMN_ID;

			// Get item text
			if( SendMessage( g_hWndListView, LVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem ) )
			{
				// Successfully got item text

				// Write item text to file
				if( WriteFile( hFile, lpszItemText, lstrlen( lpszItemText ), NULL, NULL ) )
				{
					// Successfully wrote item text to file

					// Write new line text to file
					WriteFile( hFile, NEW_LINE_TEXT, lstrlen( NEW_LINE_TEXT ), NULL, NULL );

					// Update return value
					nResult ++;

				} // End of successfully wrote item text to file
				else
				{
					// Unable to write item text to file

					// Force exit from loop
					lvItem.iItem = nItemCount;

				} // End of unable to write item text to file

			} // End of successfully got item text
			else
			{
				// Unable to get item text

				// Force exit from loop
				lvItem.iItem = nItemCount;

			} // End of unable to get item text

		}; // End of loop through items on list view window

		// Free string memory
		delete [] lpszItemText;

		// Close file
		CloseHandle( hFile );

	} // End of successfully created file

	return nResult;

} // End of function ListViewWindowSave

HWND ListViewWindowSetFocus()
{
	// Focus on list view window
	return SetFocus( g_hWndListView );

} // End of function ListViewWindowSetFocus

BOOL ListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText, DWORD dwMaximumTextLength )
{
	BOOL bResult = FALSE;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= dwMaximumTextLength;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Add item to list view window
	bResult = SendMessage( g_hWndListView, LVM_SETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function ListViewWindowSetItemText

void ListViewWindowSetFont( HFONT hFont )
{
	// Set list view window font
	SendMessage( g_hWndListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListViewWindowSetFont
