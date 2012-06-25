#ifndef _PROC2RDO_RDOPROCESS_SHAPE_DECIDE_DLG1_H_
#define _PROC2RDO_RDOPROCESS_SHAPE_DECIDE_DLG1_H_

#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_decide.h"

class RPProcessShapeDecideDlg1 : public CDialog
{
public:
	RPProcessShapeDecideDlg1(CWnd* pParent = NULL, RPProcessShapeDecide* ppParent=NULL);
	virtual BOOL OnInitDialog();

	RPProcessShapeDecide* m_pParent; // ссылка на родителя

	enum { IDD = IDD_DIALOG_DECIDE_1 };
	CString	m_name;
	double	mp_true;
	double	mp_false;

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // _PROC2RDO_RDOPROCESS_SHAPE_DECIDE_DLG1_H_
