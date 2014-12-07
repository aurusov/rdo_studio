// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/array.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

 //----------------------------------------------------------------------------
 //---------- RDOArrayType
 //----------------------------------------------------------------------------
RDOArrayType::RDOArrayType(const LPTypeInfo& pItemType, const RDOParserSrcInfo& src_info)
    : rdo::runtime::RDOArrayType(pItemType->type())
    , RDOParserSrcInfo(src_info)
    , m_pItemType(pItemType) // TODO кажется лишним
{
    ASSERT(pItemType);
    setSrcText(name());
}

RDOArrayType::~RDOArrayType()
{}

std::string RDOArrayType::name() const
{
    return rdo::format("array<%s>", m_pItemType->itype()->name().c_str());
}

LPIType RDOArrayType::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    LPRDOArrayType pFromArrayType = pFrom.object_dynamic_cast<RDOArrayType>();
    if (pFromArrayType)
    {
        LPRDOArrayType pThisArrayType(const_cast<RDOArrayType*>(this));

        if (pThisArrayType->getItemType()->itype()->type_cast(pFromArrayType->getItemType()->itype(), pFromArrayType->src_info(), pThisArrayType->src_info(), pFromArrayType->src_info()))
        {
            // Это один и тот же тип
            return pThisArrayType;
        }

        parser::g_error().push_only(src_info,    rdo::format("Несоответствие размерности массива"));
        parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
        parser::g_error().push_done();
    }

    parser::g_error().push_only(src_info,    rdo::format("Ожидается тип массива, найдено: %s", from_src_info.src_text().c_str()));
    parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
    parser::g_error().push_done();

    return NULL;
}

LPRDOValue RDOArrayType::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    LPRDOArrayType pFromArrayType = pFrom->typeInfo()->itype().object_dynamic_cast<RDOArrayType>();
    if (pFromArrayType)
    {
        LPRDOArrayType  pThisArrayType(const_cast<RDOArrayType*>(this));
        LPRDOArrayValue pThisArrayValue = rdo::Factory<RDOArrayValue>::create(pThisArrayType);
        ASSERT(pThisArrayValue);

        rdo::runtime::LPRDOArrayValue pFromArrayValue = pFrom->get<RDOArrayValue>()->createRuntimeValue();
        ASSERT(pFromArrayValue);

        for (rdo::runtime::LPRDOArrayIterator it = pFromArrayValue->begin(); !it->equal(pFromArrayValue->end()); it->next())
        {
            LPRDOValue pItemValue = rdo::Factory<RDOValue>::create(it->getValue(), src_info, pThisArrayType->getItemType());
            ASSERT(pItemValue);
            pThisArrayValue->insertItem(pThisArrayType->getItemType()->itype()->value_cast(pItemValue, to_src_info, src_info));
        }
        return rdo::Factory<RDOValue>::create(pThisArrayType->typeInfo(), pThisArrayValue, pFrom->src_info());
    }

    parser::g_error().push_only(src_info,    rdo::format("Ожидается массив, найдено: %s", pFrom->src_text().c_str()));
    parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
    parser::g_error().push_done();

    return NULL;
}

rdo::runtime::LPRDOCalc RDOArrayType::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& pType) const
{
    return pCalc;
}

rdo::runtime::RDOValue RDOArrayType::get_default() const
{
    const rdo::runtime::LPRDOArrayType pThis(const_cast<RDOArrayType*>(this));
    return rdo::runtime::RDOValue(pThis, rdo::Factory<rdo::runtime::RDOArrayValue>::create(pThis));
}

void RDOArrayType::writeModelStructure(std::ostream& stream) const
{
    stream << "A ";
    dynamic_cast<IModelStructure*>(m_pItemType->type().get())->writeModelStructure(stream);
}

const LPTypeInfo& RDOArrayType::getItemType() const
{
    return m_pItemType;
}

LPTypeInfo RDOArrayType::typeInfo() const
{
    LPTypeInfo pType = rdo::Factory<TypeInfo>::create(
        const_cast<RDOArrayType*>(this),
        src_info()
    );
    ASSERT(pType);
    return pType;
}

CLOSE_RDO_PARSER_NAMESPACE
