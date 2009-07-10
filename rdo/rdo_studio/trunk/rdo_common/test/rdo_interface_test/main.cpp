// ====================================================================== INCLUDES
#include <string>
#include <iostream>
// ====================================================================== SYNOPSIS
#include "../../rdocommon.h"
#include "../../namespace.h"
#include "../../rdomacros.h"
#include "../../rdotypes.h"
// ===============================================================================

OPEN_RDO_NAMESPACE
template <class T, ruint id>
class InterfaceRegistrator
{
public:
	typedef T Type;
	enum { ID = id };
};
CLOSE_RDO_NAMESPACE

#define INTERFACE_REGISTRATOR(I, ID) \
typedef rdo::InterfaceRegistrator<I, ID> Registered##I; \
class ___InterfaceRegistrator___DoubleID___##ID {};

#define IID(I) \
Registered##I::ID

OPEN_RDO_NAMESPACE

class IUnknow;
typedef PTR(IUnknow) LPIUnknow;

class UnknowPointer
{
public:
	UnknowPointer ();
	UnknowPointer (PTR(void) aInterface, LPIUnknow smt_ptr);
	UnknowPointer (CREF(UnknowPointer) unknowPointer);
	~UnknowPointer();

	operator rbool() const;

	UnknowPointer queryInterface(ruint id);
	ruint         getInterfaceID() const;

protected:
	PTR(void)  m_interface;

private:
	LPIUnknow  m_smt_ptr;
};

template<class T>
class Interface: public UnknowPointer
{
public:
	Interface(CREF(UnknowPointer) unknowPointer)
		: UnknowPointer(unknowPointer)
	{}
	PTR(T) operator-> ()
	{
		return static_cast<PTR(T)>(m_interface);
	}
};

class IUnknow
{
public:
	virtual void          AddRef () = 0;
	virtual void          Release() = 0;
	virtual UnknowPointer queryInterface(ruint id) = 0;
};
typedef PTR(IUnknow) LPIUnknow;

inline UnknowPointer::UnknowPointer()
	: m_interface(NULL)
	, m_smt_ptr  (NULL)
{}

inline UnknowPointer::UnknowPointer(PTR(void) aInterface, LPIUnknow smt_ptr)
	: m_interface(aInterface)
	, m_smt_ptr  (smt_ptr   )
{
	m_smt_ptr->AddRef();
}

inline UnknowPointer::UnknowPointer(CREF(UnknowPointer) unknowPointer)
	: m_interface(unknowPointer.m_interface)
	, m_smt_ptr  (unknowPointer.m_smt_ptr  )
{
	m_smt_ptr->AddRef();
}

inline UnknowPointer::~UnknowPointer()
{
	if (m_smt_ptr)
		m_smt_ptr->Release();
}

inline UnknowPointer::operator rbool () const
{
	return m_interface != NULL;
}

inline UnknowPointer UnknowPointer::queryInterface(ruint id)
{
	return m_smt_ptr ? m_smt_ptr->queryInterface(id) : UnknowPointer();
}

inline ruint UnknowPointer::getInterfaceID() const
{
}

template<class T>
class smart_ptr: public IUnknow
{
public:
	typedef T            object_type;
	typedef smart_ptr<T> this_type;

	smart_ptr(PTR(T) obj)
		: m_object(obj)
	{
		allocateCounter();
		counter() = 1;
	}
	smart_ptr(CREF(this_type) sptr)
		: m_counter(sptr.m_counter)
		, m_object (sptr.m_object )
	{
		AddRef();
	}
	~smart_ptr()
	{
		Release();
		if (counter() == 0)
			deallocateCounter();
	}
	PTR(T) get()
	{
		return m_object;
	}
	operator rbool () const
	{
		return get() != NULL;
	}
	PTR(T) operator-> ()
	{
		return get();
	}

private:
	PTR(unsigned int) m_counter;
	PTR(T)            m_object;

	void allocateCounter()
	{
		m_counter = new unsigned int;
	}
	void deallocateCounter()
	{
		delete m_counter;
	}
	REF(unsigned int) counter()
	{
		return *m_counter;
	}
	virtual void AddRef()
	{
		counter()++;
	}
	virtual void Release()
	{
		counter()--;
		if (counter() == 0)
		{
			if (m_object)
			{
				Factory<T>::destroy(m_object);
				m_object = NULL;
			}
		}
	}
	virtual UnknowPointer queryInterface(ruint id)
	{
		if (get() != NULL)
		{
			PTR(void) pInterface = get()->queryInterface(id);
			if (pInterface)
				return UnknowPointer(pInterface, this);
		}
		return UnknowPointer();
	}
};

template <class T>
class Factory
{
public:
	static smart_ptr<T> create()
	{
		return smart_ptr<T>(new T());
	}
	template <typename P1>
	static smart_ptr<T> create(P1 p1)
	{
		return smart_ptr<T>(new T(p1));
	}
	static void destroy(PTR(T) obj)
	{
		delete obj;
	}
};

CLOSE_RDO_NAMESPACE

#define DEFINE_THIS_TYPE(A)    typedef A this_type;
#define DEFINE_IPARENT_TYPE(P) typedef P iparent_type;
#define DEFINE_FACTORY(A)      friend class rdo::Factory<A>;
#define DEFINE_CLASS_NAME(A)   static std::string className() { return #A; }

#define RDO_IOBJECT(A, BASE) \
protected: \
DEFINE_THIS_TYPE(A) \
DEFINE_FACTORY(A) \
DEFINE_IPARENT_TYPE(BASE) \
public: \
DEFINE_CLASS_NAME(A)

#define QUERY_INTERFACE_BEGIN \
public: \
PTR(void) queryInterface(ruint id) \
{ \
	PTR(void) pIterface = iparent_type::queryInterface(id); \
	if (pIterface) \
		return pIterface; \
 \
	switch (id) \
	{

#define QUERY_INTERFACE(A)        case IID(A): return static_cast<PTR(A)>(this);
#define QUERY_INTERFACE_END \
	} \
	return NULL; \
}

OPEN_RDO_NAMESPACE
class IObjectBase
{
public:
	PTR(void) queryInterface(ruint id)
	{
		return NULL;
	}
};
CLOSE_RDO_NAMESPACE

class IMy1
{
public:
	virtual void fun1() = 0;
};

class IMy2
{
public:
	virtual void fun2() = 0;
};

class IMy3
{
public:
	virtual void fun3() = 0;
};

class IMy4
{
public:
	virtual void fun4() = 0;
};

INTERFACE_REGISTRATOR(IMy1, 1);
INTERFACE_REGISTRATOR(IMy2, 2);
INTERFACE_REGISTRATOR(IMy3, 3);
INTERFACE_REGISTRATOR(IMy4, 4);

class MyClass: public rdo::IObjectBase, public IMy1, public IMy2
{
RDO_IOBJECT(MyClass, IObjectBase);

QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IMy1)
	QUERY_INTERFACE(IMy2)
QUERY_INTERFACE_END

protected:
	MyClass(int i)
		: m_i(i)
	{
		std::cout << "MyClass(int " << i << ");" << std::endl;
	}
	~MyClass()
	{
		std::cout << "~MyClass(): " << m_i << std::endl;
	}

private:
	int m_i;

	void unknow()
	{
		std::cout << "void unknow()" << std::endl;
	}
	void fun1()
	{
		std::cout << "void fun1()" << std::endl;
	}
	void fun2()
	{
		std::cout << "void fun2()" << std::endl;
	}
};

class MyClass2: public MyClass, public IMy3
{
RDO_IOBJECT(MyClass2, MyClass);

QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IMy3)
QUERY_INTERFACE_END

private:
	MyClass2(int i): MyClass(i)
	{
		std::cout << "MyClass2(int " << i << ");" << std::endl;
	}
	~MyClass2()
	{
		std::cout << "~MyClass2()" << std::endl;
	}
	void fun3()
	{
		std::cout << "void fun3()" << std::endl;
	}
};

void main()
{
	std::vector< rdo::LPIUnknow > list;
	rdo::smart_ptr<MyClass2> smptr = rdo::Factory<MyClass2>::create(1);
	list.push_back(&smptr);
	rdo::Interface<IMy3> ptr = list.back()->queryInterface(IID(IMy3));
	if (ptr)
	{
		ptr->fun3();
		rdo::Interface<IMy1> ptr2 = ptr.queryInterface(IID(IMy2));
		if (ptr2)
			ptr2->fun1();
	}
	int i = 1;
}
