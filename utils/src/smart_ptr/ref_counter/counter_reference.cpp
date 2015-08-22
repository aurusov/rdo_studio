// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/ref_counter/counter_reference.h"
// --------------------------------------------------------------------------------

namespace rdo {

counter_reference::counter_reference()
    : m_intrusive_counter(0)
{}

counter_reference::~counter_reference()
{}

} // namespace rdo
