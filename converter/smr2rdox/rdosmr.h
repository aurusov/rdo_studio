/*!
  \copyright (c) RDO-Team, 2011
  \file      rdosmr.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date
  \brief
  \indent    4T
*/

#ifndef _CONVERTOR_RDOSMR_H_
#define _CONVERTOR_RDOSMR_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/service/src/show_mode.h"
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdofun.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int cnv_smr_file_parse(void* lexer);
int cnv_smr_file_lex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);
void cnv_smr_file_error(YYLTYPE* llocp, void* lexer, const char* message);

int cnv_smr_sim_parse(void* lexer);
int cnv_smr_sim_lex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);
void cnv_smr_sim_error(YYLTYPE* llocp, void* lexer, const char* message);

// --------------------------------------------------------------------------------
// -------------------- RDOSMR
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOSMR);
class RDOSMR: public rdo::counter_reference
{
DECLARE_FACTORY(RDOSMR);
public:
	typedef std::map<std::string, std::string> StringTable;

	void setFile(const std::string& file_type, const std::string& file_name)
	{
		m_files[file_type] = file_name;
	}
	bool hasFile(const std::string& file_type) const
	{
		return m_files.find(file_type) != m_files.end();
	}
	std::string getFile(const std::string& file_type) const
	{
		StringTable::const_iterator it = m_files.find(file_type);
		return it != m_files.end() ? it->second : "";
	}
	std::string modelName() const
	{
		return getFile("Model_name");
	}
#ifdef CORBA_ENABLE
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
#endif

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

private:
	RDOSMR(const std::string& modelName);

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

	StringTable                         m_files;
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
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOSMR_H_
