#ifndef RDO_FUZZY_H
#define RDO_FUZZY_H

#include "rdo_type.h"
#include "rdoruntime_object.h"

namespace rdoRuntime {

struct RDOFuzzyValueItem;

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyType
// ----------------------------------------------------------------------------
class RDOValue;
class RDOFuzzyValue;
class RDOFuzzySetDefinition;

class RDOFuzzyType: public RDOType, public RDORuntimeObject
{
public:
	RDOFuzzyType( RDOFuzzySetDefinition* setDefinition );
	virtual std::string asString() const;

	typedef std::map< std::string, RDOFuzzyValue > Terms;

	bool inRange( const RDOValue& value ) const;

private:
	Terms                  m_terms;
	RDOFuzzySetDefinition* m_setDefinition;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinition
// ----------------------------------------------------------------------------
class RDOFuzzySetDefinition: public RDORuntimeObject
{
public:
	RDOFuzzySetDefinition( RDORuntimeParent* parent );
	virtual ~RDOFuzzySetDefinition() {}
	virtual bool inRange( const RDOValue& value ) const = 0;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyFixedSet
// ----------------------------------------------------------------------------
class RDOFuzzyFixedSet: public RDOFuzzySetDefinition
{
public:
	RDOFuzzyFixedSet( RDORuntimeParent* parent );
	virtual ~RDOFuzzyFixedSet();

	typedef std::vector< RDOFuzzyValueItem > FuzzySet;

	void append( const RDOValue& value, double appertain );
	virtual bool inRange( const RDOValue& value ) const;

private:
	FuzzySet* m_set;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValue
// ----------------------------------------------------------------------------
class RDOFuzzyValue
{
public:
	RDOFuzzyValue( const RDOFuzzyType& type );
	RDOFuzzyValue( const RDOFuzzyValue& value );
	~RDOFuzzyValue();

	typedef std::vector< RDOFuzzyValueItem > FuzzySet;

	const RDOFuzzyType& type() const { return *m_type; }

	RDOValue defuzzyfication();

private:
	FuzzySet*           m_set;
	const RDOFuzzyType* m_type;
};

} // namespace rdoRuntime

#endif // RDO_FUZZY_H
