#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/service/src/show_mode.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSMR
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOSMR);
class RDOSMR: public rdo::counter_reference
{
DECLARE_FACTORY(RDOSMR);
public:
	typedef std::map<std::string, std::string> StringTable;

	void setExternalModelName(const std::string& alias, const std::string& modelID)
	{
		m_extModelList[alias] = modelID;
	}
	std::string getExternalModelName(const std::string& alias) const
	{
		StringTable::const_iterator it = m_extModelList.find(alias);
		return it != m_extModelList.end() ? it->second : "";
	}
	const StringTable& getExternalModelList() const
	{
		return m_extModelList;
	}

	rdo::service::simulation::ShowMode getShowMode      () const { return m_showMode;       }
	int                                getFrameNumber   () const { return m_frameNumber;    }
	double                             getShowRate      () const { return m_showRate;       }
	double                             getRunStartTime  () const { return m_runStartTime;   }
	double                             getTraceStartTime() const { return m_traceStartTime; }
	double                             getTraceEndTime  () const { return m_traceEndTime;   }

	void setShowMode      (rdo::service::simulation::ShowMode showMode);
	void setFrameNumber   (int value,    const YYLTYPE& pos);
	void setShowRate      (double value, const YYLTYPE& pos);
	void setRunStartTime  (double value, const YYLTYPE& pos);
	void setTraceStartTime(double value, const YYLTYPE& pos);
	void setTraceEndTime  (double value, const YYLTYPE& pos);

	void setTerminateIf  (LPRDOFUNLogic& pLogic);
	void setConstValue   (const RDOParserSrcInfo& const_info, LPRDOFUNArithm& pArithm);
	void setResParValue  (const RDOParserSrcInfo& res_info, const RDOParserSrcInfo& par_info, LPRDOFUNArithm& pArithm);
	void setSeed         (const RDOParserSrcInfo& seq_info, int base);
	void insertBreakPoint(const RDOParserSrcInfo& src_info, LPRDOFUNLogic& pLogic);

	std::vector<rdo::runtime::LPRDOCalc> getCalcList() const { return m_SMRCalcList; }

private:
	RDOSMR();

	PREDECLARE_POINTER(BreakPoint);
	class BreakPoint
		: public rdo::counter_reference
		, public RDOParserSrcInfo
	{
	DECLARE_FACTORY(BreakPoint);
	private:
		BreakPoint(const RDOParserSrcInfo& src_info, LPRDOFUNLogic pLogic);
	};
	typedef std::vector<LPBreakPoint> BreakPointList;

	StringTable                         m_extModelList;
	rdo::service::simulation::ShowMode  m_showMode;
	int                                 m_frameNumber;
	double                              m_showRate;
	double                              m_runStartTime;
	double                              m_traceStartTime;
	double                              m_traceEndTime;
	YYLTYPE                             m_traceStartTime_pos;
	YYLTYPE                             m_traceEndTime_pos;
	LPRDOFUNLogic                       m_pTerminateIf;
	BreakPointList                      m_breakPointList;

	void addCalc(const rdo::runtime::LPRDOCalc pCalc) { m_SMRCalcList.push_back(pCalc); }

	std::vector<rdo::runtime::LPRDOCalc> m_SMRCalcList;
};

CLOSE_RDO_PARSER_NAMESPACE
