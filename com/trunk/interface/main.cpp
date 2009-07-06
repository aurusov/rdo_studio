#include <vector>
#include <iostream>
#include <string>

class IUnknow
{
public:
	virtual void AddRef () = 0;
	virtual void Release() = 0;
};
typedef IUnknow* LPIUnknow;

template<class T>
class Interface
{
public:
	Interface()
		: m_interface(NULL)
		, m_counter  (NULL)
	{}
	Interface(T* aInterface, LPIUnknow counter)
		: m_interface(aInterface)
		, m_counter  (counter   )
	{
		m_counter->AddRef();
	}
	~Interface()
	{
		if (m_counter)
			m_counter->Release();
	}
	operator bool() const
	{
		return m_interface != NULL;
	}
	T* operator-> ()
	{
		return m_interface;
	}

private:
	T*        m_interface;
	LPIUnknow m_counter;
};

template<class T>
class LocalInterface
{
public:
	LocalInterface(T* p)
		: m_pInterface(p)
	{}
	operator bool () const
	{
		return m_pInterface != NULL;
	}
	T* get()
	{
		return m_pInterface;
	}
	T* operator-> ()
	{
		return get();
	}

public:
	T* m_pInterface;
};

template<class T>
class smart_ptr: public IUnknow
{
public:
	typedef T            object_type;
	typedef smart_ptr<T> this_type;

	smart_ptr(T* obj)
		: m_object(obj)
	{
		allocateCounter();
		counter() = 1;
	}
	smart_ptr(const this_type& sptr)
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
	T* get()
	{
		return m_object;
	}
	operator bool () const
	{
		return get() != NULL;
	}
	T* operator-> ()
	{
		return get();
	}
	template<class I> Interface<I> query()
	{
		if (get() != NULL)
		{
			LocalInterface<I> localInterface = get()->query<I>();
			if (localInterface)
				return Interface<I>(localInterface.get(), this);
		}
		return Interface<I>();
	}

private:
	unsigned int* m_counter;
	T*            m_object;

	void allocateCounter()
	{
		m_counter = new unsigned int;
	}
	void deallocateCounter()
	{
		delete m_counter;
	}
	unsigned int& counter()
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
	static void destroy(T* obj)
	{
		delete obj;
	}
};

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

#define QUERY_INTERFACE_NULL         template<class T> LocalInterface<T> query() { return NULL;                  }
#define QUERY_INTERFACE_PARENT(A)    template<class T> LocalInterface<T> query() { return A::query<T>();         }
#define QUERY_INTERFACE_PARENT2(A,B) template<class T> LocalInterface<T> query() { LocalInterface<T> lpintA = A::query<T>(); return lpintA ? lpintA : B::query<T>(); }
#define QUERY_INTERFACE(A)           template<>        LocalInterface<A> query() { return static_cast<A*>(this); }

#define DEFINE_THIS_TYPE(A)  typedef A this_type;
#define DEFINE_FACTORY(A)    friend class Factory<A>;
#define DEFINE_CLASS_NAME(A) static std::string className() { return #A; }

#define RDO_IOBJECT(A, BASE) \
protected: \
DEFINE_THIS_TYPE(A) \
DEFINE_FACTORY(A) \
public: \
DEFINE_CLASS_NAME(A) \
QUERY_INTERFACE_PARENT(BASE)

class IObjectBase
{
public:
	QUERY_INTERFACE_NULL;
};

class MyClass: public IObjectBase, public IMy1, public IMy2
{
RDO_IOBJECT(MyClass, IObjectBase);

public:
	QUERY_INTERFACE(IMy1);
	QUERY_INTERFACE(IMy2);

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

public:
	QUERY_INTERFACE(IMy3);

private:
	MyClass2(int i)
		: MyClass(i)
	{}
	void fun3()
	{
		std::cout << "void fun3()" << std::endl;
	}
};

void main()
{
	typedef smart_ptr<MyClass2> SPMyClass;
	std::cout << SPMyClass::object_type::className() << std::endl;
	std::vector<SPMyClass> list;
	list.push_back(Factory<MyClass2>::create(1));
	list.push_back(Factory<MyClass2>::create(3));
	list.push_back(Factory<MyClass2>::create(2));

	Interface<IMy2> lpIMy2 = list.back().query<IMy2>();
	if (lpIMy2)
		lpIMy2->fun2();
}
