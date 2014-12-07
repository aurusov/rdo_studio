#ifndef _UTILS_MODEL_OBJECTS_H_
#define _UTILS_MODEL_OBJECTS_H_

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo {
namespace model {

enum FileType
{
	RTP = 0,
	RSS,
	EVN,
	PAT,
	DPT,
	PRC,
	FRM,
	FUN,
	SMR,
	PMD,
	PMV,
	TRC,
	RDOX,
	PRCX
};

std::string getFileTypeString(FileType type);

}} // namespace rdo::model

#endif // _UTILS_MODEL_OBJECTS_H_
