#ifndef RDO_PROCESS_METHOD_MANAGER_H
#define RDO_PROCESS_METHOD_MANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------
// ---------- RPMethodPlugin
// ----------------------------------------------------------------------------
namespace rpMethod {
class RPMethod;
}

class RPMethodPlugin
{
private:
	rp::string          file_name;
	HMODULE             lib;
	rpMethod::RPMethod* method;

public:
	RPMethodPlugin( const rp::string& _file_name );
	~RPMethodPlugin();

	const rpMethod::RPMethod* getMethod() const { return method; }
	HMODULE getLib() const                      { return lib;    }

	static bool isMethod( const std::string& file_name );
};

// ----------------------------------------------------------------------------
// ---------- RPMethodManager
// ----------------------------------------------------------------------------
class RPMethodManager
{
protected:
	std::vector< RPMethodPlugin* > methods;
	void enumPlugins( const std::string& mask );

public:
	RPMethodManager();
	virtual ~RPMethodManager();
	
	RPMethodPlugin* find( rpMethod::RPMethod* method ) const {
		std::vector< RPMethodPlugin* >::const_iterator it = methods.begin();
		while ( it != methods.end() ) {
			if ( (*it)->getMethod() == method ) return *it;
			it++;
		}
		return NULL;
	}

	void init();
	void close();
};

#endif // RDO_PROCESS_METHOD_MANAGER_H
