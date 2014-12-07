#ifndef _CONVERTOR_UPDATE_H_
#define _CONVERTOR_UPDATE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"

#include "converter/smr2rdox/update/update_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- UpdateInsert
// --------------------------------------------------------------------------------
class UpdateInsert: public DocUpdate
{
DECLARE_FACTORY(UpdateInsert)
private:
	UpdateInsert(const Position& pos, const std::string& value, IDocument::Type file = IDocument::UNDEFINED);

	Position m_pos;
	std::string m_value;

	DECLARE_IDocUpdate;
};

// --------------------------------------------------------------------------------
// -------------------- UpdateDelete
// --------------------------------------------------------------------------------
class UpdateDelete: public DocUpdate
{
DECLARE_FACTORY(UpdateDelete)
private:
	UpdateDelete(const Position& posFrom, const Position& posTo);

	Position m_posFrom;
	Position m_posTo;

	DECLARE_IDocUpdate;
};

// --------------------------------------------------------------------------------
// -------------------- UpdateReplace
// --------------------------------------------------------------------------------
class UpdateReplace: public DocUpdate
{
DECLARE_FACTORY(UpdateReplace)
private:
	UpdateReplace(const Position& posFrom, const Position& posTo, const std::string& value, IDocument::Type file = IDocument::UNDEFINED);

	LPDocUpdate pDelete;
	LPDocUpdate pInsert;

	DECLARE_IDocUpdate;
};

// --------------------------------------------------------------------------------
// -------------------- UpdateMove
// --------------------------------------------------------------------------------
class UpdateMove: public DocUpdate
{
DECLARE_FACTORY(UpdateMove)
private:
	UpdateMove(const Position& posFromBegin,
	           const Position& posFromEnd,
	           const Position& posTo,
	           IDocument::Type fileTo   = IDocument::UNDEFINED,
	           IDocument::Type fileFrom = IDocument::UNDEFINED);

	Position        m_posFromBegin;
	Position        m_posFromEnd;
	Position        m_posTo;
	IDocument::Type m_fileFrom;

	DECLARE_IDocUpdate;
};

// --------------------------------------------------------------------------------
// -------------------- UpdateSwap
// --------------------------------------------------------------------------------
class UpdateSwap: public DocUpdate
{
DECLARE_FACTORY(UpdateSwap)
private:
	UpdateSwap(const Position& pos1Begin,
	           const Position& pos1End,
	           const Position& pos2Begin,
	           const Position& pos2End,
	           IDocument::Type file = IDocument::UNDEFINED);

	Position m_pos1Begin;
	Position m_pos1End;
	Position m_pos2Begin;
	Position m_pos2End;

	void insert(const Position& from, const std::size_t& size, Position& posBegin, Position& posEnd);
	void remove(const Position& from, const Position& to, Position& posBegin, Position& posEnd);

	DECLARE_IDocUpdate;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_UPDATE_H_
