/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      rdoprocess_method_manager.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Шувалов Андрей (dronbas@gmail.com)
  \date      23.08.2006
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_PROCESS_METHOD_MANAGER_H_
#define _RDO_STUDIO_MFC_RDO_PROCESS_METHOD_MANAGER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rpMethod {
class RPMethod;
}

#define RDO_METHOD_DLL
#undef  RDO_METHOD_DLL

#ifdef RDO_METHOD_DLL
// --------------------------------------------------------------------------------
// -------------------- RPMethodPlugin
// --------------------------------------------------------------------------------
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

	static rbool isMethod( CREF(tstring) file_name );
};
#endif

// --------------------------------------------------------------------------------
// -------------------- RPMethodManager
// --------------------------------------------------------------------------------
class RPMethodManager
{
public:
#ifdef RDO_METHOD_DLL
	typedef std::vector<PTR(RPMethodPlugin)>     MethodList;
#else
	typedef std::vector<PTR(rpMethod::RPMethod)> MethodList;
#endif

protected:
#ifdef RDO_METHOD_DLL
	void enumPlugins( CREF(tstring) mask );
#else
	void insertMethod( rpMethod::RPMethod* method );
#endif

	MethodList                 methods;
	std::list<PTR(CImageList)> im_lists;

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
	CREF(MethodList) getList() const { return methods; }
#else
	CREF(MethodList) getList() const { return methods; }
#endif

	void init();
	void close();
};

// --------------------------------------------------------------------------------
// -------------------- CListCtrlDraw
// --------------------------------------------------------------------------------
class CListCtrlDraw: public CListCtrl
{
public:
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
};

// --------------------------------------------------------------------------------
// -------------------- RPMethodNewDlg
// --------------------------------------------------------------------------------
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

	CListCtrlDraw methods;
	CStatic       desc;

private:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnAboutEmail();
	afx_msg void OnAboutWeb();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMethodListItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMethodListDblClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMethodListClick(NMHDR *pNMHDR, LRESULT *pResult);
};

#endif // _RDO_STUDIO_MFC_RDO_PROCESS_METHOD_MANAGER_H_
