#ifndef RDOTRACERRDOCLASSES_H
#define RDOTRACERRDOCLASSES_H
#pragma once

#include <vector>

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerResParamInfo
// ----------------------------------------------------------------------------
enum RDOTracerResParamType {
	RDOPT_INTEGER = 0,
	RDOPT_REAL,
	RDOPT_ENUMERATIVE
};
	
typedef vector <CString> RDOStringVector;

class RDOTracerResParamInfo
{
protected:
	RDOStringVector* enumValues;
	RDOTracerResParamType paramType;
public:
	RDOTracerResParamInfo( const RDOTracerResParamType type );
	virtual ~RDOTracerResParamInfo();

	CString Name;
	RDOTracerResParamType getParamType() const { return paramType; };
	int addEnumValue( const CString& value );
	CString& getEnumValue( const int index ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerTreeItem
// ----------------------------------------------------------------------------
class RDOTracerTreeItem
{
protected:
	HTREEITEM treeItem;
	bool drawable; // set to true ONLY for RDOTracerSerie and descendants
public:
	RDOTracerTreeItem( const bool _drawable = false ) : treeItem ( NULL ), drawable( _drawable ) {};
	~RDOTracerTreeItem() {};
	void setTreeItem( HTREEITEM item ) { treeItem = item; };
	const HTREEITEM getTreeItem() const { return treeItem; };
	const bool isDrawable() const { return drawable; };
};


// ----------------------------------------------------------------------------
// ---------- RDOTracerResType
// ----------------------------------------------------------------------------
enum RDOTracerResTypeKind {
	RDOTK_PERMANENT = 0,
	RDOTK_TEMPORARY
};

class RDOTracerResType : public RDOTracerTreeItem
{
protected:
	vector <RDOTracerResParamInfo*> paramsInfo;
	RDOTracerResTypeKind restypeKind;
public:
	RDOTracerResType( const RDOTracerResTypeKind kind );
	virtual ~RDOTracerResType();

	CString Name;
	RDOTracerResTypeKind getResTypeKind() const { return restypeKind; };
	int addParamInfo( RDOTracerResParamInfo* const value );
	RDOTracerResParamInfo* getParamInfo( const int index ) const;
	const int getParamsCount() const { return paramsInfo.size(); };
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerValue
// ----------------------------------------------------------------------------
struct RDOTracerTimeNow {
	double time;
	int eventCount;
};

class RDOTracerValue
{
friend class RDOTracerSerie;

protected:
	RDOTracerTimeNow* modeltime;
	int eventIndex;
public:
	double value;
	RDOTracerValue( RDOTracerTimeNow* const timenow );
	~RDOTracerValue();
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerSerie
// ----------------------------------------------------------------------------

class RDOTracerSerie : public RDOTracerTreeItem
{
protected:
	CString title;
	vector <RDOTracerValue*> values;
	double minValue;
	double maxValue;
public:
	RDOTracerSerie();
	virtual ~RDOTracerSerie();

	CString getTitle() const;
	void setTitle( const CString value );

	int addValue( RDOTracerValue* const value );
	RDOTracerValue* getValue( const int index ) const;
	int getValueCount() const;
	RDOTracerValue* getLastValue() const;

	void drawSerie( CDC &dc, CRect &rect );
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerResParam
// ----------------------------------------------------------------------------
class RDOTracerResource;

class RDOTracerResParam: public RDOTracerSerie
{
protected:
	RDOTracerResource* resource;
public:
	RDOTracerResParam( RDOTracerResource* const res );
	virtual ~RDOTracerResParam();
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerResource
// ----------------------------------------------------------------------------
class RDOTracerResource : public RDOTracerTreeItem
{
protected:
	vector <RDOTracerResParam*> params;
	RDOTracerResType* resType;
	bool erased;
public:
	RDOTracerResource( RDOTracerResType* const type, CString& name );
	virtual ~RDOTracerResource();

	CString Name;
	int id;
	RDOTracerResType* getType() const { return resType; };
	int addParam( RDOTracerResParam* const value );
	RDOTracerResParam* getParam( const int index ) const;
	void setParams( CString& line, RDOTracerTimeNow* const time, const bool erasing = false );
	void setErased( const bool value );
	bool isErased() { return erased; };
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerPattern
// ----------------------------------------------------------------------------
enum RDOTracerPatternKind {
	RDOPK_OPERATION = 0,
	RDOPK_IRREGULAREVENT,
	RDOPK_RULE,
	RDOPK_KEYBOARD
};

class RDOTracerPattern : public RDOTracerTreeItem
{
protected:
	RDOTracerPatternKind patKind;
public:
	RDOTracerPattern( const RDOTracerPatternKind kind );
	virtual ~RDOTracerPattern();

	CString Name;
	RDOTracerPatternKind getPatternKind() const { return patKind; };
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerOperation
// ----------------------------------------------------------------------------
class RDOTracerOperation: public RDOTracerSerie
{
protected:
	RDOTracerPattern* pattern;
	CString Name;
public:
	RDOTracerOperation( RDOTracerPattern* const pat );
	virtual ~RDOTracerOperation();

	RDOTracerPattern* getPattern() const { return pattern; };
	void start( RDOTracerTimeNow* const time );
	void accomplish( RDOTracerTimeNow* const time );
	void setName( const CString& name ) { Name = name; if ( title.IsEmpty() ) title = Name; };
	const CString getName() const { return Name; };
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerResult
// ----------------------------------------------------------------------------
enum RDOTracerResultKind {
	RDORK_WATCHPAR = 0,
	RDORK_WATCHSTATE,
	RDORK_WATCHQUANT,
	RDORK_WATCHVALUE
};

class RDOTracerResult: public RDOTracerSerie
{
protected:
	RDOTracerResultKind resultKind;
	CString Name;
public:
	RDOTracerResult( const RDOTracerResultKind kind );
	virtual ~RDOTracerResult();

	int id;
	RDOTracerResultKind getResultKind() const { return resultKind; };
	void setName( const CString& name ) {
		Name = name;
		if ( title.IsEmpty() ) title = Name;
	};
	const CString getName() const { return Name; };
};

#endif // RDOTRACERRDOCLASSES_H