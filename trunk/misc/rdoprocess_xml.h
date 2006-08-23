#ifndef RDO_PROCESS_XML_H
#define RDO_PROCESS_XML_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_string.h"

namespace rp {

// ----------------------------------------------------------------------------
// ---------- RPXMLNode
// ----------------------------------------------------------------------------
class RPXMLNode
{
protected:
	RPXMLNode() {}
	virtual ~RPXMLNode() {}
public:
	virtual rp::string getName() const = 0;
	virtual RPXMLNode* makeChild( const rp::string& name ) = 0;
	virtual RPXMLNode* findFirstChild( const rp::string& name ) = 0;
	virtual RPXMLNode* nextChild( RPXMLNode* after = NULL ) = 0;
	virtual void insertAttribute( const rp::string& name, const rp::string& value ) = 0;
	virtual void insertAttribute( const rp::string& name, int value ) = 0;
	virtual void insertAttribute( const rp::string& name, double value ) = 0;
	void insertAttributeInt( const rp::string& name, int value ) {
		insertAttribute( name, value );
	}
	virtual rp::string getAttribute( const rp::string& attribute ) = 0;
	int getAttributeInt( const rp::string& attribute ) {
		return getAttribute( attribute ).toint();
	}
	double getAttributeDouble( const rp::string& attribute ) {
		return getAttribute( attribute ).todouble();
	}	
};

// ----------------------------------------------------------------------------
// ---------- RPXML
// ----------------------------------------------------------------------------
class RPXML
{
public:
	RPXML()          {}
	virtual ~RPXML() {}
};

// ----------------------------------------------------------------------------
// ---------- RPXMLException
// ----------------------------------------------------------------------------
class RPXMLException
{
private:
	rp::string message;
	
public:
	RPXMLException( const rp::string& _message ): message( _message ) {}
	rp::string getError() const { return message; }
};

} // namespace rp

#endif // RDO_PROCESS_XML_H
