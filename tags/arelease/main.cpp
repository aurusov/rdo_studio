#include "mainform.h"
#include "wget_process.h"

#include <qapplication.h>

int main( int argc, char** argv )
{
	QApplication app( argc, argv );

	WGProcess_s = new WGProcessList();

	WGMainForm window;
	app.setMainWidget( &window );

	window.resize( 600, 400 );
	window.show();

	int res = app.exec();
	
	delete WGProcess_s;

	return res;
}
