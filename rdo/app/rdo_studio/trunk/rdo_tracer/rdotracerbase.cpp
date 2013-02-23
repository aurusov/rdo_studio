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
	, log(NULL)
	, tree(NULL)
	, clipboardFormat(0)
	, eventIndex(0)
	, drawTrace(true)
	, action(RUA_NONE)
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

	TracerResType* type = new TracerResType(RDOTK_PERMANENT);
	stream >> type->Name;
	int paramcount;
	stream >> paramcount;
	for (int i = 0; i < paramcount; i++)
	{
		type->addParamInfo(getParam(stream));
	}
	resTypes.push_back(type);
	tree->addResourceType(type);
}

void TracerBase::addResource(REF(tstring) s, rdo::textstream& stream)
{
	int rtp;
	tstring res_name;
	stream >> res_name;
	stream >> rtp;
	TracerResource* res = new TracerResource(resTypes.at(rtp - 1), res_name);
	res->id = atoi(s.c_str());

	/*int pos = s.find(' ');
	 int endpos = s.rfind(' ');
	 string rtpstr = s.substr(endpos);
	 trim(rtpstr);
	 string name = s.substr(pos, endpos - pos);
	 trim(name);
	 TracerResource* res = new TracerResource(resTypes.at(atoi(rtpstr.c_str()) - 1), name);
	 rtpstr = getNextValue(s);
	 res->id = atoi(rtpstr.c_str());*/

	resources.push_back(res);
	tree->addResource(res);
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

	TracerPattern* pat = new TracerPattern(kind);
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

	patterns.push_back(pat);
	tree->addPattern(pat);
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
	TracerPattern* pattern = patterns.at(pat_id - 1);

	TracerOperationBase* opr = NULL;

	if (pattern->getPatternKind() != RDOPK_RULE && pattern->getPatternKind() != RDOPK_IRREGULAREVENT)
	{
		opr = new TracerOperation(pattern);
	}
	else
	{
		opr = new TracerEvent(pattern);
	}

	opr->setName(opr_name);

	if (pattern->getPatternKind() != RDOPK_IRREGULAREVENT)
	{
		operations.push_back(opr);
	}
	else
	{
		irregularEvents.push_back(static_cast<TracerEvent*>(opr));
	}

	tree->addOperation(opr);

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

/*void TracerBase::addIrregularEvent(REF(tstring) s, rdo::textstream& stream)
{
int pos = s.find(' ');
int endpos = s.rfind(' ');
string patstr = s.substr(endpos);
trim(patstr);
TracerEvent* event = new TracerEvent(patterns.at(atoi(patstr.c_str()) -1));

patstr = s.substr(pos, endpos - pos);
trim(patstr);
event->setName(patstr);

irregularEvents.push_back(event);
tree->addOperation(event);
}*/

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

	TracerResult* res = new TracerResult(resKind);
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
	results.push_back(res);
	tree->addResult(res);
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
		timeNow = timeList.back();
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
		resource = resourceCreation(line, timeNow);
		action = RUA_ADD;
#ifdef RDOSIM_COMPATIBLE
	}
	else if (key == "RE" || key == "SRE")
	{
		tstring copy1 = line;
		resource = resourceElimination(line, timeNow);
		if (!resource)
		{
			tstring copy2 = copy1;
			resource = resourceCreation(copy1, timeNow);
			tree->addResource(resource);
			resource = resourceElimination(copy2, timeNow);
		}
		action = RUA_UPDATE;
	}
	else if (key == "RK" || key == "SRK")
	{
		tstring copy = line;
		TracerResource* res = resourceChanging(line, timeNow);
		if (!res)
		{
			resource = resourceCreation(copy, timeNow);
			action = RUA_ADD;
		}
#else
	}
	else if (key == "RK" || key == "SRK" || key == "RE" || key == "SRE")
	{
		rbool re = key == "RE" || key == "SRE";
		tstring copy1 = line;
		TracerResource* res = resourceChanging(line, timeNow);
		if (!res)
		{
			tstring copy2 = copy1;
			resource = resourceCreation(copy2, timeNow);
			tree->addResource(resource);
		}
		if (re)
		{
			resource = resourceElimination(copy1, timeNow);
			action = RUA_UPDATE;
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
	rbool empty = timeList.empty();
	TracerTimeNow* last = NULL;
	if (!empty)
	{
		last = timeList.back();
	}
	if (empty || last->time != val)
	{
		TracerTimeNow* timeNow = new TracerTimeNow(val);
		timeList.push_back(timeNow);
		eventIndex = 0;
		for (std::vector <TracerOperationBase*>::iterator it = operations.begin(); it != operations.end(); ++it)
		{
			(*it)->monitorTime(timeNow, eventIndex);
		}
		for (std::vector <TracerEvent*>::iterator it_ie = irregularEvents.begin(); it_ie != irregularEvents.end(); ++it_ie)
		{
			(*it_ie)->monitorTime(timeNow, eventIndex);
		}
	}
	else
	{
		last->eventCount++;
		eventIndex++;
		for (std::vector< RDOStudioChartDoc* >::iterator it = charts.begin(); it != charts.end(); ++it)
		{
			(*it)->incTimeEventsCount(last);
		}
	}
	return timeList.back();
}

TracerOperationBase* TracerBase::getOperation(REF(tstring) line)
{
	getNextValue(line);
	return operations.at(atoi(getNextValue(line).c_str()) - 1);
}

void TracerBase::startAction(REF(tstring) line, TracerTimeNow* const time)
{
	static_cast<TracerOperation*>(getOperation(line))->start(time, eventIndex);
}

void TracerBase::accomplishAction(REF(tstring) line, TracerTimeNow* const time)
{
	static_cast<TracerOperation*>(getOperation(line))->accomplish(time, eventIndex);
}

void TracerBase::irregularEvent(REF(tstring) line, TracerTimeNow* const time)
{
#ifdef RDOSIM_COMPATIBLE
	irregularEvents.at(atoi(getNextValue(line).c_str()) - 1)->occurs(time, eventIndex);
#else
	irregularEvents.at(atoi(getNextValue(line).c_str()) - 1)->occurs(time, eventIndex);
#endif
}

void TracerBase::productionRule(REF(tstring) line, TracerTimeNow* const time)
{
	static_cast<TracerEvent*>(getOperation(line))->occurs(time, eventIndex);
}

TracerResource* TracerBase::getResource(REF(tstring) line)
{
	getNextValue(line);
	TracerResource* res = NULL;
	int findid = atoi(getNextValue(line).c_str());
	int i = 0;
	for (std::vector< TracerResource* >::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if ((*it)->id == findid)
		if ((*it)->id == findid && !(*it)->isErased())
		{
			res = *it;
			break;
		}
		i++;
	}
	return res;
}

TracerResource* TracerBase::resourceCreation(REF(tstring) line, TracerTimeNow* const time)
{
	ruint typeID = atoi(getNextValue(line).c_str()) - 1;
	ASSERT(typeID < resTypes.size());
	TracerResType* type = resTypes.at(typeID);
	int id = atoi(getNextValue(line).c_str());
	TracerResource* res = new TracerResource(type, rdo::format("%s #%d", type->Name.c_str(), id));
	res->id = id;
	res->setParams(line, time, eventIndex);

	resources.push_back(res);
	//tree->addResource(res);
	return res;
}

TracerResource* TracerBase::resourceElimination(REF(tstring) line, TracerTimeNow* const time)
{
	TracerResource* res = getResource(line);
	if (!res)
		return NULL;
#ifdef RDOSIM_COMPATIBLE
	res->setParams(line, time, eventIndex, true);
#else
	res->setParams(line, time, eventIndex, false);
#endif
	res->setErased(true);
	//tree->updateResource(res);
	return res;
}

TracerResource* TracerBase::resourceChanging(REF(tstring) line, TracerTimeNow* const time)
{
	TracerResource* res = getResource(line);
	if (res)
	{
		res->setParams(line, time, eventIndex);
	}
	return res;
}

TracerResult* TracerBase::getResult(REF(tstring) line)
{
	TracerResult* res = NULL;
	int findid = atoi(getNextValue(line).c_str());
	for (std::vector< TracerResult* >::iterator it = results.begin(); it != results.end(); ++it)
	{
		if ((*it)->id == findid)
		{
			res = *it;
			break;
		}
	}
	return res;
}

void TracerBase::resultChanging(REF(tstring) line, TracerTimeNow* const time)
{
	getResult(line)->setValue(line, time, eventIndex);
}

void TracerBase::deleteTrace()
{
	int count = resources.size();
	int i;
	for (i = 0; i < count; i++)
	{
		delete resources.at(i);
	}
	resources.clear();
	count = resTypes.size();
	for (i = 0; i < count; i++)
	{
		delete resTypes.at(i);
	}
	resTypes.clear();
	count = operations.size();
	for (i = 0; i < count; i++)
	{
		delete operations.at(i);
	}
	operations.clear();
	count = irregularEvents.size();
	for (i = 0; i < count; i++)
	{
		delete irregularEvents.at(i);
	}
	irregularEvents.clear();
	count = patterns.size();
	for (i = 0; i < count; i++)
	{
		delete patterns.at(i);
	}
	patterns.clear();
	count = results.size();
	for (i = 0; i < count; i++)
	{
		delete results.at(i);
	}
	results.clear();
	std::list<TracerTimeNow*>::iterator it = timeList.begin();
	while (it != timeList.end())
	{
		delete *it;
		++it;
	}
	timeList.clear();
}

void TracerBase::clear()
{
	clearCharts();
	deleteTrace();
	if (tree)
		tree->clear();
	if (log)
		log->view().clear();
}

void TracerBase::clearCharts()
{
	while (!charts.empty())
	{
		charts.front()->getFirstView()->parentWidget()->parentWidget()->close();
	}
}

void TracerBase::setLog(PTR(LogMainWnd) pTracerLog)
{
	log = pTracerLog;
}

void TracerBase::setTree(PTR(ChartTree) pTreeCtrl)
{
	tree = pTreeCtrl;
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
	if (log)
	{
		log->view().push_back(trace_string);
	}

	action = RUA_NONE;
	resource = NULL;

	dispatchNextString(trace_string);

	switch (action)
	{
	case RUA_ADD:
		tree->addResource(resource);
		break;

	case RUA_UPDATE:
		tree->updateResource(resource);
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

RDOStudioChartDoc* TracerBase::addSerieToChart(TracerSerie* const serie, RDOStudioChartDoc* chart)
{
	if (!chart)
	{
		chart = createNewChart();
	}
	if (chart)
	{
		chart->addSerie(serie);
	}

	return chart;
}

void TracerBase::addChart(RDOStudioChartDoc* const chart)
{
	charts.push_back(chart);
}

void TracerBase::removeChart(RDOStudioChartDoc* chart)
{
	for (std::vector<RDOStudioChartDoc*>::iterator it = charts.begin(); it != charts.end(); ++it)
	{
		if ((*it) == chart)
		{
			charts.erase(it);
			break;
		}
	}
}

void TracerBase::updateChartsStyles() const
{
	BOOST_FOREACH(RDOStudioChartDoc * pDoc, charts)
	{
		pDoc->setStyle(&studioApp.getStyle()->style_chart);
	}
}

void TracerBase::setModelName(CREF(QString) name) const
{
	if (tree)
	{
		tree->setModelName(name);
	}
}

void TracerBase::setDrawTrace(const rbool value)
{
	if (drawTrace != value)
	{
		drawTrace = value;
		if (!drawTrace)
		{
			clearCharts();
		}
		log->view().setDrawLog(value);
	}
}
