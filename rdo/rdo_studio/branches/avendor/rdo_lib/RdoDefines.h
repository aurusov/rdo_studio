#ifndef RDO_DEFINES_H_
#define RDO_DEFINES_H_

typedef std::list<double> ListDouble;
typedef std::list<double>::iterator ListDoubleIt;

template <class Arg>
inline std::string toString(Arg i)
{
   std::ostringstream str;
   str << i;
   return str.str();
}

template <class Res, class Sim> Res *ChoiceFirstFrom(std::list<Res *> &storage, 
       bool (Res::*func)(Sim *), Sim *sim)
{
   for(std::list<Res *>::iterator i = storage.begin(); 
         i != storage.end(); i++)
   {
      if(((*i)->*func)(sim))
          return (*i);
   }

   return NULL;
}

template <class Stor> void DeleteAllObjects(Stor &storage)
{
//   for(Stor::iterator i = storage.begin(); 
//         i != storage.end(); i++)

//	std::for_each<Stor::reverse_iterator, void (*)(Stor::value_type)>(storage.rbegin(), storage.rend(), ::operator delete<Stor::value_type>);

   for(Stor::reverse_iterator i = storage.rbegin(); 
         i != storage.rend(); i++)
   {
		if(*i != NULL)
	      delete(*i);
   }

   storage.clear();
}

#endif //RDO_DEFINES_H_
