#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <cstddef>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// Определим YYSTYPE именно в этом файле, чтобы убрать зависимость от rdogramma.h
#ifdef YYSTYPE
#undef YYSTYPE
#endif
typedef int YYSTYPE;

#ifdef YYLTYPE
#undef YYLTYPE
#endif

struct yyltype
{
    std::size_t m_first_line;
    std::size_t m_first_pos;

    std::size_t m_last_line;
    std::size_t m_last_pos;

    std::size_t m_first_seek;
    std::size_t m_last_seek;
};
#define YYLTYPE yyltype

#ifdef YYLLOC_DEFAULT
#undef YYLLOC_DEFAULT
#endif

#define YYLLOC_DEFAULT(Current, Rhs, N)                             \
    do {                                                            \
        if (N)                                                      \
        {                                                           \
            (Current).m_first_line = YYRHSLOC(Rhs, 1).m_first_line; \
            (Current).m_first_pos  = YYRHSLOC(Rhs, 1).m_first_pos;  \
            (Current).m_first_seek = YYRHSLOC(Rhs, 1).m_first_seek; \
            (Current).m_last_line  = YYRHSLOC(Rhs, N).m_last_line;  \
            (Current).m_last_pos   = YYRHSLOC(Rhs, N).m_last_pos;   \
            (Current).m_last_seek  = YYRHSLOC(Rhs, N).m_last_seek;  \
        }                                                           \
        else                                                        \
        {                                                           \
            (Current).m_first_line = YYRHSLOC(Rhs, 0).m_last_line;  \
            (Current).m_last_line  = YYRHSLOC(Rhs, 0).m_last_line;  \
            (Current).m_first_pos  = YYRHSLOC(Rhs, 0).m_last_pos;   \
            (Current).m_last_pos   = YYRHSLOC(Rhs, 0).m_last_pos;   \
            (Current).m_first_seek = YYRHSLOC(Rhs, 0).m_last_seek;  \
            (Current).m_last_seek  = YYRHSLOC(Rhs, 0).m_last_seek;  \
        }                                                           \
    }                                                               \
    while (0)
