#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo
{
    namespace gui
    {
        struct ScrollMetric
        {
            int position;
            int posMax;
            int pageSize;

            ScrollMetric();

            bool applyInc(int delta);
        };
    } // namespace gui
} // namespace rdo
