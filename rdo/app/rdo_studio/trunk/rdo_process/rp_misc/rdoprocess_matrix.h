#ifndef RDO_PROCESS_MATRIX_H
#define RDO_PROCESS_MATRIX_H

#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_point.h"

namespace rp {

// --------------------------------------------------------------------------------
// -------------------- matrix
// --------------------------------------------------------------------------------
class matrix
{
public:
	double data[3][3];

	matrix();
	matrix( const matrix& m );
	~matrix();

#ifdef _DEBUG
	void trace() const {
		TRACE3( "%f, %f, %f\n", data[0][0], data[0][1], data[0][2] );
		TRACE3( "%f, %f, %f\n", data[1][0], data[1][1], data[1][2] );
		TRACE3( "%f, %f, %f\n", data[2][0], data[2][1], data[2][2] );
	}
#endif

	double& dx() { return data[0][2]; }
	double& dy() { return data[1][2]; }
	double& sx() { return data[0][0]; }
	double& sy() { return data[1][1]; }
	double dx_const() const { return data[0][2]; }
	double dy_const() const { return data[1][2]; }
	double sx_const() const { return data[0][0]; }
	double sy_const() const { return data[1][1]; }

	matrix& operator= ( const matrix& m );

	matrix t() const;
	matrix obr() const;
};

} // namespace rp

rp::matrix operator* ( const rp::matrix& m1, const rp::matrix& m2    );
rp::point  operator* ( const rp::matrix& m,  const rp::point&  point );
CPoint     operator* ( const rp::matrix& m,  const CPoint&     point );

#endif // RDO_PROCESS_FLOWCHART_H
