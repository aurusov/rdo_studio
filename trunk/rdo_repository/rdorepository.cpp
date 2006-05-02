#include "rdorepository.h"

#include <rdorepositoryfile.h>

using namespace rdoRepository;

// ----------------------------------------------------------------------------
// ---------- RDORepository
// ----------------------------------------------------------------------------
RDORepository::RDORepository()
{
	repositoryFile = new RDORepositoryFile;
}

RDORepository::~RDORepository()
{
	if ( repositoryFile ) { delete repositoryFile; repositoryFile = 0L; };
}

std::string RDORepository::getName() const
{
	return repositoryFile->getName();
}

std::string RDORepository::getFullName() const
{
	return repositoryFile->getFullName();
}

void RDORepository::newModel()
{
	repositoryFile->newModel();
}

bool RDORepository::openModel( const std::string& modelName )
{
	return repositoryFile->openModel( modelName );
}

bool RDORepository::saveModel()
{
	return repositoryFile->saveModel();
}

void RDORepository::saveAsModel()
{
	repositoryFile->saveAsModel();
}

void RDORepository::closeModel()
{
	repositoryFile->closeModel();
}

std::string RDORepository::getFileName( rdoModelObjects::RDOFileType type ) const
{
	return repositoryFile->getFileName( type );
}

std::string RDORepository::getExtention( rdoModelObjects::RDOFileType type ) const
{
	return repositoryFile->getExtention( type );
}

std::string RDORepository::getFileExtName( rdoModelObjects::RDOFileType type ) const
{
	return repositoryFile->getFileExtName( type );
}

std::string RDORepository::getFullFileName( rdoModelObjects::RDOFileType type ) const
{
	return repositoryFile->getFullFileName( type );
}

bool RDORepository::isReadOnly( rdoModelObjects::RDOFileType type ) const
{
	return repositoryFile->isReadOnly( type );
}

bool RDORepository::isDescribed( rdoModelObjects::RDOFileType type ) const
{
	return repositoryFile->isDescribed( type );
}

bool RDORepository::isMustExist( rdoModelObjects::RDOFileType type ) const
{
	return repositoryFile->isMustExist( type );
}

void RDORepository::load( rdoModelObjects::RDOFileType type, rdo::binarystream& stream ) const
{
	repositoryFile->load( type, stream );
}

void RDORepository::save( rdoModelObjects::RDOFileType type, rdo::binarystream& stream ) const
{
	repositoryFile->save( type, stream );
}

void RDORepository::loadBMP( const std::string& name, rdo::binarystream& stream ) const
{
	repositoryFile->loadBMP( name, stream );
}
