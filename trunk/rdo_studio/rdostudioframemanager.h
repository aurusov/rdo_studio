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
		Frame(): hitem( 0 ), doc( NULL ), view( NULL ) {};
		HTREEITEM           hitem;
		RDOStudioFrameDoc*  doc;
		RDOStudioFrameView* view;
	};
	static std::vector< Frame* > frames;

	int initFrameNumber;

public:
	RDOStudioFrameManager();
	virtual ~RDOStudioFrameManager();

	void insertItem( const std::string& name );
	int findFrameIndex( const HTREEITEM hitem ) const;
	int findFrameIndex( const RDOStudioFrameDoc* doc ) const;
	RDOStudioFrameDoc* connectFrameDoc( const HTREEITEM hitem ) const;
	void disconnectFrameDoc( const RDOStudioFrameDoc* doc ) const;
	RDOStudioFrameDoc*  getFrameDoc( const int index ) const  { return frames[index]->doc;  };
	RDOStudioFrameView* getFrameView( const int index ) const { return frames[index]->view; };
	void clear();

	void expand() const;

	bool isValidFrameDoc( const RDOStudioFrameDoc* const frame ) const;
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOFRAMEMANAGER_H
