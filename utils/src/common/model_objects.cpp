// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/model_objects.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace model {

std::string getFileTypeString(FileType type)
{
    std::string string;
    switch (type)
    {
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
    case PMV : string = "PMV" ; break;
    case TRC : string = "TRC" ; break;
    case RDOX: string = "RDOX" ; break;
    default: string = "";
    }
    return string;
}

}} // namespace rdo::model
