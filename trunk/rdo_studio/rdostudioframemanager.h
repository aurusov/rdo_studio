#ifndef RDOSTUDIOFRAMEMANAGER_H
#define RDOSTUDIOFRAMEMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameManager
// ----------------------------------------------------------------------------
class RDOStudioFrameDoc;
class RDOStudioFrameView;

class RDOStudioFrameManager
{
friend class RDOStudioModel;

private:
	CMultiDocTemplate* frameDocTemplate;

	class Frame {
	friend class RDOStudioFrameManager;
	public:
		Frame(): hitem( 0 ), doc( NULL ), view( NULL )/*, lock_deleted( &deleted ) */{};
		HTREEITEM           hitem;
		RDOStudioFrameDoc*  doc;
		RDOStudioFrameView* view;
		CMutex              used;
		CMutex              draw;
//		CMutex              deleted;
//		CSingleLock         lock_deleted;
	};
	static std::vector< Frame* > frames;

	int initFrameNumber;

public:
	RDOStudioFrameManager();
	virtual ~RDOStudioFrameManager();

	void insertItem( const std::string& name );
	int findFrameIndex( const HTREEITEM hitem ) const;
	int findFrameIndex( const RDOStudioFrameDoc* doc ) const;
	int findFrameIndex( const RDOStudioFrameView* view ) const;
	RDOStudioFrameDoc* connectFrameDoc( const HTREEITEM hitem ) const;
	void disconnectFrameDoc( const RDOStudioFrameDoc* doc ) const;
	RDOStudioFrameDoc*  getFrameDoc( const int index ) const     { return frames[index]->doc;      };
	RDOStudioFrameView* getFrameView( const int index ) const    { return frames[index]->view;     };
	CMutex*             getFrameUsed( const int index ) const    { return &frames[index]->used;    };
	CMutex*             getFrameDraw( const int index ) const    { return &frames[index]->draw;    };
//	CMutex*             getFrameDeleted( const int index ) const { return &frames[index]->deleted; };
//	void setDeleted( const int index, const bool value ) const   { value ? frames[index]->lock_deleted.Lock() : frames[index]->lock_deleted.Unlock(); };
//	bool isDeleted( const int index ) const                      { return frames[index]->lock_deleted.IsLocked() ? true : false; };
	void clear();

	void expand() const;

	bool isValidFrameDoc( const RDOStudioFrameDoc* const frame ) const;
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOFRAMEMANAGER_H
