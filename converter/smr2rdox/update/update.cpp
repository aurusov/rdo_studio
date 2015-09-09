// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/update/update.h"
#include "converter/smr2rdox/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- DocUpdate
// --------------------------------------------------------------------------------
DocUpdate::DocUpdate(IDocument::Type file)
    : m_file(file)
{
    if (m_file == IDocument::Type::UNDEFINED)
        m_file = getCurrentType();
}

IDocument::Type DocUpdate::getCurrentType() const
{
    switch (Converter::getFileToParse())
    {
    case rdo::converter::smr2rdox::FileTypeIn::PAT: return IDocument::Type::PAT;
    case rdo::converter::smr2rdox::FileTypeIn::RTP: return IDocument::Type::RTP;
    case rdo::converter::smr2rdox::FileTypeIn::RSS: return IDocument::Type::RSS;
    case rdo::converter::smr2rdox::FileTypeIn::OPR: return IDocument::Type::OPR;
    case rdo::converter::smr2rdox::FileTypeIn::FRM: return IDocument::Type::FRM;
    case rdo::converter::smr2rdox::FileTypeIn::FUN: return IDocument::Type::FUN;
    case rdo::converter::smr2rdox::FileTypeIn::DPT: return IDocument::Type::DPT;
    case rdo::converter::smr2rdox::FileTypeIn::SMR: return IDocument::Type::SMR;
    case rdo::converter::smr2rdox::FileTypeIn::PMD: return IDocument::Type::PMD;
    case rdo::converter::smr2rdox::FileTypeIn::PMV: return IDocument::Type::PMV;
    case rdo::converter::smr2rdox::FileTypeIn::TRC: return IDocument::Type::TRC;
    default: NEVER_REACH_HERE;
    }
    NEVER_REACH_HERE;
    return IDocument::Type::TRC;
}

// --------------------------------------------------------------------------------
// -------------------- UpdateInsert
// --------------------------------------------------------------------------------
UpdateInsert::UpdateInsert(const Position& pos, const std::string& value, IDocument::Type file)
    : DocUpdate(file )
    , m_pos    (pos  )
    , m_value  (value)
{}

void UpdateInsert::dump(LPIDocument& pDocument) const
{
    const std::string log = rdo::format("update insert <%d>, %d, '%s', dump: '%s'\n"
        , m_file
        , m_pos.get()
        , m_value.c_str()
        , pDocument->get(m_file, m_pos.get(), m_pos.get()).c_str()
    );
    TRACE(log.c_str());
}

void UpdateInsert::apply(LPIDocument& pDocument) const
{
    pDocument->insert(m_file, m_pos.get(), m_value);
}

void UpdateInsert::insert(IDocument::Type type, const Position& to, std::size_t size)
{
    if (m_file != type)
        return;

    ASSERT(to <= m_pos || to >= m_pos + m_value.length());

    if (to <= m_pos)
    {
        m_pos += size;
    }
}

void UpdateInsert::remove(IDocument::Type type, const Position& from, const Position& to)
{
    if (m_file != type)
        return;

    ASSERT(to <= m_pos || (from >= m_pos + m_value.length() && to > m_pos + m_value.length()));

    if (to <= m_pos)
    {
        m_pos -= to - from;
    }
}

// --------------------------------------------------------------------------------
// -------------------- UpdateDelete
// --------------------------------------------------------------------------------
UpdateDelete::UpdateDelete(const Position& posFrom, const Position& posTo)
    : m_posFrom(posFrom)
    , m_posTo  (posTo  )
{
    ASSERT(m_posFrom < m_posTo);
}

void UpdateDelete::dump(LPIDocument& pDocument) const
{
    const std::string log = rdo::format("update delete <%d>, [%d - %d), dump: '%s'\n"
        , m_file
        , m_posFrom.get()
        , m_posTo.get()
        , pDocument->get(m_file, m_posFrom.get(), m_posTo.get()).c_str()
    );
    TRACE(log.c_str());
}

void UpdateDelete::apply(LPIDocument& pDocument) const
{
    pDocument->remove(m_file, m_posFrom.get(), m_posTo.get());
}

void UpdateDelete::insert(IDocument::Type type, const Position& to, std::size_t size)
{
    if (m_file != type)
        return;

    if (to <= m_posFrom)
    {
        m_posFrom += size;
        m_posTo   += size;
    }
    else if (to <= m_posTo)
    {
        m_posTo += size;
    }
}

void UpdateDelete::remove(IDocument::Type type, const Position& from, const Position& to)
{
    if (m_file != type)
        return;

    ASSERT(to <= m_posFrom || (from >= m_posTo && to >= m_posTo));

    if (to <= m_posFrom)
    {
        m_posFrom -= to - from;
        m_posTo   -= to - from;
    }
}

// --------------------------------------------------------------------------------
// -------------------- UpdateReplace
// --------------------------------------------------------------------------------
UpdateReplace::UpdateReplace(const Position& posFrom, const Position& posTo, const std::string& value, IDocument::Type file)
    : DocUpdate(file)
{
    pDelete = rdo::Factory<UpdateDelete>::create(posFrom, posTo);
    ASSERT(pDelete);

    pInsert = rdo::Factory<UpdateInsert>::create(posFrom, value, file);
    ASSERT(pInsert);
}

void UpdateReplace::dump(LPIDocument& pDocument) const
{
    TRACE("update replace...\n");
    pDelete->dump(pDocument);
    pInsert->dump(pDocument);
    TRACE("update replace... ok\n");
}

void UpdateReplace::apply(LPIDocument& pDocument) const
{
    pDelete->apply(pDocument);
    pInsert->apply(pDocument);
}

void UpdateReplace::insert(IDocument::Type type, const Position& to, std::size_t size)
{
    pDelete->insert(type, to, size);
    pInsert->insert(type, to, size);
}

void UpdateReplace::remove(IDocument::Type type, const Position& from, const Position& to)
{
    pDelete->remove(type, from, to);
    pInsert->remove(type, from, to);
}

// --------------------------------------------------------------------------------
// -------------------- UpdateMove
// --------------------------------------------------------------------------------
UpdateMove::UpdateMove(const Position& posFromBegin, const Position& posFromEnd, const Position& posTo, IDocument::Type fileTo, IDocument::Type fileFrom)
    : DocUpdate     (fileTo      )
    , m_posFromBegin(posFromBegin)
    , m_posFromEnd  (posFromEnd  )
    , m_posTo       (posTo       )
    , m_fileFrom    (fileFrom    )
{
    if (static_cast<int>(m_fileFrom) == static_cast<int>(rdo::converter::smr2rdox::FileTypeOut::UNDEFINED))
        m_fileFrom = getCurrentType();
}

void UpdateMove::dump(LPIDocument& pDocument) const
{
    const std::string log = rdo::format("update move <%d->%d>, [%d - %d) '%s', %d\n"
        , m_fileFrom
        , m_file
        , m_posFromBegin.get()
        , m_posFromEnd.get()
        , pDocument->get(m_fileFrom, m_posFromBegin.get(), m_posFromEnd.get()).c_str()
        , m_posTo.get()
    );
    TRACE(log.c_str());
}

void UpdateMove::apply(LPIDocument& pDocument) const
{
    const std::string cut = pDocument->get(m_fileFrom, m_posFromBegin.get(), m_posFromEnd.get());
    pDocument->remove(m_fileFrom, m_posFromBegin.get(), m_posFromEnd.get());

    Position pos = m_posTo;
    if (pos.real())
    {
        if (m_posFromEnd < pos)
        {
            // Удалили перед собой, сдвинемся к началу
            pos -= m_posFromEnd - m_posFromBegin;
        }
    }
    pDocument->insert(m_file, pos.get(), cut);
}

void UpdateMove::insert(IDocument::Type type, const Position& to, std::size_t size)
{
    if (to.end())
        return;

    if (m_fileFrom == type)
    {
        if (to.begin())
        {
            // Вставка до, сдвинемся к концу
            m_posFromBegin += size;
            m_posFromEnd   += size;
        }
        else
        {
            if (to < m_posFromBegin && m_posFromBegin != m_posFromEnd)
            {
                // Вставка до, сдвинемся к концу
                m_posFromBegin += size;
                m_posFromEnd   += size;
            }
            else if (to == m_posFromBegin && to + size <= m_posFromEnd && m_posFromBegin != m_posFromEnd)
            {
                // Вставка внутри, расширим конец
                m_posFromEnd += size;
            }
            else if (to == m_posFromBegin && m_posFromBegin == m_posFromEnd)
            {
                // Вставка в пустой интервал, расширим конец
                m_posFromEnd += size;
            }
            else if (to > m_posFromBegin && to <= m_posFromEnd)
            {
                // Вставка внутрь, расширим конец
                m_posFromEnd += size;
            }
        }
    }

    if (m_file == type)
    {
        if (m_posTo.real())
        {
            if (to.begin() || to <= m_posTo)
            {
                // Вставка до, сдвинемся к концу
                m_posTo += size;
            }
        }
    }
}

void UpdateMove::remove(IDocument::Type type, const Position& from, const Position& to)
{
    if (m_fileFrom == type)
    {
        if (to < m_posFromBegin)
        {
            // Удаление до, сдвинемся к началу
            m_posFromBegin -= to - from;
            m_posFromEnd   -= to - from;
        }
        else if (m_posFromBegin <= from && to <= m_posFromEnd)
        {
            // Удаление внутри, подрежем конец
            m_posFromEnd -= to - from;
        }
    }

    if (m_file == type)
    {
        if (m_posTo.real())
        {
            if (to < m_posTo)
            {
                // Удаление до, сдвинемся к началу
                m_posTo -= to - from;
            }
        }
    }
}

// --------------------------------------------------------------------------------
// -------------------- UpdateSwap
// --------------------------------------------------------------------------------
UpdateSwap::UpdateSwap(const Position& pos1Begin, const Position& pos1End, const Position& pos2Begin, const Position& pos2End, IDocument::Type file)
    : DocUpdate  (file     )
    , m_pos1Begin(pos1Begin)
    , m_pos1End  (pos1End  )
    , m_pos2Begin(pos2Begin)
    , m_pos2End  (pos2End  )
{}

void UpdateSwap::dump(LPIDocument& pDocument) const
{
    const std::string log = rdo::format("update swap <%d>: [%d, %d) <-> [%d - %d), '%s' <-> '%s'\n"
        , m_file
        , m_pos1Begin.get()
        , m_pos1End.get()
        , m_pos2Begin.get()
        , m_pos2End.get()
        , pDocument->get(m_file, m_pos1Begin.get(), m_pos1End.get()).c_str()
        , pDocument->get(m_file, m_pos2Begin.get(), m_pos2End.get()).c_str()
    );
    TRACE(log.c_str());
}

void UpdateSwap::apply(LPIDocument& pDocument) const
{
    // Запомним значения
    const std::string cut1 = pDocument->get(m_file, m_pos1Begin.get(), m_pos1End.get());
    const std::string cut2 = pDocument->get(m_file, m_pos2Begin.get(), m_pos2End.get());

    // Удалим первый интервал
    pDocument->remove(m_file, m_pos1Begin.get(), m_pos1End.get());

    Position pos2Begin = m_pos2Begin;
    Position pos2End   = m_pos2End;
    if (m_pos1End <= pos2Begin)
    {
        // Удаление перед вторым интервалом, сдвинемся к началу
        pos2Begin -= m_pos1End - m_pos1Begin;
        pos2End   -= m_pos1End - m_pos1Begin;
    }

    // Удалим второй интервал
    pDocument->remove(m_file, pos2Begin.get(), pos2End.get());

    Position pos1Begin = m_pos1Begin;
    Position pos1End   = m_pos1End;
    if (pos2End <= pos1Begin)
    {
        // Удаление перед первым интервалом, сдвинемся к началу
        pos1Begin -= pos2End - pos2Begin;
        pos1End   -= pos2End - pos2Begin;
    }

    // Вставка первого буффера на место второго
    pDocument->insert(m_file, pos2Begin.get(), cut1);

    if (pos2Begin <= pos1Begin)
    {
        // Вставка перед первым интервалом, сдвинемся к концу
        pos1Begin += cut1.length();
        pos1End   += cut1.length();
    }

    // Вставка второго буффера на место первого
    pDocument->insert(m_file, pos1Begin.get(), cut2);
}

void UpdateSwap::insert(IDocument::Type type, const Position& to, std::size_t size)
{
    if (m_file != type)
        return;

    insert(to, size, m_pos1Begin, m_pos1End);
    insert(to, size, m_pos2Begin, m_pos2End);
}

void UpdateSwap::remove(IDocument::Type type, const Position& from, const Position& to)
{
    if (m_file != type)
        return;

    remove(from, to, m_pos1Begin, m_pos1End);
    remove(from, to, m_pos2Begin, m_pos2End);
}

void UpdateSwap::insert(const Position& from, const std::size_t& size, Position& posBegin, Position& posEnd)
{
    if (from < posBegin)
    {
        posBegin += size;
        posEnd   += size;
    }

    if (posBegin <= from && from + size <= posEnd)
    {
        posEnd += size;
    }

    if (from <= posEnd && posEnd < from + size)
    {
        ASSERT(posBegin <= from);
        posEnd += from + size - posEnd;
    }
}

void UpdateSwap::remove(const Position& from, const Position& to, Position& posBegin, Position& posEnd)
{
    if (to <= posBegin)
    {
        posBegin -= to - from;
        posEnd   -= to - from;
    }
    else if (posBegin <= from && to <= posEnd)
    {
        posEnd -= to - from;
    }
    else if (from < posEnd && posEnd < to)
    {
        posEnd = from;
    }
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
