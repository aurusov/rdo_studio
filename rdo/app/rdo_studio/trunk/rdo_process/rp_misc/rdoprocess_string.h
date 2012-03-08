#ifndef RDO_PROCESS_GLOBAL_H
#define RDO_PROCESS_GLOBAL_H

#include <string>
#include <locale>
#include <algorithm>

namespace rp {

class string: public tstring
{
protected:
	//! Структура (функторал), которая используется для перевода символа в нижний регистр
	struct toLower {
		//! Конструктор
		/*! \param loc - Локаль. Через неё определяется нижний регистр для символа.
\nПример:\code
toLower tr( std::locale("rus") );
		\endcode
		*/
		toLower( std::locale loc ): m_loc(loc) {};
		char operator()(char c) {
			return std::tolower( c, m_loc );
		}
		std::locale m_loc;
	};
	//! Структура (функторал), которая используется для перевода символа в верхний регистр
	struct toUpper {
		toUpper( std::locale loc ): m_loc(loc) {};
		char operator()(char c) {
			return std::toupper( c, m_loc );
		}
		std::locale m_loc;
	};

public:
	//! Конструктор по-умолчанию, создает пустую строку.
	string(): std::basic_string< char >() {};
	//! Создает строку по указателю.
	string( const char* str ): std::basic_string< char >( str ) {};
	//! Конструктор копии.
	string( CREF(tstring) str ): std::basic_string< char >( str ) {};

	int      toint() const    { return atoi( c_str() );                     }
	long int tolong() const   { return atol( c_str() );                     }
	float    tofloat() const  { return static_cast<float>(atof( c_str() )); }
	double   todouble() const { return strtod( c_str(), NULL );             }
	int      tohex() const;
	bool     tobool() const   { return tolower() == "true";                 }

	//! Переводит целое число в строчку.
	/*! \param value - целое число для перевода в строку.
		\return Строковое представление целого числа.
		\par Пример:
		\code
rp::string str = rp::string::fromint( 20 );
		\endcode
		\sa fromdouble()
	*/
	static rp::string fromint( int value ) {
		char buffer[20];
		_itoa_s( value, buffer, 10 );
		return rp::string( buffer );
	}

	//! Переводит вещественное число в строчку.
	/*! \param value - вещественное число для перевода в строку.
		\return Строковое представление вещественного числа.
		\par Пример:
		\code
rp::string str = rp::string::fromdouble( 20.2 );
		\endcode
		\sa fromint()
	*/
	static rp::string fromdouble( double value ) {
		char buffer[40];
		//_gcvt( value, 10, buffer );
		_gcvt_s(buffer,value,10);
		rp::string str( buffer );
		if ( str[str.length()-1] == '.' ) {
			str += '0';
		} else if ( str[str.length()-1] == ',' ) {
			str[str.length()-1] = '.';
			str += '0';
		}
		return str;
	}

	static rp::string frombool( bool value ) {
		return value ? "true" : "false";
	}

	//! Переводит все символы строки в строчные.
	/*! \return Новая строка, все символы которой строчные, исходная остается без изменений.
		\sa toupper()
	*/
	rp::string tolower() const {
		rp::string str( *this );
		toLower tr( std::locale("rus") );
		std::transform( str.begin(), str.end(), str.begin(), tr );
		return str;
	}
	//! Переводит все символы строки в заглавные.
	/*! \return Новая строка, все символы которой заглавные, исходная остается без изменений.
		\sa tolower()
	*/
	rp::string toupper() const {
		rp::string str( *this );
		toUpper tr( std::locale("rus") );
		std::transform( str.begin(), str.end(), str.begin(), tr );
		return str;
	}
	
	void trim_left() {
		static char szDelims[] = " \t\n\r";
		erase( 0, find_first_not_of( szDelims ) );
	}
	void trim_right() {
		static char szDelims[] = " \t\n\r";
		erase( find_last_not_of( szDelims ) + 1, tstring::npos );
	}
	void trim() {
		static char szDelims[] = " \t\n\r";
		erase( 0, find_first_not_of( szDelims ) );
		erase( find_last_not_of( szDelims ) + 1, tstring::npos );
	}

};

rp::string format( const char* str, ... );
rp::string format( ruint resource, ... );
rp::string extractFilePath( const rp::string& fileName );

} // namespace rp

#endif // RDO_PROCESS_GLOBAL_H
