#ifndef RDOREPOSITORY_H
#define RDOREPOSITORY_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <rdobinarystream.h>
#include <rdocommon.h>

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

	std::string getFileName( rdoModelObjects::RDOFileType type ) const;
	std::string getExtention( rdoModelObjects::RDOFileType type ) const;
	std::string getFileExtName( rdoModelObjects::RDOFileType type ) const;
	std::string getFullFileName( rdoModelObjects::RDOFileType type ) const;
	bool isReadOnly( rdoModelObjects::RDOFileType type ) const;
	bool isDescribed( rdoModelObjects::RDOFileType type ) const;
	bool isMustExist( rdoModelObjects::RDOFileType type ) const;

	void load( rdoModelObjects::RDOFileType type, rdo::binarystream& stream ) const;
	void save( rdoModelObjects::RDOFileType type, rdo::binarystream& stream ) const;

	void loadBMP( const std::string& name, rdo::binarystream& stream ) const;
};

} // namespace rdoRepository

#endif // RDOREPOSITORY_H
