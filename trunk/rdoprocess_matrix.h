#ifndef RDO_PROCESS_MATRIX_H
#define RDO_PROCESS_MATRIX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_string.h"

// ----------------------------------------------------------------------------
// ---------- RDOPROCMatrix
// ----------------------------------------------------------------------------
class RDOPROCMatrix
{
public:
	double data[3][3];

	RDOPROCMatrix();
	RDOPROCMatrix( const RDOPROCMatrix& matrix );
	~RDOPROCMatrix();

#ifdef _DEBUG
	void trace() const {
		TRACE( "%f, %f, %f\n", data[0][0], data[0][1], data[0][2] );
		TRACE( "%f, %f, %f\n", data[1][0], data[1][1], data[1][2] );
		TRACE( "%f, %f, %f\n", data[2][0], data[2][1], data[2][2] );
	}
#endif

	double& dx() { return data[0][2]; }
	double& dy() { return data[1][2]; }
	double& sx() { return data[0][0]; }
	double& sy() { return data[1][1]; }
	double dx_const() const { return data[0][2]; }
	double dy_const() const { return data[1][2]; }

	RDOPROCMatrix& operator= ( const RDOPROCMatrix& matrix );

	RDOPROCMatrix t() const;
	RDOPROCMatrix obr() const;
};

RDOPROCMatrix operator* ( const RDOPROCMatrix& matrix1, const RDOPROCMatrix& matrix2 );
CPoint operator* ( const RDOPROCMatrix& matrix, const CPoint& point );

#endif // RDO_PROCESS_FLOWCHART_H
