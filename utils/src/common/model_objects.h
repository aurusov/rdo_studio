#ifndef _UTILS_MODEL_OBJECTS_H_
#define _UTILS_MODEL_OBJECTS_H_

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo {
namespace model {

//! \details Типы файлов
enum FileType
{
	RTP = 0, // Файл типов ресурсов
	RSS,     // Файл ресурсов
	EVN,     // Файл событий
	PAT,     // Файл паттернов
	DPT,     // Файл точек принятия решений
	PRC,     // Файл процессов
	FRM,     // Файл анимации
	FUN,     // Файл функций
	SMR,     // Файл прогона
	PMD,     // Файл описания показателей
	PMV,     // Файл со значениями показателей
	TRC,     // Файл трассировки
	RDOX,    // Файл проекта
	PRCX     // Файл GUI-процессов, XML
};

std::string getFileTypeString(FileType type);

}} // namespace rdo::model

#endif // _UTILS_MODEL_OBJECTS_H_
