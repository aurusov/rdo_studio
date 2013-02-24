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

TracerResourceParamInfo* TracerBase::getParamType(rdo::textstream& stream)
{
	boost::optional<TracerResourceParamInfo::ParamType> parType;

	tstring parTypeName;
	stream >> parTypeName;
	if (parTypeName == "E")
	{
		parType = TracerResourceParamInfo::PT_ENUMERATIVE;
	}
	if (parTypeName == "I")
	{
		parType = TracerResourceParamInfo::PT_INTEGER;
	}
	else if (parTypeName == "R")
	{
		parType = TracerResourceParamInfo::PT_REAL;
	}
	else if (parTypeName == "B")
	{
		parType = TracerResourceParamInfo::PT_BOOL;
	}
	else if (parTypeName == "A")
	{
		parType = TracerResourceParamInfo::PT_ARRAY;
	}
	else if (parTypeName == "S")
	{
		parType = TracerResourceParamInfo::PT_STRING;
	}
	ASSERT(parType.is_initialized());

	TracerResourceParamInfo* pParam = new TracerResourceParamInfo(parType.get());
	if (parType == TracerResourceParamInfo::PT_ENUMERATIVE)
	{
		ruint enumCount;
		stream >> enumCount;
		for (ruint j = 0; j < enumCount; j++)
		{
			tstring enumID;
			stream >> enumID;

			tstring enumName;
			stream >> enumName;
			pParam->addEnumValue(enumName);
		}
	}
	else if (parType == TracerResourceParamInfo::PT_BOOL)
	{
		pParam->addEnumValue(_T("false"));
		pParam->addEnumValue(_T("true"));
	}
	else if (parType == TracerResourceParamInfo::PT_ARRAY)
	{
		TracerResourceParamInfo* pArrayItem = getParamType(stream);
		UNUSED(pArrayItem);
	}
	return pParam;
}

TracerResourceParamInfo* TracerBase::getParam(rdo::textstream& stream)
{
	tstring paramType;
	tstring paramName;
	stream >> paramType;
	stream >> paramName;
	TracerResourceParamInfo* pParam = getParamType(stream);
	pParam->setName(QString::fromLocal8Bit(paramName.c_str()));
	return pParam;
}

void TracerBase::addResourceType(REF(tstring), rdo::textstream& stream)
{
	LPTracerResourceType pResourceType = rdo::Factory<TracerResourceType>::create(TracerResourceType::RDOTK_PERMANENT);
	tstring resourceTypeName;
	stream >> resourceTypeName;
	pResourceType->setName(QString::fromLocal8Bit(resourceTypeName.c_str()));
	int paramCount;
	stream >> paramCount;
	for (int i = 0; i < paramCount; i++)
	{
		pResourceType->addParamInfo(getParam(stream));
	}
	m_resourceTypeList.push_back(pResourceType);
	m_pChartTree->addResourceType(pResourceType);
}

void TracerBase::addResource(REF(tstring) s, rdo::textstream& stream)
{
	int resourceTypeID;
	tstring resourceName;
	stream >> resourceName;
	stream >> resourceTypeID;
	LPTracerResource pResource = rdo::Factory<TracerResource>::create(
		m_resourceTypeList.at(resourceTypeID - 1),
		QString::fromLocal8Bit(resourceName.c_str()),
		atoi(s.c_str())
	);

	m_resourceList.push_back(pResource);
	m_pChartTree->addResource(pResource);
}

void TracerBase::addPattern(REF(tstring), rdo::textstream& stream)
{
	tstring patternName;
	stream >> patternName;
	tstring patternType;
	stream >> patternType;
	TracerPattern::Kind kind;
	if (patternType == "A")
	{
		kind = TracerPattern::PK_OPERATION;
	}
	else if (patternType == "I")
	{
		kind = TracerPattern::PK_EVENT;
	}
	else if (patternType == "R")
	{
		kind = TracerPattern::PK_RULE;
	}
	else if (patternType == "K")
	{
		kind = TracerPattern::PK_KEYBOARD;
	}
	else
	{
		kind = TracerPattern::PK_UNDEFINED;
	}

	LPTracerPattern pPattern = rdo::Factory<TracerPattern>::create(kind);
	pPattern->setName(QString::fromLocal8Bit(patternName.c_str()));

	m_patternList.push_back(pPattern);
	m_pChartTree->addPattern(pPattern);
	int relResCount;
	stream >> relResCount;
	tstring dummy;
	for (int i = 0; i < relResCount; i++)
	{
		stream >> dummy;
	}
}

void TracerBase::addOperation(REF(tstring), rdo::textstream& stream)
{
	tstring operationName;
	stream >> operationName;
	int patternID;
	stream >> patternID;
	LPTracerPattern pPattern = m_patternList.at(patternID - 1);

	LPTracerOperationBase pOperationBase;
	QString name = QString::fromLocal8Bit(operationName.c_str());

	if (pPattern->getKind() != TracerPattern::PK_RULE && pPattern->getKind() != TracerPattern::PK_EVENT)
	{
		pOperationBase = rdo::Factory<TracerOperation>::create(pPattern, name);
	}
	else
	{
		pOperationBase = rdo::Factory<TracerEvent>::create(pPattern, name);
	}

	if (pPattern->getKind() != TracerPattern::PK_EVENT)
	{
		m_operationList.push_back(pOperationBase);
	}
	else
	{
		LPTracerEvent pEvent = pOperationBase.object_dynamic_cast<TracerEvent>();
		ASSERT(pEvent);
		m_eventList.push_back(pEvent);
	}

	m_pChartTree->addOperation(pOperationBase);
}

void TracerBase::addResult(REF(tstring) s, rdo::textstream& stream)
{
	int resultID;
	stream >> resultID;
	tstring resultKind;
	stream >> resultKind;
	TracerResult::Kind kind;
	if (resultKind == "watch_par")
	{
		kind = TracerResult::RK_WATCHPAR;
	}
	else if (resultKind == "watch_state")
	{
		kind = TracerResult::RK_WATCHSTATE;
	}
	else if (resultKind == "watch_quant")
	{
		kind = TracerResult::RK_WATCHQUANT;
	}
	else if (resultKind == "watch_value")
	{
		kind = TracerResult::RK_WATCHVALUE;
	}
	else
	{
		kind = TracerResult::RK_UNDEFINED;
	}
	ASSERT(kind != TracerResult::RK_UNDEFINED);

	LPTracerResult pResult = rdo::Factory<TracerResult>::create(QString::fromLocal8Bit(s.c_str()), kind, resultID);
	m_resultList.push_back(pResult);
	m_pChartTree->addResult(pResult);
}

void TracerBase::dispatchNextString(REF(tstring) line)
{
	if (line.empty())
		return;

	tstring key = getNextValue(line);
	TracerTimeNow* pTimeNow;
	if (key != "SO" && key.find("ST") == tstring::npos && key != "SD" && key.find("SE") == tstring::npos)
	{
		pTimeNow = addTime(getNextValue(line));
	}
	else
	{
		pTimeNow = m_timeList.back();
	}

	if (key == "ES")
	{
	}
	else if (key == "EB")
	{
		startAction(line, pTimeNow);
	}
	else if (key == "EF")
	{
		accomplishAction(line, pTimeNow);
	}
	else if (key == "EI")
	{
		irregularEvent(line, pTimeNow);
	}
	else if (key == "ER")
	{
		productionRule(line, pTimeNow);
	}
	else if (key == "RC" || key == "SRC")
	{
		m_pResource = resourceCreation(line, pTimeNow);
		m_updateAction = RUA_ADD;
#ifdef RDOSIM_COMPATIBLE
	}
	else if (key == "RE" || key == "SRE")
	{
		tstring copy1 = line;
		m_pResource = resourceElimination(line, pTimeNow);
		if (!m_pResource)
		{
			tstring copy2 = copy1;
			m_pResource = resourceCreation(copy1, pTimeNow);
			m_pChartTree->addResource(m_pResource);
			m_pResource = resourceElimination(copy2, pTimeNow);
		}
		m_updateAction = RUA_UPDATE;
	}
	else if (key == "RK" || key == "SRK")
	{
		tstring copy = line;
		TracerResource* res = resourceChanging(line, pTimeNow);
		if (!res)
		{
			m_pResource = resourceCreation(copy, pTimeNow);
			m_updateAction = RUA_ADD;
		}
#else
	}
	else if (key == "RK" || key == "SRK" || key == "RE" || key == "SRE")
	{
		rbool re = key == "RE" || key == "SRE";
		tstring copy1 = line;
		LPTracerResource pResource = resourceChanging(line, pTimeNow);
		if (!pResource)
		{
			tstring copy2 = copy1;
			m_pResource = resourceCreation(copy2, pTimeNow);
			m_pChartTree->addResource(m_pResource);
		}
		if (re)
		{
			m_pResource = resourceElimination(copy1, pTimeNow);
			m_updateAction = RUA_UPDATE;
		}
#endif
	}
	else if (key == "V")
	{
		resultChanging(line, pTimeNow);
	}
}

tstring TracerBase::getNextValue(REF(tstring) line)
{
	int posStart = line.find_first_not_of(' ');
	int posEnd;
	tstring result;
	if (line[posStart] == _T('['))
	{
		//! @todo Массив просто игнорируется, график по нему не строится. Заплатка.
		posEnd = line.find_first_of(']', posStart);
		result = _T("0");
	}
	else
	{
		posEnd = line.find_first_of(' ', posStart);
		result = line.substr(posStart, posEnd - posStart);
	}
	line.erase(0, posEnd + 1);
	rdo::trim(result);
	return result;
}

TracerTimeNow* TracerBase::addTime(CREF(tstring) time)
{
	double val = atof(time.c_str());
	rbool empty = m_timeList.empty();
	TracerTimeNow* pLastTime = NULL;
	if (!empty)
	{
		pLastTime = m_timeList.back();
	}
	if (empty || pLastTime->time != val)
	{
		TracerTimeNow* pTimeNow = new TracerTimeNow(val);
		m_timeList.push_back(pTimeNow);
		m_eventIndex = 0;
		BOOST_FOREACH(const LPTracerOperationBase& pOperationBase, m_operationList)
		{
			pOperationBase->monitorTime(pTimeNow, m_eventIndex);
		}
		BOOST_FOREACH(const LPTracerEvent& pEvent, m_eventList)
		{
			pEvent->monitorTime(pTimeNow, m_eventIndex);
		}
	}
	else
	{
		pLastTime->eventCount++;
		m_eventIndex++;
		BOOST_FOREACH(RDOStudioChartDoc* pDocument, m_documentList)
		{
			pDocument->incTimeEventsCount(pLastTime);
		}
	}
	return m_timeList.back();
}

LPTracerOperationBase TracerBase::getOperation(REF(tstring) line)
{
	getNextValue(line);
	return m_operationList.at(atoi(getNextValue(line).c_str()) - 1);
}

void TracerBase::startAction(REF(tstring) line, TracerTimeNow* const pTime)
{
	LPTracerOperation pOperation = getOperation(line).object_dynamic_cast<TracerOperation>();
	ASSERT(pOperation);
	pOperation->start(pTime, m_eventIndex);
}

void TracerBase::accomplishAction(REF(tstring) line, TracerTimeNow* const pTime)
{
	LPTracerOperation pOperation = getOperation(line).object_dynamic_cast<TracerOperation>();
	ASSERT(pOperation);
	pOperation->accomplish(pTime, m_eventIndex);
}

void TracerBase::irregularEvent(REF(tstring) line, TracerTimeNow* const pTime)
{
#ifdef RDOSIM_COMPATIBLE
	m_eventList.at(atoi(getNextValue(line).c_str()) - 1)->occurs(pTime, m_eventIndex);
#else
	m_eventList.at(atoi(getNextValue(line).c_str()) - 1)->occurs(pTime, m_eventIndex);
#endif
}

void TracerBase::productionRule(REF(tstring) line, TracerTimeNow* const pTime)
{
	LPTracerEvent pEvent = getOperation(line).object_dynamic_cast<TracerEvent>();
	ASSERT(pEvent);
	pEvent->occurs(pTime, m_eventIndex);
}

LPTracerResource TracerBase::getResource(REF(tstring) line)
{
	getNextValue(line);
	LPTracerResource pResult;
	int findID = atoi(getNextValue(line).c_str());
	int i = 0;
	BOOST_FOREACH(const LPTracerResource& pResource, m_resourceList)
	{
		if (pResource->getID() == findID && !pResource->isErased())
		{
			pResult = pResource;
			break;
		}
		i++;
	}
	return pResult;
}

LPTracerResource TracerBase::resourceCreation(REF(tstring) line, TracerTimeNow* const pTime)
{
	ruint typeID = atoi(getNextValue(line).c_str()) - 1;
	ASSERT(typeID < m_resourceTypeList.size());
	LPTracerResourceType pResourceType = m_resourceTypeList.at(typeID);
	int id = atoi(getNextValue(line).c_str());
	LPTracerResource pResource = rdo::Factory<TracerResource>::create(pResourceType, QString("%1 #%2").arg(pResourceType->getName()).arg(id), id);
	pResource->setParams(line, pTime, m_eventIndex);

	m_resourceList.push_back(pResource);
	return pResource;
}

LPTracerResource TracerBase::resourceElimination(REF(tstring) line, TracerTimeNow* const pTime)
{
	LPTracerResource pResource = getResource(line);
	if (!pResource)
		return pResource;
#ifdef RDOSIM_COMPATIBLE
	pResource->setParams(line, pTime, m_eventIndex, true);
#else
	pResource->setParams(line, pTime, m_eventIndex, false);
#endif
	pResource->setErased(true);
	return pResource;
}

LPTracerResource TracerBase::resourceChanging(REF(tstring) line, TracerTimeNow* const pTime)
{
	LPTracerResource pResource = getResource(line);
	if (pResource)
	{
		pResource->setParams(line, pTime, m_eventIndex);
	}
	return pResource;
}

LPTracerResult TracerBase::getResult(REF(tstring) line)
{
	LPTracerResult pResult;
	int findid = atoi(getNextValue(line).c_str());
	BOOST_FOREACH(const LPTracerResult& pResultItem, m_resultList)
	{
		if (pResultItem->getID() == findid)
		{
			pResult = pResultItem;
			break;
		}
	}
	return pResult;
}

void TracerBase::resultChanging(REF(tstring) line, TracerTimeNow* const pTime)
{
	getResult(line)->setValue(line, pTime, m_eventIndex);
}

void TracerBase::deleteTrace()
{
	m_resourceList.clear();
	m_resourceTypeList.clear();
	m_operationList.clear();
	m_eventList.clear();
	m_patternList.clear();
	m_resultList.clear();
	TimeList::iterator it = m_timeList.begin();
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
	{
		m_pChartTree->clear();
	}
	if (m_pLog)
	{
		m_pLog->view().clear();
	}
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
}

void TracerBase::getTraceString(tstring trace_string)
{
	if (m_pLog)
	{
		m_pLog->view().push_back(trace_string);
	}

	m_updateAction = RUA_NONE;
	m_pResource    = NULL;

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

RDOStudioChartDoc* TracerBase::addSerieToChart(CREF(LPTracerSerie) pSerie, RDOStudioChartDoc* pDocument)
{
	if (!pDocument)
	{
		pDocument = createNewChart();
	}
	if (pDocument)
	{
		pDocument->addSerie(pSerie);
	}

	return pDocument;
}

void TracerBase::addChart(RDOStudioChartDoc* const pDocument)
{
	m_documentList.push_back(pDocument);
}

void TracerBase::removeChart(RDOStudioChartDoc* pDocument)
{
	DocumentList::iterator it = boost::range::find(m_documentList, pDocument);
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
