#ifndef RDOEDITORPROJECT_H
#define RDOEDITORPROJECT_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- RDOEditorProject
// ----------------------------------------------------------------------------
class RDOEditorEdit;

class RDOEditorProject
{
private:
	bool loadFile( const CString& fName, RDOEditorEdit* edit, const bool mustExist = false ) const;

protected:
	CString name;
	CString lastModelPath;

	bool running;

	void save() const;
	bool saveAsDlg();
	void extractName( const CFileDialog& dlg );
	void extractName( CString str );

public:
	CString buf1;
	CString buf2;
	CString buf3;
	CString buf4;
	bool resetBuf1;
	bool resetBuf2;
	bool resetBuf3;
	bool resetBuf4;
	int  currentBuffer;
	bool canClearBuffer;
	int  clearBufferDelay;

	CString modelFileName;
	CString rdoFileName;

	RDOEditorProject();
	~RDOEditorProject();

	void newProject();
	CString loadProject( const CString& projectName = "" );
	void loadPMV() const;
	void loadTRC() const;
	bool saveProject();
	void saveAsProject();
	bool closeProject();

	bool isModify() const;

	void start();
	void stop();
	bool canRun() const           { return !modelFileName.IsEmpty() && !rdoFileName.IsEmpty(); };
	bool isRunning() const        { return running;                                            };
	bool canUpdateResults() const { return !isRunning() && !name.IsEmpty();                    };

	CString getName() const             { return name; };
	void setName( const CString str );

	void changeLastModelPath( CString str );
};

// ----------------------------------------------------------------------------
extern RDOEditorProject rdoEditorProject;

#endif // RDOEDITORPROJECT_H
