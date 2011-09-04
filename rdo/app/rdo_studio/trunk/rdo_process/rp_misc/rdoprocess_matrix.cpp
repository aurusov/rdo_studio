#include "app/rdo_studio_mfc/rdo_process/rp_misc/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_matrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rp;

// --------------------------------------------------------------------------------
// -------------------- matrix
// --------------------------------------------------------------------------------
matrix::matrix()
{
	for ( int i = 0; i < 3; i++ )
		for ( int j = 0; j < 3; j++ )
			data[i][j] = 0.0;
	data[0][0] = 1.0;
	data[1][1] = 1.0;
	data[2][2] = 1.0;
}

matrix::matrix( const matrix& m )
{
	*this = m;
}

matrix::~matrix()
{
}

matrix& matrix::operator= ( const matrix& m )
{
	data[0][0] = m.data[0][0];
	data[1][0] = m.data[1][0];
	data[2][0] = m.data[2][0];
	data[0][1] = m.data[0][1];
	data[1][1] = m.data[1][1];
	data[2][1] = m.data[2][1];
	data[0][2] = m.data[0][2];
	data[1][2] = m.data[1][2];
	data[2][2] = m.data[2][2];
	return *this;
}

matrix matrix::t() const
{
	matrix m;
	m.data[0][0] = data[0][0];
	m.data[1][1] = data[1][1];
	m.data[2][2] = data[2][2];
	m.data[0][1] = data[1][0];
	m.data[1][0] = data[0][1];
	m.data[0][2] = data[2][0];
	m.data[2][0] = data[0][2];
	m.data[1][2] = data[2][1];
	m.data[2][1] = data[1][2];
	return m;
}

matrix matrix::obr() const
{
	matrix m;
	double a11 = data[1][1] * data[2][2] - data[1][2] * data[2][1];
	double a12 = data[1][2] * data[2][0] - data[1][0] * data[2][2];
	double a13 = data[1][0] * data[2][1] - data[2][0] * data[1][1];
	double a21 = data[0][2] * data[2][1] - data[0][1] * data[2][2];
	double a22 = data[0][0] * data[2][2] - data[0][2] * data[2][0];
	double a23 = data[0][1] * data[2][0] - data[0][0] * data[2][1];
	double a31 = data[0][1] * data[1][2] - data[0][2] * data[1][1];
	double a32 = data[1][0] * data[0][2] - data[0][0] * data[1][2];
	double a33 = data[0][0] * data[1][1] - data[0][1] * data[1][0];
	double md = data[0][0] * a11 + data[0][1] * a12 + data[0][2] * a13;
	m.data[0][0] = a11 / md;
	m.data[0][1] = a21 / md;
	m.data[0][2] = a31 / md;
	m.data[1][0] = a12 / md;
	m.data[1][1] = a22 / md;
	m.data[1][2] = a32 / md;
	m.data[2][0] = a13 / md;
	m.data[2][1] = a23 / md;
	m.data[2][2] = a33 / md;
	return m;
}

rp::matrix operator* ( const rp::matrix& m1, const rp::matrix& m2 )
{
	rp::matrix m;
	m.data[0][0] = m1.data[0][0] * m2.data[0][0] + m1.data[0][1] * m2.data[1][0] + m1.data[0][2] * m2.data[2][0];
	m.data[1][0] = m1.data[1][0] * m2.data[0][0] + m1.data[1][1] * m2.data[1][0] + m1.data[1][2] * m2.data[2][0];
	m.data[2][0] = m1.data[2][0] * m2.data[0][0] + m1.data[2][1] * m2.data[1][0] + m1.data[2][2] * m2.data[2][0];
	m.data[0][1] = m1.data[0][0] * m2.data[0][1] + m1.data[0][1] * m2.data[1][1] + m1.data[0][2] * m2.data[2][1];
	m.data[1][1] = m1.data[1][0] * m2.data[0][1] + m1.data[1][1] * m2.data[1][1] + m1.data[1][2] * m2.data[2][1];
	m.data[2][1] = m1.data[2][0] * m2.data[0][1] + m1.data[2][1] * m2.data[1][1] + m1.data[2][2] * m2.data[2][1];
	m.data[0][2] = m1.data[0][0] * m2.data[0][2] + m1.data[0][1] * m2.data[1][2] + m1.data[0][2] * m2.data[2][2];
	m.data[1][2] = m1.data[1][0] * m2.data[0][2] + m1.data[1][1] * m2.data[1][2] + m1.data[1][2] * m2.data[2][2];
	m.data[2][2] = m1.data[2][0] * m2.data[0][2] + m1.data[2][1] * m2.data[1][2] + m1.data[2][2] * m2.data[2][2];
	return m;
}

rp::point operator* ( const rp::matrix& m, const rp::point& point )
{
	double x = m.data[0][0] * point.x + m.data[0][1] * point.y + m.data[0][2];
	double y = m.data[1][0] * point.x + m.data[1][1] * point.y + m.data[1][2];
	double w = m.data[2][0] * point.x + m.data[2][1] * point.y + m.data[2][2];
	x /= w;
	y /= w;
	return rp::point( x, y );
}

CPoint operator* ( const rp::matrix& m, const CPoint& point )
{
	double x = m.data[0][0] * point.x + m.data[0][1] * point.y + m.data[0][2];
	double y = m.data[1][0] * point.x + m.data[1][1] * point.y + m.data[1][2];
	double w = m.data[2][0] * point.x + m.data[2][1] * point.y + m.data[2][2];
	x /= w;
	y /= w;
	return CPoint( static_cast<int>(x), static_cast<int>(y) );
}
