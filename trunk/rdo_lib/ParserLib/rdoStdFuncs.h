#ifndef RDO_STD_FUNCS_
#define RDO_STD_FUNCS_

using namespace std;


struct RDOException
{
   string mess;
   virtual string getType() const = 0;
   RDOException(const char *str): mess(str) {}
};

namespace rdoRuntime
{
typedef double RDOValue;
}

using namespace rdoRuntime;

namespace rdoParse 
{

struct RDODeletable
{
	RDODeletable();
	virtual ~RDODeletable();
};
template <class T> 
struct comparePointers
{
	const T * const val;
	comparePointers(const T * const _val): val(_val) {}
	bool operator() (const T * const other) { return ((*other) == (*val)); }
};

template <class T> 
struct compareName
{
	const string * const name;
	compareName(const string * const _name): name(_name) {}
	bool operator() (const T * other) { return ((*(other->getName())) == (*name)); }
};

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


inline string operator +(char *str1, string &str2)
{
	return string(str1).append(str2);
}

inline string operator +(string &str1, char *str2)
{
	return string(str1).append(str2);
}

#endif //RDO_STD_FUNCS_
