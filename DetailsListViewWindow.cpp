// DetailsListViewWindow.cpp

#include "DetailsListViewWindow.h"

// Global variables
static HWND g_hWndDetailsListView;

BOOL IsDetailsListViewWindow( HWND hWnd )
{
	// See if supplied window is details list view wimdow
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

	// Add item to details list view wimdow
	nResult = SendMessage( g_hWndDetailsListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	return nResult;

} // End of function DetailsListViewWindowAddItem

int DetailsListViewWindowAddLine( LPCTSTR lpszName, LPCTSTR lpszDetails )
{
	int nResult;

	LVFINDINFO lvFindInformation;
	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= 0;

	// Clear list view find information structure
	ZeroMemory( &lvFindInformation, sizeof( lvFindInformation ) );

	// Initialise list view find information structure
	lvFindInformation.flags	= LVFI_STRING;
	lvFindInformation.psz	= lpszName;

	// Attempt to find name on details list view window
	nResult = SendMessage( g_hWndDetailsListView, LVM_FINDITEM, ( WPARAM )-1, ( LPARAM )&lvFindInformation );

	// See if name was found on details list view window
	if( nResult < 0 )
	{
		// Unable to find name on details list view window

		// Update list view item structure for name
		lvItem.iSubItem		= DETAILS_LIST_VIEW_WINDOW_NAME_COLUMN_ID;
		lvItem.pszText		= ( LPTSTR )lpszName;

		// Add name to details list view wimdow
		nResult = SendMessage( g_hWndDetailsListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	} // End of unable to find name on details list view window

	// Ensure that name was added to details list view wimdow
	if( nResult >= 0 )
	{
		// Successfully added name to details list view wimdow

		// Update list view item structure for details
		lvItem.iItem		= nResult;
		lvItem.iSubItem		= DETAILS_LIST_VIEW_WINDOW_DETAILS_COLUMN_ID;
		lvItem.pszText		= ( LPTSTR )lpszDetails;

		// Add name to details list view wimdow
		SendMessage( g_hWndDetailsListView, LVM_SETITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	} // End of successfully added name to details list view wimdow

	return nResult;

} // End of function DetailsListViewWindowAddLine

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

	// Create details list view wimdow
	g_hWndDetailsListView = ::CreateWindowEx( DETAILS_LIST_VIEW_WINDOW_EXTENDED_STYLE, DETAILS_LIST_VIEW_WINDOW_CLASS_NAME, DETAILS_LIST_VIEW_WINDOW_TEXT, DETAILS_LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that details list view wimdow was created
	if( g_hWndDetailsListView )
	{
		// Successfully created details list view wimdow
		LVCOLUMN lvColumn;
		LPCTSTR lpszColumnTitles[] = DETAILS_LIST_VIEW_WINDOW_COLUMN_TITLES;

		// Clear list view column structure
		ZeroMemory( &lvColumn, sizeof( lvColumn ) );

		// Initialise list view column structure
		lvColumn.mask	= ( LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM );
		lvColumn.cx		= DETAILS_LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH;

		// Set extended details list view wimdow style
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

	} // End of successfully created details list view wimdow
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
	// Get details list view wimdow rect
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
			// A details list view wimdow column click notification code
			LPNMLISTVIEW lpNmListView;

			// Get list view notify message handler
			lpNmListView = ( LPNMLISTVIEW )lpNmHdr;

			// Sort the list view
			::SendMessage( g_hWndDetailsListView, LVM_SORTITEMSEX, ( WPARAM )( lpNmListView->iSubItem ), ( LPARAM )&DetailsListViewWindowCompareProcedure );

			// Update return value
			bResult = TRUE;

			// Break out of switch
			break;

		} // End of a details list view wimdow column click notification code
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

int DetailsListViewWindowLoad( LPCTSTR lpszFileName )
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
				LPTSTR lpszTab;

				// Allocate string memory
				LPTSTR lpszName		= new char[ STRING_LENGTH ];
				LPTSTR lpszDetails	= new char[ STRING_LENGTH ];

				// Terminate file text
				lpszFileText[ dwFileSize ] = ( char )NULL;

				// Get first line
				lpszLine = strtok( lpszFileText, NEW_LINE_TEXT );

				// Loop through all lines
				while( lpszLine )
				{
					// Find tab character in line
					lpszTab = strchr( lpszLine, ASCII_TAB_CHARACTER );

					// See if tab character was found in line
					if( lpszTab )
					{
						// Successfully found tab character in line

						// Use text before tab character as name
						lstrcpyn( lpszName, lpszLine, ( ( lpszTab - lpszLine ) + sizeof( char ) ) );

						// Use text after tab as details
						lstrcpy( lpszDetails, ( lpszTab + sizeof( char ) ) );

					} // End of successfully found tab character in line
					else
					{
						// Unable to find tab character in line

						// Use entire line as name
						lstrcpy( lpszName, lpszLine );

						// Clear detaile
						lpszDetails[ 0 ] = ( char )NULL;

					} // End of unable to find tab character in line

					// Add item to details list view wimdow
					DetailsListViewWindowAddLine( lpszName, lpszDetails );

					// Get next line
					lpszLine = strtok( NULL, NEW_LINE_TEXT );

				}; // End of loop through all lines

				// Auto-size details list view window
				DetailsListViewWindowAutoSizeAllColumns();

				// Free string memory
				delete [] lpszLine;
				delete [] lpszDetails;

			} // End of successfully read file text

			// Free string memory
			delete [] lpszFileText;

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return nResult;

} // End of function DetailsListViewWindowLoad

BOOL DetailsListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move details list view wimdow
	return ::MoveWindow( g_hWndDetailsListView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function DetailsListViewWindowMove

int DetailsListViewWindowSave( LPCTSTR lpszFileName )
{
	int nResult = 0;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Ensure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file

		/*
		DWORD dwTextLength;
		dwTextLength = GetWindowTextLength(hEdit);
		// No need to bother if there's no text.
		if(dwTextLength > 0)
		{
		LPSTR pszText;
		DWORD dwBufferSize = dwTextLength + 1;
		pszText = GlobalAlloc(GPTR, dwBufferSize);
		if(pszText != NULL)
		{
		if(GetWindowText(hEdit, pszText, dwBufferSize))
		{
		DWORD dwWritten;
		if(WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
		bSuccess = TRUE;
		}
		GlobalFree(pszText);
		}
		}
		*/

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return nResult;

} // End of function DetailsListViewWindowSave

HWND DetailsListViewWindowSetFocus()
{
	// Focus on details list view wimdow
	return ::SetFocus( g_hWndDetailsListView );

} // End of function DetailsListViewWindowSetFocus

void DetailsListViewWindowSetFont( HFONT hFont )
{
	// Set details list view wimdow font
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