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
		Frame(): hitem( 0 ), doc( NULL ), view( NULL ), timer( false, true ) {};
		HTREEITEM           hitem;
		std::string         name;
		RDOStudioFrameDoc*  doc;
		RDOStudioFrameView* view;
		CMutex              used;
		CMutex              draw;
		CEvent              timer;
		CEvent              close;
	};
	static std::vector< Frame* > frames;

public:
	RDOStudioFrameManager();
	virtual ~RDOStudioFrameManager();

	void insertItem( const std::string& name );
	int findFrameIndex( const HTREEITEM hitem ) const;
	int findFrameIndex( const RDOStudioFrameDoc* doc ) const;
	int findFrameIndex( const RDOStudioFrameView* view ) const;
	RDOStudioFrameDoc* connectFrameDoc( const HTREEITEM hitem ) const;
	RDOStudioFrameDoc* connectFrameDoc( const int index ) const;
	void disconnectFrameDoc( const RDOStudioFrameDoc* doc ) const;
	std::string         getFrameName( const int index ) const    { return frames[index]->name;   };
	RDOStudioFrameDoc*  getFrameDoc( const int index ) const     { return frames[index]->doc;    };
	RDOStudioFrameView* getFrameView( const int index ) const    { return frames[index]->view;   };
	CMutex*             getFrameUsed( const int index ) const    { return &frames[index]->used;  };
	CMutex*             getFrameDraw( const int index ) const    { return &frames[index]->draw;  };
	CEvent*             getFrameTimer( const int index ) const   { return &frames[index]->timer; };
	CEvent*             getFrameClose( const int index ) const   { return &frames[index]->close; };
	int count() const { return frames.size(); };
	void clear();

	void expand() const;

	bool isValidFrameDoc( const RDOStudioFrameDoc* const frame ) const;
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOFRAMEMANAGER_H
