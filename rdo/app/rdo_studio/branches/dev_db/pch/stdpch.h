#ifndef _RDO_STUDIO_STDPCH_H_
#define _RDO_STUDIO_STDPCH_H_

#pragma warning(push)
#pragma warning(disable: 4512)
#include <boost/signal.hpp>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4127)
#include <QString>
#include <QHash>
#include <QList>
#pragma warning(pop)

#pragma warning(disable: 4786)

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <list>
#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "utils/rdodebug.h"
#include "utils/rdocommon.h"
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "utils/rdodebug.h"

int  roundDouble(const double val);

#endif // _RDO_STUDIO_STDPCH_H_
