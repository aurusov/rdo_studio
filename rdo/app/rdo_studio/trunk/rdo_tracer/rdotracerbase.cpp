/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerbase.cpp
  \author    Захаров Павел
  \date      01.04.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
#include <boost/range/algorithm/find.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotracerbase.h"
#include "app/rdo_studio/rdo_tracer/rdotracerrestype.h"
#include "app/rdo_studio/rdo_tracer/rdotracerresource.h"
#include "app/rdo_studio/rdo_tracer/rdotracerpattern.h"
#include "app/rdo_studio/rdo_tracer/rdotraceroperation.h"
#include "app/rdo_studio/rdo_tracer/rdotracerresult.h"
#include "app/rdo_studio/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio/src/tracer/ctrls/log_main_wnd.h"
#include "app/rdo_studio/src/chart/chart_tree.h"
#include "app/rdo_studio/src/chart/chart_doc.h"
#include "app/rdo_studio/src/chart/chart_view.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_windows_base.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdo::gui::tracer;

// --------------------------------------------------------------------------------
// -------------------- TracerBase
// --------------------------------------------------------------------------------
TracerBase::TracerBase(CREF(tstring) _thread_name, RDOKernelGUI* _kernel_gui)
	: RDOThreadGUI(_thread_name, _kernel_gui)
	, m_pLog(NULL)
	, m_pChartTree(NULL)
	, m_clipboardFormat(0)
	, m_eventIndex(0)
	, m_drawTrace(true)
	, m_updateAction(RUA_NONE)
{}

TracerBase::~TracerBase()
{
	deleteTrace();
}

TracerResParamInfo* TracerBase::getParamType(rdo::textstream& stream)
{
	boost::optional < TracerResParamType > parType;

	tstring parTypeName;
	stream >> parTypeName;
	if (parTypeName == "E")
	{
		parType = RDOPT_ENUMERATIVE;
	}
	if (parTypeName == "I")
	{
		parType = RDOPT_INTEGER;
	}
	else if (parTypeName == "R")
	{
		parType = RDOPT_REAL;
	}
	else if (parTypeName == "B")
	{
		parType = RDOPT_BOOL;
	}
	else if (parTypeName == "A")
	{
		parType = RDOPT_ARRAY;
	}
	else if (parTypeName == "S")
	{
		parType = RDOPT_STRING;
	}
	ASSERT(parType.is_initialized());

	TracerResParamInfo* param = new TracerResParamInfo(parType.get());
	if (parType == RDOPT_ENUMERATIVE)
	{
		ruint enum_count;
		stream >> enum_count;
		for (ruint j = 0; j < enum_count; j++)
		{
			tstring en_id;
			stream >> en_id;

			tstring en_name;
			stream >> en_name;
			param->addEnumValue(en_name);
		}
	}
	else if (parType == RDOPT_BOOL)
	{
		param->addEnumValue(_T("false"));
		param->addEnumValue(_T("true"));
	}
	else if (parType == RDOPT_ARRAY)
	{
		TracerResParamInfo* arrayItem = getParamType(stream);
		UNUSED(arrayItem);
	}
	return param;
}

TracerResParamInfo* TracerBase::getParam(rdo::textstream& stream)
{
	tstring par_type;
	tstring par_name;
	stream >> par_type;
	stream >> par_name;
	TracerResParamInfo* param = getParamType(stream);
	param->Name = par_name;
	return param;
}

void TracerBase::addResourceType(REF(tstring) s, rdo::textstream& stream)
{
	UNUSED(s);

	LPTracerResType pResType = rdo::Factory<TracerResType>::create(RDOTK_PERMANENT);
	stream >> pResType->Name;
	int paramcount;
	stream >> paramcount;
	for (int i = 0; i < paramcount; i++)
	{
		pResType->addParamInfo(getParam(stream));
	}
	m_resourceTypeList.push_back(pResType);
	m_pChartTree->addResourceType(pResType);
}

void TracerBase::addResource(REF(tstring) s, rdo::textstream& stream)
{
	int rtp;
	tstring res_name;
	stream >> res_name;
	stream >> rtp;
	LPTracerResource pResource = rdo::Factory<TracerResource>::create(m_resourceTypeList.at(rtp - 1), res_name);
	pResource->id = atoi(s.c_str());

	/*int pos = s.find(' ');
	 int endpos = s.rfind(' ');
	 string rtpstr = s.substr(endpos);
	 trim(rtpstr);
	 string name = s.substr(pos, endpos - pos);
	 trim(name);
	 TracerResource* res = new TracerResource(resTypes.at(atoi(rtpstr.c_str()) - 1), name);
	 rtpstr = getNextValue(s);
	 res->id = atoi(rtpstr.c_str());*/

	m_resourceList.push_back(pResource);
	m_pChartTree->addResource(pResource);
}

void TracerBase::addPattern(REF(tstring) s, rdo::textstream& stream)
{
	UNUSED(s);

	tstring pat_name;
	stream >> pat_name;
	tstring pat_type;
	stream >> pat_type;
	TracerPatternKind kind;
	if (pat_type == "A")
	{
		kind = RDOPK_OPERATION;
	}
	else if (pat_type == "I")
	{
		kind = RDOPK_IRREGULAREVENT;
	}
	else if (pat_type == "R")
	{
		kind = RDOPK_RULE;
	}
	else if (pat_type == "K")
	{
		kind = RDOPK_KEYBOARD;
	}
	else
	{
		kind = RDOPK_UNDEFINED;
	}

	LPTracerPattern pat = rdo::Factory<TracerPattern>::create(kind);
	pat->Name = pat_name;

	/*string str = s;
	 string part = getNextValue(str);
	 string name = getNextValue(str);
	 part = getNextValue(str);
	 TracerPatternKind kind;
	 if (part == "A")
	 kind = RDOPK_OPERATION;
	 else if (part == "I")
	 kind = RDOPK_IRREGULAREVENT;
	 else if (part == "R")
	 kind = RDOPK_RULE;
	 else if (part == "K")
	 kind = RDOPK_KEYBOARD;
	 TracerPattern* pat = new TracerPattern(kind);
	 pat->Name = name;*/

	m_patternList.push_back(pat);
	m_pChartTree->addPattern(pat);
	int rel_res_count;
	tstring dummy;
	stream >> rel_res_count;
	for(int i = 0; i < rel_res_count; i++)
	stream >> dummy;
}

void TracerBase::addOperation(REF(tstring) s, rdo::textstream& stream)
{
	UNUSED(s);

	tstring opr_name;
	stream >> opr_name;
	int pat_id;
	stream >> pat_id;
	LPTracerPattern pattern = m_patternList.at(pat_id - 1);

	LPTracerOperationBase opr;

	if (pattern->getPatternKind() != RDOPK_RULE && pattern->getPatternKind() != RDOPK_IRREGULAREVENT)
	{
		opr = rdo::Factory<TracerOperation>::create(pattern);
	}
	else
	{
		opr = rdo::Factory<TracerEvent>::create(pattern);
	}

	opr->setName(opr_name);

	if (pattern->getPatternKind() != RDOPK_IRREGULAREVENT)
	{
		m_operationList.push_back(opr);
	}
	else
	{
		LPTracerEvent pEvent = opr.object_dynamic_cast<TracerEvent>();
		ASSERT(pEvent);
		m_eventList.push_back(pEvent);
	}

	m_pChartTree->addOperation(opr);

	/*int pos = s.find(' ');
	int endpos = s.rfind(' ');
	string patstr = s.substr(endpos);
	trim(patstr);
	TracerPattern* pattern = patterns.at(atoi(patstr.c_str()) -1);
	
	TracerOperationBase* opr = NULL;

	if (pattern->getPatternKind() != RDOPK_RULE)
	opr = new TracerOperation(pattern);
	else
	opr = new TracerEvent(pattern);

	patstr = s.substr(pos, endpos - pos);
	trim(patstr);
	opr->setName(patstr);
	
	operations.push_back(opr);
	tree->addOperation(opr);*/
}

void TracerBase::addResult(REF(tstring) s, rdo::textstream& stream)
{
	int resid;
	stream >> resid;
	tstring res_kind;
	stream >> res_kind;
	TracerResultKind resKind;
	if (res_kind == "watch_par")
	{
		resKind = RDORK_WATCHPAR;
	}
	else if (res_kind == "watch_state")
	{
		resKind = RDORK_WATCHSTATE;
	}
	else if (res_kind == "watch_quant")
	{
		resKind = RDORK_WATCHQUANT;
	}
	else if (res_kind == "watch_value")
	{
		resKind = RDORK_WATCHVALUE;
	}
	else
	{
		resKind = RDORK_UNDEFINED;
	}
	ASSERT(resKind != RDORK_UNDEFINED);

	LPTracerResult res = rdo::Factory<TracerResult>::create(resKind);
	res->setName(s);
	res->id = resid;
	/*string str = s;
	string name = getNextValue(str);
	string part = getNextValue(str);
	int resid = atoi(part.c_str());
	part = getNextValue(str);
	TracerResultKind resKind;
	if (part == "watch_par")
	resKind = RDORK_WATCHPAR;
	else if (part == "watch_state")
	resKind = RDORK_WATCHSTATE;
	else if (part == "watch_quant")
	resKind = RDORK_WATCHQUANT;
	else if (part == "watch_value")
	resKind = RDORK_WATCHVALUE;
	TracerResult* res = new TracerResult(resKind);
	res->setName(name);
	res->id = resid;*/
	m_resultList.push_back(res);
	m_pChartTree->addResult(res);
}

void TracerBase::dispatchNextString(REF(tstring) line)
{
	if (line.empty())
	return;

	tstring key = getNextValue(line);
	TracerTimeNow* timeNow;
	if (key != "SO" && key.find("ST") == tstring::npos && key != "SD" && key.find("SE") == tstring::npos)
	{
		timeNow = addTime(getNextValue(line));
	}
	else
	{
		timeNow = m_timeList.back();
	}

	if (key == "ES")
	{
	}
	else if (key == "EB")
	{
		startAction(line, timeNow);
	}
	else if (key == "EF")
	{
		accomplishAction(line, timeNow);
	}
	else if (key == "EI")
	{
		irregularEvent(line, timeNow);
	}
	else if (key == "ER")
	{
		productionRule(line, timeNow);
	}
	else if (key == "RC" || key == "SRC")
	{
		m_pResource = resourceCreation(line, timeNow);
		m_updateAction = RUA_ADD;
#ifdef RDOSIM_COMPATIBLE
	}
	else if (key == "RE" || key == "SRE")
	{
		tstring copy1 = line;
		m_pResource = resourceElimination(line, timeNow);
		if (!m_pResource)
		{
			tstring copy2 = copy1;
			m_pResource = resourceCreation(copy1, timeNow);
			m_pChartTree->addResource(m_pResource);
			m_pResource = resourceElimination(copy2, timeNow);
		}
		m_updateAction = RUA_UPDATE;
	}
	else if (key == "RK" || key == "SRK")
	{
		tstring copy = line;
		TracerResource* res = resourceChanging(line, timeNow);
		if (!res)
		{
			m_pResource = resourceCreation(copy, timeNow);
			m_updateAction = RUA_ADD;
		}
#else
	}
	else if (key == "RK" || key == "SRK" || key == "RE" || key == "SRE")
	{
		rbool re = key == "RE" || key == "SRE";
		tstring copy1 = line;
		LPTracerResource res = resourceChanging(line, timeNow);
		if (!res)
		{
			tstring copy2 = copy1;
			m_pResource = resourceCreation(copy2, timeNow);
			m_pChartTree->addResource(m_pResource);
		}
		if (re)
		{
			m_pResource = resourceElimination(copy1, timeNow);
			m_updateAction = RUA_UPDATE;
		}
#endif
	}
	else if (key == "V")
	{
		resultChanging(line, timeNow);
	}
	/* else if (key == "$Status") {
	} else if (key.Find("DPS", 0) != -1) {
	} else if (key == "SB") {
	} else if (key == "SO") {
	} else if (key == "STN") {
	} else if (key == "STD") {
	} else if (key == "STR") {
	} else if (key == "SD") {
	} else if (key == "SES") {
	} else if (key == "SEN") {
	} else if (key == "SEM") {
	} else if (key == "SEF") {
	} else if (key == "SEU") {
	}*/
}

tstring TracerBase::getNextValue(REF(tstring) line)
{
	int posstart = line.find_first_not_of(' ');
	int posend;
	tstring res;
	if (line[posstart] == _T('['))
	{
		//! @todo Массив просто игнорируется, график по нему не строится. Заплатка.
		posend = line.find_first_of(']', posstart);
		res = _T("0");
	}
	else
	{
		posend = line.find_first_of(' ', posstart);
		res = line.substr(posstart, posend - posstart);
	}
	line.erase(0, posend + 1);
	rdo::trim(res);
	return res;
}

TracerTimeNow* TracerBase::addTime(CREF(tstring) time)
{
	double val = atof(time.c_str());
	rbool empty = m_timeList.empty();
	TracerTimeNow* last = NULL;
	if (!empty)
	{
		last = m_timeList.back();
	}
	if (empty || last->time != val)
	{
		TracerTimeNow* timeNow = new TracerTimeNow(val);
		m_timeList.push_back(timeNow);
		m_eventIndex = 0;
		BOOST_FOREACH(const LPTracerOperationBase& pOperationBase, m_operationList)
		{
			pOperationBase->monitorTime(timeNow, m_eventIndex);
		}
		BOOST_FOREACH(const LPTracerEvent& pEvent, m_eventList)
		{
			pEvent->monitorTime(timeNow, m_eventIndex);
		}
	}
	else
	{
		last->eventCount++;
		m_eventIndex++;
		BOOST_FOREACH(RDOStudioChartDoc* pDocument, m_documentList)
		{
			pDocument->incTimeEventsCount(last);
		}
	}
	return m_timeList.back();
}

LPTracerOperationBase TracerBase::getOperation(REF(tstring) line)
{
	getNextValue(line);
	return m_operationList.at(atoi(getNextValue(line).c_str()) - 1);
}

void TracerBase::startAction(REF(tstring) line, TracerTimeNow* const time)
{
	LPTracerOperation pOperation = getOperation(line).object_dynamic_cast<TracerOperation>();
	ASSERT(pOperation);
	pOperation->start(time, m_eventIndex);
}

void TracerBase::accomplishAction(REF(tstring) line, TracerTimeNow* const time)
{
	LPTracerOperation pOperation = getOperation(line).object_dynamic_cast<TracerOperation>();
	ASSERT(pOperation);
	pOperation->accomplish(time, m_eventIndex);
}

void TracerBase::irregularEvent(REF(tstring) line, TracerTimeNow* const time)
{
#ifdef RDOSIM_COMPATIBLE
	m_eventList.at(atoi(getNextValue(line).c_str()) - 1)->occurs(time, m_eventIndex);
#else
	m_eventList.at(atoi(getNextValue(line).c_str()) - 1)->occurs(time, m_eventIndex);
#endif
}

void TracerBase::productionRule(REF(tstring) line, TracerTimeNow* const time)
{
	LPTracerEvent pEvent = getOperation(line).object_dynamic_cast<TracerEvent>();
	ASSERT(pEvent);
	pEvent->occurs(time, m_eventIndex);
}

LPTracerResource TracerBase::getResource(REF(tstring) line)
{
	getNextValue(line);
	LPTracerResource result;
	int findid = atoi(getNextValue(line).c_str());
	int i = 0;
	BOOST_FOREACH(const LPTracerResource& pResource, m_resourceList)
	{
		if (pResource->id == findid)
		if (pResource->id == findid && !pResource->isErased())
		{
			result = pResource;
			break;
		}
		i++;
	}
	return result;
}

LPTracerResource TracerBase::resourceCreation(REF(tstring) line, TracerTimeNow* const time)
{
	ruint typeID = atoi(getNextValue(line).c_str()) - 1;
	ASSERT(typeID < m_resourceTypeList.size());
	LPTracerResType type = m_resourceTypeList.at(typeID);
	int id = atoi(getNextValue(line).c_str());
	LPTracerResource res = rdo::Factory<TracerResource>::create(type, rdo::format("%s #%d", type->Name.c_str(), id));
	res->id = id;
	res->setParams(line, time, m_eventIndex);

	m_resourceList.push_back(res);
	//tree->addResource(res);
	return res;
}

LPTracerResource TracerBase::resourceElimination(REF(tstring) line, TracerTimeNow* const time)
{
	LPTracerResource res = getResource(line);
	if (!res)
		return NULL;
#ifdef RDOSIM_COMPATIBLE
	res->setParams(line, time, m_eventIndex, true);
#else
	res->setParams(line, time, m_eventIndex, false);
#endif
	res->setErased(true);
	//tree->updateResource(res);
	return res;
}

LPTracerResource TracerBase::resourceChanging(REF(tstring) line, TracerTimeNow* const time)
{
	LPTracerResource res = getResource(line);
	if (res)
	{
		res->setParams(line, time, m_eventIndex);
	}
	return res;
}

LPTracerResult TracerBase::getResult(REF(tstring) line)
{
	LPTracerResult result;
	int findid = atoi(getNextValue(line).c_str());
	BOOST_FOREACH(const LPTracerResult& pResult, m_resultList)
	{
		if (pResult->id == findid)
		{
			result = pResult;
			break;
		}
	}
	return result;
}

void TracerBase::resultChanging(REF(tstring) line, TracerTimeNow* const time)
{
	getResult(line)->setValue(line, time, m_eventIndex);
}

void TracerBase::deleteTrace()
{
	m_resourceList.clear();
	m_resourceTypeList.clear();
	m_operationList.clear();
	m_eventList.clear();
	m_patternList.clear();
	m_resultList.clear();
	std::list<TracerTimeNow*>::iterator it = m_timeList.begin();
	while (it != m_timeList.end())
	{
		delete *it;
		++it;
	}
	m_timeList.clear();
}

void TracerBase::clear()
{
	clearCharts();
	deleteTrace();
	if (m_pChartTree)
		m_pChartTree->clear();
	if (m_pLog)
		m_pLog->view().clear();
}

void TracerBase::clearCharts()
{
	BOOST_FOREACH(RDOStudioChartDoc* pDocument, m_documentList)
	{
		pDocument->getFirstView()->parentWidget()->parentWidget()->close();
	}
}

void TracerBase::setLog(PTR(LogMainWnd) pTracerLog)
{
	m_pLog = pTracerLog;
}

void TracerBase::setTree(PTR(ChartTree) pTreeCtrl)
{
	m_pChartTree = pTreeCtrl;
}

void TracerBase::getModelStructure(rdo::textstream& stream)
{
	tstring s;

	while (!stream.eof())
	{
		stream >> s;
		if (!s.empty())
		{
			tstring::size_type pos = tstring::npos;
			if (s.find("$Resource_type") != tstring::npos)
			{
				do
				{
					stream >> s;
					pos = s.find("$Resources");

					// AB if model has no $Resources at all,
					// we get endless cycle here
					if (pos == tstring::npos)
					{
						pos = s.find("$Pattern");
					}

					if (!s.empty() && pos == tstring::npos)
					{
						addResourceType(s, stream);
					}
				} while (pos == tstring::npos && !stream.eof());
			}
			if (s.find("$Resources") != tstring::npos)
			{
				do
				{
					stream >> s;
					pos = s.find("$Pattern");
					if (!s.empty() && pos == tstring::npos)
					{
						addResource(s, stream);
					}
				} while (pos == tstring::npos && !stream.eof());
			}
			if (s.find("$Pattern") != tstring::npos)
			{
				do
				{
					stream >> s;
					pos = s.find("$Activities");
					if (!s.empty() && pos == tstring::npos)
					{
						addPattern(s, stream);
					}
				} while (pos == tstring::npos && !stream.eof());
			}
			if (s.find("$Activities") != tstring::npos)
			{
				do
				{
					stream >> s;
					pos = s.find("$Watching");
					if (!s.empty() && pos == tstring::npos)
					{
						addOperation(s, stream);
					}
				} while (pos == tstring::npos && !stream.eof());
			}
			if (s.find("$Watching") != tstring::npos && !stream.eof())
			{
				do
				{
					s = "";
					stream >> s;
					if (!s.empty())
					{
						addResult(s, stream);
					}
				} while (!stream.eof());
			}
		}
	}

	/*stream >> s;

	if (s.find("$Resource_type") != string::npos) {
	do {
	stream >> s;
	if (!s.empty())
	addResourceType(s, stream);
	} while (s != "");
	}
	
	if (s.find("$Resources") != string::npos) {
	do {
	stream >> s;
	if (!s.empty())
	addResource(s, stream);
	} while (s != "");
	}

	if (s.find("$Pattern") != string::npos) {
	do {
	stream >> s;
	if (!s.empty())
	addPattern(s, stream);
	} while (s != "");
	}

	if (s.find("$Activities") != string::npos) {
	do {
	stream >> s;
	if (!s.empty())
	addOperation(s, stream);
	} while (s != "");
	do {
	stream >> s;
	if (!s.empty())
	addIrregularEvent(s, stream);
	} while (s != "");
	}

	if (s.find("$Watching") != string::npos) {
	do {
	stream >> s;
	if (!s.empty())
	addResult(s, stream);
	} while (s != "");
	}*/
}

void TracerBase::getTraceString(tstring trace_string)
{
	if (m_pLog)
	{
		m_pLog->view().push_back(trace_string);
	}

	m_updateAction = RUA_NONE;
	m_pResource = NULL;

	dispatchNextString(trace_string);

	switch (m_updateAction)
	{
	case RUA_ADD:
		m_pChartTree->addResource(m_pResource);
		break;

	case RUA_UPDATE:
		m_pChartTree->updateResource(m_pResource);
		break;

	default:
		break;
	}
}

RDOStudioChartDoc* TracerBase::createNewChart()
{
	RDOStudioChartDoc* pDoc = new RDOStudioChartDoc();
	ChartViewMainWnd* pView = new ChartViewMainWnd(NULL, pDoc, false);
	studioApp.getIMainWnd()->addSubWindow(pView);
	pDoc->attachView(&pView->view());
	pDoc->autoTitle();

	pView->parentWidget()->setWindowIcon(QIcon(QString::fromUtf8(":/images/images/mdi_chart.png")));

	return pDoc;
}

RDOStudioChartDoc* TracerBase::addSerieToChart(CREF(LPTracerSerie) pSerie, RDOStudioChartDoc* chart)
{
	if (!chart)
	{
		chart = createNewChart();
	}
	if (chart)
	{
		chart->addSerie(pSerie);
	}

	return chart;
}

void TracerBase::addChart(RDOStudioChartDoc* const chart)
{
	m_documentList.push_back(chart);
}

void TracerBase::removeChart(RDOStudioChartDoc* chart)
{
	DocumentList::iterator it = boost::range::find(m_documentList, chart);
	if (it != m_documentList.end())
	{
		m_documentList.erase(it);
	}
}

void TracerBase::updateChartsStyles() const
{
	BOOST_FOREACH(RDOStudioChartDoc* pDocument, m_documentList)
	{
		pDocument->setStyle(&studioApp.getStyle()->style_chart);
	}
}

void TracerBase::setModelName(CREF(QString) name) const
{
	if (m_pChartTree)
	{
		m_pChartTree->setModelName(name);
	}
}

void TracerBase::setDrawTrace(const rbool value)
{
	if (m_drawTrace != value)
	{
		m_drawTrace = value;
		if (!m_drawTrace)
		{
			clearCharts();
		}
		m_pLog->view().setDrawLog(value);
	}
}

rbool TracerBase::getDrawTrace() const
{
	return m_drawTrace;
}

void TracerBase::registerClipboardFormat()
{
	m_clipboardFormat = ::RegisterClipboardFormat(rdo::format(ID_RAO_CLIPBRD).c_str());
}

UINT const TracerBase::getClipboardFormat() const
{
	return m_clipboardFormat;
}
