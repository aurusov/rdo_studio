#include "stdafx.h"
#include "bkemultaperecorder.h"
#include "bkemul.h"
#include "resource.h"

#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace bkemul;
using namespace std;

// --------------------------------------------------------------
// ---------- BKEmulTapeRecorder
// --------------------------------------------------------------
BKEmulTapeRecorder::BKEmulTapeRecorder():
	findFile( NULL ),
	last_file( false )
{
}

BKEmulTapeRecorder::~BKEmulTapeRecorder()
{
	closeFind();
}

void BKEmulTapeRecorder::reset()
{
}

void BKEmulTapeRecorder::closeFind()
{
	if ( findFile ) {
		findFile->Close();
		delete findFile;
		findFile = NULL;
	}
}

void BKEmulTapeRecorder::copyToBK( const char* from, WORD to, WORD size ) const
{
	for ( WORD i = 0; i < size; i++ ) {
		emul.setMemoryByte( to + i, *(from + i) );
	}
}

void BKEmulTapeRecorder::copyFromBK( WORD from, char* to, WORD size ) const
{
	for ( WORD i = 0; i < size; i++ ) {
		*to++ = emul.getMemoryByte( from + i );
	}
}

bool BKEmulTapeRecorder::isFileExists( const string& fileName )
{
	CFileFind finder;
	return finder.FindFile( fileName.c_str() ) ? true : false;
}

void BKEmulTapeRecorder::doEMT36()
{
	WORD memory  = emul.cpu.R1();
	BYTE command = emul.getMemoryByte( memory );
	// Инициализируем ответ значением останова по команде оператора
	BYTE answer  = 4;
	WORD address = emul.getMemoryWord( memory+2 );
	WORD length  = emul.getMemoryWord( memory+4 );
	char name[17];
	copyFromBK( memory + 6, name, 16 );
	name[16] = 0;
	string file_name = name;

	char* buf = NULL;

	try {
		switch ( command ) {
			// Останов двигателя магнитофона
			// Операция завершена без ошибок
			case 0: answer = 0; break;
			// Пуск двигателя магнитофона
			// Операция завершена без ошибок
			case 1: answer = 0; break;
			// Запись файла
			case 2: {
				bool canSave = true;
				if ( isFileExists( file_name ) ) {
					emul.setPause( true );
					std::string name = file_name;
					trim( name );
					canSave = AfxMessageBox( format( IDS_FILEEXISTS, name.c_str() ).c_str(), MB_ICONQUESTION | MB_YESNO ) == IDYES;
					emul.setPause( false );
				}
				if ( canSave ) {
					ofstream file( file_name.c_str(), ios_base::out | ios_base::binary );
					// Записываем адрес и длину файла в поток
					file.write( reinterpret_cast<char*>(&address), 2 );
					file.write( reinterpret_cast<char*>(&length), 2 );
					// Записываем побайтно файл из памяти БК в выделенную память со всеми проверками
					buf = new char[ length ];
					copyFromBK( address, buf, length );
					// Переписываем файл в поток
					file.write( buf, length );
					delete buf;
					buf = NULL;
					// Операция завершена без ошибок
					answer = 0;
				} else {
					// Останов по команде оператора
					answer = 4;
				}
				break;
			}
			// Фиктивное чтение файла (переходит в раздел чтения файла)
			case 4: file_name = "";
			// Чтение файла
			case 3: {
				if ( isFileExists( file_name ) ) {
					ifstream file( file_name.c_str(), ios_base::in | ios_base::binary );
					WORD load_address;
					WORD load_length;
					file.read( reinterpret_cast<char*>(&load_address), 2 );
					file.read( reinterpret_cast<char*>(&load_length), 2 );
					// Адрес текущего массива
					emul.setMemoryWord( memory + 22, load_address );
					// Длина текущего массива
					emul.setMemoryWord( memory + 24, load_length );
					// Имя текущего массива
					int len = 16 - file_name.length();
					for ( int i = 0; i < len; i++ ) {
						file_name += ' ';
					}
					copyToBK( file_name.c_str(), memory + 26, 16 );
					// Если задан нулевой адрес назначения, то читаем в адрес, прочитанный из файла
					if ( address == 0 ) {
						address = load_address;
					}
					length = load_length;
					// Читаем из потока в выделенную память
					buf = new char[ length ];
					file.read( buf, length );
					// Копируем побайтно в память БК со всеми проверками
					copyToBK( buf, address, length );
					// Выполняем действия стандарного обработчика EMT36, которые он
					// выполняет после чтения массива
					// Адрес начала массива
					emul.setMemoryWord( 0264, load_address );
					// Длина массива
					emul.setMemoryWord( 0266, load_length );
					// Операция завершена без ошибок
					answer = 0;
					delete buf;
					buf = NULL;
				} else {
					// Не найден файл с таким именем
					answer = 1;
					// Открываем указатель на первый найденный файл в директории,
					// если он уже не открыт (этот механиз похож на callback-функцию, т.к.
					// вызов этого обработчика происходит из системного монитора после
					// каждого найденного имени)
					if ( last_file ) {
						last_file = false;
						// Все файлы перечислены, необходимо остановить вывод имен
						closeFind();
						// Останов по команде оператора
						answer = 4;
					} else {
						bool canFind = true;
						if ( !findFile ) {
							// Настраиваем поиск
							findFile = new CFileFind;
							char dir[ MAX_PATH + 1 ];
							::GetCurrentDirectory( MAX_PATH, dir );
							dir[ MAX_PATH ] = 0;
							string mask = dir;
							string::size_type pos = mask.find_last_of( '\\' );
							if ( pos == string::npos ) {
								pos = mask.find_last_of( '/' );
							}
							if ( pos != mask.length() - 1 ) {
								mask += '\\';
							}
							trimRight( file_name );
							mask += file_name + "*.*";
							canFind = findFile->FindFile( mask.c_str() ) ? true : false;
						}
						if ( canFind ) {
							// Находим первый/следующий файл в директории
							if ( findFile->FindNextFile() ) {
								bool flag = true;
								while ( flag && ( findFile->IsDots() || findFile->IsDirectory() ) ) {
									flag = findFile->FindNextFile() ? true : false;
								}
								if ( !findFile->IsDots() && !findFile->IsDirectory() ) {
									file_name = findFile->GetFileName();
								} else {
									// Все файлы перечислены, необходимо остановить вывод имен
									closeFind();
									// Останов по команде оператора
									answer = 4;
								}
							} else {
								// Находим последний файл в директрии
								if ( !findFile->IsDots() && !findFile->IsDirectory() ) {
									file_name = findFile->GetFileName();
									last_file = true;
								} else {
									// Все файлы перечислены, необходимо остановить вывод имен
									closeFind();
									// Останов по команде оператора
									answer = 4;
								}
							}
						}
					}
					// Передача имени найденного файла
					if ( answer == 1 ) {
						// Имя текущего массива
						int len = 16 - file_name.length();
						for ( int i = 0; i < len; i++ ) {
							file_name += ' ';
						}
						copyToBK( file_name.c_str(), memory + 26, 16 );

						ifstream file( file_name.c_str(), ios_base::in | ios_base::binary );
						WORD load_address;
						WORD load_length;
						file.read( reinterpret_cast<char*>(&load_address), 2 );
						file.read( reinterpret_cast<char*>(&load_length), 2 );
						// Адрес текущего массива
						emul.setMemoryWord( memory + 22, load_address );
						// Длина текущего массива
						emul.setMemoryWord( memory + 24, load_length );
					}
				}
				break;
			}
		}
		// Общая часть для всех режимов
		// Байт ответа
		emul.setMemoryByte( memory + 1, answer );
		// Дубль байта ответа
		emul.setMemoryByte( 0301, answer );

		emul.cpu.BK_doRTI();

	} catch ( BKEmul::BKMemoryAccessError& /*e*/ ) {
		// Ошибка доступа к памяти БК (грузили в ПЗУ)
		answer = 4;
		emul.setMemoryByte( memory + 1, answer );
		emul.setMemoryByte( 0301, answer );
		emul.cpu.BK_doRTI();
		if ( buf ) {
			delete buf;
			buf = NULL;
		}
		throw;
	}
}
