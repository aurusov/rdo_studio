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
	CString res;
	if ( !traceFile )
		return;
	//graphicsPanel->addToLog( _T("\r\nGetting model structure...\r\n") );
	CString str;
	while ( traceFile->ReadString( str ) && str != "$Tracing" ) {
		res += str;
		res += "\r\n";
		parseStructureData( str );
	}
	//graphicsPanel->addToLog(res);
	//graphicsPanel->addToLog( _T("... Done\r\n\r\n") );
	return;
}

void RDOTracerTrace::parseStructureData( CString& structure )
{
	//graphicsPanel->addToLog( _T("\r\nParsing model structure...\r\n") );
	CString s = "Model_name";
	int startpos = 0;
	int pos = structure.Find( s );
	if ( pos != -1 ) {
		startpos = structure.Find( "=", pos + s.GetLength() ) + 1;
		modelName = structure.Right( structure.GetLength() - startpos );
		modelName.TrimLeft();
		modelName.TrimRight();
		tree->setModelName( modelName );
	}

	if ( structure.Find( "$Resource_type" ) != -1 ) {
		do {
			traceFile->ReadString( s );
			if ( !s.IsEmpty() )
				addResourceType( s );
		} while ( s != "" );
	}
	
	if ( structure.Find( "$Resources" ) != -1 ) {
		do {
			traceFile->ReadString( s );
			if ( !s.IsEmpty() )
				addResource( s );
		} while ( s != "" );
	}

	if ( structure.Find( "$Pattern" ) != -1 ) {
		do {
			traceFile->ReadString( s );
			if ( !s.IsEmpty() )
				addPattern( s );
		} while ( s != "" );
	}

	if ( structure.Find( "$Activities" ) != -1 ) {
		do {
			traceFile->ReadString( s );
			if ( !s.IsEmpty() )
				addOperation( s );
		} while ( s != "" );
		do {
			traceFile->ReadString( s );
			if ( !s.IsEmpty() )
				addIrregularEvent( s );
		} while ( s != "" );
	}

	if ( structure.Find( "$Watching" ) != -1 ) {
		do {
			traceFile->ReadString( s );
			if ( !s.IsEmpty() )
				addResult( s );
		} while ( s != "" );
	}
	//graphicsPanel->addToLog( _T("... Done\r\n\r\n") );
}

void RDOTracerTrace::addResourceType( CString& s )
{
	RDOTracerResType* type = new RDOTracerResType( RDOTK_PERMANENT );
	int pos = s.Find( ' ' );
	int endpos = s.ReverseFind( ' ' );
	type->Name = s.Mid( pos, endpos - pos );
	type->Name.TrimLeft();
	type->Name.TrimRight();
	
	CString pcountstr = s.Right( s.GetLength() - endpos );
	pcountstr.TrimLeft();
	pcountstr.TrimRight();
	int paramcount = atoi( pcountstr );
	CString par;
	for ( int i = 0; i < paramcount; i++ ) {
		traceFile->ReadString( par );
		pos = par.ReverseFind( ' ' );
		CString partype = par.Right( par.GetLength() - pos );
		partype.TrimLeft();
		partype.TrimRight();
		int enumcount = atoi( partype );
		RDOTracerResParamType parType;
		if ( enumcount )
			parType = RDOPT_ENUMERATIVE;
		else if ( partype == "I" )
			parType = RDOPT_INTEGER;
		else if ( partype == "R" )
			parType = RDOPT_REAL;
		RDOTracerResParamInfo* param = new RDOTracerResParamInfo( parType );
		pos = par.Find( ' ', 2 );
		//pos = par.Find( ' ', pos );
		endpos = par.Find( ' ', pos + 1 );
		param->Name = par.Mid( pos, endpos - pos );
		param->Name.TrimLeft();
		param->Name.TrimRight();
		
		if ( parType == RDOPT_ENUMERATIVE ) {
			CString en;
			for ( int j = 0; j < enumcount; j++ ) {
				traceFile->ReadString( en );
				pos = en.ReverseFind( ' ' );
				CString enname = en.Right( en.GetLength() - pos );
				enname.TrimLeft();
				enname.TrimRight();
				param->addEnumValue( enname );
			}
		}
		type->addParamInfo( param );
	}
	
	resTypes.push_back( type );
	tree->addResourceType( type );
}

void RDOTracerTrace::addResource( CString& s )
{
	int pos = s.Find( ' ' );
	int endpos = s.ReverseFind( ' ' );
	CString rtpstr = s.Right( s.GetLength() - endpos );
	rtpstr.TrimLeft();
	rtpstr.TrimRight();
	CString name = s.Mid( pos, endpos - pos );
	name.TrimLeft();
	name.TrimRight();
	RDOTracerResource* res = new RDOTracerResource( resTypes.at( atoi( rtpstr ) - 1 ), name );
	rtpstr = s.SpanExcluding( " " );
	rtpstr.TrimLeft();
	rtpstr.TrimRight();
	res->id = atoi( rtpstr );
	
	resources.push_back( res );
	tree->addResource( res );
}

void RDOTracerTrace::addPattern( CString& s )
{
	CString str = s;
	CString part = str.SpanExcluding( " " );
	str.Delete( 0, part.GetLength() + 1 );
	CString name = str.SpanExcluding( " " );
	str.Delete( 0, name.GetLength() + 1 );
	name.TrimLeft();
	name.TrimRight();
	part = str.SpanExcluding( " " );
	part.TrimLeft();
	part.TrimRight();
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

void RDOTracerTrace::addOperation( CString& s )
{
	int pos = s.Find( ' ' );
	int endpos = s.ReverseFind( ' ' );
	CString patstr = s.Right( s.GetLength() - endpos );
	patstr.TrimLeft();
	patstr.TrimRight();
	RDOTracerOperation* opr = new RDOTracerOperation( patterns.at( atoi( patstr) -1 ) );

	patstr = s.Mid( pos, endpos - pos );
	patstr.TrimLeft();
	patstr.TrimRight();
	opr->setName( patstr );
	
	operations.push_back( opr );
	tree->addOperation( opr );
}

void RDOTracerTrace::addIrregularEvent( CString& s )
{
	addOperation( s );
}

void RDOTracerTrace::addResult( CString& s )
{
	CString str = s;
	CString part = str.SpanIncluding( " " );
	str.Delete( 0, part.GetLength() );
	CString name = str.SpanExcluding( " " );
	str.Delete( 0, name.GetLength() + 1 );
	name.TrimLeft();
	name.TrimRight();
	part = str.SpanExcluding( " " );
	str.Delete( 0, part.GetLength() + 1 );
	part.TrimLeft();
	part.TrimRight();
	int resid = atoi( part );
	int pos = str.ReverseFind( ' ' );
	part = str.Right( str.GetLength() - pos );
	part.TrimLeft();
	part.TrimRight();
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

CString RDOTracerTrace::getNextString()
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

void RDOTracerTrace::dispathNextString( CString& line )
{
	//graphicsPanel->addToLog( _T("\r\nDispatching next string...\r\n") );
	if ( line.IsEmpty() )
		return;
	if ( line == "Unexpected EOF" ) {
		statusStr = line;
		doStopTrace();
		return;
	}
	if ( line.Find( _T("$Status"), 0 ) != -1 ) {
		line.Delete( 0, 10 );
		statusStr = line;
		return;
	}
	if ( line.Find( _T("DPS_MM"), 0 ) != -1 ) {
		doStopTrace();
		return;
	}

	CString key = getNextValue( line );
	while ( line.GetAt( 0 ) == ' ' )
		line.Delete( 0, 1 );
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

CString RDOTracerTrace::getNextValue( CString& line )
{
	CString res = line.SpanExcluding( " " );
	line.Delete( 0, res.GetLength() + 1 );
	res.TrimLeft();
	res.TrimRight();
	return res;
}

RDOTracerTimeNow* RDOTracerTrace::addTime( CString& time )
{
	double val = atof( time );
	if ( timeVector.empty() || timeVector.back()->time != val ) {
		RDOTracerTimeNow* timeNow = new RDOTracerTimeNow();
		timeNow->time = val;
		timeNow->eventCount = 0;
		timeVector.push_back( timeNow );
	}
	return timeVector.back();
}

RDOTracerOperation* RDOTracerTrace::getOperation( CString& line )
{
	getNextValue( line );
	return operations.at( atoi( getNextValue( line ) ) - 1 );
}

void RDOTracerTrace::startAction( CString& line, RDOTracerTimeNow* const time  )
{
	getOperation( line )->start( time );
}

void RDOTracerTrace::accomplishAction( CString& line, RDOTracerTimeNow* const time  )
{
	getOperation( line )->accomplish( time );
}

void RDOTracerTrace::irregularEvent( CString& line, RDOTracerTimeNow* const time  )
{
}

void RDOTracerTrace::productionRule( CString& line, RDOTracerTimeNow* const time  )
{
}

RDOTracerResource* RDOTracerTrace::getResource( CString& line )
{
	getNextValue( line );
	RDOTracerResource* res = NULL;
	int findid = atoi( getNextValue( line ) );
	for ( vector< RDOTracerResource* >::iterator it = resources.begin(); it != resources.end(); it++ ) {
		if ( (*it)->id == findid && !(*it)->isErased() ) {
			res = *it;
			break;
		}
	}
	return res;
}

void RDOTracerTrace::resourceCreation( CString& line, RDOTracerTimeNow* const time  )
{
	CString name;
	RDOTracerResType* type = resTypes.at( atoi( getNextValue( line ) ) - 1 );
	int id = atoi( getNextValue( line ) );
	name.Format( "%s #%d", type->Name, id );
	RDOTracerResource* res = new RDOTracerResource( type, name );
	res->id = id;
	res->setParams( line, time );

	resources.push_back( res );
	tree->addResource( res );

	/*RDOTracerResource* res = new RDOTracerResource( resTypes.at( atoi( getNextValue( line ) ) - 1 ) );
	res->id = atoi( getNextValue( line ) );
	CString format = "%s #%d";
	res->Name.Format( format, res->getType()->Name, res->id );
	res->setParams( line, time );

	resources.push_back( res );
	tree->addResource( res );*/
}

void RDOTracerTrace::resourceElimination( CString& line, RDOTracerTimeNow* const time  )
{
	RDOTracerResource* res = getResource( line );
	res->setParams( line, time, true );
	res->setErased( true );
	tree->updateResource( res );
}

void RDOTracerTrace::resourceChanging( CString& line, RDOTracerTimeNow* const time  )
{
	RDOTracerResource* res = getResource( line );
	res->setParams( line, time );
}

void RDOTracerTrace::resultChanging( CString& line, RDOTracerTimeNow* const time  )
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
	CString filter = _T("Trace files (*.trc)|*.TRC||");

	CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter );
	if ( dlg.DoModal() != IDOK )
		return;
	clear();
	log->startTrace();
//	setControls( TRUE );
	startTime = CTime::GetCurrentTime();
	//CString timestr = time.Format("Trace started at %c");
	CString str;
	str.Format( _T("%s\r\nFile name: \"%s\"\r\n\r\n"), startTime.Format( _T("Trace started at %c") ), dlg.GetPathName() );
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
	CString str = getNextString();
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
	CString str;
	str.Format( _T("\r\nEnd of trace with status %s\r\n"), statusStr );
	/*if ( ::IsWindow( graphicsPanel->m_hWnd ) )
		graphicsPanel->addToLog( str );*/
//	setControls( FALSE );
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
