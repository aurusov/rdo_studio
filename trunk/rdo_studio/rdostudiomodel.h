#ifndef RDOSTUDIOMODEL_H
#define RDOSTUDIOMODEL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioframemanager.h"
#include <rdosimwin.h>

// ----------------------------------------------------------------------------
// ---------- binarystream
// ----------------------------------------------------------------------------
class binarystream: public iostream
{
private:
	class binarybuf: public streambuf
	{
	friend class binarystream;
	protected:
		vector< char > vec;
		int current;

		virtual streambuf* setbuf( char* s, streamsize n ) {
			vec.reserve( n );
			vec.assign( s, s + n );
			current = 0;
			setg( vec.begin(), vec.begin(), vec.end() );
			setp( vec.begin(), vec.end() );
			return this;
		}
		virtual int_type overflow( int_type c = traits_type::eof() ) {
			if ( c != traits_type::eof() ) {
				vec.push_back( traits_type::to_char_type( c ) );
				return traits_type::not_eof( c );
			} else {
				return traits_type::eof();
			}
		}
		virtual int_type underflow() {
			return traits_type::to_int_type( vec[current] );
		}
		virtual int_type uflow() {
			int_type c = traits_type::to_int_type( vec[current++] );
			setg( vec.begin(), &vec[current], vec.end() );
			return c;
		}
		virtual pos_type seekoff( off_type off, ios_base::seekdir way, ios_base::openmode which = ios_base::in | ios_base::out) {
			switch ( way ) {
				case ios_base::beg: current = off; break;
				case ios_base::cur: current += off; break;
				case ios_base::end: current = vec.size() - off; break;
			}
			return current;
		}
		virtual pos_type seekpos( pos_type sp, ios_base::openmode which = ios_base::in | ios_base::out ) {
			current = sp;
			return current;
		}

	public:
		binarybuf(): streambuf(), current( 0 ) {
			setg( vec.begin(), vec.begin(), vec.end() );
			setp( vec.begin(), vec.end() );
		}
		virtual ~binarybuf() { vec.clear(); };
	};
	binarybuf buf;

public:
	binarystream(): iostream( &buf ) {};
	vector< char >& vec()    { return buf.vec;     };
	const char* date() const { return &buf.vec[0]; };
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioModel
// ----------------------------------------------------------------------------
class RDOStudioModelDoc;
namespace rdoEditor {
	class RDOEditorTabCtrl;
}

class RDOStudioModel
{
friend class RDOStudioModelDoc;
friend class RDOStudioMainFrame;
friend class RDOStudioFrameView;
friend class RDOStudioFrameTreeCtrl;

private:
	CMultiDocTemplate* modelDocTemplate;
	RDOStudioFrameManager frameManager;

	bool useTemplate;
	bool closeWithDocDelete;
	bool showCanNotCloseModelMessage;

	bool openError;

	bool frmDescribed;
	double modelTime;
	RDOSimulatorNS::ShowMode showMode;

	static void newModelNotify();
	static void openModelNotify();
	static void saveModelNotify();
	static bool canCloseModelNotify();
	static void closeModelNotify();
	static void canNotCloseModelNotify();

	static void beforeModelStartNotify();
	static void afterModelStartNotify();
	static void stopModelNotify();
	static void parseSuccessNotify();
	static void parseErrorNotify();
	static void executeErrorNotify();
	static void showFrameNotify();

	static void buildNotify( std::string str );
	static void debugNotify( std::string str );

	void newModelFromRepository();
	void openModelFromRepository();
	void saveModelToRepository();
	void closeModelFromRepository();
	void canNotCloseModelFromRepository() const;

	void beforeModelStart();
	void showFrame();

public:
	RDOStudioModel();
	~RDOStudioModel();

	void newModel( const bool _useTemplate = false );
	bool openModel( const std::string& modelName = "" ) const;
	bool saveModel() const;
	void saveAsModel() const;
	bool canCloseModel();
	void closeModel() const;

	void buildModel() const;
	void runModel() const;
	void stopModel() const;

	std::string getName() const;
	void setName( const std::string& str );

	bool isModify() const;
	bool isRunning() const;
	double getModelTime() const { return modelTime; };

	bool isFrmDescribed() const                  { return frmDescribed; };
	RDOSimulatorNS::ShowMode getShowMode() const { return showMode;     };
	void setShowMode( const RDOSimulatorNS::ShowMode value );
	double getShowRate() const;
	void setShowRate( const double value ) const;

	RDOStudioModelDoc* getModelDoc() const;
	void updateModify() const;

	rdoEditor::RDOEditorTabCtrl* getTab() const;

	void updateStyleOfAllModel() const;
};

// ----------------------------------------------------------------------------
extern RDOStudioModel* model;

#endif // RDOSTUDIOMODEL_H
