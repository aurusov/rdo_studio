#ifndef RDO_PROCESS_SHAPE_H
#define RDO_PROCESS_SHAPE_H

#include <vector>
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_matrix.h"

// --------------------------------------------------------------------------------
// -------------------- RPShape
// --------------------------------------------------------------------------------
class RPConnectorDock;

class RPShape: public RPObjectMatrix
{
protected:
//	CPoint snap_to_point;

	CBrush   bg_brush;
	CFont    text_font;
	COLORREF text_color;
	rbool    text_show;

	std::vector< RPConnectorDock* > docks;
	RPConnectorDock* find_dock( const rp::point& global_chart_pos );

	virtual rp::string get_xml_node_name() const { return "shape"; }

	// Дискретный угол поворота (дискрета 90 градусов)
	enum angle90 {
		angle90_0 = 0,  //!< Угол поворота объекта alpha > 270 + 45 || alpha <= 45
		angle90_90,     //!< Угол поворота объекта alpha > 45       && alpha <= 90 + 45
		angle90_180,    //!< Угол поворота объекта alpha > 90 + 45  && alpha <= 180 + 45
		angle90_270     //!< Угол поворота объекта alpha > 180 + 45 && alpha <= 270 + 45
	};
	// Дискретный угол поворота (дискрета 45 градусов)
	enum angle45 {
		angle45_0 = 0,  //!< Угол поворота объекта alpha > 360 - 22 || alpha <= 22
		angle45_90,     //!< Угол поворота объекта alpha > 90 - 22  && alpha <= 90 + 22
		angle45_180,    //!< Угол поворота объекта alpha > 180 - 22 && alpha <= 180 + 22
		angle45_270,    //!< Угол поворота объекта alpha > 270 - 22 && alpha <= 270 + 22
		angle45_45,     //!< Угол поворота объекта alpha > 45 -  22 && alpha <= 45 + 22
		angle45_135,    //!< Угол поворота объекта alpha > 135 - 22 && alpha <= 135 + 22
		angle45_225,    //!< Угол поворота объекта alpha > 225 - 22 && alpha <= 225 + 22
		angle45_315,    //!< Угол поворота объекта alpha > 315 - 22 && alpha <= 315 + 22
	};
	// Возможная команда над объектом
	enum PossibleCommand {
		pcmd_none = 0,      //!< Над объектом не может быть произведено никакое действие
		pcmd_move,          //!< Объект может быть перемещен
		pcmd_rotate_tl,     //!< Объект можно повернуть за левый верхний угол
		pcmd_rotate_tr,     //!< Объект можно повернуть за правый верхний угол
		pcmd_rotate_bl,     //!< Объект можно повернуть за левый нижний угол
		pcmd_rotate_br,     //!< Объект можно повернуть за левый нижний угол
		pcmd_rotate_center, //!< Может быть изменен центр врещения объекта
		pcmd_scale_l,       //!< Объект может быть масштабирован за левую сторону
		pcmd_scale_r,       //!< Объект может быть масштабирован за правую сторону
		pcmd_scale_t,       //!< Объект может быть масштабирован за верх
		pcmd_scale_b,       //!< Объект может быть масштабирован за низ
		pcmd_scale_tl,      //!< Объект может быть масштабирован за левый верхний угол
		pcmd_scale_tr,      //!< Объект может быть масштабирован за правый верхний угол
		pcmd_scale_bl,      //!< Объект может быть масштабирован за левый нижний угол
		pcmd_scale_br,      //!< Объект может быть масштабирован за правый нижний угол
		pcmd_dock_in,       //!< Доступен входной док объекта
		pcmd_dock_out,      //!< Доступен выходной док объекта
		pcmd_dock_inout,    //!< Доступен двунаправленный док объекта
		pcmd_dock_fly,      //!< Док может быть перемещен
		pcmd_dock_not       //!< Док недоступен
	};
	PossibleCommand pcmd;

	// Возможная команда над объектом
	PossibleCommand getPossibleCommand( const rp::point& global_chart_pos, rbool for_cursor = false );

	// Вернуть дискретный угол поворота (дискрета 90 градусов)
	angle90 getAngle90() const {
		double alpha = getRotationGlobal();
		if ( alpha > 270 + 45 || alpha <= 45       ) return RPShape::angle90_0;
		if ( alpha > 45       && alpha <= 90 + 45  ) return RPShape::angle90_90;
		if ( alpha > 90 + 45  && alpha <= 180 + 45 ) return RPShape::angle90_180;
		if ( alpha > 180 + 45 && alpha <= 270 + 45 ) return RPShape::angle90_270;
		return RPShape::angle90_0;
	}

	// Вернуть дискретный угол поворота (дискрета 45 градусов)
	angle45 getAngle45() const {
		double alpha = getRotationGlobal();
		if ( alpha > 360 - 22 || alpha <= 22       ) return RPShape::angle45_0;
		if ( alpha > 90 - 22  && alpha <= 90 + 22  ) return RPShape::angle45_90;
		if ( alpha > 180 - 22 && alpha <= 180 + 22 ) return RPShape::angle45_180;
		if ( alpha > 270 - 22 && alpha <= 270 + 22 ) return RPShape::angle45_270;
		if ( alpha > 45 -  22 && alpha <= 45 + 22  ) return RPShape::angle45_45;
		if ( alpha > 135 - 22 && alpha <= 135 + 22 ) return RPShape::angle45_135;
		if ( alpha > 225 - 22 && alpha <= 225 + 22 ) return RPShape::angle45_225;
		if ( alpha > 315 - 22 && alpha <= 315 + 22 ) return RPShape::angle45_315;
		return RPShape::angle45_0;
	}

	static void getScaleDelta( rp::point& delta, RPShape::angle90 a90, RPShape::PossibleCommand pcmd );
	static void getRectDelta( rp::rect& rect_old, rp::rect& rect_new, rp::point& delta, RPShape::angle90 a90, RPShape::PossibleCommand pcmd );

	struct trans {
		trans( rp::matrix& _matrix ): matrix( _matrix ) {};
		rp::point operator()( const rp::point& point ) {
			return matrix * point;
		}
		rp::matrix& matrix;
	};
	rp::polyline pa_src;
	rp::polyline pa_global;

	virtual void drawPolyline( CDC& dc );
	virtual void drawDocks( CDC& dc );

	virtual RPProject::Cursor getCursor( const rp::point& global_chart_pos );

	virtual rbool isShape() const { return true; }
	virtual RPObjectChart* find( const rp::point& global_chart_pos );

	virtual void onRButtonDown( UINT nFlags, CPoint global_chart_pos );

public:
	RPShape( RPObject* parent, const rp::string& name = "shape" );
	virtual ~RPShape();

	virtual rp::string getClassName() const { return "RPShape"; }
	virtual void load( rp::RPXMLNode* node );
	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );
	void saveToXML   (REF (pugi::xml_node) parentNode) const;
	void loadFromXML (CREF(pugi::xml_node) node);

	std::list< RPShape* > getNextBlock();

	const std::vector< RPConnectorDock* >& getDocks() const { return docks; }
	int getDockIndex( const RPConnectorDock* dock ) const;
	RPConnectorDock* getDock( unsigned int index ) const    { return index < docks.size() ? docks[index] : NULL; }

	virtual void setPosition( double posx, double posy );

	void setBgBrush( const CBrush& brush );

	const CFont& getTextFont( COLORREF& color ) const {
		color = text_color;
		return text_font;
	}
	void setTextFont( const CFont& font, COLORREF color, rbool show );

//	const CPoint& getSnapToPoint() const { return snap_to_point; }

	virtual void draw(REF(CDC) dc);
	virtual void draw_selected( CDC& dc );

	// Габориты фигуры
	virtual rp::rect getBoundingRect( rbool global = true ) const;
	virtual rp::rect getMaxRect();
	virtual rp::rect getBoundingRectNoRotateOuter() {
		transformToGlobal();
		return getBoundingRect().getBoundingRect();
	}
	virtual rp::rect getBoundingRectNoRotateIner() {
		transformToGlobal();
		return pa_global.getBoundingRect();
	}

	// Перевод всех элементов фигуры в глобальные координаты
	virtual void transformToGlobal();

	// Находится ли точка внутри фигуры
	virtual rbool pointInPolygon( const rp::point& global_chart_pos ) {
		transformToGlobal();
		return pa_global.extendByPerimetr( static_cast<double>(main_pen_width) / 2.0 ).pointInPolygon( global_chart_pos );
	}
	// Находится ли точка в служебной (неклиентской) части фигуры (прямоугольник выделения, к примеру)
	virtual rbool pointInNCArea( const rp::point& global_chart_pos );

	// Перед выполнение команды (объект должен подготовить команду)
	virtual void command_before( const rp::point& global_chart_pos, rbool first_click );
	// Выполнить команду над объектом
	virtual void command_make( const rp::point& global_chart_pos );

	virtual rbool can_delete() const { return pcmd == pcmd_move; }
};

#endif // RDO_PROCESS_SHAPE_H
