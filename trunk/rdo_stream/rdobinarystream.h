#ifndef RDOBINARYSTREAM_H
#define RDOBINARYSTREAM_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <iostream>
#include <vector>

namespace rdo {

// ----------------------------------------------------------------------------
// ---------- binarystream
// ----------------------------------------------------------------------------
class binarystream: public std::iostream
{
private:
	class binarybuf: public std::streambuf
	{
	friend class binarystream;
	protected:
		std::vector< char > vec;
		int current;
		ios_base::openmode openmode;

		virtual std::streambuf* setbuf( char* s, std::streamsize n ) {
			vec.resize( n );
			vec.assign( s, s + n );
			initPtr();
			return this;
		}
		virtual int_type overflow( int_type c = traits_type::eof() ) {
			if ( c != traits_type::eof() ) {
				vec.push_back( traits_type::to_char_type( c ) );
				return traits_type::not_eof( c );
			} else {
				return traits_type::eof();
			}
		}
		virtual int_type underflow() {
			return traits_type::to_int_type( vec[current] );
		}
		virtual int_type uflow() {
			int_type c = traits_type::to_int_type( vec[current++] );
			setg( vec.begin(), &vec[current], vec.end() );
			return c;
		}
		virtual pos_type seekoff( off_type off, ios_base::seekdir way, ios_base::openmode which = ios_base::in | ios_base::out) {
			switch ( way ) {
				case ios_base::beg: current = off; break;
				case ios_base::cur: current += off; break;
				case ios_base::end: current = vec.size() - off; break;
			}
			return current;
		}
		virtual pos_type seekpos( pos_type sp, ios_base::openmode which = ios_base::in | ios_base::out ) {
			current = sp;
			return current;
		}

	protected:
		binarybuf();
		virtual ~binarybuf() { vec.clear(); }

		void initPtr() {
			current = 0;
			setg( vec.begin(), vec.begin(), vec.end() );
			setp( vec.begin(), vec.end() );
		}
	};
	binarybuf buf;

public:
	binarystream( ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary );
	char* data()                                    { return buf.vec.begin();             }
	std::vector< char >::size_type size()           { return buf.vec.size();              }
	void resize( std::vector< char >::size_type n ) { buf.vec.resize( n ); buf.initPtr(); }
//	std::vector< char >& vec()                      { return buf.vec;                     }
	ios_base::openmode getOpenMode() const          { return buf.openmode;                }
};

// ----------------------------------------------------------------------------
// ---------- stringstream
// ----------------------------------------------------------------------------
class stringstream: public binarystream
{
	stringstream(): binarystream( ios_base::in | ios_base::out ) {};
};

}; // namespace rdo

#endif // RDOBINARYSTREAM_H
