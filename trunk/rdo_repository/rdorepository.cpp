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

string RDORepository::getName() const
{
	return repositoryFile->getName();
}

string RDORepository::getFullName() const
{
	return repositoryFile->getFullName();
}

void RDORepository::newModel()
{
	repositoryFile->newModel();
}

bool RDORepository::openModel( const string& modelName )
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

bool RDORepository::isReadOnly() const
{
	return repositoryFile->isReadOnly();
}

void RDORepository::loadPAT( strstream& stream ) const
{
	repositoryFile->loadPAT( stream );
}

void RDORepository::loadRTP( strstream& stream ) const
{
	repositoryFile->loadRTP( stream );
}

void RDORepository::loadRSS( strstream& stream ) const
{
	repositoryFile->loadRSS( stream );
}

void RDORepository::loadOPR( strstream& stream ) const
{
	repositoryFile->loadOPR( stream );
}

void RDORepository::loadFRM( strstream& stream ) const
{
	repositoryFile->loadFRM( stream );
}

void RDORepository::loadFUN( strstream& stream ) const
{
	repositoryFile->loadFUN( stream );
}

void RDORepository::loadDPT( strstream& stream ) const
{
	repositoryFile->loadDPT( stream );
}

void RDORepository::loadSMR( strstream& stream ) const
{
	repositoryFile->loadSMR( stream );
}

void RDORepository::loadPMD( strstream& stream ) const
{
	repositoryFile->loadPMD( stream );
}

void RDORepository::loadPMV( strstream& stream ) const
{
	repositoryFile->loadPMV( stream );
}

void RDORepository::loadTRC( strstream& stream ) const
{
	repositoryFile->loadTRC( stream );
}

void RDORepository::savePAT( strstream& stream ) const
{
	repositoryFile->savePAT( stream );
}

void RDORepository::saveRTP( strstream& stream ) const
{
	repositoryFile->saveRTP( stream );
}

void RDORepository::saveRSS( strstream& stream ) const
{
	repositoryFile->saveRSS( stream );
}

void RDORepository::saveOPR( strstream& stream ) const
{
	repositoryFile->saveOPR( stream );
}

void RDORepository::saveFRM( strstream& stream ) const
{
	repositoryFile->saveFRM( stream );
}

void RDORepository::saveFUN( strstream& stream ) const
{
	repositoryFile->saveFUN( stream );
}

void RDORepository::saveDPT( strstream& stream ) const
{
	repositoryFile->saveDPT( stream );
}

void RDORepository::saveSMR( strstream& stream ) const
{
	repositoryFile->saveSMR( stream );
}

void RDORepository::savePMD( strstream& stream ) const
{
	repositoryFile->savePMD( stream );
}

void RDORepository::savePMV( strstream& stream ) const
{
	repositoryFile->savePMV( stream );
}

void RDORepository::saveTRC( strstream& stream ) const
{
	repositoryFile->saveTRC( stream );
}
