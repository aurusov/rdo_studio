#ifndef BKEMULTAPERECORDER_H
#define BKEMULTAPERECORDER_H

#if _MSC_VER > 1000
#pragma once
#endif

namespace bkemul {

// --------------------------------------------------------------
// ---------- BKEmulTapeRecorder
// --------------------------------------------------------------
class BKEmulTapeRecorder
{
private:

	CFileFind* findFile;
	void closeFind();

	void copyToBK( const char* from, WORD to, WORD size ) const;
	void copyFromBK( WORD from, char* to, WORD size ) const;

	static bool isFileExists( const std::string& fileName );

public:
	BKEmulTapeRecorder();
	virtual ~BKEmulTapeRecorder();

	void reset();
	void doEMT36();
};

} // namespace bkemul

#endif // BKEMULTAPERECORDER_H
