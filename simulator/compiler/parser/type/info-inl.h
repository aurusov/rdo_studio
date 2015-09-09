// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- TypeInfo
// --------------------------------------------------------------------------------
template <class T>
inline LPTypeInfo TypeInfo::create(const RDOParserSrcInfo& srcInfo)
{
    rdo::runtime::LPRDOType pType = rdo::Factory<T>::create();
    ASSERT(pType);

    LPTypeInfo pTypeInfo = rdo::Factory<TypeInfo>::create(pType.object_dynamic_cast<IType>(), srcInfo);
    ASSERT(pTypeInfo);

    return pTypeInfo;
}

CLOSE_RDO_PARSER_NAMESPACE
