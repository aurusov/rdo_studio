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

void RDORepository::loadPAT( rdo::binarystream& stream ) const
{
	repositoryFile->loadPAT( stream );
}

void RDORepository::loadRTP( rdo::binarystream& stream ) const
{
	repositoryFile->loadRTP( stream );
}

void RDORepository::loadRSS( rdo::binarystream& stream ) const
{
	repositoryFile->loadRSS( stream );
}

void RDORepository::loadOPR( rdo::binarystream& stream ) const
{
	repositoryFile->loadOPR( stream );
}

void RDORepository::loadFRM( rdo::binarystream& stream ) const
{
	repositoryFile->loadFRM( stream );
}

void RDORepository::loadFUN( rdo::binarystream& stream ) const
{
	repositoryFile->loadFUN( stream );
}

void RDORepository::loadDPT( rdo::binarystream& stream ) const
{
	repositoryFile->loadDPT( stream );
}

void RDORepository::loadSMR( rdo::binarystream& stream ) const
{
	repositoryFile->loadSMR( stream );
}

void RDORepository::loadPMD( rdo::binarystream& stream ) const
{
	repositoryFile->loadPMD( stream );
}

void RDORepository::loadPMV( rdo::binarystream& stream ) const
{
	repositoryFile->loadPMV( stream );
}

void RDORepository::loadTRC( rdo::binarystream& stream ) const
{
	repositoryFile->loadTRC( stream );
}

void RDORepository::savePAT( rdo::binarystream& stream ) const
{
	repositoryFile->savePAT( stream );
}

void RDORepository::saveRTP( rdo::binarystream& stream ) const
{
	repositoryFile->saveRTP( stream );
}

void RDORepository::saveRSS( rdo::binarystream& stream ) const
{
	repositoryFile->saveRSS( stream );
}

void RDORepository::saveOPR( rdo::binarystream& stream ) const
{
	repositoryFile->saveOPR( stream );
}

void RDORepository::saveFRM( rdo::binarystream& stream ) const
{
	repositoryFile->saveFRM( stream );
}

void RDORepository::saveFUN( rdo::binarystream& stream ) const
{
	repositoryFile->saveFUN( stream );
}

void RDORepository::saveDPT( rdo::binarystream& stream ) const
{
	repositoryFile->saveDPT( stream );
}

void RDORepository::saveSMR( rdo::binarystream& stream ) const
{
	repositoryFile->saveSMR( stream );
}

void RDORepository::savePMD( rdo::binarystream& stream ) const
{
	repositoryFile->savePMD( stream );
}

void RDORepository::savePMV( rdo::binarystream& stream ) const
{
	repositoryFile->savePMV( stream );
}

void RDORepository::saveTRC( rdo::binarystream& stream ) const
{
	repositoryFile->saveTRC( stream );
}

void RDORepository::loadBMP( const string& name, rdo::binarystream& stream ) const
{
	repositoryFile->loadBMP( name, stream );
}
