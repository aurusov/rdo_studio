#include <fstream>
#include <string>
#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
	ofstream trace_file;
	trace_file.open( "trace.txt", ios::out | ios::binary );
	string str = "1234567890";
	SYSTEMTIME t1;
	SYSTEMTIME t2;
	::GetSystemTime( &t1 );
	for ( int i = 0; i < 10000000; i++ ) {
//		trace_file << str << endl;
		trace_file.write( str.c_str(), str.length() );
		trace_file.write( "\n", 1 );
		trace_file.flush();
	}
	::GetSystemTime( &t2 );
	cout << (t2.wMinute * 1000 * 60 + t2.wSecond * 1000 + t2.wMilliseconds) - (t1.wMinute * 1000 * 60 + t1.wSecond * 1000 + t1.wMilliseconds) << endl;
	char c;
	cin >> c;
	return 0;

	// << + flush = 36547
	// <<,  flush = 35391
	// << + flush = 33109
	// <<,  flush = 10868
}
