#ifndef _PROC2RDO_RDOPROCESS_GENERATION_TYPE_H_
#define _PROC2RDO_RDOPROCESS_GENERATION_TYPE_H_

#include "app/rdo_studio_mfc/resource.h"

class RPProcessGenerationType : public CDialog
{
public:
	RPProcessGenerationType(CWnd* pParent = NULL);
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_DIALOG_GEN_TYPE };
	CComboBox	m_combo;
	CEdit	m_time;
	double	m_time_value;

private:
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg void OnCloseupCombo1();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // _PROC2RDO_RDOPROCESS_GENERATION_TYPE_H_
