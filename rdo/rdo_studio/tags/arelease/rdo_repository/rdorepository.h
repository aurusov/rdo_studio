#ifndef RDOREPOSITORY_H
#define RDOREPOSITORY_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <strstream>

using namespace std;

namespace rdoRepository {
	class RDORepositoryFile;
};

// ----------------------------------------------------------------------------
// ---------- RDORepository
// ----------------------------------------------------------------------------
class RDORepository
{
private:
	rdoRepository::RDORepositoryFile* repositoryFile;

public:
	RDORepository();
	virtual ~RDORepository();

	string getName() const;
	string getFullName() const;

	void newModel();
	bool openModel( const string& modelName = "" );
	void saveModel();
	void saveAsModel();
	void closeModel();

	bool isReadOnly() const;

	void loadPAT( strstream& stream ) const;
	void loadRTP( strstream& stream ) const;
	void loadRSS( strstream& stream ) const;
	void loadOPR( strstream& stream ) const;
	void loadFRM( strstream& stream ) const;
	void loadFUN( strstream& stream ) const;
	void loadDPT( strstream& stream ) const;
	void loadSMR( strstream& stream ) const;
	void loadPMD( strstream& stream ) const;
	void loadPMV( strstream& stream ) const;
	void loadTRC( strstream& stream ) const;

	void savePAT( strstream& stream ) const;
	void saveRTP( strstream& stream ) const;
	void saveRSS( strstream& stream ) const;
	void saveOPR( strstream& stream ) const;
	void saveFRM( strstream& stream ) const;
	void saveFUN( strstream& stream ) const;
	void saveDPT( strstream& stream ) const;
	void saveSMR( strstream& stream ) const;
	void savePMD( strstream& stream ) const;
	void savePMV( strstream& stream ) const;
	void saveTRC( strstream& stream ) const;
};

#endif // RDOREPOSITORY_H
