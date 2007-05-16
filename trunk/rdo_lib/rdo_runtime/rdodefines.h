#ifndef RDODEFINES_H
#define RDODEFINES_H

template < class Arg > inline std::string toString( Arg i )
{
   std::ostringstream str;
   str << i;
   return str.str();
}

template < class Stor > void DeleteAllObjects( Stor& storage )
{
	Stor::reverse_iterator it = storage.rbegin();
	while ( it != storage.rend() ) {
		delete *it;
		it++;
	}
	storage.clear();
}

#endif // RDODEFINES_H
