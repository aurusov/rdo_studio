/*!
  \copyright (c) RDO-Team, 2011
  \file      model_objects.cpp
  \authors   Евгений Пройдаков (lord.tiran@gmail.com)
  \date      13.06.2009
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/model_objects.h"
// --------------------------------------------------------------------------------

OPEN_RDO_MODEL_OBJECTS_NAMESPACE

tstring getFileTypeString(RDOFileType type)
{
	tstring string(_T(""));
	switch ( type ) {
			case RTP : string = _T("RTP") ; break;
			case RSS : string = _T("RSS") ; break;
			case EVN : string = _T("EVN") ; break;
			case PAT : string = _T("PAT") ; break;
			case DPT : string = _T("DPT") ; break;
			case PRC : string = _T("PRC") ; break;
			case PRCX: string = _T("PRCX"); break;
			case FRM : string = _T("FRM") ; break;
			case FUN : string = _T("FUN") ; break;
			case SMR : string = _T("SMR") ; break;
			case PMD : string = _T("PMD") ; break;
			default: string = _T("");
	}
	return string;
}

CLOSE_RDO_MODEL_OBJECTS_NAMESPACE