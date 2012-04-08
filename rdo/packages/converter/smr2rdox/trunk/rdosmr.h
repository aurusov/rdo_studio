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
#include "utils/rdocommon.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/service/show_mode.h"
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdofun.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int  cnv_smr_file_parse(PTR(void) lexer);
int  cnv_smr_file_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_smr_file_error(PTR(char) message);

int  cnv_smr_sim_parse (PTR(void) lexer);
int  cnv_smr_sim_lex   (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_smr_sim_error (PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDOSMR
// --------------------------------------------------------------------------------
OBJECT(RDOSMR)
{
DECLARE_FACTORY(RDOSMR);
public:
	typedef std::map<tstring, tstring> StringTable;

	void setFile(CREF(tstring) file_type, CREF(tstring) file_name)
	{
		m_files[file_type] = file_name;
	}
	rbool hasFile(CREF(tstring) file_type) const
	{
		return m_files.find(file_type) != m_files.end();
	}
	tstring getFile(CREF(tstring) file_type) const
	{
		StringTable::const_iterator it = m_files.find(file_type);
		return it != m_files.end() ? it->second : _T("");
	}
	tstring modelName() const
	{
		return getFile(_T("Model_name"));
	}
	void setExternalModelName(CREF(tstring) alias, CREF(tstring) modelID)
	{
		m_extModelList[alias] = modelID;
	}
	tstring getExternalModelName(CREF(tstring) alias) const
	{
		StringTable::const_iterator it = m_extModelList.find(alias);
		return it != m_extModelList.end() ? it->second : _T("");
	}
	CREF(StringTable) getExternalModelList() const
	{
		return m_extModelList;
	}

	rdoSimulator::ShowMode getShowMode      () const { return m_showMode;       }
	int                    getFrameNumber   () const { return m_frameNumber;    }
	double                 getShowRate      () const { return m_showRate;       }
	double                 getRunStartTime  () const { return m_runStartTime;   }
	double                 getTraceStartTime() const { return m_traceStartTime; }
	double                 getTraceEndTime  () const { return m_traceEndTime;   }

	void setShowMode      (rdoSimulator::ShowMode showMode);
	void setFrameNumber   (int value,    CREF(YYLTYPE) pos);
	void setShowRate      (double value, CREF(YYLTYPE) pos);
	void setRunStartTime  (double value, CREF(YYLTYPE) pos);
	void setTraceStartTime(double value, CREF(YYLTYPE) pos);
	void setTraceEndTime  (double value, CREF(YYLTYPE) pos);

	void setTerminateIf  (REF(LPRDOFUNLogic) pLogic);
	void setConstValue   (CREF(RDOParserSrcInfo) const_info, REF(LPRDOFUNArithm)    pArithm);
	void setResParValue  (CREF(RDOParserSrcInfo) res_info,   CREF(RDOParserSrcInfo) par_info, REF(LPRDOFUNArithm) pArithm);
	void setSeed         (CREF(RDOParserSrcInfo) seq_info,   int base);
	void insertBreakPoint(CREF(RDOParserSrcInfo) src_info,   REF(LPRDOFUNLogic) pLogic);

private:
	RDOSMR(CREF(tstring) modelName);

	OBJECT(BreakPoint) IS INSTANCE_OF(RDOParserSrcInfo)
	{
	DECLARE_FACTORY(BreakPoint);
	private:
		BreakPoint(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic);
	};
	typedef std::vector<LPBreakPoint> BreakPointList;

	StringTable            m_files;
	StringTable            m_extModelList;
	rdoSimulator::ShowMode m_showMode;
	int                    m_frameNumber;
	double                 m_showRate;
	double                 m_runStartTime;
	double                 m_traceStartTime;
	double                 m_traceEndTime;
	YYLTYPE                m_traceStartTime_pos;
	YYLTYPE                m_traceEndTime_pos;
	LPRDOFUNLogic          m_pTerminateIf;
	BreakPointList         m_breakPointList;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOSMR_H_
