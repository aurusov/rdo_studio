#ifndef BKEMULSPEAKER_H
#define BKEMULSPEAKER_H

#if _MSC_VER > 1000
#pragma once
#endif

namespace bkemul {

// --------------------------------------------------------------
// ---------- BKEmulSpeaker
// --------------------------------------------------------------
class BKEmulSpeaker
{
private:
	bool check() const;

public:
	BKEmulSpeaker();
	virtual ~BKEmulSpeaker();

	void reset();
	void down();
	void up();
};

} // namespace bkemul

#endif // BKEMULSPEAKER_H
