#ifndef RDOTRACERLOGSTYLE_H
#define RDOTRACERLOGSTYLE_H
#pragma once

#include "rdologstyle.h"

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerLogStyle
// ----------------------------------------------------------------------------
typedef struct {
	COLORREF foregroundColor;
	COLORREF backgroundColor;
} RDOColorPair;


class RDOTracerLogStyle : public RDOLogStyle
{
friend class RDOTracerLogOptions;

protected:
	RDOColorPair es;
	RDOColorPair eb;
	RDOColorPair ef;
	RDOColorPair ei;
	RDOColorPair er;
	RDOColorPair rc;
	RDOColorPair re;
	RDOColorPair rk;
	RDOColorPair v;
	RDOColorPair s;
	RDOColorPair dps;
	RDOColorPair sb;
	RDOColorPair so;
	RDOColorPair stn;
	RDOColorPair std;
	RDOColorPair str;
	RDOColorPair src;
	RDOColorPair sre;
	RDOColorPair srk;
	RDOColorPair sd;
	RDOColorPair ses;
	RDOColorPair sen;
	RDOColorPair sem;
	RDOColorPair sef;
	RDOColorPair seu;

public:
	RDOTracerLogStyle();
	virtual ~RDOTracerLogStyle();

	bool getItemColors( const string& item, COLORREF& textColor, COLORREF& backColor ) const;

	RDOTracerLogStyle& operator =( const RDOTracerLogStyle& style );
	bool operator ==( const RDOTracerLogStyle& style ) const;
	bool operator !=( const RDOTracerLogStyle& style ) const;

	static RDOTracerLogStyle getDefaultStyle();

	void load();
	void save() const;
};

#endif // RDOTRACERLOGSTYLE_H
