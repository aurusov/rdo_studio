#ifndef RDOREPOSITORY_H
#define RDOREPOSITORY_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <rdobinarystream.h>

// ----------------------------------------------------------------------------
// ---------- RDORepository
// ----------------------------------------------------------------------------
namespace rdoRepository {

class RDORepositoryFile;

class RDORepository
{
private:
	rdoRepository::RDORepositoryFile* repositoryFile;

public:
	RDORepository();
	virtual ~RDORepository();

	std::string getName() const;
	std::string getFullName() const;

	void newModel();
	bool openModel( const std::string& modelName = "" );
	bool saveModel();
	void saveAsModel();
	void closeModel();

	bool isReadOnly() const;

	void loadPAT( rdo::binarystream& stream ) const;
	void loadRTP( rdo::binarystream& stream ) const;
	void loadRSS( rdo::binarystream& stream ) const;
	void loadOPR( rdo::binarystream& stream ) const;
	void loadFRM( rdo::binarystream& stream ) const;
	void loadFUN( rdo::binarystream& stream ) const;
	void loadDPT( rdo::binarystream& stream ) const;
	void loadSMR( rdo::binarystream& stream ) const;
	void loadPMD( rdo::binarystream& stream ) const;
	void loadPMV( rdo::binarystream& stream ) const;
	void loadTRC( rdo::binarystream& stream ) const;

	void savePAT( rdo::binarystream& stream ) const;
	void saveRTP( rdo::binarystream& stream ) const;
	void saveRSS( rdo::binarystream& stream ) const;
	void saveOPR( rdo::binarystream& stream ) const;
	void saveFRM( rdo::binarystream& stream ) const;
	void saveFUN( rdo::binarystream& stream ) const;
	void saveDPT( rdo::binarystream& stream ) const;
	void saveSMR( rdo::binarystream& stream ) const;
	void savePMD( rdo::binarystream& stream ) const;
	void savePMV( rdo::binarystream& stream ) const;
	void saveTRC( rdo::binarystream& stream ) const;

	void loadBMP( const std::string& name, rdo::binarystream& stream ) const;
};

} // namespace rdoRepository

#endif // RDOREPOSITORY_H
