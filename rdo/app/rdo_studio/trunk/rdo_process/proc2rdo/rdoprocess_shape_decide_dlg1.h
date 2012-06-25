#if !defined(AFX_RDOPROCESS_SHAPE_DECIDE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_DECIDE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_

//
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_decide.h"

class RPProcessShapeDecideDlg1 : public CDialog
{
public:
	RPProcessShapeDecideDlg1(CWnd* pParent = NULL, RPProcessShapeDecide* ppParent=NULL);
    virtual BOOL OnInitDialog();

	RPProcessShapeDecide* pParentMJ; // ссылка на родителя

	enum { IDD = IDD_DIALOG_DECIDE_1 };
	CString	m_name;
	double	mp_true;
	double	mp_false;

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_)
