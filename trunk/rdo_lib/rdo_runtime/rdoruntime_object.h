#ifndef RDORUNTIME_OBJECT_H
#define RDORUNTIME_OBJECT_H

#include <rdocommon.h>

namespace rdoRuntime {

typedef double RDOValue;

// ----------------------------------------------------------------------------
// ---------- RDOException
// ----------------------------------------------------------------------------
class RDOException
{
public:
	std::string mess;
	virtual std::string getType() const = 0;
	RDOException( const std::string& str ): mess( str ) {}
};

class RDORuntimeException: public RDOException
{
public:
	std::string getType() const { return "RDO Runtime Error"; }
	RDORuntimeException( const std::string& str ): RDOException( str ) {}
};

class RDOInternalException: public RDORuntimeException
{
public:
	std::string getType() const { return "RDO Internal Error"; }
	RDOInternalException( const std::string& str ): RDORuntimeException( str ) {}
};

// ----------------------------------------------------------------------------
// ---------- RDORuntimeObject
// ----------------------------------------------------------------------------
class RDORuntime;
class RDORuntimeParent;

class RDORuntimeObject
{
private:
	size_t object_size;

protected:
	RDORuntimeParent* parent;

	RDORuntimeObject( RDORuntimeParent* _parent );

public:
	virtual ~RDORuntimeObject();
	void reparent( RDORuntimeParent* _parent );
	RDORuntimeParent* getParent() const { return parent;  }
	RDORuntime*       getRuntime();

	void detach();

#ifndef _DEBUG
	void* operator new( size_t sz );
	void operator delete( void* v );
#endif
};

// ----------------------------------------------------------------------------
// ---------- RDORuntimeParent
// ----------------------------------------------------------------------------
class RDORuntimeParent: public RDORuntimeObject
{
protected:
	std::list< RDORuntimeObject* > objects;

public:
	RDORuntimeParent( RDORuntimeParent* _parent );
	virtual ~RDORuntimeParent();

	void insertObject( RDORuntimeObject* object );
	void removeObject( RDORuntimeObject* object );
	void deleteObjects();
};

// ----------------------------------------------------------------------------
// ---------- RDOErrorPos
// ----------------------------------------------------------------------------
class RDOErrorPos
{
public:
typedef struct {
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} Data;

protected:
	Data error_pos;

public:
	RDOErrorPos() {
		error_pos.first_line   = -1;
		error_pos.first_column = -1;
		error_pos.last_line    = -1;
		error_pos.last_column  = -1;
	}
	void setErrorPos( const Data& _error_pos ) {
		error_pos.first_line   = _error_pos.first_line;
		error_pos.first_column = _error_pos.first_column;
		error_pos.last_line    = _error_pos.last_line;
		error_pos.last_column  = _error_pos.last_column;
	}
	void setErrorPos( int first_line, int first_column, int last_line, int last_column ) {
		error_pos.first_line   = first_line;
		error_pos.first_column = first_column;
		error_pos.last_line    = last_line;
		error_pos.last_column  = last_column;
	}
	const Data& error() const { return error_pos; }
};

} // namespace rdoRuntime

namespace std
{

template <class _Ret, class _Tp>
class const_mem_fun_t: public unary_function<const _Tp*,_Ret> {
public:
	explicit const_mem_fun_t(_Ret (_Tp::*__pf)() const) : _M_f(__pf) {}
	_Ret operator()(const _Tp* __p) const { return (__p->*_M_f)(); }
private:
	_Ret (_Tp::*_M_f)() const;
};

template <class _Ret, class _Tp, class _Arg>
class const_mem_fun1_t: public binary_function<const _Tp*,_Arg,_Ret> {
public:
	explicit const_mem_fun1_t(_Ret (_Tp::*__pf)(_Arg) const) : _M_f(__pf) {}
	_Ret operator()(const _Tp* __p, _Arg __x) const { return (__p->*_M_f)(__x); }
private:
	_Ret (_Tp::*_M_f)(_Arg) const;
};

template <class _Ret, class _Tp>
inline const_mem_fun_t<_Ret,_Tp> mem_fun(_Ret (_Tp::*__f)() const)
{
	return const_mem_fun_t<_Ret,_Tp>(__f);
}

template <class _Ret, class _Tp, class _Arg>
inline const_mem_fun1_t<_Ret,_Tp,_Arg> mem_fun(_Ret (_Tp::*__f)(_Arg) const)
{
	return const_mem_fun1_t<_Ret,_Tp,_Arg>(__f);
}

template <class _Ret, class _Tp, class _Arg>
inline const_mem_fun1_t<_Ret,_Tp,_Arg> mem_fun1(_Ret (_Tp::*__f)(_Arg) const)
{
	return const_mem_fun1_t<_Ret,_Tp,_Arg>(__f);
}

} // namespace std

inline std::string operator +(char *str1, std::string &str2)
{
	return std::string(str1).append(str2);
}

inline std::string operator +(std::string &str1, char *str2)
{
	return std::string(str1).append(str2);
}

#endif // RDORUNTIME_OBJECT_H
