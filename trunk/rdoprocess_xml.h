#ifndef RDO_PROCESS_XML_MFC_H
#define RDO_PROCESS_XML_MFC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <rdoprocess_xml.h>
#import <msxml3.dll> raw_interfaces_only

// ----------------------------------------------------------------------------
// ---------- RPXMLMFC
// ----------------------------------------------------------------------------
class RPXMLNodeMFC: public rp::RPXMLNode
{
friend class RPXMLMFC;
private:
	MSXML2::IXMLDOMNode*                   inode;
	std::list< MSXML2::IXMLDOMAttribute* > iattrs;
	std::list< RPXMLNodeMFC* >             childs;
	rp::string                             node_name;

	RPXMLNodeMFC( MSXML2::IXMLDOMNode* _inode = NULL ): inode( _inode ), node_name("") {}
	virtual ~RPXMLNodeMFC();
	void insertChild( RPXMLNodeMFC* child );
	RPXMLNodeMFC* find( const rp::string& _node_name ) const;
	RPXMLNodeMFC* find( MSXML2::IXMLDOMNode* _inode ) const;

	MSXML2::IXMLDOMNode* getFirstINode() const;
	static rp::string getNameByINode( MSXML2::IXMLDOMNode* inode );

public:
	virtual rp::string getName() const;
	virtual rp::RPXMLNode* makeChild( const rp::string& name );
	virtual rp::RPXMLNode* findFirstChild( const rp::string& name );
	virtual RPXMLNode* nextChild( RPXMLNode* after = NULL );
	virtual void insertAttribute( const rp::string& name, const rp::string& value );
	virtual void insertAttribute( const rp::string& name, int value );
	virtual void insertAttribute( const rp::string& name, double value );
	virtual rp::string getAttribute( const rp::string& attribute );
};

// ----------------------------------------------------------------------------
// ---------- RPXMLMFC
// ----------------------------------------------------------------------------
class RPXMLMFC: public rp::RPXML
{
friend class RPXMLNodeMFC;
private:
	virtual void insertHeader( const rp::string& attribute, const rp::string& value );

public:
	RPXMLMFC();
	virtual ~RPXMLMFC();

	void open( const rp::string& fname );
	void save( const rp::string& fname );

	RPXMLNodeMFC& getDocument() { return document; }

private:
	MSXML2::IXMLDOMDocument* idocument;
	RPXMLNodeMFC             document;
};

extern RPXMLMFC* xml;
 
#endif // RDO_PROCESS_XML_MFC_H
