/*!
  \copyright (c) RDO-Team, 2011
  \file      sequence.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      31.07.2011
  \brief     Последовательности
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcRandomDistribution
// --------------------------------------------------------------------------------
template<class T>
inline RDOCalcRandomDistribution<T>::RDOCalcRandomDistribution(PTR(T) gen)
	: m_gen(gen)
{}

template<class T>
inline RDOCalcRandomDistribution<T>::~RDOCalcRandomDistribution()
{
	delete m_gen;
}

template<class T>
inline REF(RDOValue) RDOCalcRandomDistribution<T>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue res = getNextValue(pRuntime);
	if (m_diap)
	{
		if (res < m_diap_min)
		{
			m_value = m_res_real ? m_diap_min : RDOValue(m_diap_min > 0 ? m_diap_min + 0.5 : m_diap_min - 0.5).getInt();
			return m_value;
		}
		if (res > m_diap_max)
		{
			m_value = m_res_real ? m_diap_max : RDOValue(m_diap_max > 0 ? m_diap_max + 0.5 : m_diap_max - 0.5).getInt();
			return m_value;
		}
		m_value = m_res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
		return m_value;
		// В новом РДО была сделана попытка выбирать новое случайное число, если текущее вышло за диапазон. Но при этом смешается среднее (оно и в другом случае может смещаться imho). Для совместимости оставим первый вариант.
//			for ( int i = 0; i < 1000; i++ ) {
//				if ( res >= diap_min && res <= diap_max ) return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
//				res = gen->next( pRuntime->getFuncArgument(0), pRuntime->getFuncArgument(1) );
//			}
//			pRuntime->error( "Не удается получить значение, попадающее в назначенный диапазон", this );
//			return res_real ? diap_min : static_cast<int>(diap_min);
	}
	else
	{
		m_value = m_res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
		return m_value;
	}
}

CLOSE_RDO_RUNTIME_NAMESPACE
