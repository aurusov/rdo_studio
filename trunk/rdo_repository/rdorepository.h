#ifndef RDOREPOSITORY_H
#define RDOREPOSITORY_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <sstream>
#include <vector>

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

	void loadPAT( std::stringstream& stream ) const;
	void loadRTP( std::stringstream& stream ) const;
	void loadRSS( std::stringstream& stream ) const;
	void loadOPR( std::stringstream& stream ) const;
	void loadFRM( std::stringstream& stream ) const;
	void loadFUN( std::stringstream& stream ) const;
	void loadDPT( std::stringstream& stream ) const;
	void loadSMR( std::stringstream& stream ) const;
	void loadPMD( std::stringstream& stream ) const;
	void loadPMV( std::stringstream& stream ) const;
	void loadTRC( std::stringstream& stream ) const;

	void savePAT( std::stringstream& stream ) const;
	void saveRTP( std::stringstream& stream ) const;
	void saveRSS( std::stringstream& stream ) const;
	void saveOPR( std::stringstream& stream ) const;
	void saveFRM( std::stringstream& stream ) const;
	void saveFUN( std::stringstream& stream ) const;
	void saveDPT( std::stringstream& stream ) const;
	void saveSMR( std::stringstream& stream ) const;
	void savePMD( std::stringstream& stream ) const;
	void savePMV( std::stringstream& stream ) const;
	void saveTRC( std::stringstream& stream ) const;

	void loadBMP( const std::string& name, std::iostream& stream ) const;

	void loadPAT( std::vector< char >& vec ) const;
	void loadRTP( std::vector< char >& vec ) const;
	void loadRSS( std::vector< char >& vec ) const;
	void loadOPR( std::vector< char >& vec ) const;
	void loadFRM( std::vector< char >& vec ) const;
	void loadFUN( std::vector< char >& vec ) const;
	void loadDPT( std::vector< char >& vec ) const;
	void loadSMR( std::vector< char >& vec ) const;
	void loadPMD( std::vector< char >& vec ) const;
	void loadPMV( std::vector< char >& vec ) const;
	void loadTRC( std::vector< char >& vec ) const;

	void loadBMP( const std::string& name, std::vector< char >& vec ) const;
};

} // namespace rdoRepository

#endif // RDOREPOSITORY_H
