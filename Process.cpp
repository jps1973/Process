#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

int main( void )
{
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
				// Display process information
				printf( "%s\n", pe32.szExeFile );

			} while ( Process32Next( hProcessSnap, &pe32 ) );
			
		} // End of successfully got first process entry

		// Close snapshot of all processes
		CloseHandle( hProcessSnap );

	} // End of successfully taken snapshot of all processes

	return 0;

} // End of function main
