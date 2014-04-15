/*!
  \copyright (c) RDO-Team, 2011
  \file      update_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      29.10.2010
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_UPDATE_I_H_
#define _CONVERTOR_UPDATE_I_H_

// ----------------------------------------------------------------------- INCLUDES
#include <fstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/namespace.h"
#include "converter/smr2rdox/rdo_common/model_objects_convertor.h"
#include "converter/smr2rdox/update/document_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IDocUpdate
// --------------------------------------------------------------------------------
struct IDocUpdate
{
public:
	class Position
	{
	public:
		enum
		{
			POSITION_BEGIN = static_cast<std::size_t>(~0),
			POSITION_END = static_cast<std::size_t>(~1)
		};

		Position(std::size_t pos);
		Position(CREF(Position) pos);

		std::size_t get() const;

		bool begin() const;
		bool end() const;
		bool real() const;

		void operator+=(CREF(Position) pos);
		void operator-=(CREF(Position) pos);
		Position operator+(CREF(Position) pos) const;
		Position operator-(CREF(Position) pos) const;
		bool operator<=(CREF(Position) pos) const;
		bool operator>=(CREF(Position) pos) const;
		bool operator<(CREF(Position) pos) const;
		bool operator>(CREF(Position) pos) const;
		bool operator==(CREF(Position) pos) const;
		bool operator!=(CREF(Position) pos) const;

	private:
		std::size_t m_position;
	};

	virtual void apply(LPIDocument& pDocument) const = 0;
	virtual void insert(IDocument::Type type, CREF(Position) to, std::size_t size) = 0;
	virtual void remove(IDocument::Type type, CREF(Position) from, CREF(Position) to) = 0;
	virtual void dump(LPIDocument& pDocument) const = 0;
};
#define DECLARE_IDocUpdate                                                     \
	void apply(LPIDocument& pDocument) const;                                  \
	void insert(IDocument::Type type, CREF(Position) to, std::size_t size);    \
	void remove(IDocument::Type type, CREF(Position) from, CREF(Position) to); \
	void dump(LPIDocument& pDocument) const;

PREDECLARE_POINTER(DocUpdate);
class DocUpdate
	: public rdo::counter_reference
	, public IDocUpdate
{
protected:
	DocUpdate(IDocument::Type file = IDocument::UNDEFINED);

	IDocument::Type m_file;

	IDocument::Type getCurrentType() const;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_UPDATE_I_H_
