#include "rdorepository.h"

#include <rdorepositoryfile.h>

using namespace std;
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

void RDORepository::loadPAT( stringstream& stream ) const
{
	repositoryFile->loadPAT( stream );
}

void RDORepository::loadRTP( stringstream& stream ) const
{
	repositoryFile->loadRTP( stream );
}

void RDORepository::loadRSS( stringstream& stream ) const
{
	repositoryFile->loadRSS( stream );
}

void RDORepository::loadOPR( stringstream& stream ) const
{
	repositoryFile->loadOPR( stream );
}

void RDORepository::loadFRM( stringstream& stream ) const
{
	repositoryFile->loadFRM( stream );
}

void RDORepository::loadFUN( stringstream& stream ) const
{
	repositoryFile->loadFUN( stream );
}

void RDORepository::loadDPT( stringstream& stream ) const
{
	repositoryFile->loadDPT( stream );
}

void RDORepository::loadSMR( stringstream& stream ) const
{
	repositoryFile->loadSMR( stream );
}

void RDORepository::loadPMD( stringstream& stream ) const
{
	repositoryFile->loadPMD( stream );
}

void RDORepository::loadPMV( stringstream& stream ) const
{
	repositoryFile->loadPMV( stream );
}

void RDORepository::loadTRC( stringstream& stream ) const
{
	repositoryFile->loadTRC( stream );
}

void RDORepository::savePAT( stringstream& stream ) const
{
	repositoryFile->savePAT( stream );
}

void RDORepository::saveRTP( stringstream& stream ) const
{
	repositoryFile->saveRTP( stream );
}

void RDORepository::saveRSS( stringstream& stream ) const
{
	repositoryFile->saveRSS( stream );
}

void RDORepository::saveOPR( stringstream& stream ) const
{
	repositoryFile->saveOPR( stream );
}

void RDORepository::saveFRM( stringstream& stream ) const
{
	repositoryFile->saveFRM( stream );
}

void RDORepository::saveFUN( stringstream& stream ) const
{
	repositoryFile->saveFUN( stream );
}

void RDORepository::saveDPT( stringstream& stream ) const
{
	repositoryFile->saveDPT( stream );
}

void RDORepository::saveSMR( stringstream& stream ) const
{
	repositoryFile->saveSMR( stream );
}

void RDORepository::savePMD( stringstream& stream ) const
{
	repositoryFile->savePMD( stream );
}

void RDORepository::savePMV( stringstream& stream ) const
{
	repositoryFile->savePMV( stream );
}

void RDORepository::saveTRC( stringstream& stream ) const
{
	repositoryFile->saveTRC( stream );
}

void RDORepository::loadBMP( const string& name, stringstream& stream ) const
{
	repositoryFile->loadBMP( name, stream );
}
