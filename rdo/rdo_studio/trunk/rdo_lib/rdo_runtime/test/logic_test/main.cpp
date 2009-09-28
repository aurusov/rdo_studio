#include <vector>
#include <rdomacros.h>
#include <rdotypes.h>
#include <rdodebug.h>

template<class T>
class ContainerVector
{
public:
	typedef T                                  Item;
	typedef std::vector<Item>                  Container;
	typedef typename Container::iterator       Iterator;
	typedef typename Container::const_iterator CIterator;

	Iterator  begin()       { return m_list.begin(); }
	Iterator  end  ()       { return m_list.end  (); }
	CIterator begin() const { return m_list.begin(); }
	CIterator end  () const { return m_list.end  (); }
	rbool     empty() const { return m_list.empty(); }

	REF(ContainerVector) operator() (CREF(Item) item)
	{
		m_list.push_back(item);
		return *this;
	}

private:
	Container m_list;
};

template <class Container>
class OrderFIFO
{
public:
	static typename Container::CIterator getFirst(CREF(Container) container)
	{
		return container.begin();
	}
};

template <class Container>
class OrderLIFO
{
public:
	static typename Container::CIterator getFirst(CREF(Container) container)
	{
		if (container.empty())
			return container.end();

		Container::CIterator last = container.end();
		--last;
		return last;
	}
};

template <class Container>
class OrderFIFOCashed
{
public:
	OrderFIFOCashed()
		: m_inited(false)
	{}
	typename Container::CIterator getFirst(CREF(Container) container)
	{
		m_lastItemIt = container.begin();
		m_inited     = true;
		return m_lastItemIt;
	}

private:
	rbool                         m_inited;
	typename Container::CIterator m_lastItemIt;
};

template <class Container, class Order>
class Queue
{
public:
	typename Container::CIterator getFirst(CREF(Container) container) const
	{
		return Order::getFirst(container);
	}
	typename Container::CIterator getFirst(CREF(Container) container, REF(Order) order) const
	{
		return order.getFirst(container);
	}
};

void main()
{
	typedef ruint                  Item;
	typedef ContainerVector<Item>  Container;
	Container ñontainer;
	ñontainer(1)(2)(4)(3);

	{
		Queue<Container, OrderFIFO<Container> > queue;
		Container::CIterator it = queue.getFirst(ñontainer);
		int i = 1;
	}
	{
		Queue<Container, OrderLIFO<Container> > queue;
		Container::CIterator it = queue.getFirst(ñontainer);
		int i = 1;
	}
	{
		OrderFIFOCashed<Container> order;
		Queue<Container, OrderFIFOCashed<Container> > queue;
		Container::CIterator it = queue.getFirst(ñontainer, order);
		int i = 1;
	}
}
