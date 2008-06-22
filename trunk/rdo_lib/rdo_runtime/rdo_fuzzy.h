#ifndef RDO_FUZZY_H
#define RDO_FUZZY_H

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValue
// ----------------------------------------------------------------------------
class RDOFuzzyValue
{
public:
	struct FuzzySetItem
	{
		RDOValue m_value;
		double   m_appertain;
		
		FuzzySetItem():
			m_value(RDOValue),
			m_appertain(0)
		{
		}
		FuzzySetItem( RDOValue value, double appertain ):
			m_value(value),
			m_appertain(appertain)
		{
			if ( appertain < 0 )
			{
				m_appertain = 0;
			}
			else if ( appertain > 1 )
			{
				m_appertain = 1;
			}
			
		}
	};
	typedef std::vector< FuzzySetItem > FuzzySet;
	RDOFuzzyValue();

	// А есть ли
	virtual bool isDefined( const RDOValue& value ) = 0;
private:
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValueContinuous
// ----------------------------------------------------------------------------
class RDOFuzzyValueContinuous: public RDOFuzzyValue
{
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValueInterrupted
// ----------------------------------------------------------------------------
class RDOFuzzyValueInterrupted: public RDOFuzzyValue
{
};

} // namespace rdoRuntime

#endif // RDO_FUZZY_H
