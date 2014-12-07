#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo {
namespace model {

enum FileType
{
    RTP = 0,
    RSS,
    EVN,
    PAT,
    DPT,
    PRC,
    FRM,
    FUN,
    SMR,
    PMD,
    PMV,
    TRC,
    RDOX,
    PRCX
};

std::string getFileTypeString(FileType type);

}} // namespace rdo::model
