// Process.cpp

#include "Process.h"

void RunningListViewWindowDoubleClickFunction( LPCTSTR lpszItemText )
{
	// Allocate string memory
	LPTSTR lpszUrl = new char[ STRING_LENGTH ];

	// Format url
	wsprintf( lpszUrl, SEARCH_URL_FORMAT_STRING, lpszItemText );

	// Open url
	ShellExecute( NULL, "open", lpszUrl, NULL, NULL, SW_SHOWNORMAL );

	// Free string memory
	delete [] lpszUrl;

} // End of function RunningListViewWindowDoubleClickFunction

void RunningListViewWindowItemSelectedFunction( LPCTSTR lpszItemText )
{
	// Show item text on status bar window
	StatusBarWindowSetText( lpszItemText );

} // End of function RunningListViewWindowItemSelectedFunction

int ShowAboutMessage( HWND hWndParent )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWndParent;
	mbp.hInstance	= NULL;
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= MAIN_WINDOW_CLASS_ICON_NAME;

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

LRESULT CALLBACK MainWndProc( HWND hWndMain, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMessage )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;
			HFONT hFont;

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Get font
			hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

			// Create running list view window
			if( RunningListViewWindowCreate( hWndMain, hInstance ) )
			{
				// Successfully created running list view window

				// Set running list view window font
				RunningListViewWindowSetFont( hFont );

				// Create details list view window
				if( DetailsListViewWindowCreate( hWndMain, hInstance ) )
				{
					// Successfully created details list view window

					// Set details list view window font
					DetailsListViewWindowSetFont( hFont );

					// Create status bar window
					if( StatusBarWindowCreate( hWndMain, hInstance ) )
					{
						// Successfully created status bar window

						// Set status bar window font
						StatusBarWindowSetFont( hFont );

					} // End of successfully created status bar window

				} // End of successfully created details list view window

			} // End of successfully created running list view window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatus;
			int nStatusWindowHeight;
			int nRunningListViewWindowHeight;
			int nDetailsListViewWindowTop;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			StatusBarWindowSize();

			// Get status window size
			StatusBarWindowGetRect( &rcStatus );

			// Calculate window sizes
			nStatusWindowHeight				= ( rcStatus.bottom - rcStatus.top );
			nRunningListViewWindowHeight	= ( ( nClientHeight - nStatusWindowHeight ) - DETAILS_LIST_VIEW_WINDOW_HEIGHT + WINDOW_BORDER_HEIGHT );

			// Calculate window positions
			nDetailsListViewWindowTop		= ( nRunningListViewWindowHeight - WINDOW_BORDER_HEIGHT );

			// Move control windows
			RunningListViewWindowMove( 0, 0, nClientWidth, nRunningListViewWindowHeight, TRUE );
			DetailsListViewWindowMove( 0, nDetailsListViewWindowTop, nClientWidth, DETAILS_LIST_VIEW_WINDOW_HEIGHT );

			// Auto-size list view window columns
			RunningListViewWindowAutoSizeAllColumns();
			DetailsListViewWindowAutoSizeAllColumns();

			// Break out of switch
			break;

		} // End of a size message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on running list view window
			RunningListViewWindowSetFocus();

			// Break out of switch
			break;

		} // End of an activate message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MINMAXINFO FAR *lpMinMaxInfo;

			// Get min max info structure
			lpMinMaxInfo = ( MINMAXINFO FAR * )lParam;

			// Update min max info structure
			lpMinMaxInfo->ptMinTrackSize.x = MAIN_WINDOW_MINIMUM_WIDTH;
			lpMinMaxInfo->ptMinTrackSize.y = MAIN_WINDOW_MINIMUM_HEIGHT;

			// Break out of switch
			break;

		} // End of a get min max info message
		case WM_DROPFILES:
		{
			// A drop files message
			UINT uFileCount;
			HDROP hDrop;
			UINT uWhichFile;
			UINT uFileSize;

			// Allocate string memory
			LPTSTR lpszFilePath = new char[ STRING_LENGTH ];

			// Get drop handle
			hDrop = ( HDROP )wParam;

			// Count dropped files
			uFileCount = DragQueryFile( hDrop, ( UINT )-1, NULL, 0 );

			// Loop through dropped files
			for( uWhichFile = 0; uWhichFile < uFileCount; uWhichFile ++ )
			{
				// Get size of dropped file
				uFileSize = DragQueryFile( hDrop, uWhichFile, NULL, 0 );

				// Ensure that file size is valid
				if( uFileSize != 0 )
				{
					// File size is valid

					// Get file path
					if( DragQueryFile( hDrop, uWhichFile, lpszFilePath, ( uFileSize + sizeof( char ) ) ) )
					{
						// Successfully got file path

						// Add file path to running list view window
						RunningListViewWindowAddItem( lpszFilePath );

					} // End of successfully got file path

				} // End of file size is valid

			}; // End of loop through dropped files

			// Free string memory
			delete [] lpszFilePath;

			// Break out of switch
			break;

		} // End of a drop files message
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy window
					DestroyWindow( hWndMain );

					// Break out of switch
					break;

				} // End of a file exit command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					// Break out of switch
					break;

				} // End of default command

			}; // End of selection for command

			// Break out of switch
			break;

		} // End of a command message
		case WM_SYSCOMMAND:
		{
			// A system command message

			// Select system command
			switch( LOWORD( wParam ) )
			{
				case IDM_HELP_ABOUT:
				{
					// A help about system command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about system command
				default:
				{
					// Default system command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					// Break out of switch
					break;

				} // End of default system command

			}; // End of selection for system command

			// Break out of switch
			break;

		} // End of a system command message
		case WM_NOTIFY:
		{
			// A notify message
			LPNMHDR lpNmHdr;

			// Get notify message handler
			lpNmHdr = ( LPNMHDR )lParam;

			// See if command message is from running list view window
			if( IsRunningListViewWindow( lpNmHdr->hwndFrom ) )
			{
				// Command message is from running list view window

				// Handle command message from running list view window
				if( !( RunningListViewWindowHandleNotifyMessage( wParam, lParam, &RunningListViewWindowDoubleClickFunction, &RunningListViewWindowItemSelectedFunction ) ) )
				{
					// Command message was not handled from running list view window

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

				} // End of command message was not handled from running list view window

			} // End of command message is from running list view window
			// Break out of switch
			break;

		} // End of a notify message
		case WM_CONTEXTMENU:
		{
			// A context menu message
			HMENU hMenuContext;

			// Load context menu
			hMenuContext = LoadMenu( NULL, MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Show context menu
			TrackPopupMenu( GetSubMenu( hMenuContext, 0 ), ( TPM_LEFTALIGN | TPM_LEFTBUTTON ), LOWORD( lParam ), HIWORD( lParam ), 0, hWndMain, NULL );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A close message

			// Destroy main window
			DestroyWindow( hWndMain );

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A destroy message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call default window procedure
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWndProc

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	MSG msg;

	WNDCLASSEX wcMain;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Clear window class structure
	ZeroMemory( &wcMain, sizeof( wcMain ) );

	// Initialise main window class structure
	wcMain.cbSize			= sizeof( WNDCLASSEX );
	wcMain.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
	wcMain.lpfnWndProc		= MainWndProc;
	wcMain.hInstance		= hInstance;
	wcMain.style			= MAIN_WINDOW_CLASS_STYLE;
	wcMain.hIcon			= MAIN_WINDOW_CLASS_ICON;
	wcMain.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
	wcMain.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;
	wcMain.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_NAME;
	wcMain.hIconSm			= MAIN_WINDOW_CLASS_ICON;

	// Register main window class
	if( RegisterClassEx( &wcMain ) )
	{
		// Successfully registered main window class
		HWND hWndMain;

		// Create main window
		hWndMain = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TEXT, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

		// Ensure that main window was created
		if( hWndMain )
		{
			// Successfully created main window
			HMENU hMenuSystem;
			int nProcessCount;

			// Allocate string memory
			LPTSTR lpszStatusMessage = new char[ STRING_LENGTH ];

			// Get system menu
			hMenuSystem = GetSystemMenu( hWndMain, FALSE );

			// Add separator to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_SEPARATOR_ITEM_POSITION, ( MF_BYPOSITION | MF_SEPARATOR ), 0, NULL );

			// Add about item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_ABOUT_ITEM_POSITION, MF_BYPOSITION, IDM_HELP_ABOUT, SYSTEM_MENU_ABOUT_ITEM_TEXT );

			// Show main window
			ShowWindow( hWndMain, nCmdShow );

			// Update main window
			UpdateWindow( hWndMain );

			// Load known process details
			DetailsListViewWindowLoad( PROCESS_FILE_NAME );

			// Populate running list view window
			nProcessCount = RunningListViewWindowPopulate();

			// Format status message
			wsprintf( lpszStatusMessage, PROCESSES_STATUS_MESSAGE_FORMAT_STRING, nProcessCount );

			// Show status message on status bar window
			StatusBarWindowSetText( lpszStatusMessage );

			// Auto-size all running list view window columns
			RunningListViewWindowAutoSizeAllColumns();

			// Main message loop
			while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
			{
				// Translate message
				TranslateMessage( &msg );

				// Dispatch message
				DispatchMessage( &msg );

			}; // End of main message loop

			// Free string memory
			delete [] lpszStatusMessage;

		} // End of successfully created main window
		else
		{
			// Unable to create main window

			// Display error message
			MessageBox( NULL, UNABLE_TO_CREATE_MAIN_WINDOW_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to create main window

	} // End of successfully registered main window class
	else
	{
		// Unable to register main window class

		// Display error message
		MessageBox( NULL, UNABLE_TO_REGISTER_MAIN_WINDOW_CLASS_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to register main window class

	return msg.wParam;

} // End of function WinMain
