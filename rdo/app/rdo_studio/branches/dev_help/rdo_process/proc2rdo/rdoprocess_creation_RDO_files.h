#if !defined(AFX_RDOPROCESS_CREATION_RDO_FILES_MJ_H__44D3514B_A298_4BBF_9E21_9A578712FCA8__INCLUDED_)
#define AFX_RDOPROCESS_CREATION_RDO_FILES_MJ_H__44D3514B_A298_4BBF_9E21_9A578712FCA8__INCLUDED_

#include <iostream>
#include <fstream>

class RPCreationRDOFilesMJ  
{
public:
	RPCreationRDOFilesMJ();
	virtual ~RPCreationRDOFilesMJ();
	std::ofstream pattern;
	std::ofstream resourse;
	std::ofstream function;
	std::ofstream typeres;
	std::ofstream operations;
	std::ofstream smr;
	std::ofstream statistic;
};

#endif // !defined(AFX_RDOPROCESS_CREATION_RDO_FILES_MJ_H__44D3514B_A298_4BBF_9E21_9A578712FCA8__INCLUDED_)
