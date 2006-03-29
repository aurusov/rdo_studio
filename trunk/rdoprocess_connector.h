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
public:
	enum Type {
		in    = 0x01,
		out   = 0x02,
		inout = 0x03,
		fly   = 0x04,
		all   = 0x07
	};
	static const int delta;

protected:
	RPObjectMatrix* _object_matrix;
	rp::point       point;
	Type            type;
	double          norm;

public:
	RPConnectorDock( RPObjectMatrix* __object_matrix, Type _type, const rp::point& _point, double _norm ): _object_matrix( __object_matrix ), type( _type ), point( _point ), norm( _norm ) {};
	virtual ~RPConnectorDock() {};

	std::list< RPConnector* > connectors;

	virtual bool can_connect() const { return true;                  }
	virtual COLORREF color() const   { return RGB(0xF0, 0xFF, 0x00); }
	Type getType() const             { return type;                  }
	bool isType( Type _type ) const {
		switch ( _type ) {
			case fly  : return type & fly ? true : false;
			case in   : return type & in  ? true : false;
			case out  : return type & out ? true : false;
			case inout: return (type & inout) == inout ? true : false;
			case all  : return (type & all  ) == all   ? true : false;
		}
		return false;
	}
	const RPObjectMatrix& object_matrix() const {
		return *_object_matrix;
	}
	rp::point getPosition( bool global = true ) const {
		return global ? _object_matrix->globalMatrix() * point : point ;
	}
	double getNorm( bool global = true ) const {
		double res = global ? _object_matrix->getRotationGlobal() + norm : norm;
		while ( res >= 360 ) res -= 360;
		while ( res <  360 ) res += 360;
		return res;
	}
};

// ----------------------------------------------------------------------------
// ---------- RPConnectorDockOne
// ----------------------------------------------------------------------------
class RPConnectorDockOne: public RPConnectorDock
{
public:
	RPConnectorDockOne( RPObjectMatrix* __object_matrix, Type _type, const rp::point& _point, double _norm ): RPConnectorDock( __object_matrix, _type, _point, _norm ) {};
	virtual ~RPConnectorDockOne() {};

	virtual bool can_connect() const { return connectors.empty(); }
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
