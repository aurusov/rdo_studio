#ifndef BKEMULVIDEO_H
#define BKEMULVIDEO_H

#if _MSC_VER > 1000
#pragma once
#endif

namespace bkemul {

// --------------------------------------------------------------
// ---------- BKEmulVideo
// --------------------------------------------------------------
class BKEmulVideo
{
friend class BKEmul;

private:
	bool colorMonitor;

public:
	BKEmulVideo();
	virtual ~BKEmulVideo();

	void updateMonitor() const;
	void updateScrolling( BYTE delta ) const;
	void updateVideoMemoryByte( WORD address ) const;
	void updateVideoMemoryWord( WORD address ) const;
};

} // namespace bkemul

#endif // BKEMULVIDEO_H
