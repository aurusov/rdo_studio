#include "app/rdo_studio_mfc/rdo_process/rp_method/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_matrix.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_flowchart.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPObjectMatrix
// --------------------------------------------------------------------------------
RPObjectMatrix::RPObjectMatrix( RPObject* _parent, const rp::string& _name ):
	RPObjectChart( _parent, _name ),
	rotate_center( 0, 0 ),
	rotation_alpha( 0 )
{
}

RPObjectMatrix::~RPObjectMatrix()
{
}

void RPObjectMatrix::load( rp::RPXMLNode* node )
{
	RPObjectChart::load( node );
	rp::RPXMLNode* geometry_node = node->findFirstChild( "geometry" );
	matrix_transform.dx()      = geometry_node->getAttributeDouble( "x" );
	matrix_transform.dy()      = geometry_node->getAttributeDouble( "y" );
	matrix_transform_post.dx() = geometry_node->getAttributeDouble( "x_post" );
	matrix_transform_post.dy() = geometry_node->getAttributeDouble( "y_post" );
	matrix_scale.sx()          = geometry_node->getAttributeDouble( "scale_x" );
	matrix_scale.sy()          = geometry_node->getAttributeDouble( "scale_y" );
	rotation_alpha             = geometry_node->getAttributeDouble( "rotation" );
	rotate_center.x            = geometry_node->getAttributeDouble( "rotate_x" );
	rotate_center.y            = geometry_node->getAttributeDouble( "rotate_y" );
	matrix_rotate.data[0][0]   = geometry_node->getAttributeDouble( "r_m00" );
	matrix_rotate.data[0][1]   = geometry_node->getAttributeDouble( "r_m01" );
	matrix_rotate.data[0][2]   = geometry_node->getAttributeDouble( "r_m02" );
	matrix_rotate.data[1][0]   = geometry_node->getAttributeDouble( "r_m10" );
	matrix_rotate.data[1][1]   = geometry_node->getAttributeDouble( "r_m11" );
	matrix_rotate.data[1][2]   = geometry_node->getAttributeDouble( "r_m12" );
	matrix_rotate.data[2][0]   = geometry_node->getAttributeDouble( "r_m20" );
	matrix_rotate.data[2][1]   = geometry_node->getAttributeDouble( "r_m21" );
	matrix_rotate.data[2][2]   = geometry_node->getAttributeDouble( "r_m22" );
}

rp::RPXMLNode* RPObjectMatrix::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPObjectChart::save( parent_node );
	rp::RPXMLNode* geometry_node = obj_node->makeChild( "geometry" );
	geometry_node->insertAttribute( "x", matrix_transform.dx() );
	geometry_node->insertAttribute( "y", matrix_transform.dy() );
	geometry_node->insertAttribute( "x_post", matrix_transform_post.dx() );
	geometry_node->insertAttribute( "y_post", matrix_transform_post.dy() );
	geometry_node->insertAttribute( "scale_x", matrix_scale.sx() );
	geometry_node->insertAttribute( "scale_y", matrix_scale.sy() );
	geometry_node->insertAttribute( "rotation", rotation_alpha );
	geometry_node->insertAttribute( "rotate_x", rotate_center.x );
	geometry_node->insertAttribute( "rotate_y", rotate_center.y );
	geometry_node->insertAttribute( "r_m00", matrix_rotate.data[0][0] );
	geometry_node->insertAttribute( "r_m01", matrix_rotate.data[0][1] );
	geometry_node->insertAttribute( "r_m02", matrix_rotate.data[0][2] );
	geometry_node->insertAttribute( "r_m10", matrix_rotate.data[1][0] );
	geometry_node->insertAttribute( "r_m11", matrix_rotate.data[1][1] );
	geometry_node->insertAttribute( "r_m12", matrix_rotate.data[1][2] );
	geometry_node->insertAttribute( "r_m20", matrix_rotate.data[2][0] );
	geometry_node->insertAttribute( "r_m21", matrix_rotate.data[2][1] );
	geometry_node->insertAttribute( "r_m22", matrix_rotate.data[2][2] );
	return obj_node;
}

void RPObjectMatrix::saveToXML(REF (pugi::xml_node) parentNode) const
{
	// Создаем текстовый подузел "BaseClass" для узла parentNode
						  parentNode.append_child(pugi::node_comment).set_value(_T("BaseClass"));
	pugi::xml_node node = parentNode.append_child                         (_T("RPObjectMatrix"));
	// Записываем атрибуты графики:
	node.append_attribute(_T("X"))       .set_value(getCenter().x);
	node.append_attribute(_T("Y"))       .set_value(getCenter().y);
	node.append_attribute(_T("scale_X")) .set_value(getScaleX()  );
	node.append_attribute(_T("scale_Y")) .set_value(getScaleY()  );
	node.append_attribute(_T("alpha"))   .set_value(getRotation());
}

void RPObjectMatrix::loadFromXML(CREF(pugi::xml_node) node)
{
	// Поиск узла <RPObjectMatrix/> в списке потомков корня поддерева:
	for (pugi::xml_node next_node = node.first_child(); next_node; next_node = next_node.next_sibling())
	{
		if (strcmp(next_node.name(), "RPObjectMatrix") == 0)
		{
			for (pugi::xml_attribute attr = next_node.first_attribute(); attr; attr = attr.next_attribute())
			{
				// Для отслеживания процесса "debug" заводим новую переменную:
				tstring attrName = attr.name();
				if (attrName == _T("X"))
				{
					setX(attr.as_double());
				}
				else if (attrName == _T("Y"))
				{
					setY(attr.as_double());
				}
				else if (attrName == _T("scale_X"))
				{
					setScaleX(attr.as_double());
				}
				else if (attrName == _T("scale_Y"))
				{
					setScaleY(attr.as_double());
				}
				else if (attrName == _T("alpha"))
				{
					setRotation(attr.as_double());
				}
			}
		}
	}
}

void RPObjectMatrix::setPosition( double posx, double posy )
{
	matrix_transform.dx() = posx;
	matrix_transform.dy() = posy;
	modify();
	if (flowChart()) flowChart()->snapToGrid( this );
}

void RPObjectMatrix::setPositionPost( double posx, double posy )
{
	matrix_transform_post.dx() = posx;
	matrix_transform_post.dy() = posy;
	modify();
}

void RPObjectMatrix::setScale( double sx, double sy ) 
{
	if ( sx >= 0 && sx < 1e-2 ) sx = 1e-2;
	if ( sy >= 0 && sy < 1e-2 ) sy = 1e-2;
	matrix_scale.sx() = sx;
	matrix_scale.sy() = sy;
	modify();
}

void RPObjectMatrix::setRotation( double alpha )
{
	while ( alpha < 0 ) alpha += 360.0;
	while ( alpha >= 360 ) alpha -= 360.0;
	double alpha_delta = alpha - rotation_alpha;
	rotation_alpha = alpha;

	rp::matrix m_rotate;
	RPObjectMatrix::fillRotateMatrix( m_rotate, alpha_delta );

	rp::matrix r_center;
	r_center.dx() = -rotate_center.x;
	r_center.dy() = -rotate_center.y;

	matrix_rotate = r_center.obr() * m_rotate * r_center * matrix_rotate;
	modify();
}

void RPObjectMatrix::setRotateCenterLocalDelta( double dx, double dy )
{
	rp::matrix m_delta;
	m_delta.dx() = dx;
	m_delta.dy() = dy;

	rp::matrix m_rotate;
	RPObjectMatrix::fillRotateMatrix( m_rotate, rotation_alpha );

	rp::matrix m = globalRotate() * m_delta;
	rp::point point = getRotateCenter();
	point.x += m.dx();
	point.y += m.dy();
	setRotateCenter( point );
}

bool RPObjectMatrix::isRotateCenter( const rp::point& global_chart_pos ) const
{
	return rp::math::getLength( getRotateCenter(), global_chart_pos ) <= RPObjectFlowChart::getSensitivity();
}

rp::rect RPObjectMatrix::getMaxRect()
{
	double max_x = 0;
	double max_y = 0;
	double min_x = 0;
	double min_y = 0;
	std::list< RPObjectChart* > objects;
	getChartObjects( objects );
	std::list< RPObjectChart* >::iterator it = objects.begin();
	if ( it != objects.end() ) {
		rp::rect rect = (*it)->getMaxRect();
		max_x  = rect.getMaxX();
		max_y  = rect.getMaxY();
		min_x  = rect.getMinX();
		min_y  = rect.getMinY();
		it++;
	}
	while ( it != objects.end() ) {
		rp::rect rect = (*it)->getMaxRect();
		double _max_x = rect.getMaxX();
		double _max_y = rect.getMaxY();
		double _min_x = rect.getMinX();
		double _min_y = rect.getMinY();
		if ( _max_x > max_x ) max_x = _max_x;
		if ( _max_y > max_y ) max_y = _max_y;
		if ( _min_x < min_x ) min_x = _min_x;
		if ( _min_y < min_y ) min_y = _min_y;
		it++;
	}
	return rp::rect( min_x, min_y, max_x, max_y );
}
