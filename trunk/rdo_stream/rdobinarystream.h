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
		binarystream* stream;

		virtual std::streambuf* setbuf( char* s, std::streamsize n ) {
			vec.resize( n );
			vec.assign( s, s + n );
			initPtr();
			return this;
		}
		virtual int_type overflow( int_type c = traits_type::eof() ) {
			if ( c != traits_type::eof() ) {
				vec.push_back( traits_type::to_char_type( c ) );
//				vec.resize( vec.size() + 1 );
//				setp( &vec[ vec.size() - 1 ], vec.end() );
				stream->setstate( goodbit );
				return traits_type::not_eof( c );
			} else {
				stream->setstate( eofbit );
				return traits_type::eof();
			}
		}
		virtual std::streamsize xsgetn( std::streambuf::char_type* s, std::streamsize n ) {
			std::streamsize len = n;
			if ( vec.size() - current < len ) len = vec.size() - current;
			memcpy( s, &vec[current], len );
			current += len;
			if ( current < vec.size() ) {
				setg( &vec[current], &vec[current], &vec[current + 1] );
				stream->setstate( goodbit );
			} else {
				setg( &vec[current], &vec[current], &vec[current] );
				stream->setstate( eofbit );
			}
			return len;
		}
		virtual std::streamsize xsputn( const std::streambuf::char_type* s, std::streamsize n ) {
			if ( current + n > vec.size() ) {
				vec.resize( current + n );
			}
			memcpy( &vec[current], s, n );
			current += n;
			return n;
		}
		virtual int_type underflow() {
			stream->setstate( eofbit );
			return traits_type::eof();
		}
		virtual int_type uflow() {
			stream->setstate( eofbit );
			return traits_type::eof();
		}
		virtual pos_type seekoff( off_type off, ios_base::seekdir way, ios_base::openmode which = ios_base::in | ios_base::out ) {
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
			if ( vec.size() ) {
				setg( &vec[current], &vec[current], &vec[current + 1] );
				setp( &vec[current], &vec[current + 1] );
			} else {
				setg( vec.end(), vec.end(), vec.end() );
				setp( vec.end(), vec.end() );
			}
		}
	};
	binarybuf buf;

public:
	binarystream( ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary );
	char* data()                                    { return buf.vec.begin();             }
	std::vector< char >::size_type size()           { return buf.vec.size();              }
	void resize( std::vector< char >::size_type n ) { buf.vec.resize( n ); buf.initPtr(); }
	ios_base::openmode getOpenMode() const          { return buf.openmode;                }
};

// ----------------------------------------------------------------------------
// ---------- stringstream
// ----------------------------------------------------------------------------
class stringstream: public binarystream
{
public:
	stringstream(): binarystream( ios_base::in | ios_base::out ) {};
};

}; // namespace rdo

#endif // RDOBINARYSTREAM_H
