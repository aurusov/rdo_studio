#ifndef RDO_PROCESS_CONNECTOR_H
#define RDO_PROCESS_CONNECTOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object_chart.h"
#include "rdoprocess_object_matrix.h"

#include <list>

// ----------------------------------------------------------------------------
// ---------- RPConnectorDock
// ----------------------------------------------------------------------------
class RPConnector;

class RPConnectorDock
{
protected:
	RPObjectMatrix* _object_matrix;
	rp::point point;

public:
	enum Type {
		in    = 0x01,
		out   = 0x02,
		inout = 0x03,
		fly   = 0x04,
		all   = 0x07
	};
	bool can_connect;
	Type type;
	std::list< RPConnector* > connectors;
	const RPObjectMatrix& object_matrix() const  { return *_object_matrix; }
	rp::point getPosition() const { return _object_matrix->globalMatrix() * point; }

	RPConnectorDock( RPObjectMatrix* __object_matrix, Type _type, const rp::point& _point ): _object_matrix( __object_matrix ), can_connect( true ), type( _type ), point( _point ) {};
	~RPConnectorDock() {};
};

// ----------------------------------------------------------------------------
// ---------- RPConnector
// ----------------------------------------------------------------------------
class RPConnectorDock;

class RPConnector: public RPObjectChart
{
friend class RPObjectFlowChart;

protected:
	RPConnectorDock* dock_begin;
	RPConnectorDock* dock_end;

	// Перевод всех элементов фигуры в глобальные координаты
	virtual void transformToGlobal() {};

	// Находится ли точка внутри фигуры
	virtual bool pointInPolygon( const rp::point& global_chart_pos ) {
		return false;
	};
	// Находится ли точка в служебной (неклиентской) части фигуры (прямоугольник выделения, к примеру)
	virtual bool pointInNCArea( const rp::point& global_chart_pos ) {
		return false;
	};

	// Габориты фигуры
	virtual rp::rect getBoundingRect( bool global = true ) const {
		return rp::rect();
	};

	// Отрисовка фигуры
	virtual void draw( CDC& dc );

	virtual bool isConnector() const   { return true; }

public:
	RPConnector( RPObject* parent, const rp::string& name = "connector" );
	virtual ~RPConnector();

	RPConnectorDock* getConnectedDock( const RPConnectorDock& dock ) const;
};

#endif // RDO_PROCESS_CONNECTOR_H
