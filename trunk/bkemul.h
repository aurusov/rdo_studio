#ifndef BKEMUL_H
#define BKEMUL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>

// --------------------------------------------------------------
// ---------- BKEmul
// --------------------------------------------------------------
class BKEmul
{
friend class BKChildView;

private:
	std::vector< unsigned char > memory;
	bool colorMonitor;

public:
	BKEmul();
	virtual ~BKEmul();

	const unsigned char* getVideoMemory() const { return &memory[0] + 040000; }
	int getVideoMemorySize() const              { return 040000;              }

	bool isColorMonitor() const { return colorMonitor; }
};

// --------------------------------------------------------------
extern BKEmul emul;

#endif // BKEMUL_H
