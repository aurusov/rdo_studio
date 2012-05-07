/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_sequence.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      31.07.2011
  \brief     Последовательности
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "simulator/runtime/namespace.h"
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
inline void RDOCalcRandomDistribution<T>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue res = getNextValue(pRuntime);
	if (m_range)
	{
		if (res < m_range.get().m_min)
		{
			pRuntime->stack().push(m_res_real
				? m_range.get().m_min
				: RDOValue(m_range.get().m_min > 0
					? m_range.get().m_min + 0.5
					: m_range.get().m_min - 0.5
				).getInt()
			);
			return;
		}
		if (res > m_range.get().m_max)
		{
			pRuntime->stack().push(m_res_real
				? m_range.get().m_max
				: RDOValue(m_range.get().m_max > 0
					? m_range.get().m_max + 0.5
					: m_range.get().m_max - 0.5
				).getInt()
			);
			return;
		}
		pRuntime->stack().push(m_res_real
			? res
			: RDOValue(res > 0
				? res + 0.5
				: res - 0.5
			).getInt()
		);
		// В новом РДО была сделана попытка выбирать новое случайное число, если текущее вышло за диапазон. Но при этом смешается среднее (оно и в другом случае может смещаться imho). Для совместимости оставим первый вариант.
//			for ( int i = 0; i < 1000; i++ ) {
//				if ( res >= m_range.get().m_min && res <= m_range.get().m_max ) return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
//				res = gen->next( pRuntime->getFuncArgument(0), pRuntime->getFuncArgument(1) );
//			}
//			pRuntime->error().push(_T("Не удается получить значение, попадающее в назначенный диапазон"), this);
//			return res_real ? m_range.get().m_min : static_cast<int>(m_range.get().m_min);
	}
	else
	{
		pRuntime->stack().push(m_res_real
			? res
			: RDOValue(res > 0
				? res + 0.5
				: res - 0.5
			).getInt()
		);
	}
}

CLOSE_RDO_RUNTIME_NAMESPACE
