#include "stdafx.h"
#include "rdoprocess_xml.h"
#include <rdoprocess_project.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPXMLMFC
// ----------------------------------------------------------------------------
RPXMLNodeMFC::~RPXMLNodeMFC()
{
	std::list< MSXML2::IXMLDOMAttribute* >::const_iterator iattr_it = iattrs.begin();
	while ( iattr_it != iattrs.end() ) {
		(*iattr_it)->Release();
		iattr_it++;
	}
	std::list< RPXMLNodeMFC* >::const_iterator child_it = childs.begin();
	while ( child_it != childs.end() ) {
		delete *child_it;
		child_it++;
	}
	if ( inode ) inode->Release();
}

rp::string RPXMLNodeMFC::getName() const
{
	return node_name;
}

RPXMLNodeMFC* RPXMLNodeMFC::find( const rp::string& _node_name ) const
{
	std::list< RPXMLNodeMFC* >::const_iterator child_it = childs.begin();
	while ( child_it != childs.end() ) {
		if ( (*child_it)->node_name == _node_name ) return *child_it;
		child_it++;
	}
	return NULL;
}

RPXMLNodeMFC* RPXMLNodeMFC::find( MSXML2::IXMLDOMNode* _inode ) const
{
	std::list< RPXMLNodeMFC* >::const_iterator child_it = childs.begin();
	while ( child_it != childs.end() ) {
		if ( (*child_it)->inode == _inode ) return *child_it;
		child_it++;
	}
	return NULL;
}

MSXML2::IXMLDOMNode* RPXMLNodeMFC::getFirstINode() const
{
	MSXML2::IXMLDOMNode* ifirst = NULL;
	if ( inode->get_firstChild( &ifirst ) != S_OK ) {
		if ( !ifirst ) return NULL;
		ifirst->Release();
		throw rp::RPXMLException( rp::format("Ошибка выбора первого потомка для %s", node_name.c_str()) );
	}
	return ifirst;
}

rp::string RPXMLNodeMFC::getNameByINode( MSXML2::IXMLDOMNode* inode )
{
	BSTR bstr = NULL;
	if ( inode->get_nodeName( &bstr ) != S_OK ) {
		if ( bstr ) ::SysFreeString( bstr );
		throw rp::RPXMLException( rp::format("Ошибка получения имени") );
	}
	rp::string name( static_cast<const char*>(_bstr_t(bstr)) );
	if ( bstr ) ::SysFreeString( bstr );
	return name;
}

rp::RPXMLNode* RPXMLNodeMFC::findFirstChild( const rp::string& name )
{
	// Сначала проверим, а нет ли уже с писке потомков
	rp::RPXMLNode* found = find( name );
	if ( found ) return found;
	// В списке потомков не нашли, поищем в xml-документе
	// Выберем первого потомка
	MSXML2::IXMLDOMNode* ichild = getFirstINode();
	while ( true ) {
		// Проверим имя
		if ( name == RPXMLNodeMFC::getNameByINode( ichild ) ) {
			RPXMLNodeMFC* child = new RPXMLNodeMFC( ichild );
			child->node_name = name;
			childs.push_back( child );
			return child;
		}
		// Перейдем к следующему потомку
		MSXML2::IXMLDOMNode* ichild_next = NULL;
		if ( ichild->get_nextSibling( &ichild_next ) != S_OK ) {
			return NULL;
		}
		ichild = ichild_next;
	}
	return NULL;
}

rp::RPXMLNode* RPXMLNodeMFC::nextChild( RPXMLNode* after )
{
	MSXML2::IXMLDOMNode* ichild = NULL;
	if ( !after ) {
		ichild = getFirstINode();
	} else {
		if ( static_cast<RPXMLNodeMFC*>(after)->inode->get_nextSibling( &ichild ) != S_OK ) {
			return NULL;
		}
	}
	if ( ichild ) {
		RPXMLNodeMFC* child = new RPXMLNodeMFC( ichild );
		child->node_name = RPXMLNodeMFC::getNameByINode( ichild );
		childs.push_back( child );
		return child;
	} else {
		return NULL;
	}
}

rp::string RPXMLNodeMFC::getAttribute( const rp::string& attribute )
{
	MSXML2::IXMLDOMNamedNodeMap* imap = NULL;
	if ( inode->get_attributes( &imap ) != S_OK ) {
		if ( imap ) imap->Release();
		throw rp::RPXMLException( rp::format("Ошибка получения списка атрибутов объекта %s", node_name.c_str()) );
	}
	MSXML2::IXMLDOMNode* iattr = NULL;
	_bstr_t bstr( attribute.c_str() );
	if ( imap->getNamedItem( bstr.GetBSTR(), &iattr ) != S_OK ) {
		if ( imap ) imap->Release();
		if ( iattr ) iattr->Release();
		throw rp::RPXMLException( rp::format("Не найден атрибут %s объекта %s", attribute.c_str(), node_name.c_str()) );
	}
	VARIANT value;
	if ( iattr->get_nodeValue( &value ) != S_OK ) {
		if ( imap ) imap->Release();
		if ( iattr ) iattr->Release();
		throw rp::RPXMLException( rp::format("Ошибка получения значения атрибута %s объекта %s", attribute.c_str(), node_name.c_str()) );
	}
	if ( imap ) imap->Release();
	if ( iattr ) iattr->Release();
	return static_cast<char*>(_bstr_t(value));
}

void RPXMLNodeMFC::insertChild( RPXMLNodeMFC* child )
{
	MSXML2::IXMLDOMNode* node_2 = NULL;
	HRESULT hr = inode->appendChild( child->inode, &node_2 ) != S_OK;
	if ( hr != S_OK ) {
		if ( node_2 ) node_2->Release();
		throw rp::RPXMLException( rp::format("Ошибка добавления потомка %s в %s", child->node_name.c_str(), node_name.c_str()) );
	}
}

rp::RPXMLNode* RPXMLNodeMFC::makeChild( const rp::string& name )
{
	MSXML2::IXMLDOMElement* elem = NULL;
	if ( xml->idocument->createElement( _bstr_t(name.c_str()), &elem ) != S_OK ) {
		if ( elem ) elem->Release();
		throw rp::RPXMLException( rp::format("Ошибка создания потомка %s", name.c_str()) );
	}
	RPXMLNodeMFC* child = new RPXMLNodeMFC( elem );
	child->node_name = name;
	childs.push_back( child );
	insertChild( child );
	return child;
}

void RPXMLNodeMFC::insertAttribute( const rp::string& name, const rp::string& value )
{
	MSXML2::IXMLDOMAttribute* iattr = NULL;
	if ( xml->idocument->createAttribute( _bstr_t(name.c_str()), &iattr ) != S_OK ) {
		if ( iattr ) iattr->Release();
		throw rp::RPXMLException( rp::format("Ошибка создания атрибута %s объекта %s", name.c_str(), node_name.c_str()) );
	}
	if ( iattr->put_value( _variant_t(value.c_str()) ) != S_OK ) {
		if ( iattr ) iattr->Release();
		throw rp::RPXMLException( rp::format("Ошибка присвоения значения %s атрибуту %s объекта %s", value.c_str(), name.c_str(), node_name.c_str()) );
	}
	MSXML2::IXMLDOMAttribute* iattr_2 = NULL;
	if ( static_cast<MSXML2::IXMLDOMElement*>(inode)->setAttributeNode( iattr, &iattr_2 ) != S_OK ) {
		if ( iattr   ) iattr->Release();
		if ( iattr_2 ) iattr_2->Release();
		throw rp::RPXMLException( rp::format("Ошибка добавления атрибута %s объекту %s", name.c_str(), node_name.c_str()) );
	}
	iattrs.push_back( iattr );
}

void RPXMLNodeMFC::insertAttribute( const rp::string& name, int value )
{
	insertAttribute( name, rp::string::fromint(value) );
}

void RPXMLNodeMFC::insertAttribute( const rp::string& name, double value )
{
	insertAttribute( name, rp::string::fromdouble(value) );
}

// ----------------------------------------------------------------------------
// ---------- RPXMLMFC
// ----------------------------------------------------------------------------
RPXMLMFC* xml = NULL;

RPXMLMFC::RPXMLMFC():
	RPXML(),
	idocument( NULL )
{
    if ( ::CoCreateInstance( __uuidof(MSXML2::DOMDocument30), NULL, CLSCTX_INPROC_SERVER, __uuidof(MSXML2::IXMLDOMDocument), reinterpret_cast<void**>(&idocument) ) != S_OK ) {
		throw rp::RPXMLException( "Ошибка создания xml-документа" );
    }
	xml = this;
    document.inode = idocument;
	idocument->put_async( VARIANT_FALSE );
	idocument->put_validateOnParse( VARIANT_FALSE );
	idocument->put_resolveExternals( VARIANT_FALSE );
	idocument->put_preserveWhiteSpace( VARIANT_FALSE );
//	insertHeader( "RDOProcess", "version 1.2" );
}

RPXMLMFC::~RPXMLMFC()
{
	xml = NULL;
}

void RPXMLMFC::open( const rp::string& fname )
{
	VARIANT_BOOL status;
	if ( idocument->load( _variant_t(fname.c_str()), &status ) != S_OK ) {
		throw rp::RPXMLException( rp::format("Ошибка загрузки файла %s", fname.c_str()) );
	}
	RPXMLNodeMFC* project_node = static_cast<RPXMLNodeMFC*>(document.findFirstChild( "project" ));
	if ( project_node ) {
		rpMethod::project->load( project_node );
	} else {
		throw rp::RPXMLException( "Не найдено описание проекта" );
	}
}

void RPXMLMFC::save( const rp::string& fname )
{
	if ( idocument->save( _variant_t(fname.c_str()) ) != S_OK ) {
		throw rp::RPXMLException( rp::format("Ошибка записи файла %s", fname.c_str()) );
	}
}

void RPXMLMFC::insertHeader( const rp::string& attribute, const rp::string& value )
{
	MSXML2::IXMLDOMProcessingInstruction* pi = NULL;
	if ( idocument->createProcessingInstruction( _bstr_t(attribute.c_str()), _bstr_t(value.c_str()), &pi ) != S_OK ) {
		if ( pi ) pi->Release();
		throw rp::RPXMLException( rp::format("Ошибка добавления заголовка %s", attribute.c_str()) );
	}
	RPXMLNodeMFC node( pi );
	document.insertChild( &node );
}

/*
#include <stdio.h>
#include <windows.h>
#import <msxml3.dll> raw_interfaces_only
using namespace MSXML2;

// Macro that calls a COM method returning HRESULT value:
#define HRCALL(a, errmsg) \
do { \
    hr = (a); \
    if (FAILED(hr)) { \
        dprintf( "%s:%d  HRCALL Failed: %s\n  0x%.8x = %s\n", \
                __FILE__, __LINE__, errmsg, hr, #a ); \
        goto clean; \
    } \
} while (0)

// Helper function that put output in stdout and debug window
// in Visual Studio:
void dprintf( char * format, ...)
{
    static char buf[1024];
    va_list args;
    va_start( args, format );
    vsprintf( buf, format, args );
    va_end( args);
    OutputDebugStringA( buf);
    printf("%s", buf);
}

// Helper function to create a DOM instance: 
IXMLDOMDocument * DomFromCOM()
{
    HRESULT hr;
    IXMLDOMDocument *pxmldoc = NULL;

    HRCALL( CoCreateInstance(__uuidof(DOMDocument30),
                    NULL,
                    CLSCTX_INPROC_SERVER,
                    __uuidof(IXMLDOMDocument),
                    (void**)&pxmldoc),
                    "Create a new DOMDocument");

    HRCALL( pxmldoc->put_async(VARIANT_FALSE),
            "should never fail");
    HRCALL( pxmldoc->put_validateOnParse(VARIANT_FALSE),
            "should never fail");
    HRCALL( pxmldoc->put_resolveExternals(VARIANT_FALSE),
            "should never fail");

    return pxmldoc;
clean:
    if (pxmldoc)
    {
        pxmldoc->Release();
    }
    return NULL;
}

VARIANT VariantString(BSTR str)
{
    VARIANT var;
    VariantInit(&var);
    V_BSTR(&var) = SysAllocString(str);
    V_VT(&var) = VT_BSTR;
    return var;
}

void ReportParseError(IXMLDOMDocument *pDom, char *desc) {
    IXMLDOMParseError *pXMLErr=NULL;
    BSTR bstrReason = NULL;
    HRESULT hr;
    HRCALL(pDom->get_parseError(&pXMLErr),
                "dom->get_parseError: ");
    HRCALL(pXMLErr->get_reason(&bstrReason),
                "parseError->get_reason: ");

    dprintf("%s %S\n",desc, bstrReason);
clean:
    if (pXMLErr) pXMLErr->Release();
    if (bstrReason) SysFreeString(bstrReason);
}

int main(int argc, char* argv[])
{
   IXMLDOMDocument *pXMLDom=NULL;
   IXMLDOMNodeList *pNodes=NULL;
   IXMLDOMNode *pNode=NULL;
   BSTR bstr = NULL;
   VARIANT_BOOL status;
   VARIANT var;
   HRESULT hr;
   long length;

   CoInitialize(NULL);

   pXMLDom = DomFromCOM();
   if (!pXMLDom) goto clean;

   VariantInit(&var);
   var = VariantString(L"stocks.xml");
   HRCALL(pXMLDom->load(var, &status), "dom->load(): ");

   if (status!=VARIANT_TRUE) {
        ReportParseError(pXMLDom, 
            "Failed to load DOM from stocks.xml");
        goto clean;
   }

   // Query a single node.
   if (bstr) SysFreeString(bstr);
   bstr = SysAllocString(L"//stock[1]/*");
   HRCALL(pXMLDom->selectSingleNode(bstr, &pNode),
      "dom->selectSingleNode: ");
   if (!pNode) {
      ReportParseError(pXMLDom, "Calling selectSingleNode ");
   }
   else {
      dprintf("Result from selectSingleNode:\n");
      if (bstr) SysFreeString(bstr);
      HRCALL(pNode->get_nodeName(&bstr)," get_nodeName ");
      dprintf("Node, <%S>:\n", bstr);
      if (bstr) SysFreeString(bstr);
      HRCALL(pNode->get_xml(&bstr), "get_xml: ");
      dprintf("\t%S\n\n", bstr);
   }

   // Query a node-set.
   if (bstr) SysFreeString(bstr);
   bstr = SysAllocString(L"//stock[1]/*");
   HRCALL(pXMLDom->selectNodes(bstr, &pNodes), "selectNodes ");
   if (!pNodes) {
      ReportParseError(pXMLDom, "Error while calling selectNodes ");
   }
   else {
      dprintf("Results from selectNodes:\n");
      HRCALL(pNodes->get_length(&length), "get_length: ");
      for (long i=0; i<length; i++) {
         if (pNode) pNode->Release();
         HRCALL(pNodes->get_item(i, &pNode), "get_item: ");
         if (bstr) SysFreeString(bstr);
         HRCALL(pNode->get_nodeName(&bstr), "get_nodeName: ");
         dprintf("Node (%d), <%S>:\n",i, bstr);
         SysFreeString(bstr);
         HRCALL(pNode->get_xml(&bstr), "get_xml: ");
         dprintf("\t%S\n", bstr);
      }
   }

clean:
   if (bstr) SysFreeString(bstr);
   if (&var) VariantClear(&var);
   if (pXMLDom) pXMLDom->Release();
   if (pNodes) pNodes->Release();
   if (pNode) pNode->Release();

   CoUninitialize();
   return 0;
}
*/
