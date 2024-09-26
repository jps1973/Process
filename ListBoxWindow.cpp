// ListBoxWindow.cpp

#include "ListBoxWindow.h"

// Global variables
HWND g_hWndListBox;

BOOL IsListBoxWindow( HWND hWnd )
{
	// See if supplied window is list box window
	return( hWnd == g_hWndListBox );

} // End of function IsListBoxWindow

int ListBoxWindowAddString( LPCTSTR lpszString )
{
	// Add string to list box window
	return ::SendMessage( g_hWndListBox, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszString );

} // End of function ListBoxWindowAddString

BOOL ListBoxWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list box window
	g_hWndListBox = ::CreateWindowEx( LIST_BOX_WINDOW_EXTENDED_STYLE, LIST_BOX_WINDOW_CLASS_NAME, LIST_BOX_WINDOW_TEXT, LIST_BOX_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list box window was created
	if( g_hWndListBox )
	{
		// Successfully created list box window

		// Update return value
		bResult = TRUE;

	} // End of successfully created list box window
	return bResult;

} // End of function ListBoxWindowCreate

int ListBoxWindowGetCurrentSelection()
{
	// Get current selection item
	return ::SendMessage( g_hWndListBox, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

} // End of function ListBoxWindowGetCurrentSelection

int ListBoxWindowGetItemText( int nWhichItem, LPTSTR lpszItemText )
{
	// Get item text
	return::SendMessage( g_hWndListBox, LB_GETTEXT, ( WPARAM )nWhichItem, ( LPARAM )lpszItemText );

} // End of function ListBoxWindowGetItemText

BOOL ListBoxWindowGetRect( LPRECT lpRect )
{
	// Get list box window rect
	return ::GetWindowRect( g_hWndListBox, lpRect );

} // End of function ListBoxWindowGetRect

BOOL ListBoxWindowHandleCommandMessage( WPARAM wParam, LPARAM, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select list box window notification code
	switch( HIWORD( wParam ) )
	{
		case LBN_DBLCLK:
		{
			// A list box window double click notification code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelectedItemText = new char[ STRING_LENGTH ];

			// Get selected item
			nSelectedItem = ListBoxWindowGetCurrentSelection();

			// Get selected item text
			if( ListBoxWindowGetItemText( nSelectedItem, lpszSelectedItemText ) )
			{
				// Successfully got selected item text

				// Call double click function
				( *lpDoubleClickFunction )( lpszSelectedItemText );

				// Update return value
				bResult = TRUE;

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelectedItemText;

			// Break out of switch
			break;

		} // End of a list box window double click notification code
		case LBN_SELCHANGE:
		{
			// A list box window selection change notification code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelectedItemText = new char[ STRING_LENGTH ];

			// Get selected item
			nSelectedItem = ListBoxWindowGetCurrentSelection();

			// Get selected item text
			if( ListBoxWindowGetItemText( nSelectedItem, lpszSelectedItemText ) )
			{
				// Successfully got selected item text

				// Call selection changed function
				( *lpSelectionChangedFunction )( lpszSelectedItemText );

				// Update return value
				bResult = TRUE;

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelectedItemText;

			// Break out of switch
			break;

		} // End of a list box window selection change notification code

	}; // End of selection for list box window notification code

	return bResult;

} // End of function ListBoxWindowHandleCommandMessage

BOOL ListBoxWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list box window
	return ::MoveWindow( g_hWndListBox, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListBoxWindowMove

int ListBoxWindowPopulate( HWND hWndMain )
{
	int nResult = 0;

	HANDLE hProcessSnap;

	// Take snapshot of all processes
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

	// Ensure that snapshot of all processes was taken
	if( hProcessSnap != INVALID_HANDLE_VALUE )
	{
		// Successfully taken snapshot of all processes
		PROCESSENTRY32 pe32;

		// Clear process entry structure
		ZeroMemory( &pe32, sizeof( pe32 ) );

		// Initialise process entry structure
		pe32.dwSize = sizeof( PROCESSENTRY32 );

		// Get first process entry
		if( Process32First( hProcessSnap, &pe32 ) )
		{
			// Successfully got first process entry

			// Loop through all processes
			do
			{
				// Add process name to list box window
				if( ListBoxWindowAddString( pe32.szExeFile ) >= 0 )
				{
					// Successfully added process name to list box window

					// Update return value
					nResult ++;

				} // End of successfully added process name to list box window

			} while ( Process32Next( hProcessSnap, &pe32 ) );
			
		} // End of successfully got first process entry
		else
		{
			// Unable to get first process entry

			// Display error message
			MessageBox( hWndMain, LIST_BOX_WINDOW_UNABLE_TO_GET_FIRST_PROCESS_ENTRY_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to get first process entry

		// Close snapshot of all processes
		CloseHandle( hProcessSnap );

	} // End of successfully taken snapshot of all processes
	else
	{
		// Unable to take snapshot of all processes

		// Display error message
		MessageBox( hWndMain, LIST_BOX_WINDOW_UNABLE_TO_TAKE_SNAPSHOT_OF_ALL_PROCESSES_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to take snapshot of all processes

	return nResult;

} // End of function ListBoxWindowPopulate

BOOL ListBoxWindowSave( LPCTSTR lpszFileName )
{
	BOOL bResult = FALSE;

	HANDLE hFile;

	// Create file
	hFile = CreateFile( lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	// Ensure that file was created
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully created file
		int nItemCount;
		int nWhichItem;

		// Allocate string memory
		LPTSTR lpszItemText = new char[ STRING_LENGTH ];

		// Count items on list box window
		nItemCount = SendMessage( g_hWndListBox, LB_GETCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Assume success
		bResult = TRUE;

		// Loop through items on list box window
		for( nWhichItem = 0; nWhichItem < nItemCount; nWhichItem ++ )
		{
			// Get item text
			if( SendMessage( g_hWndListBox, LB_GETTEXT, ( WPARAM )nWhichItem, ( LPARAM )lpszItemText ) != LB_ERR )
			{
				// Successfully got item text

				// Write item text to file
				if( WriteFile( hFile, lpszItemText, lstrlen( lpszItemText ), NULL, NULL ) )
				{
					// Successfully wrote item text to file

					// Write new line text to file
					WriteFile( hFile, NEW_LINE_TEXT, lstrlen( NEW_LINE_TEXT ), NULL, NULL );

				} // End of successfully wrote item text to file
				else
				{
					// Unable to write item text to file

					// Update return value
					bResult = FALSE;

					// Force exit from loop
					nWhichItem = nItemCount;

				} // End of unable to write item text to file

			} // End of successfully got item text
			else
			{
				// Unable to get item text

				// Update return value
				bResult = FALSE;

				// Force exit from loop
				nWhichItem = nItemCount;

			} // End of unable to get item text

		}; // End of loop through items on list box window

		// Free string memory
		delete [] lpszItemText;

		// Close file
		CloseHandle( hFile );

	} // End of successfully created file

	return bResult;

} // End of function ListBoxWindowSave

HWND ListBoxWindowSetFocus()
{
	// Focus on list box window
	return ::SetFocus( g_hWndListBox );

} // End of function ListBoxWindowSetFocus

void ListBoxWindowSetFont( HFONT hFont )
{
	// Set list box window font
	::SendMessage( g_hWndListBox, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListBoxWindowSetFont
