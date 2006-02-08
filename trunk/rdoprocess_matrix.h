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

	rp::string print() const;

	double& dx() { return data[0][2]; }
	double& dy() { return data[1][2]; }
	double& sx() { return data[0][0]; }
	double& sy() { return data[1][1]; }
	double dx_const() const { return data[0][2]; }
	double dy_const() const { return data[1][2]; }

	RDOPROCMatrix& operator= ( const RDOPROCMatrix& matrix );

	CPoint operator* ( const CPoint& point );

	RDOPROCMatrix t() const;
	RDOPROCMatrix obr() const;
};

RDOPROCMatrix operator* ( const RDOPROCMatrix& matrix1, const RDOPROCMatrix& matrix2 );

#endif // RDO_PROCESS_FLOWCHART_H
