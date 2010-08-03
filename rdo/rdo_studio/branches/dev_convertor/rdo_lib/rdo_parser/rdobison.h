#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;

  int last_line;
  int last_column;

  int linear_pos;
} yyltype;

# define YYLTYPE yyltype
#endif
