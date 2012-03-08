#ifndef RDOTREECTRL_H
#define RDOTREECTRL_H

// --------------------------------------------------------------------------------
// -------------------- RDOTreeCtrl
// --------------------------------------------------------------------------------
class RDOTreeCtrl: public CTreeCtrl
{
DECLARE_DYNAMIC( RDOTreeCtrl )

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

private:
	DECLARE_MESSAGE_MAP()

public:
	RDOTreeCtrl();
	virtual ~RDOTreeCtrl();

	void deleteChildren( const HTREEITEM parent );
};

#endif // RDOTREECTRL_H
