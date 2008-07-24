#ifndef RDO_FUZZY_H
#define RDO_FUZZY_H

#include "rdo_type.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyType
// ----------------------------------------------------------------------------
class RDOValue;
class RDOFuzzyValue;

class RDOFuzzyType: public RDOType
{
public:
	RDOFuzzyType();
	virtual std::string asString() const;

	typedef std::map   < std::string, RDOFuzzyValue > Terms;
	typedef std::vector< RDOValue >                   ValidRange;

	bool inRange() const;

private:
	Terms      m_terms;
	ValidRange m_range;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValue
// ----------------------------------------------------------------------------
struct RDOFuzzyValueItem;

class RDOFuzzyValue
{
public:
	RDOFuzzyValue( const RDOFuzzyType& type );
	RDOFuzzyValue( const RDOFuzzyValue& value );

	typedef std::vector< RDOFuzzyValueItem > FuzzySet;

	const RDOFuzzyType& type() const { return *m_type; }

	RDOValue defuzzyfication();

private:
	FuzzySet*           m_set;
	const RDOFuzzyType* m_type;
};

} // namespace rdoRuntime

#endif // RDO_FUZZY_H
