/*!
  \copyright (c) RDO-Team, 2011
  \file      update.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      29.10.2010
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_UPDATE_H_
#define _CONVERTOR_UPDATE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"
#include "utils/src/common/rdomacros.h"

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
	UpdateInsert(CREF(Position) pos, CREF(std::string) value, IDocument::Type file = IDocument::UNDEFINED);

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
	UpdateDelete(CREF(Position) posFrom, CREF(Position) posTo);

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
	UpdateReplace(CREF(Position) posFrom, CREF(Position) posTo, CREF(std::string) value, IDocument::Type file = IDocument::UNDEFINED);

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
	UpdateMove(CREF(Position)  posFromBegin,
	           CREF(Position)  posFromEnd,
	           CREF(Position)  posTo,
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
	UpdateSwap(CREF(Position)  pos1Begin,
	           CREF(Position)  pos1End,
	           CREF(Position)  pos2Begin,
	           CREF(Position)  pos2End,
	           IDocument::Type file = IDocument::UNDEFINED);

	Position m_pos1Begin;
	Position m_pos1End;
	Position m_pos2Begin;
	Position m_pos2End;

	void insert(CREF(Position) from, CREF(ruint)  size, REF(Position) posBegin, REF(Position) posEnd);
	void remove(CREF(Position) from, CREF(Position) to, REF(Position) posBegin, REF(Position) posEnd);

	DECLARE_IDocUpdate;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_UPDATE_H_
