#ifndef _UTILS_SMART_PTR_REF_COUNTER_I_H_
#define _UTILS_SMART_PTR_REF_COUNTER_I_H_

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

#endif // _UTILS_SMART_PTR_REF_COUNTER_I_H_
