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
	tstring string("");
	switch ( type ) {
			case RTP : string = "RTP" ; break;
			case RSS : string = "RSS" ; break;
			case EVN : string = "EVN" ; break;
			case PAT : string = "PAT" ; break;
			case DPT : string = "DPT" ; break;
			case PRC : string = "PRC" ; break;
			case PRCX: string = "PRCX"; break;
			case FRM : string = "FRM" ; break;
			case FUN : string = "FUN" ; break;
			case SMR : string = "SMR" ; break;
			case PMD : string = "PMD" ; break;
			default: string = "";
	}
	return string;
}

CLOSE_RDO_MODEL_OBJECTS_NAMESPACE