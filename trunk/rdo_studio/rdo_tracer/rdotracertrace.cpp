#include "stdafx.h"
#include "rdotracertrace.h"
#include "rdotracerlogctrl.h"
#include "rdotracertreectrl.h"
#include "../rdostudiochartdoc.h"
#include "../rdostudiochartview.h"
#include "../rdostudioapp.h"
#include "../rdostudiochildfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

RDOTracerTrace trace;

void CALLBACK traceTimerFunc( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
	trace.getTraceString();
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerTrace
// ----------------------------------------------------------------------------

RDOTracerTrace::RDOTracerTrace():
	log( NULL ),
	tree( NULL ),
	traceFile( NULL ),
	timerID( 0 ),
	tracing( false ),
	clipboardFormat( 0 ),
	chartDocTemplate( NULL )
{
}

RDOTracerTrace::~RDOTracerTrace()
{
	stopTrace();
	deleteTrace();
}

void RDOTracerTrace::cleanupObjects()
{
	if ( timerID ) {
		::KillTimer( NULL, timerID );
		timerID = 0;
	}
	if ( traceFile ) {
		delete traceFile;
		traceFile = NULL;
	}
}

void RDOTracerTrace::getStructureData()
{
	string res;
	if ( !traceFile )
		return;
	//graphicsPanel->addToLog( _T("\r\nGetting model structure...\r\n") );
	//string str;
	CString str;
	while ( traceFile->ReadString( str ) && str != "$Tracing" ) {
		res = str;
		parseStructureData( res );
	}
	//graphicsPanel->addToLog(res);
	//graphicsPanel->addToLog( _T("... Done\r\n\r\n") );
	return;
}

void RDOTracerTrace::parseStructureData( string& structure )
{
	//graphicsPanel->addToLog( _T("\r\nParsing model structure...\r\n") );
	CString str;
	string s = "Model_name";
	int startpos = 0;
	int pos = structure.find( s );
	if ( pos != string::npos ) {
		startpos = structure.find( "=", pos + s.length() ) + 1;
		//modelName = structure.Right( structure.GetLength() - startpos );
		modelName = structure.substr( startpos );
		trim( modelName );
		tree->setModelName( modelName );
	}

	if ( structure.find( "$Resource_type" ) != string::npos ) {
		do {
			traceFile->ReadString( str );
			s = str;
			if ( !s.empty() )
				addResourceType( s );
		} while ( s != "" );
	}
	
	if ( structure.find( "$Resources" ) != string::npos ) {
		do {
			traceFile->ReadString( str );
			s = str;
			if ( !s.empty() )
				addResource( s );
		} while ( s != "" );
	}

	if ( structure.find( "$Pattern" ) != string::npos ) {
		do {
			traceFile->ReadString( str );
			s = str;
			if ( !s.empty() )
				addPattern( s );
		} while ( s != "" );
	}

	if ( structure.find( "$Activities" ) != string::npos ) {
		do {
			traceFile->ReadString( str );
			s = str;
			if ( !s.empty() )
				addOperation( s );
		} while ( s != "" );
		do {
			traceFile->ReadString( str );
			s = str;
			if ( !s.empty() )
				addIrregularEvent( s );
		} while ( s != "" );
	}

	if ( structure.find( "$Watching" ) != string::npos ) {
		do {
			traceFile->ReadString( str );
			s = str;
			if ( !s.empty() )
				addResult( s );
		} while ( s != "" );
	}
	//graphicsPanel->addToLog( _T("... Done\r\n\r\n") );
}

void RDOTracerTrace::addResourceType( string& s )
{
	RDOTracerResType* type = new RDOTracerResType( RDOTK_PERMANENT );
	int pos = s.find( ' ' );
	int endpos = s.rfind( ' ' );
	//type->Name = s.Mid( pos, endpos - pos );
	type->Name = s.substr( pos, endpos - pos );
	//type->Name.TrimLeft();
	//type->Name.TrimRight();
	trim( type->Name );
	
	//string pcountstr = s.Right( s.GetLength() - endpos );
	string pcountstr = s.substr( endpos );
	trim( pcountstr );
	int paramcount = atoi( pcountstr.c_str() );
	CString str;
	string par;
	for ( int i = 0; i < paramcount; i++ ) {
		traceFile->ReadString( str );
		par = str;
		pos = par.rfind( ' ' );
		string partype = par.substr( pos );
		trim( partype );
		int enumcount = atoi( partype.c_str() );
		RDOTracerResParamType parType;
		if ( enumcount )
			parType = RDOPT_ENUMERATIVE;
		else if ( partype == "I" )
			parType = RDOPT_INTEGER;
		else if ( partype == "R" )
			parType = RDOPT_REAL;
		RDOTracerResParamInfo* param = new RDOTracerResParamInfo( parType );
		pos = par.find( ' ', 2 );
		//pos = par.Find( ' ', pos );
		endpos = par.find( ' ', pos + 1 );
		param->Name = par.substr( pos, endpos - pos );
		trim( param->Name );
		
		if ( parType == RDOPT_ENUMERATIVE ) {
			string en;
			for ( int j = 0; j < enumcount; j++ ) {
				traceFile->ReadString( str );
				en = str;
				pos = en.rfind( ' ' );
				string enname = en.substr( pos );
				trim( enname );
				param->addEnumValue( enname );
			}
		}
		type->addParamInfo( param );
	}
	
	resTypes.push_back( type );
	tree->addResourceType( type );
}

void RDOTracerTrace::addResource( string& s )
{
	int pos = s.find( ' ' );
	int endpos = s.rfind( ' ' );
	string rtpstr = s.substr( endpos );
	trim( rtpstr );
	string name = s.substr( pos, endpos - pos );
	trim( name );
	RDOTracerResource* res = new RDOTracerResource( resTypes.at( atoi( rtpstr.c_str() ) - 1 ), name );
	rtpstr = getNextValue( s );
	res->id = atoi( rtpstr.c_str() );
	
	resources.push_back( res );
	tree->addResource( res );
}

void RDOTracerTrace::addPattern( string& s )
{
	string str = s;
	string part = getNextValue( str );
	string name = getNextValue( str );
	part = getNextValue( str );
	RDOTracerPatternKind kind;
	if ( part == "A" )
		kind = RDOPK_OPERATION;
	else if ( part == "I" )
		kind = RDOPK_IRREGULAREVENT;
	else if ( part == "R" )
		kind = RDOPK_RULE;
	else if ( part == "K" )
		kind = RDOPK_KEYBOARD;
	RDOTracerPattern* pat = new RDOTracerPattern( kind );
	pat->Name = name;
	patterns.push_back( pat );
	tree->addPattern( pat );
}

void RDOTracerTrace::addOperation( string& s )
{
	int pos = s.find( ' ' );
	int endpos = s.rfind( ' ' );
	string patstr = s.substr( endpos );
	trim( patstr );
	RDOTracerOperation* opr = new RDOTracerOperation( patterns.at( atoi( patstr.c_str() ) -1 ) );

	patstr = s.substr( pos, endpos - pos );
	trim( patstr );
	opr->setName( patstr );
	
	operations.push_back( opr );
	tree->addOperation( opr );
}

void RDOTracerTrace::addIrregularEvent( string& s )
{
	addOperation( s );
}

void RDOTracerTrace::addResult( string& s )
{
	string str = s;
	string part = str.substr( 0, str.find_first_not_of( " " ) );
	str.erase( 0, part.length() );
	string name = getNextValue( str );
	part = getNextValue( str );
	int resid = atoi( part.c_str() );
	int pos = str.rfind( ' ' );
	part = str.substr( pos );
	trim( part );
	RDOTracerResultKind resKind;
	if ( part == "watch_par" )
		resKind = RDORK_WATCHPAR;
	else if ( part == "watch_state" )
		resKind = RDORK_WATCHSTATE;
	else if ( part == "watch_quant" )
		resKind = RDORK_WATCHQUANT;
	else if ( part == "watch_value" )
		resKind = RDORK_WATCHVALUE;
	RDOTracerResult* res = new RDOTracerResult( resKind );
	res->setName( name );
	res->id = resid;
	results.push_back( res );
	tree->addResult( res );
}

string RDOTracerTrace::getNextString()
{
	CString res;
	//graphicsPanel->addToLog( _T("\r\nGetting next string...\r\n") );
	if ( !traceFile->ReadString( res ) )
		res = "Unexpected EOF";
	log->addStringToLog( res );
	//graphicsPanel->addToLog( _T(" " + res + "\r\n") );
	//graphicsPanel->addToLog( _T("... Done\r\n\r\n") );
	return res;
}

void RDOTracerTrace::dispathNextString( string& line )
{
	//graphicsPanel->addToLog( _T("\r\nDispatching next string...\r\n") );
	if ( line.empty() )
		return;
	if ( line == "Unexpected EOF" ) {
		statusStr = line;
		doStopTrace();
		return;
	}
	if ( line.find( "$Status", 0 ) != string::npos ) {
		line.erase( 0, 10 );
		statusStr = line;
		return;
	}
	if ( line.find( "DPS_MM", 0 ) != string::npos ) {
		doStopTrace();
		return;
	}

	string key = getNextValue( line );
	/*while ( line.GetAt( 0 ) == ' ' )
		line.Delete( 0, 1 );*/
	RDOTracerTimeNow* timeNow = addTime( getNextValue( line ) );
	timeNow->eventCount ++;

	if ( key == _T("ES") ) {
		/*textColor = es.foregroundColor;
		backColor = es.backgroundColor;*/
	} else if ( key == _T("EB") ) {
		startAction( line, timeNow );
	} else if ( key == _T("EF") ) {
		accomplishAction( line, timeNow );
	} else if ( key == _T("EI") ) {
		irregularEvent( line, timeNow );
	} else if ( key == _T("ER") ) {
		productionRule( line, timeNow );
	} else if ( key == _T("RC") ) {
		resourceCreation( line, timeNow );
	} else if ( key == _T("RE") ) {
		resourceElimination( line, timeNow );
	} else if ( key == _T("RK") ) {
		resourceChanging( line, timeNow );
	} else if ( key == _T("V") ) {
		resultChanging( line, timeNow );
	}/* else if ( key == _T("$Status") ) {
		textColor = s.foregroundColor;
		backColor = s.backgroundColor;
	} else if ( key.Find( "DPS", 0) != -1 ) {
		textColor = dps.foregroundColor;
		backColor = dps.backgroundColor;
	} else if ( key == _T("SB") ) {
		textColor = sb.foregroundColor;
		backColor = sb.backgroundColor;
	} else if ( key == _T("SO") ) {
		textColor = so.foregroundColor;
		backColor = so.backgroundColor;
	} else if ( key == _T("STN") ) {
		textColor = stn.foregroundColor;
		backColor = stn.backgroundColor;
	} else if ( key == _T("STD") ) {
		textColor = std.foregroundColor;
		backColor = std.backgroundColor;
	} else if ( key == _T("STR") ) {
		textColor = str.foregroundColor;
		backColor = str.backgroundColor;
	} else if ( key == _T("SRK") ) {
		textColor = srk.foregroundColor;
		backColor = srk.backgroundColor;
	} else if ( key == _T("SRC") ) {
		textColor = src.foregroundColor;
		backColor = src.backgroundColor;
	} else if ( key == _T("SRE") ) {
		textColor = sre.foregroundColor;
		backColor = sre.backgroundColor;
	} else if ( key == _T("SRK") ) {
		textColor = srk.foregroundColor;
		backColor = srk.backgroundColor;
	} else if ( key == _T("SD") ) {
		textColor = sd.foregroundColor;
		backColor = sd.backgroundColor;
	} else if ( key == _T("SES") ) {
		textColor = ses.foregroundColor;
		backColor = ses.backgroundColor;
	} else if ( key == _T("SEN") ) {
		textColor = sen.foregroundColor;
		backColor = sen.backgroundColor;
	} else if ( key == _T("SEM") ) {
		textColor = sem.foregroundColor;
		backColor = sem.backgroundColor;
	} else if ( key == _T("SEF") ) {
		textColor = sef.foregroundColor;
		backColor = sef.backgroundColor;
	} else if ( key == _T("SEU") ) {
		textColor = seu.foregroundColor;
		backColor = seu.backgroundColor;
	}*/

	//graphicsPanel->addToLog( _T("... Done\r\n\r\n") );
}

string RDOTracerTrace::getNextValue( string& line )
{
	int pos = line.find_first_not_of( ' ' );
	string res = line.substr( pos, line.find_first_of( ' ' ) );
	line.erase( 0, pos + res.length() + 1 );
	trim( res );
	return res;
}

RDOTracerTimeNow* RDOTracerTrace::addTime( string& time )
{
	double val = atof( time.c_str() );
	if ( timeVector.empty() || timeVector.back()->time != val ) {
		RDOTracerTimeNow* timeNow = new RDOTracerTimeNow();
		timeNow->time = val;
		timeNow->eventCount = 0;
		timeVector.push_back( timeNow );
	}
	return timeVector.back();
}

RDOTracerOperation* RDOTracerTrace::getOperation( string& line )
{
	getNextValue( line );
	return operations.at( atoi( getNextValue( line ).c_str() ) - 1 );
}

void RDOTracerTrace::startAction( string& line, RDOTracerTimeNow* const time  )
{
	getOperation( line )->start( time );
}

void RDOTracerTrace::accomplishAction( string& line, RDOTracerTimeNow* const time  )
{
	getOperation( line )->accomplish( time );
}

void RDOTracerTrace::irregularEvent( string& line, RDOTracerTimeNow* const time  )
{
}

void RDOTracerTrace::productionRule( string& line, RDOTracerTimeNow* const time  )
{
}

RDOTracerResource* RDOTracerTrace::getResource( string& line )
{
	getNextValue( line );
	RDOTracerResource* res = NULL;
	int findid = atoi( getNextValue( line ).c_str() );
	for ( vector< RDOTracerResource* >::iterator it = resources.begin(); it != resources.end(); it++ ) {
		if ( (*it)->id == findid && !(*it)->isErased() ) {
			res = *it;
			break;
		}
	}
	return res;
}

void RDOTracerTrace::resourceCreation( string& line, RDOTracerTimeNow* const time  )
{
	RDOTracerResType* type = resTypes.at( atoi( getNextValue( line ).c_str() ) - 1 );
	int id = atoi( getNextValue( line ).c_str() );
	RDOTracerResource* res = new RDOTracerResource( type, format( "%s #%d", type->Name.c_str(), id ) );
	res->id = id;
	res->setParams( line, time );

	resources.push_back( res );
	tree->addResource( res );

	/*RDOTracerResource* res = new RDOTracerResource( resTypes.at( atoi( getNextValue( line ) ) - 1 ) );
	res->id = atoi( getNextValue( line ) );
	string format = "%s #%d";
	res->Name.Format( format, res->getType()->Name, res->id );
	res->setParams( line, time );

	resources.push_back( res );
	tree->addResource( res );*/
}

void RDOTracerTrace::resourceElimination( string& line, RDOTracerTimeNow* const time  )
{
	RDOTracerResource* res = getResource( line );
	res->setParams( line, time, true );
	res->setErased( true );
	tree->updateResource( res );
}

void RDOTracerTrace::resourceChanging( string& line, RDOTracerTimeNow* const time  )
{
	RDOTracerResource* res = getResource( line );
	res->setParams( line, time );
}

void RDOTracerTrace::resultChanging( string& line, RDOTracerTimeNow* const time  )
{
}

void RDOTracerTrace::deleteTrace()
{
	int count = resources.size();
	for ( int i = 0; i < count; i++ ) {
		delete resources.at( i );
	}
	resources.clear();
	count = resTypes.size();
	for ( i = 0; i < count; i++ ) {
		delete resTypes.at( i );
	}
	resTypes.clear();
	count = operations.size();
	for ( i = 0; i < count; i++ ) {
		delete operations.at( i );
	}
	operations.clear();
	count = irregularEvents.size();
	for ( i = 0; i < count; i++ ) {
		delete irregularEvents.at( i );
	}
	irregularEvents.clear();
	count = patterns.size();
	for ( i = 0; i < count; i++ ) {
		delete patterns.at( i );
	}
	patterns.clear();
	count = results.size();
	for ( i = 0; i < count; i++ ) {
		delete results.at( i );
	}
	results.clear();
	count = timeVector.size();
	for ( i = 0; i < count; i++ ) {
		delete timeVector.at( i );
	}
	timeVector.clear();
	charts.clear();
}

void RDOTracerTrace::clear()
{
	//graphicsPanel->clearLog();
	deleteTrace();
	tree->clear();
}

CMultiDocTemplate* RDOTracerTrace::createDocTemplate()
{
	chartDocTemplate = new CMultiDocTemplate( IDR_CHARTTYPE, RUNTIME_CLASS(RDOStudioChartDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioChartView) );
	return chartDocTemplate;
}

RDOTracerLogCtrl* RDOTracerTrace::createLog()
{
	log = new RDOTracerLogCtrl();
	return log;
}

RDOTracerTreeCtrl* RDOTracerTrace::createTree()
{
	tree = new RDOTracerTreeCtrl();
	return tree;
}

void RDOTracerTrace::startTrace()
{
	string filter = _T("Trace files (*.trc)|*.TRC||");

	CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter.c_str() );
	if ( dlg.DoModal() != IDOK )
		return;
	clear();
	log->startTrace();
//	setControls( TRUE );
	startTime = CTime::GetCurrentTime();
	//string timestr = time.Format("Trace started at %c");
	//graphicsPanel->addToLog( str );

	//treeView->setModelName( modelName );
	//mainFrame->setModelName( dlg.GetFileTitle() );

	traceFile = new CStdioFile( dlg.GetPathName(), CFile::modeRead | CFile::shareDenyNone );
	
	try {
		getModelStructure();
		timerID = ::SetTimer( NULL, 0, 50, traceTimerFunc );
		tracing = true;
	} catch ( CException* ) {
		if (traceFile) {
			delete traceFile;
			traceFile = NULL;
			tracing = false;
		}
		throw;
	}
}

void RDOTracerTrace::getModelStructure()
{
	getStructureData();
	//parseStructureData( str );
}

void RDOTracerTrace::getTraceString()
{
	string str = getNextString();
	dispathNextString( str );
}

void RDOTracerTrace::stopTrace()
{
	statusStr = "Trace stopped by user";
	doStopTrace();
}

void RDOTracerTrace::doStopTrace()
{
	cleanupObjects();
	stopTime = CTime::GetCurrentTime();
	tracing = false;
}
	
const bool RDOTracerTrace::isTracing() const
{
	return tracing;
}

RDOStudioChartDoc* RDOTracerTrace::createNewChart()
{
	RDOStudioChartDoc* doc = NULL;
	if ( chartDocTemplate ) {
		doc = (RDOStudioChartDoc*)chartDocTemplate->OpenDocumentFile( NULL );
		charts.push_back( doc );
	}
	return doc;
}

RDOStudioChartDoc* RDOTracerTrace::addSerieToChart( RDOTracerSerie* const serie, RDOStudioChartDoc* chart )
{
	if ( !chart )
		chart = createNewChart();
	if( chart )
		chart->addSerie( serie );
	return chart;
}

void RDOTracerTrace::addChart( RDOStudioChartDoc* const chart )
{
	charts.push_back( chart );
}

void RDOTracerTrace::removeChart( const RDOStudioChartDoc* const chart )
{
	for ( vector< RDOStudioChartDoc* >::iterator it = charts.begin(); it != charts.end(); it++ ) {
		if ( (*it) == chart ) {
			charts.erase( it );
			break;
		}
	}
}
