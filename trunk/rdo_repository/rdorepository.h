#ifndef RDOREPOSITORY_H
#define RDOREPOSITORY_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <sstream>

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
	bool saveModel();
	void saveAsModel();
	void closeModel();

	bool isReadOnly() const;

	void loadPAT( stringstream& stream ) const;
	void loadRTP( stringstream& stream ) const;
	void loadRSS( stringstream& stream ) const;
	void loadOPR( stringstream& stream ) const;
	void loadFRM( stringstream& stream ) const;
	void loadFUN( stringstream& stream ) const;
	void loadDPT( stringstream& stream ) const;
	void loadSMR( stringstream& stream ) const;
	void loadPMD( stringstream& stream ) const;
	void loadPMV( stringstream& stream ) const;
	void loadTRC( stringstream& stream ) const;

	void savePAT( stringstream& stream ) const;
	void saveRTP( stringstream& stream ) const;
	void saveRSS( stringstream& stream ) const;
	void saveOPR( stringstream& stream ) const;
	void saveFRM( stringstream& stream ) const;
	void saveFUN( stringstream& stream ) const;
	void saveDPT( stringstream& stream ) const;
	void saveSMR( stringstream& stream ) const;
	void savePMD( stringstream& stream ) const;
	void savePMV( stringstream& stream ) const;
	void saveTRC( stringstream& stream ) const;
};

#endif // RDOREPOSITORY_H
