#ifndef BKEMULTIMER_H
#define BKEMULTIMER_H

#if _MSC_VER > 1000
#pragma once
#endif

namespace bkemul {

// --------------------------------------------------------------
// ---------- BKEmulTimer
// --------------------------------------------------------------
class BKEmulTimer
{
private:
	enum Type { none, t20, t22, t24 }; // Системный таймер БК запущен/остановлен
	Type type;
	int  tick_count;
	int  tick_koef;
	bool next20;

public:
	BKEmulTimer();
	virtual ~BKEmulTimer();

	void reset();
	void tick();
	bool isWork() const { return type != none; }
	void set177712( WORD value );
	void set177712( BYTE value );
	void set177713( BYTE value );
};

} // namespace bkemul


#endif // BKEMULTIMER_H
