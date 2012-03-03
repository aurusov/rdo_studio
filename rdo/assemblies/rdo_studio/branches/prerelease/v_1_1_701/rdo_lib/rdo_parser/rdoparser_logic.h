#ifndef RDOPARSER_LOGIC_H
#define RDOPARSER_LOGIC_H

#include "rdoparser_object.h"
#include <rdo_logic_interface.h>
#include "rdofun.h"

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOLogicActivity
// ----------------------------------------------------------------------------
template< class RTLogic, class Activity >
class RDOLogicActivity: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOLogicActivity( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info )
	{}

	const std::string& name() const { return src_info().src_text(); }

	Activity* addNewActivity( const RDOParserSrcInfo& activity_src_info, const RDOParserSrcInfo& pattern_src_info )
	{
		Activity* activity = new Activity( this, activity_src_info, pattern_src_info );
		m_activities.push_back( activity );
		return activity;
	}

	Activity* getLastActivity() const
	{
		return !m_activities.empty() ? m_activities.back() : NULL;
	}
	const std::vector< Activity* >& getActivities() const { return m_activities; }

	bool setPrior(RDOFUNArithm* prior)
	{
		LPIPriority priority = m_rt_logic;
		if (priority)
		{
			return priority->setPrior(prior->createCalc());
		}
		return false;
	}

protected:
	LPILogic                 m_rt_logic;

private:
	std::vector< Activity* > m_activities;
};

} // namespace rdoParse

#endif // RDOPARSER_LOGIC_H
