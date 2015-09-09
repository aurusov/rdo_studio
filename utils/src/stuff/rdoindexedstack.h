#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "utils/src/debug/rdodebug.h"
#include "utils/src/stuff/rdoidgenerator.h"
// --------------------------------------------------------------------------------

namespace rdo {

template<class T>
class IndexedStack
{
public:
    typedef IDGenerator::ID ID;

    IndexedStack()
    {}

    ID push(const T& object)
    {
        std::pair<typename Stack::iterator, bool> result = m_stack.insert(
            std::pair<typename Stack::key_type, typename Stack::mapped_type>(m_generator.get(), object)
        );
        ASSERT(result.second);
        return result.first->first;
    }
    T pop(ID index)
    {
        typename Stack::iterator it = m_stack.find(index);
        ASSERT(it != m_stack.end());
        T object = it->second;
        m_stack.erase(it);
        return object;
    }

protected:
    typedef std::map<IDGenerator::ID, T> Stack;
    Stack       m_stack;
    IDGenerator m_generator;
};

} // namespace rdo
