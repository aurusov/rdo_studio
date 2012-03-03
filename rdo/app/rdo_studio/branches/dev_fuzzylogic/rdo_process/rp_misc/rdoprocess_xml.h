#ifndef RDO_PROCESS_XML_H
#define RDO_PROCESS_XML_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_string.h"
#import <msxml3.dll> raw_interfaces_only

namespace rp {

// --------------------------------------------------------------------------------
// -------------------- RPXMLNode
// --------------------------------------------------------------------------------
class RPXMLNode
{
friend class RPXML;

protected:
	MSXML2::IXMLDOMNode*                   inode;
	std::list< MSXML2::IXMLDOMAttribute* > iattrs;
	std::list< RPXMLNode* >                childs;
	rp::string                             node_name;

	void insertChild( RPXMLNode* child );
	RPXMLNode* find( const rp::string& _node_name ) const;
	RPXMLNode* find( MSXML2::IXMLDOMNode* _inode ) const;

	MSXML2::IXMLDOMNode* getFirstINode() const;
	static rp::string getNameByINode( MSXML2::IXMLDOMNode* inode );

public:
	RPXMLNode( MSXML2::IXMLDOMNode* _inode = NULL ): inode( _inode ), node_name("") {}
	virtual ~RPXMLNode();

	virtual rp::string getName() const;
	virtual rp::RPXMLNode* makeChild( const rp::string& name );
	virtual rp::RPXMLNode* findFirstChild( const rp::string& name );
	virtual RPXMLNode* nextChild( RPXMLNode* after = NULL );

	// INSERT
	virtual void insertAttribute( const rp::string& name, const rp::string& value );
	virtual void insertAttribute( const rp::string& name, int value );
	virtual void insertAttribute( const rp::string& name, double value );
	void insertAttributeInt( const rp::string& name, int value ) {
		insertAttribute( name, value );
	}
	// GET
	virtual rp::string getAttribute( const rp::string& attribute );
	int getAttributeInt( const rp::string& attribute ) {
		return getAttribute( attribute ).toint();
	}
	double getAttributeDouble( const rp::string& attribute ) {
		return getAttribute( attribute ).todouble();
	}	
};

// --------------------------------------------------------------------------------
// -------------------- RPXML
// --------------------------------------------------------------------------------
class RPXML
{
friend class RPXMLNode;

private:
	MSXML2::IXMLDOMDocument* idocument;
	RPXMLNode                document;
	virtual void insertHeader( const rp::string& attribute, const rp::string& value );

public:
	RPXML();
	virtual ~RPXML();

	RPXMLNode* open( const rp::string& fname );
	void save( const rp::string& fname );

	RPXMLNode& getDocument() { return document; }
};

// --------------------------------------------------------------------------------
// -------------------- RPXMLException
// --------------------------------------------------------------------------------
class RPXMLException
{
private:
	rp::string message;
	
public:
	RPXMLException( const rp::string& _message ): message( _message ) {}
	rp::string getError() const { return message; }
};

extern RPXML* xml;

} // namespace rp

#endif // RDO_PROCESS_XML_H
