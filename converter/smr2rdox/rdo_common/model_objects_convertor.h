#include <string>

#pragma once

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
        : m_error(false)
    {}
};

enum class FileTypeIn
{
    UNDEFINED = 0,
    PAT,
    RTP,
    RSS,
    OPR,
    FRM,
    FUN,
    DPT,
    SMR,
    PMD,
    PMV,
    TRC
};

enum class FileTypeOut
{
    UNDEFINED = 0,
    PAT,
    RTP,
    RSS,
    FRM,
    FUN,
    DPT,
    SMR,
    PMD,
    PMV,
    TRC,
    EVN,
    PRC
};

enum class ParseType
{
    PRE  = 0x0001,
    RTP  = 0x0002,
    RSS  = 0x0004,
    FUN  = 0x0008,
    PAT  = 0x0010,
    OPR  = 0x0020,
    DPT  = 0x0040,
    PMD  = 0x0080,
    FRM  = 0x0100,
    SMR  = 0x0200,
    POST = 0x0400
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
