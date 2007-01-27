#ifndef RDO_STD_FUNCS_
#define RDO_STD_FUNCS_

#include "rdogramma.h"

class RDOException
{
public:
   std::string mess;
   virtual std::string getType() const = 0;
   RDOException(const char *str): mess(str) {}
};

namespace rdoRuntime
{
typedef double RDOValue;
}

namespace rdoParse 
{

class RDODeletable
{
private:
	size_t object_size;

public:
	RDODeletable();
	virtual ~RDODeletable();
#ifndef _DEBUG
	void* operator new( size_t sz );
	void operator delete( void* v );
#endif
};

class RDOParser;

class RDOParserObject: public RDODeletable
{
protected:
	RDOParser*       parser;
	const RDOParserObject* parent;
	RDOParserObject( RDOParser* _parser );
	RDOParserObject( const RDOParserObject* _parent );
	virtual ~RDOParserObject();
};

class RDOErrorPos
{
private:
	YYLTYPE error_pos;

public:
	RDOErrorPos() {
		error_pos.first_line   = -1;
		error_pos.first_column = -1;
		error_pos.last_line    = -1;
		error_pos.last_column  = -1;
	}
	void setErrorPos( const YYLTYPE& _error_pos ) {
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
	const YYLTYPE& error() const { return error_pos; }
};

template <class T> class comparePointers
{
public:
	const T * const val;
	comparePointers(const T * const _val): val(_val) {}
	bool operator() (const T * const other) { return ((*other) == (*val)); }
};

template <class T> class compareName
{
public:
	const std::string * const name;
	compareName(const std::string * const _name): name(_name) {}
	bool operator() (const T * other) { return ((*(other->getName())) == (*name)); }
};

/*
enum FileToParse
{
	RTP_FILE,
	RSS_FILE,
	FUN_FILE,
	PAT_FILE,
	OPR_FILE,
	DPT_FILE,
	PMD_FILE,
	FRM_FILE,
	SMR1_FILE,
	SMR2_FILE
};
*/

}

namespace std
{
template <class _Ret, class _Tp>
class const_mem_fun_t : public unary_function<const _Tp*,_Ret> {
public:
  explicit const_mem_fun_t(_Ret (_Tp::*__pf)() const) : _M_f(__pf) {}
  _Ret operator()(const _Tp* __p) const { return (__p->*_M_f)(); }
private:
  _Ret (_Tp::*_M_f)() const;
};

template <class _Ret, class _Tp, class _Arg>
class const_mem_fun1_t : public binary_function<const _Tp*,_Arg,_Ret> {
public:
  explicit const_mem_fun1_t(_Ret (_Tp::*__pf)(_Arg) const) : _M_f(__pf) {}
  _Ret operator()(const _Tp* __p, _Arg __x) const
    { return (__p->*_M_f)(__x); }
private:
  _Ret (_Tp::*_M_f)(_Arg) const;
};


template <class _Ret, class _Tp>
inline const_mem_fun_t<_Ret,_Tp> mem_fun(_Ret (_Tp::*__f)() const)
  { return const_mem_fun_t<_Ret,_Tp>(__f); }

template <class _Ret, class _Tp, class _Arg>
inline const_mem_fun1_t<_Ret,_Tp,_Arg> mem_fun(_Ret (_Tp::*__f)(_Arg) const)
  { return const_mem_fun1_t<_Ret,_Tp,_Arg>(__f); }

template <class _Ret, class _Tp, class _Arg>
inline const_mem_fun1_t<_Ret,_Tp,_Arg> mem_fun1(_Ret (_Tp::*__f)(_Arg) const)
  { return const_mem_fun1_t<_Ret,_Tp,_Arg>(__f); }

}	// namespace std


inline std::string operator +(char *str1, std::string &str2)
{
	return std::string(str1).append(str2);
}

inline std::string operator +(std::string &str1, char *str2)
{
	return std::string(str1).append(str2);
}

#endif // RDO_STD_FUNCS_
