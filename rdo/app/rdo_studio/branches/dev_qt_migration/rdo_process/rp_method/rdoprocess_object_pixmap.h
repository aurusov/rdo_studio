#ifndef RDO_PROCESS_OBJECT_PIXMAP_H
#define RDO_PROCESS_OBJECT_PIXMAP_H

#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"

// --------------------------------------------------------------------------------
// -------------------- RPObjectPixmap
// --------------------------------------------------------------------------------
class RPPixmap;

class RPObjectPixmap: public RPShape
{
protected:
	RPPixmap* pixmap;

	virtual void setPosition( double posx, double posy );
	virtual void draw(REF(CDC) dc);

public:
	RPObjectPixmap( RPObject* parent, RPPixmap* pixmap );
	virtual ~RPObjectPixmap();

	virtual rp::string getClassName() const { return "RPObjectPixmap"; }
	virtual void load( rp::RPXMLNode* node );
	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );
	// Переопределение виртуальных функций saveToXML и loadFromXML для RPObjectPixmap:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);
};

#endif // RDO_PROCESS_OBJECT_PIXMAP_H
