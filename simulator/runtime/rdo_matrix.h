#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_as_string_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOMatrixType    );
PREDECLARE_POINTER(RDOMatrixIterator);

PREDECLARE_POINTER(RDOMatrixValue);
class RDOMatrixValue: public IAsString
{
DECLARE_FACTORY(RDOMatrixValue)
public:
    typedef std::vector<RDOValue> Container;

    const LPRDOMatrixType& type() const;

    void push_back(const RDOValue& item);
    LPRDOMatrixIterator begin();
    LPRDOMatrixIterator end  ();
    void insert(const LPRDOMatrixIterator& pWhere, const LPRDOMatrixIterator& pFromFirst, const LPRDOMatrixIterator& pFromLast);
    void  erase(const LPRDOMatrixIterator& pFirst, const LPRDOMatrixIterator& pLast);

    std::size_t size() const;

    const RDOValue& getItem(const RDOValue& index) const;
    void            setItem(const RDOValue& index, const RDOValue& item);

    DECLARE_IAsString;

private:
    RDOMatrixValue(const LPRDOMatrixType&  pType );
    RDOMatrixValue(const LPRDOMatrixValue& pValue);
    virtual ~RDOMatrixValue();

    Container        m_container;
    LPRDOMatrixType  m_pMatrixType;
};

class RDOMatrixIterator: public RDOType, public IAsString
{
DECLARE_FACTORY(RDOMatrixIterator)
public:
    typedef RDOMatrixValue::Container::iterator Iterator;

    Iterator             getIterator() const;
    const RDOValue&      getValue   () const;
    LPRDOMatrixIterator  preInc     (int delta);
    LPRDOMatrixIterator  postInc    (int delta);
    LPRDOMatrixIterator  next       ();
    bool                 equal      (const LPRDOMatrixIterator& pIterator) const;
    LPRDOMatrixIterator  clone      () const;

    DECLARE_IAsString;

private:
    RDOMatrixIterator(const LPRDOMatrixIterator& pIterator);
    RDOMatrixIterator(const RDOMatrixIterator&   iterator );
    RDOMatrixIterator(const Iterator&            iterator );
    virtual ~RDOMatrixIterator();

    Iterator  m_iterator;
};

class RDOMatrixType: public RDOType
{
DECLARE_FACTORY(RDOMatrixType);
public:
    typedef  LPRDOType       LPItemType;
    typedef  RDOMatrixValue  value_type;

    const LPItemType& getItemType() const;

private:
    RDOMatrixType(const LPItemType& pItemType);
    virtual ~RDOMatrixType();

    LPItemType  m_pItemType;
};

CLOSE_RDO_RUNTIME_NAMESPACE
