#ifndef RDO_PROCESS_OBJECT_H
#define RDO_PROCESS_OBJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_string.h"
#include <list>
//#include "rdoprocess_shape_process_dlg2_MJ.h" // MJ 02.04.06 диалог процесса передает ссылку на себя в list_resource_find_MJ чтобы она у него создавала ресурсы 
// ----------------------------------------------------------------------------
// ---------- RPObject
// ----------------------------------------------------------------------------
namespace rp {
class msg;
}

class RPObject
{
friend class rp::msg;

protected:
	// --> manager
	RPObject*              parent;
	std::list< RPObject* > child;
	bool isChildNameCorrect( const RPObject* obj ) const;
	void setCorrectChildName( RPObject* obj );

	std::list< RPObject* >::const_iterator begin() const { return child.begin(); }
	std::list< RPObject* >::const_iterator end()   const { return child.end();   }
	void clear();

	void selectChildOff( RPObject* withoutObj = NULL );
	// <-- manager

	rp::string name;
	bool       selected;

	virtual void notify( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam ) {};

	virtual void modify() {};

public:
	
	RPObject( RPObject* parent = NULL, const rp::string& name = "object" );
	virtual ~RPObject();

	void list_name();
	void list_name_for_type_res();
    virtual void generate_MJ() {}; // MJ 29.03.06 ф-ия генериовнаия
	virtual void find_next_block_MJ() {};// MJ 30.03.06 создает ф-ию которая перекрывается в RPShape для поиска следующего блока
	CString id_next; //MJ 30.03.06 id следующего блока, заведено здесь для функции, которая пробегает полисту
	CString type; // MJ 1.04.06 для определения типа блоков
	std::list< CString > list_pattern_names; // MJ 7.04.06 хранятся имена всех паттернов для записи в файл *.opr generate() заполняет его

	void list_name_for_resource_MJ(std::list< RPObject* >* list_resource);

	rp::string getName() const { return name; }
	virtual bool setName( const rp::string& value );

	bool isSelected() const { return selected; }
	virtual void setSelected( bool value );

	virtual bool isChartObject() const { return false; }

	bool hasChild() const { return !child.empty(); }
	std::list< RPObject* >::const_iterator find_child( const RPObject* object ) {
		return std::find( child.begin(), child.end(), object );
	};

};

#endif // RDO_PROCESS_OBJECT_H
