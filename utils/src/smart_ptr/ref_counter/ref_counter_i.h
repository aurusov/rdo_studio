#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo {

struct IRefCounter
{
	virtual void addref() = 0;
	virtual void release() = 0;
	virtual bool owner() const = 0;
};
#define DECLARE_IRefCounter \
	void addref();          \
	void release();         \
	bool owner() const;

typedef IRefCounter* LPIRefCounter;

} // namespace rdo
