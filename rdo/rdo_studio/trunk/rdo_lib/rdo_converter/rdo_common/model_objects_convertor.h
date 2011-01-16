/*
 * copyright: (c) RDO-Team, 2009
 * filename : model_objects_convertor.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 13.06.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCOMMON_MODEL_OBJECTS_CONVERTOR_H_
#define _RDOCOMMON_MODEL_OBJECTS_CONVERTOR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

#define OPEN_RDO_MODEL_OBJECTS_CONVERTOR_NAMESPACE	namespace rdoModelObjectsConvertor {
#define CLOSE_RDO_MODEL_OBJECTS_CONVERTOR_NAMESPACE	};

OPEN_RDO_MODEL_OBJECTS_CONVERTOR_NAMESPACE
	struct RDOSMRFileInfo
	{
		tstring  model_name;
		tstring  resource_file;
		tstring  oprIev_file;
		tstring  frame_file;
		tstring  statistic_file;
		tstring  results_file;
		tstring  trace_file;
		rbool    error;

		RDOSMRFileInfo()
			: model_name    (_T(""))
			, resource_file (_T(""))
			, oprIev_file   (_T(""))
			, frame_file    (_T(""))
			, statistic_file(_T(""))
			, results_file  (_T(""))
			, trace_file    (_T(""))
			, error         (false )
		{}
	};

	enum RDOFileType  { PAT = 0, RTP, RSS, OPR, FRM, FUN, DPT, SMR, PMD, PMV, TRC };

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
CLOSE_RDO_MODEL_OBJECTS_CONVERTOR_NAMESPACE

#endif //! _RDOCOMMON_MODEL_OBJECTS_CONVERTOR_H_
