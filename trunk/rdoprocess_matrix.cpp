#include "stdafx.h"
#include "rdoprocess_matrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCMatrix
// ----------------------------------------------------------------------------
RDOPROCMatrix::RDOPROCMatrix()
{
	for ( int i = 0; i < 3; i++ )
		for ( int j = 0; j < 3; j++ )
			data[i][j] = 0.0;
	data[0][0] = 1.0;
	data[1][1] = 1.0;
	data[2][2] = 1.0;
}

RDOPROCMatrix::RDOPROCMatrix( const RDOPROCMatrix& matrix )
{
	*this = matrix;
}

RDOPROCMatrix::~RDOPROCMatrix()
{
}

RDOPROCMatrix& RDOPROCMatrix::operator= ( const RDOPROCMatrix& matrix )
{
	data[0][0] = matrix.data[0][0];
	data[1][0] = matrix.data[1][0];
	data[2][0] = matrix.data[2][0];
	data[0][1] = matrix.data[0][1];
	data[1][1] = matrix.data[1][1];
	data[2][1] = matrix.data[2][1];
	data[0][2] = matrix.data[0][2];
	data[1][2] = matrix.data[1][2];
	data[2][2] = matrix.data[2][2];
	return *this;
}

RDOPROCMatrix RDOPROCMatrix::t() const
{
	RDOPROCMatrix m;
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

RDOPROCMatrix RDOPROCMatrix::obr() const
{
	RDOPROCMatrix m;
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

RDOPROCMatrix operator* ( const RDOPROCMatrix& matrix1, const RDOPROCMatrix& matrix2 )
{
	RDOPROCMatrix matrix;
	matrix.data[0][0] = matrix1.data[0][0] * matrix2.data[0][0] + matrix1.data[0][1] * matrix2.data[1][0] + matrix1.data[0][2] * matrix2.data[2][0];
	matrix.data[1][0] = matrix1.data[1][0] * matrix2.data[0][0] + matrix1.data[1][1] * matrix2.data[1][0] + matrix1.data[1][2] * matrix2.data[2][0];
	matrix.data[2][0] = matrix1.data[2][0] * matrix2.data[0][0] + matrix1.data[2][1] * matrix2.data[1][0] + matrix1.data[2][2] * matrix2.data[2][0];
	matrix.data[0][1] = matrix1.data[0][0] * matrix2.data[0][1] + matrix1.data[0][1] * matrix2.data[1][1] + matrix1.data[0][2] * matrix2.data[2][1];
	matrix.data[1][1] = matrix1.data[1][0] * matrix2.data[0][1] + matrix1.data[1][1] * matrix2.data[1][1] + matrix1.data[1][2] * matrix2.data[2][1];
	matrix.data[2][1] = matrix1.data[2][0] * matrix2.data[0][1] + matrix1.data[2][1] * matrix2.data[1][1] + matrix1.data[2][2] * matrix2.data[2][1];
	matrix.data[0][2] = matrix1.data[0][0] * matrix2.data[0][2] + matrix1.data[0][1] * matrix2.data[1][2] + matrix1.data[0][2] * matrix2.data[2][2];
	matrix.data[1][2] = matrix1.data[1][0] * matrix2.data[0][2] + matrix1.data[1][1] * matrix2.data[1][2] + matrix1.data[1][2] * matrix2.data[2][2];
	matrix.data[2][2] = matrix1.data[2][0] * matrix2.data[0][2] + matrix1.data[2][1] * matrix2.data[1][2] + matrix1.data[2][2] * matrix2.data[2][2];
	return matrix;
}

CPoint operator* ( const RDOPROCMatrix& matrix, const CPoint& point )
{
	double x = matrix.data[0][0] * point.x + matrix.data[0][1] * point.y + matrix.data[0][2];
	double y = matrix.data[1][0] * point.x + matrix.data[1][1] * point.y + matrix.data[1][2];
	double w = matrix.data[2][0] * point.x + matrix.data[2][1] * point.y + matrix.data[2][2];
	x /= w;
	y /= w;
	return CPoint( x, y );
}
