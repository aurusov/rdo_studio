#ifndef RDORUNTIME_OBJECT_H
#define RDORUNTIME_OBJECT_H

#include "rdodefines.h"
#include <rdocommon.h>

namespace rdoRuntime {

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

class RDORuntimeRDOValue: public RDOException
{
public:
	std::string getType() const { return "RDOValue Error"; }
	RDORuntimeRDOValue( const std::string& str ): RDOException( str ) {}
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
	size_t        object_size; // Размер текущего объекта
	static size_t memory_size; // Сумма размеров всех объектов

protected:
	RDORuntimeParent* parent;

	RDORuntimeObject( RDORuntimeParent* _parent );

public:
	virtual ~RDORuntimeObject();
	void reparent( RDORuntimeParent* _parent );
	RDORuntimeParent* getParent() const { return parent;  }
//	RDORuntime*       getRuntime() const;

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
	std::vector< RDORuntimeObject* > objects;

public:
	RDORuntimeParent( RDORuntimeParent* _parent );
	virtual ~RDORuntimeParent();

	void insertObject( RDORuntimeObject* object ) {
		if ( object ) {
//			TRACE( "insert object: %d\n", object );
			if ( object == this ) {
//				TRACE( "insert parent himself %d !!!!!!!!!!!!!!!!!!!\n", this );
			} else {
				objects.push_back( object );
			}
		} else {
//			TRACE( "insert object NULL !!!!!!!!!!!!!!!\n" );
		}
	}
	void removeObject( RDORuntimeObject* object ) {
		std::vector< RDORuntimeObject* >::reverse_iterator it = std::find( objects.rbegin(), objects.rend(), object );
		if ( it != objects.rend() ) {
//			TRACE( "remove object: %d\n", object );
			// Комнада it.base() приводит реверсивный итератор к нормальному,
			// но перед этим необходимо сделать инкремент
			it++;
			objects.erase( it.base() );
		} else {
//			TRACE( "remove object: %d faild !!!!!!!!!!!!!!!!!!!!\n", object );
		}
	}
	void deleteObjects() {
		std::vector< RDORuntimeObject* >::reverse_iterator it = objects.rbegin();
		while ( it != objects.rend() ) {
			delete *it;
			it = objects.rbegin();
		}
		objects.clear();
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOSrcInfo
// ----------------------------------------------------------------------------
class RDOSrcInfo
{
public:
	struct Position {
		int first_line;
		int first_pos;
		int last_line;
		int last_pos;
		Position::Position():
			first_line( -1 ),
			first_pos( -1 ),
			last_line( -1 ),
			last_pos( -1 )
		{
		}
		Position::Position( int _first_line, int _first_pos, int _last_line, int _last_pos ):
			first_line( _first_line ),
			first_pos( _first_pos ),
			last_line( _last_line ),
			last_pos( _last_pos )
		{
		}
	};

private:
	Position                     position;
	std::string                  text_data;
	rdoModelObjects::RDOFileType file_type;

public:
	RDOSrcInfo():
		text_data( "" ),
		file_type( rdoModelObjects::TRC )
	{
	}
	void setSrcInfo( const RDOSrcInfo& _info ) {
		setSrcPos( _info.position );
		setSrcText( _info.text_data );
		setSrcFileType( _info.file_type );
	}
	void setSrcInfo( const RDOSrcInfo& begin, const std::string& delim, const RDOSrcInfo& end ) {
		setSrcPos( begin.src_pos().first_line, begin.src_pos().first_pos, end.src_pos().last_line, end.src_pos().last_pos );
		setSrcText( begin.src_text() + delim + end.src_text() );
		setSrcFileType( begin.src_filetype() );
	}
	void setSrcPos( const Position& _position ) {
		position.first_line = _position.first_line;
		position.first_pos  = _position.first_pos;
		position.last_line  = _position.last_line;
		position.last_pos   = _position.last_pos;
	}
	void setSrcPos( const Position& _position_begin, const Position& _position_end ) {
		position.first_line = _position_begin.first_line;
		position.first_pos  = _position_begin.first_pos;
		position.last_line  = _position_end.last_line;
		position.last_pos   = _position_end.last_pos;
	}
	void setSrcPos( int first_line, int first_pos, int last_line, int last_pos ) {
		position.first_line = first_line;
		position.first_pos  = first_pos;
		position.last_line  = last_line;
		position.last_pos   = last_pos;
	}
	virtual void setSrcText( const std::string& value ) {
		text_data = value;
	}
	void setSrcFileType( rdoModelObjects::RDOFileType value ) {
		file_type = value;
	}
	const RDOSrcInfo&                  src_info() const     { return *this;     }
	const Position&                    src_pos()  const     { return position;  }
	const std::string&                 src_text() const     { return text_data; }
	const rdoModelObjects::RDOFileType src_filetype() const { return file_type; }
};

// ----------------------------------------------------------------------------
// ---------- RDOEnum
// ----------------------------------------------------------------------------
class RDOEnum: public RDORuntimeObject
{
private:
	std::vector< std::string > enumVals;

public:
	RDOEnum( RDORuntimeParent* _parent ):
		RDORuntimeObject( _parent )
	{
	}
	void add( const std::string& next ) {
		enumVals.push_back( next );
	}
	int findEnum( const std::string& val ) const {
		std::vector< std::string >::const_iterator it = std::find( enumVals.begin(), enumVals.end(), val );
		return it != enumVals.end() ? it - enumVals.begin() : -1;
	}
	const std::vector< std::string >& getValues() const { return enumVals; }
};

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
// typedef double RDOValue;

class RDOValue
{
friend bool operator< ( const RDOValue& value1, const RDOValue& value2 );
friend bool operator> ( const RDOValue& value1, const RDOValue& value2 );

public:
	enum ParamType {
		pt_int    = 0,
		pt_real   = 1,
		pt_enum   = 2,
		pt_unknow = 3
	};

private:
	ParamType type;
	union {
		int    i_value;
		double d_value;
	} value;
	void* data;

public:
	RDOValue(): type( pt_int ) {
		value.i_value = 0;
	}
	~RDOValue() {
	}
	RDOValue( const RDOValue& _value ):
		type( _value.type ),
		value( _value.value ),
		data( _value.data )
	{
	}
	RDOValue( ParamType _type ): type( _type ) {
		switch ( type ) {
			case pt_int : value.i_value = 0;
			case pt_real: value.d_value = 0;
			default     : throw RDORuntimeRDOValue("");
		}
	}
	RDOValue( int _value ): type( pt_int ) {
		value.i_value = _value;
	}
	RDOValue( unsigned int _value ): type( pt_int ) {
		value.i_value = _value;
	}
	RDOValue( double _value ): type( pt_real ) {
		value.d_value = _value;
	}
	RDOValue( RDOEnum* enums, const std::string& val ): type( pt_enum ) {
		data          = enums;
		value.i_value = enums->findEnum( val );
	}
	ParamType getType() const { return type; }
	int getInt() const {
		switch ( type ) {
			case pt_int : return value.i_value;
			case pt_real: return value.d_value;
			case pt_enum: return value.i_value;
			default     : throw RDORuntimeRDOValue("");
		}
	}
	int getEnumAsInt() const {
		switch ( type ) {
			case pt_int : return value.i_value;
			case pt_real: return value.d_value;
			case pt_enum: return value.i_value;
			default     : throw RDORuntimeRDOValue("");
		}
	}
	RDOEnum* getEnum() const {
		switch ( type ) {
			case pt_enum: return reinterpret_cast<RDOEnum*>(data);
			default     : throw RDORuntimeRDOValue("");
		}
	}
	double getDouble() const {
		switch ( type ) {
			case pt_int : return value.i_value;
			case pt_real: return value.d_value;
			case pt_enum: return value.i_value;
			default     : throw RDORuntimeRDOValue("");
		}
	}
	bool getBool() const {
		switch ( type ) {
			case pt_int : return value.i_value ? true : false;
			case pt_real: return value.d_value ? true : false;
			case pt_enum: return value.i_value ? true : false;
			default     : throw RDORuntimeRDOValue("");
		}
	}
	std::string getAsString() const {
		switch ( type ) {
			case pt_int : return rdo::format( "%d", value.i_value );
			case pt_real: return toString( value.d_value );
			case pt_enum: return reinterpret_cast<RDOEnum*>(data)->getValues().at( value.i_value );
			default     : throw RDORuntimeRDOValue("");
		}
	}
	std::string getAsStringForTrace() const {
		switch ( type ) {
			case pt_int : return rdo::format( "%d", value.i_value );
			case pt_real: return toString( value.d_value );
			case pt_enum: return rdo::format( "%d", value.i_value );
			default     : throw RDORuntimeRDOValue("");
		}
	}
	RDOValue& operator= ( const RDOValue& _value ) {
		type  = _value.type;
		value = _value.value;
		data  = _value.data;
		return *this;
	}
	bool operator< ( const RDOValue& _value ) {
		switch ( type ) {
			case pt_int : switch ( _value.type ) {
								case pt_int : return value.i_value < _value.value.i_value;
								case pt_real: return value.i_value < _value.value.d_value;
								default     : throw RDORuntimeRDOValue("");
							}
			case pt_real: switch ( _value.type ) {
								case pt_int : return value.d_value < _value.value.i_value;
								case pt_real: return value.d_value < _value.value.d_value;
								default     : throw RDORuntimeRDOValue("");
							}
			default     : throw RDORuntimeRDOValue("");
		}
	}
	bool operator> ( const RDOValue& _value ) {
		switch ( type ) {
			case pt_int : switch ( _value.type ) {
								case pt_int : return value.i_value > _value.value.i_value;
								case pt_real: return value.i_value > _value.value.d_value;
								default     : throw RDORuntimeRDOValue("");
							}
			case pt_real: switch ( _value.type ) {
								case pt_int : return value.d_value > _value.value.i_value;
								case pt_real: return value.d_value > _value.value.d_value;
								default     : throw RDORuntimeRDOValue("");
							}
			default     : throw RDORuntimeRDOValue("");
		}
	}
	bool operator!= ( const RDOValue& _value ) const {
		switch ( type ) {
			case pt_int : switch ( _value.type ) {
								case pt_int : return value.i_value != _value.value.i_value;
								case pt_real: return value.i_value != _value.value.d_value;
								default     : throw RDORuntimeRDOValue("");
							}
			case pt_real: switch ( _value.type ) {
								case pt_int : return value.d_value != _value.value.i_value;
								case pt_real: return value.d_value != _value.value.d_value;
								default     : throw RDORuntimeRDOValue("");
							}
			case pt_enum: switch ( _value.type ) {
								case pt_enum: return value.i_value != _value.value.i_value;
								default     : throw RDORuntimeRDOValue("");
							}
			default     : throw RDORuntimeRDOValue("");
		}
	}
	bool operator== ( const RDOValue& _value ) const {
		switch ( type ) {
			case pt_int : switch ( _value.type ) {
								case pt_int : return value.i_value == _value.value.i_value;
								case pt_real: return value.i_value == _value.value.d_value;
								default     : throw RDORuntimeRDOValue("");
							}
			case pt_real: switch ( _value.type ) {
								case pt_int : return value.d_value  == _value.value.i_value;
								case pt_real: return value.d_value  == _value.value.d_value;
								default     : throw RDORuntimeRDOValue("");
							}
			case pt_enum: switch ( _value.type ) {
								case pt_enum: return data == _value.data && value.i_value == _value.value.i_value;
								default     : throw RDORuntimeRDOValue("");
							}
			default     : throw RDORuntimeRDOValue("");
		}
	}
	bool operator<= ( const RDOValue& _value ) const {
		switch ( type ) {
			case pt_int : switch ( _value.type ) {
								case pt_int : return value.i_value <= _value.value.i_value;
								case pt_real: return value.i_value <= _value.value.d_value;
								default     : throw RDORuntimeRDOValue("");
							}
			case pt_real: switch ( _value.type ) {
								case pt_int : return value.d_value <= _value.value.i_value;
								case pt_real: return value.d_value <= _value.value.d_value;
								default     : throw RDORuntimeRDOValue("");
							}
			default     : throw RDORuntimeRDOValue("");
		}
	}
	bool operator>= ( const RDOValue& _value ) const {
		switch ( type ) {
			case pt_int : switch ( _value.type ) {
								case pt_int : return value.i_value >= _value.value.i_value;
								case pt_real: return value.i_value >= _value.value.d_value;
								default     : throw RDORuntimeRDOValue("");
							}
			case pt_real: switch ( _value.type ) {
								case pt_int : return value.d_value >= _value.value.i_value;
								case pt_real: return value.d_value >= _value.value.d_value;
								default     : throw RDORuntimeRDOValue("");
							}
			default     : throw RDORuntimeRDOValue("");
		}
	}
	RDOValue operator- () const {
		RDOValue _value( *this );
		switch ( type ) {
			case pt_int : _value.value.i_value = -value.i_value; break;
			case pt_real: _value.value.d_value = -value.d_value; break;
			default     : throw RDORuntimeRDOValue("");
		}
		return _value;
	}
	void operator+= ( const RDOValue& _value ) {
		switch ( type ) {
			case pt_int : switch ( _value.type ) {
								case pt_int :
								case pt_real: value.i_value += _value.getInt(); return;
								default     : throw RDORuntimeRDOValue("");
							}
			case pt_real: switch ( _value.type ) {
								case pt_int :
								case pt_real: value.d_value += _value.getDouble(); return;
								default     : throw RDORuntimeRDOValue("");
							}
			default     : throw RDORuntimeRDOValue("");
		}
	}
	void operator-= ( const RDOValue& _value ) {
		switch ( type ) {
			case pt_int : switch ( _value.type ) {
								case pt_int :
								case pt_real: value.i_value -= _value.getInt(); return;
								default     : throw RDORuntimeRDOValue("");
							}
			case pt_real: switch ( _value.type ) {
								case pt_int :
								case pt_real: value.d_value -= _value.getDouble(); return;
								default     : throw RDORuntimeRDOValue("");
							}
			default     : throw RDORuntimeRDOValue("");
		}
	}
	void operator*= ( const RDOValue& _value ) {
		switch ( type ) {
			case pt_int : switch ( _value.type ) {
								case pt_int : value.i_value *= _value.value.i_value; return;
								case pt_real: value.d_value = ((double)value.i_value) * _value.value.d_value; type = pt_real; return;
								default     : throw RDORuntimeRDOValue("");
							}
			case pt_real: switch ( _value.type ) {
								case pt_int :
								case pt_real: value.d_value *= _value.getDouble(); return;
								default     : throw RDORuntimeRDOValue("");
							}
			default     : throw RDORuntimeRDOValue("");
		}
	}
	void operator/= ( const RDOValue& _value ) {
		switch ( type ) {
			case pt_int : switch ( _value.type ) {
								case pt_int :
								case pt_real: value.d_value = ((double)value.i_value) / _value.getDouble(); type = pt_real; return;
								default     : throw RDORuntimeRDOValue("");
							}
			case pt_real: switch ( _value.type ) {
								case pt_int :
								case pt_real: value.d_value = value.d_value / _value.getDouble(); return;
								default     : throw RDORuntimeRDOValue("");
							}
			default     : throw RDORuntimeRDOValue("");
		}
	}
	RDOValue operator+ ( const RDOValue& _value ) const {
		RDOValue value2( *this );
		value2 += _value;
		return value2;
	}
	RDOValue operator- ( const RDOValue& _value ) const {
		RDOValue value2( *this );
		value2 -= _value;
		return value2;
	}
	RDOValue operator* ( const RDOValue& _value ) const {
		RDOValue value2( *this );
		value2 *= _value;
		return value2;
	}
	RDOValue operator/ ( const RDOValue& _value ) const {
		RDOValue value2( *this );
		value2 /= _value;
		return value2;
	}
};

inline std::ostream& operator<< ( std::ostream& stream, const RDOValue& value ) {
	stream << value.getAsStringForTrace();
	return stream;
}

inline bool operator< ( const RDOValue& value1, const RDOValue& value2 ) {
	return const_cast<RDOValue&>(value1).operator<( value2 );
}

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

inline std::string operator+ ( char* str1, std::string& str2 )
{
	return std::string(str1).append(str2);
}

inline std::string operator+ ( std::string& str1, char* str2 )
{
	return std::string(str1).append(str2);
}

#endif // RDORUNTIME_OBJECT_H
