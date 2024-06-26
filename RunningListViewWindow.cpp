// RunningListViewWindow.cpp

#include "RunningListViewWindow.h"

// Global variables
static HWND g_hWndRunningListView;

BOOL IsRunningListViewWindow( HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndRunningListView );

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
	lvItem.iItem		= SendMessage( g_hWndRunningListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );
	lvItem.iSubItem		= RUNNING_LIST_VIEW_WINDOW_NAME_COLUMN_ID;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Add item to list view window
	nResult = SendMessage( g_hWndRunningListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

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
		if( SendMessage( g_hWndRunningListView, LVM_SETCOLUMNWIDTH,( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER ) )
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
	SendMessage( g_hWndRunningListView, LVM_GETITEMTEXT, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	// Update list view item structure for second item
	lvItem.iItem	= lParam2;
	lvItem.pszText	= lpszBuffer2;

	// Get second item text
	SendMessage( g_hWndRunningListView, LVM_GETITEMTEXT, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

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
	g_hWndRunningListView = ::CreateWindowEx( RUNNING_LIST_VIEW_WINDOW_EXTENDED_STYLE, RUNNING_LIST_VIEW_WINDOW_CLASS_NAME, RUNNING_LIST_VIEW_WINDOW_TEXT, RUNNING_LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndRunningListView )
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
		SendMessage( g_hWndRunningListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )0, ( LPARAM )RUNNING_LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Insert columns
		for( lvColumn.iSubItem = 0; lvColumn.iSubItem < RUNNING_LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; lvColumn.iSubItem ++ )
		{
			// Update list view column structure for column
			lvColumn.pszText = ( LPTSTR )lpszColumnTitles[ lvColumn.iSubItem ];

			// Insert column
			SendMessage( g_hWndRunningListView, LVM_INSERTCOLUMN, ( WPARAM )lvColumn.iSubItem, ( LPARAM )&lvColumn );

		}; // End of loop to insert columns

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window
	return bResult;

} // End of function RunningListViewWindowCreate

int RunningListViewWindowGetCurrentSelection()
{
	// Get current selection item
	return ::SendMessage( g_hWndRunningListView, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

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
	bResult = SendMessage( g_hWndRunningListView, LVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function RunningListViewWindowGetItemText

BOOL RunningListViewWindowGetRect( LPRECT lpRect )
{
	// Get list view window rect
	return ::GetWindowRect( g_hWndRunningListView, lpRect );

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
			::SendMessage( g_hWndRunningListView, LVM_SORTITEMSEX, ( WPARAM )( lpNmListView->iSubItem ), ( LPARAM )&RunningListViewWindowCompareProcedure );

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
			nSelectedItem = SendMessage( g_hWndRunningListView, LVM_GETNEXTITEM, ( WPARAM )-1, ( LPARAM )LVNI_FOCUSED );

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
	return ::MoveWindow( g_hWndRunningListView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function RunningListViewWindowMove

int RunningListViewWindowPopulate()
{
	int nResult = 0;

	WTS_PROCESS_INFO *lpProcessInformation = NULL;
	DWORD dwProcessCount = 0;

	// Enumerate processes
	if( WTSEnumerateProcesses( WTS_CURRENT_SERVER_HANDLE, 0, 1, &lpProcessInformation, &dwProcessCount ) ) 
	{
		// Successfully enumerated processes
		DWORD dwWhichProcess;
		LVITEM lvItem;
		int nItem;

		// Allocate string memory
		LPTSTR lpszProcessID	= new char[ STRING_LENGTH ];
		LPTSTR lpszDetails		= new char[ STRING_LENGTH ];
		LPTSTR lpszName			= new char[ STRING_LENGTH ];

		// Clear list view item structure
		ZeroMemory( &lvItem, sizeof( lvItem ) );

		// Initialise list view item structure
		lvItem.mask			= LVIF_TEXT;
		lvItem.cchTextMax	= STRING_LENGTH;
		lvItem.iItem		= 0;

		// Loop through all processes
		for( dwWhichProcess = 0; dwWhichProcess < dwProcessCount; dwWhichProcess ++ )
		{
			// See if this is the system idle process
			if( lpProcessInformation[ dwWhichProcess ].ProcessId == RUNNING_LIST_VIEW_WINDOW_SYSTEM_IDLE_PROCESS_ID )
			{
				// This is the system idle process

				// Update name text
				lstrcpy( lpszName, RUNNING_LIST_VIEW_WINDOW_SYSTEM_IDLE_PROCESS_NAME );

				// Update list view item structure for process name
				lvItem.iSubItem		= RUNNING_LIST_VIEW_WINDOW_NAME_COLUMN_ID;
				lvItem.pszText		= lpszName;

			} // End of this is the system idle process
			else
			{
				// This is not the system idle process

				// Update name text
				lstrcpy( lpszName, lpProcessInformation[ dwWhichProcess ].pProcessName );

				// Update list view item structure for process name
				lvItem.iSubItem		= RUNNING_LIST_VIEW_WINDOW_NAME_COLUMN_ID;
				lvItem.pszText		= lpszName;

			} // End of this is not the system idle process

			// Add process to running list view window
			nItem = SendMessage( g_hWndRunningListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

			// Ensure that process was added to running list view window
			if( nItem >= 0 )
			{
				// Successfully added process to running list view window

				// Format process id
				wsprintf( lpszProcessID, RUNNING_LIST_VIEW_WINDOW_PROCESS_ID_FORMAT_STRING, lpProcessInformation[ dwWhichProcess ].ProcessId );

				// Update list view item structure for process id
				lvItem.iItem		= nItem;
				lvItem.iSubItem		= RUNNING_LIST_VIEW_WINDOW_ID_COLUMN_ID;
				lvItem.pszText		= lpszProcessID;

				// Add process id to running list view window
				nItem = SendMessage( g_hWndRunningListView, LVM_SETITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

				// Get details for process
				if( DetailsListViewWindowGetDetails( lpszName, lpszDetails ) )
				{
					// Successfully got details for process

					// Update list view item structure for process details
					lvItem.iSubItem		= RUNNING_LIST_VIEW_WINDOW_DETAILS_COLUMN_ID;
					lvItem.pszText		= lpszDetails;

					// Add process details to running list view window
					nItem = SendMessage( g_hWndRunningListView, LVM_SETITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

				} // End of successfully got details for process

				// Update list view item structure for next process
				lvItem.iItem ++;

				// Update return value
				nResult ++;

			} // End of successfully added process to running list view window

		} // End of loop through all processes

		// Free string memory
		delete [] lpszProcessID;
		delete [] lpszDetails;
		delete [] lpszName;

	} // End of successfully enumerated processes
	else
	{
		// Unable to enumerate processes

		// Display error message
		MessageBox( NULL, RUNNING_LIST_VIEW_WINDOW_UNABLE_TO_ENUMERATE_PROCESSES_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to enumerate processes

	// Free memory
	WTSFreeMemory( lpProcessInformation );

	return nResult;

} // End of function RunningListViewWindowPopulate

HWND RunningListViewWindowSetFocus()
{
	// Focus on list view window
	return ::SetFocus( g_hWndRunningListView );

} // End of function RunningListViewWindowSetFocus

void RunningListViewWindowSetFont( HFONT hFont )
{
	// Set list view window font
	::SendMessage( g_hWndRunningListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

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
	bResult = SendMessage( g_hWndRunningListView, LVM_SETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function RunningListViewWindowSetItemText