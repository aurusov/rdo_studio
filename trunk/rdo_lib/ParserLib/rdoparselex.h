#ifndef RDOPARSE_LEX
#define RDOPARSE_LEX

namespace rdoParse 
{

int rtpparse(void);
void rtperror( char *mes );
int rtplex(void);
extern int rtplval;

int rssparse(void);
void rsserror( char *mes );
int rsslex(void);
extern int rsslval;

int funparse(void);
void funerror( char *mes );
int funlex(void);
extern int funlval;

int patparse(void);
void paterror( char *mes );
int patlex(void);
extern int patlval;

int oprparse(void);
void oprerror( char *mes );
int oprlex(void);
extern int oprlval;

int dptparse(void);
void dpterror( char *mes );
int dptlex(void);
extern int dptlval;

int pmdparse(void);
void pmderror( char *mes );
int pmdlex(void);
extern int pmdlval;

int frmparse(void);
void frmerror( char *mes );
int frmlex(void);
extern int frmlval;

int smr1parse(void);
void smr1error( char *mes );
int smr1lex(void);
extern int smr1lval;

int smr2parse(void);
void smr2error( char *mes );
int smr2lex(void);
extern int smr2lval;

}

#endif //RDOPARSE_LEX

