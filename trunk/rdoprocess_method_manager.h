#ifndef RDO_PROCESS_METHOD_MANAGER_H
#define RDO_PROCESS_METHOD_MANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace rpMethod {
class RPMethod;
}

#define RDO_METHOD_DLL
#undef  RDO_METHOD_DLL

#ifdef RDO_METHOD_DLL
// ----------------------------------------------------------------------------
// ---------- RPMethodPlugin
// ----------------------------------------------------------------------------
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

	rpMethod::RPMethod* getMethod() const { return method; }
	HMODULE getLib() const                { return lib;    }

	static bool isMethod( const std::string& file_name );
};
#endif

// ----------------------------------------------------------------------------
// ---------- RPMethodManager
// ----------------------------------------------------------------------------
class RPMethodManager
{
protected:
#ifdef RDO_METHOD_DLL
	std::vector< RPMethodPlugin* > methods;
	void enumPlugins( const std::string& mask );
#else
	std::vector< rpMethod::RPMethod* > methods;
	void insertMethod( rpMethod::RPMethod* method );
#endif
	std::list< CImageList* > im_lists;

public:
	RPMethodManager();
	virtual ~RPMethodManager();

#ifdef RDO_METHOD_DLL
	RPMethodPlugin* find( rpMethod::RPMethod* method ) const {
		std::vector< RPMethodPlugin* >::const_iterator it = methods.begin();
		while ( it != methods.end() ) {
			if ( (*it)->getMethod() == method ) return *it;
			it++;
		}
		return NULL;
	}
	const std::vector< RPMethodPlugin* >& getList() const { return methods; }
#else
	const std::vector< rpMethod::RPMethod* >& getList() const { return methods; }
#endif

	void init();
	void close();
};

// ----------------------------------------------------------------------------
// ---------- CListCtrlDraw
// ----------------------------------------------------------------------------
class CListCtrlDraw: public CListCtrl
{
public:
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
};

// ----------------------------------------------------------------------------
// ---------- RPMethodNewDlg
// ----------------------------------------------------------------------------
class RPMethodNewDlg: public CDialog
{
friend static int CALLBACK MethodsCompareProc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort );
friend class CListCtrlDraw;

public:
	RPMethodNewDlg();
	virtual ~RPMethodNewDlg();

	rpMethod::RPMethod* getMethod() const { return method_last; }

protected:
	rpMethod::RPMethod* method_last;
	CImageList im_list;

	//{{AFX_DATA(RPMethodNewDlg)
	CListCtrlDraw methods;
	CStatic       desc;
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
