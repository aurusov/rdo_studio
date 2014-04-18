/*!
  \copyright (c) RDO-Team, 2011
  \file      model_objects_convertor.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.06.2009
  \brief     
  \indent    4T
*/

#include <string>

#ifndef _RDOCOMMON_MODEL_OBJECTS_CONVERTOR_H_
#define _RDOCOMMON_MODEL_OBJECTS_CONVERTOR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE
	struct RDOSMRFileInfo
	{
		std::string m_modelName;
		std::string m_resourceFile;
		std::string m_frameFile;
		std::string m_statisticFile;
		std::string m_resultsFile;
		std::string m_traceFile;
		bool m_error;

		RDOSMRFileInfo()
			: m_modelName    ("")
			, m_resourceFile ("")
			, m_frameFile    ("")
			, m_statisticFile("")
			, m_resultsFile  ("")
			, m_traceFile    ("")
			, m_error        (false )
		{}
	};

	enum RDOFileTypeIn
	{
		UNDEFINED_IN = 0,
		PAT_IN,
		RTP_IN,
		RSS_IN,
		OPR_IN,
		FRM_IN,
		FUN_IN,
		DPT_IN,
		SMR_IN,
		PMD_IN,
		PMV_IN,
		TRC_IN 
	};
	enum RDOFileTypeOut
	{
		UNDEFINED_OUT = 0,
		PAT_OUT,
		RTP_OUT,
		RSS_OUT,
		FRM_OUT,
		FUN_OUT,
		DPT_OUT,
		SMR_OUT,
		PMD_OUT,
		PMV_OUT,
		TRC_OUT,
		EVN_OUT,
		PRC_OUT
	};

	enum RDOParseType
	{
		obNONE = 0x0000,
		obPRE  = 0x0001,
		obRTP  = 0x0002,
		obRSS  = 0x0004,
		obFUN  = 0x0008,
		obPAT  = 0x0010,
		obOPR  = 0x0020,
		obDPT  = 0x0040,
		obPMD  = 0x0080,
		obFRM  = 0x0100,
		obSMR  = 0x0200,
		obPOST = 0x0400,
		obALL  = 0x07FF
	};
CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _RDOCOMMON_MODEL_OBJECTS_CONVERTOR_H_
