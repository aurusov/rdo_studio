#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdorss.h"
#include "rdortp.h"
#include "rdoparser.h"

namespace rdoParse 
{

RDORSSResource::RDORSSResource(const string *const _name, const RDORTPResType *const _resType, const int _number)
	: name(_name), resType(_resType), number(_number)	
{
	currParam = resType->getParams().begin();
}

int RDORSSResource::writeModelStructure()
{
	currParser->modelStructure << (number + 1) << " " << *getName() << " " << getType()->getType() << endl;
	return 0;
}

}		// namespace rdoParse 
