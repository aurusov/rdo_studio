// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/model_objects.h"
// --------------------------------------------------------------------------------

namespace rdo {

std::string getFileTypeString(FileType type)
{
    std::string string;
    switch (type)
    {
    case FileType::RTP : string = "RTP" ; break;
    case FileType::RSS : string = "RSS" ; break;
    case FileType::EVN : string = "EVN" ; break;
    case FileType::PAT : string = "PAT" ; break;
    case FileType::DPT : string = "DPT" ; break;
    case FileType::PRC : string = "PRC" ; break;
    case FileType::PRCX: string = "PRCX"; break;
    case FileType::FRM : string = "FRM" ; break;
    case FileType::FUN : string = "FUN" ; break;
    case FileType::SMR : string = "SMR" ; break;
    case FileType::PMD : string = "PMD" ; break;
    case FileType::PMV : string = "PMV" ; break;
    case FileType::TRC : string = "TRC" ; break;
    case FileType::RDOX: string = "RDOX" ; break;
    default: string = "";
    }
    return string;
}

} // namespace rdo
