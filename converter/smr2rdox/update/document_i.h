#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/interface_ptr/interface_ptr.h"

#include "converter/smr2rdox/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IDocument
// --------------------------------------------------------------------------------
PREDECLARE_OBJECT_INTERFACE(IDocument)
struct IDocument: public rdo::RefCounter<IDocument>
{
DECLARE_FACTORY(IDocument)
public:
    enum class Type
    {
        UNDEFINED = 0,
        PAT, RTP, RSS, OPR, FRM,
        FUN, DPT, SMR, PMD, PMV,
        TRC, EVN, PRC
    };

    virtual void insert(Type type, std::size_t to, const std::string& value) = 0;
    virtual void remove(Type type, std::size_t from, std::size_t to) = 0;
    virtual std::string get(Type type, std::size_t from, std::size_t to) = 0;

protected:
    IDocument()
    {}
    virtual ~IDocument()
    {}
};
#define DECLARE_IDocument                                             \
    void insert(Type type, std::size_t to, const std::string& value); \
    void remove(Type type, std::size_t from, std::size_t to);         \
    std::string get(Type type, std::size_t from, std::size_t to);

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
