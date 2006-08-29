#ifndef RDO_PROCESS_METHOD_MANAGER_H
#define RDO_PROCESS_METHOD_MANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------
// ---------- RPMethodPlugin
// ----------------------------------------------------------------------------
namespace rpMethod {
class RPMethod;
}

class RPMethodPlugin
{
friend class RPMethodManager;
private:
	rp::string          file_name;
	HMODULE             lib;
	rpMethod::RPMethod* method;

public:
	RPMethodPlugin( const rp::string& _file_name );
	~RPMethodPlugin();

	const rpMethod::RPMethod* getMethod() const { return method; }
	HMODULE getLib() const                      { return lib;    }

	static bool isMethod( const std::string& file_name );
};

// ----------------------------------------------------------------------------
// ---------- RPMethodManager
// ----------------------------------------------------------------------------
class RPMethodManager
{
protected:
	std::vector< RPMethodPlugin* > methods;
	void enumPlugins( const std::string& mask );
	std::list< CImageList* > im_lists;

public:
	RPMethodManager();
	virtual ~RPMethodManager();

	RPMethodPlugin* find( rpMethod::RPMethod* method ) const {
		std::vector< RPMethodPlugin* >::const_iterator it = methods.begin();
		while ( it != methods.end() ) {
			if ( (*it)->getMethod() == method ) return *it;
			it++;
		}
		return NULL;
	}
	const std::vector< RPMethodPlugin* >& getList() const { return methods; }

	void init();
	void close();
};

// ----------------------------------------------------------------------------
// ---------- RPMethodNewDlg
// ----------------------------------------------------------------------------
class CListCtrlDraw: public CListCtrl
{
public:
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
};

class RPMethodNewDlg: public CDialog
{
public:
	RPMethodNewDlg();
	virtual ~RPMethodNewDlg();

	rpMethod::RPMethod* getMethod() const { return method; }

protected:
	rpMethod::RPMethod* method;
	CImageList im_list;

	//{{AFX_DATA(RPMethodNewDlg)
	CListCtrlDraw methods;
	CStatic   desc;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RPMethodNewDlg)
	protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RPMethodNewDlg)
	afx_msg void OnAboutEmail();
	afx_msg void OnAboutWeb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMethodListItem—hanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMethodListDblClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMethodListClick(NMHDR *pNMHDR, LRESULT *pResult);
};

#endif // RDO_PROCESS_METHOD_MANAGER_H
