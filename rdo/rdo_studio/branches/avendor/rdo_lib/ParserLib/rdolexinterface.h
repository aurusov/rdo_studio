#ifndef RDO_LEX_INTERFACE_H_
#define RDO_LEX_INTERFACE_H_

namespace rdoParse 
{

class rdoLexer
{
public:
	rdoLexer(std::istream* arg_yyin, std::ostream* arg_yyout);
	~rdoLexer();
};

int rdoLineno();
const char *rdoYYText();
int rdoYylex();

}

#endif //RDO_LEX_INTERFACE_H_
