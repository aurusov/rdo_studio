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

		virtual std::streambuf* setbuf( char* s, std::streamsize n ) {
			vec.reserve( n );
			vec.assign( s, s + n );
			current = 0;
			setg( vec.begin(), vec.begin(), vec.end() );
			setp( vec.begin(), vec.end() );
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

	public:
		binarybuf();
		virtual ~binarybuf() { vec.clear(); };
	};
	binarybuf buf;

public:
	binarystream();
	std::vector< char >& vec()    { return buf.vec;     };
	const char* date() const      { return &buf.vec[0]; };
};

}; // namespace rdo

#endif // RDOBINARYSTREAM_H
