/*
 * copyright: (c) RDO-Team, 2010
 * filename : update_i.h
 * author   : Урусов Андрей
 * date     : 29.10.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_UPDATE_I_H_
#define _CONVERTOR_UPDATE_I_H_

// ====================================================================== INCLUDES
#include <fstream>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/namespace.h"
#include "rdo_lib/rdo_converter/rdo_common/model_objects_convertor.h"
#include "rdo_lib/rdo_converter/update/document_i.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IDocUpdate
// ----------------------------------------------------------------------------
S_INTERFACE(IDocUpdate)
{
public:
	virtual void apply (REF(LPIDocument) pDocument) const           = 0;
	virtual void insert(IDocument::Type type, ruint to, ruint size) = 0;
	virtual void remove(IDocument::Type type, ruint from, ruint to) = 0;
	virtual void dump  (REF(LPIDocument) pDocument) const           = 0;
};
#define DECLARE_IDocUpdate                                   \
	void apply (REF(LPIDocument) pDocument) const;           \
	void insert(IDocument::Type type, ruint to, ruint size); \
	void remove(IDocument::Type type, ruint from, ruint to); \
	void dump  (REF(LPIDocument) pDocument) const;

OBJECT(DocUpdate) IS IMPLEMENTATION_OF(IDocUpdate)
{
protected:
	DocUpdate(IDocument::Type file = rdoModelObjectsConvertor::UNDEFINED_OUT);

	IDocument::Type m_file;

	IDocument::Type getCurrentType() const;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_UPDATE_I_H_
