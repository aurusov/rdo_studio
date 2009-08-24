#ifndef RDORUNTIME_OBJECT_H
#define RDORUNTIME_OBJECT_H

#include <rdocommon.h>
#include <rdotypes.h>

#include <vector>
#include <algorithm>

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDORuntimeObject
// ----------------------------------------------------------------------------
class RDORuntime;
class RDORuntimeParent;

class RDORuntimeObject
{
friend class RDORuntime;
public:
	virtual ~RDORuntimeObject();
	void reparent( RDORuntimeParent* parent );
	RDORuntimeParent* getParent() const { return m_parent;  }

	void detach();
/*
#ifndef _DEBUG
	void* operator new( size_t sz );
	void operator delete( void* v );
#endif
*/
protected:
	RDORuntimeParent* m_parent;

	RDORuntimeObject( RDORuntimeParent* parent );

private:
	size_t        m_object_size; // Размер текущего объекта
	static size_t s_memory_size; // Сумма размеров всех объектов
};

// ----------------------------------------------------------------------------
// ---------- RDORuntimeContainer
// ----------------------------------------------------------------------------
class RDORuntimeContainer
{
protected:
	RDORuntimeContainer(RDORuntime* runtime)
		: m_runtime(runtime)
	{}

	RDORuntime* getRuntime() const              { return m_runtime;    }
	void        setRuntime(RDORuntime* runtime) { m_runtime = runtime; }

private:
	RDORuntime* m_runtime;
};

// ----------------------------------------------------------------------------
// ---------- RDORuntimeParent
// ----------------------------------------------------------------------------
class RDORuntimeParent: public RDORuntimeObject
{
public:
	RDORuntimeParent( RDORuntimeParent* parent );
	virtual ~RDORuntimeParent();

	void insertObject( RDORuntimeObject* object ) {
		if ( object ) {
//			TRACE( "insert object: %d\n", object );
			if ( object == this ) {
//				TRACE( "insert parent himself %d !!!!!!!!!!!!!!!!!!!\n", this );
			} else {
				m_objects.push_back( object );
			}
		} else {
//			TRACE( "insert object NULL !!!!!!!!!!!!!!!\n" );
		}
	}
	void removeObject( RDORuntimeObject* object ) {
		std::vector< RDORuntimeObject* >::reverse_iterator it = std::find( m_objects.rbegin(), m_objects.rend(), object );
		if ( it != m_objects.rend() ) {
//			TRACE( "remove object: %d\n", object );
			// Комнада it.base() приводит реверсивный итератор к нормальному,
			// но перед этим необходимо сделать инкремент
			it++;
			m_objects.erase( it.base() );
		} else {
//			TRACE( "remove object: %d faild !!!!!!!!!!!!!!!!!!!!\n", object );
		}
	}
	void deleteObjects() {
		std::vector< RDORuntimeObject* >::reverse_iterator it = m_objects.rbegin();
		while ( it != m_objects.rend() ) {
			delete *it;
			it = m_objects.rbegin();
		}
		m_objects.clear();
	}

protected:
	std::vector< RDORuntimeObject* > m_objects;
};

// ----------------------------------------------------------------------------
// ---------- RDOSrcInfo
// ----------------------------------------------------------------------------
class RDOSrcInfo
{
public:
	struct Position {
		enum { UNDEFINE_POS  = ~0 };
		enum { UNDEFINE_LINE = ~0 };

		int m_first_line;
		int m_first_pos;
		int m_last_line;
		int m_last_pos;
		Position::Position():
			m_first_line( UNDEFINE_LINE ),
			m_first_pos ( UNDEFINE_POS  ),
			m_last_line ( UNDEFINE_LINE ),
			m_last_pos  ( UNDEFINE_POS  )
		{
		}
		Position::Position( int first_line, int first_pos, int last_line, int last_pos ):
			m_first_line( first_line ),
			m_first_pos( first_pos ),
			m_last_line( last_line ),
			m_last_pos( last_pos )
		{
		}
		bool empty() const
		{
			return m_first_line == UNDEFINE_LINE && m_first_pos == UNDEFINE_POS && m_last_line == UNDEFINE_LINE && m_last_pos == UNDEFINE_POS;
		}
		bool point() const
		{
			return m_first_line == m_last_line && m_first_pos == m_last_pos;
		}
	};

	RDOSrcInfo():
		m_text_data( "" ),
		m_file_type( rdoModelObjects::TRC )
	{
	}
	void setSrcInfo( const RDOSrcInfo& info ) {
		setSrcPos( info.m_position );
		setSrcText( info.m_text_data );
		setSrcFileType( info.m_file_type );
	}
	void setSrcInfo( const RDOSrcInfo& begin, const std::string& delim, const RDOSrcInfo& end ) {
		setSrcPos( begin.src_pos().m_first_line, begin.src_pos().m_first_pos, end.src_pos().m_last_line, end.src_pos().m_last_pos );
		setSrcText( begin.src_text() + delim + end.src_text() );
		setSrcFileType( begin.src_filetype() );
	}
	void setSrcPos( const Position& position ) {
		m_position.m_first_line = position.m_first_line;
		m_position.m_first_pos  = position.m_first_pos;
		m_position.m_last_line  = position.m_last_line;
		m_position.m_last_pos   = position.m_last_pos;
	}
	void setSrcPos( const Position& position_begin, const Position& position_end ) {
		m_position.m_first_line = position_begin.m_first_line;
		m_position.m_first_pos  = position_begin.m_first_pos;
		m_position.m_last_line  = position_end.m_last_line;
		m_position.m_last_pos   = position_end.m_last_pos;
	}
	void setSrcPos( int first_line, int first_pos, int last_line, int last_pos ) {
		m_position.m_first_line = first_line;
		m_position.m_first_pos  = first_pos;
		m_position.m_last_line  = last_line;
		m_position.m_last_pos   = last_pos;
	}
	virtual void setSrcText( const std::string& value ) {
		m_text_data = value;
	}
	void setSrcFileType( rdoModelObjects::RDOFileType value ) {
		m_file_type = value;
	}
	const RDOSrcInfo&                  src_info() const     { return *this;       }
	const Position&                    src_pos()  const     { return m_position;  }
	const std::string&                 src_text() const     { return m_text_data; }
	const rdoModelObjects::RDOFileType src_filetype() const { return m_file_type; }
	bool                               src_empty() const
	{
		return m_position.empty() && m_text_data.empty() && m_file_type == rdoModelObjects::TRC;
	}

private:
	Position                     m_position;
	std::string                  m_text_data;
	rdoModelObjects::RDOFileType m_file_type;
};

} // namespace rdoRuntime

// ----------------------------------------------------------------------------
// ---------- INotify
// ----------------------------------------------------------------------------
class INotify
{
public:
	virtual void notify(ruint message, PTR(void) param) = 0;
};
#define DECLARE_INotify \
	virtual void notify(ruint message, PTR(void) param);

#endif // RDORUNTIME_OBJECT_H
