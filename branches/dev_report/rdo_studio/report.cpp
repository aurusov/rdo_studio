#include "stdafx.h"
#include "report.h"

using namespace std ;

// Ключевые слова
const std::string KeyWords[] = 
{
	"//#groop", 
    "//#endgroop",
    "watch_state",      // шесть показателей - после переменной
    "watch_par",        // шесть показателей - после переменной
    "get_value",		// один показатель - после переменной
	"watch_quant",      // шесть показателей - после переменной
    "watch_value"		// шесть показателей - после переменной
} ;

int StringToList(std::string Str, vector <std::string> *List)
{
    int i = 0 ;
	std::string ListBuffer = "" ;
	while(Str[i] != '\0')
	{
		// Пропускаем пробелы, переносы строки и табуляцию
		while(Str[i] == ' ' || Str[i] == '\n' || Str[i] == 9 || Str[i] == 13)
		  i++ ;
		// Собираем слово
		while(Str[i] != ' ' && Str[i] != '\n' && Str[i] != '\0' && Str[i] != ':' && Str[i] != 9 && Str[i] != 13)
	      ListBuffer += Str[i++] ;
		// Добавляем непустое слово в список
		if (ListBuffer != "")
			List -> push_back(ListBuffer) ;
		// Очищаем буфер
		ListBuffer = "" ;
		// Если встретили двоеточие - выделить его в отдельное слово
		if (Str[i] == ':')
		{
			List -> push_back(":") ;
			i++ ;
		}
	}
	return 0 ;
}


int DeleteComments(std::string Source, std::string *Result)
{
    int ParseIndex = 0 ;	
	int ParseFlag = NOCOMMENT ;
	while (ParseIndex < Source.length())
	{
       switch(Source[ParseIndex])
	   {
	  	  case '/' : {
			  if (Source[ParseIndex + 1] == '/' && Source[ParseIndex+2] != '#' && ParseFlag == NOCOMMENT)
	  				   ParseFlag = LINECOMMENT ;
	                 if (Source[ParseIndex+1] == '*' && ParseFlag == NOCOMMENT)
					   ParseFlag = LONGCOMMENT ;
	                 break ;
	                 } 
		  case '*' : if (Source[ParseIndex+1] == '/' && ParseFlag == LONGCOMMENT)
					 {
					   ParseFlag = ENDLONGCOMMENT ;
					   ParseIndex++;
					 }
			break ;

		  case '\n': if (ParseFlag == LINECOMMENT)
					   ParseFlag = NOCOMMENT ; 
			break ;
	    }
	  
	   // Записываем новую букву
      if (ParseFlag == NOCOMMENT)
        *Result  += Source[ParseIndex] ; 
	  
	  if (ParseFlag == ENDLONGCOMMENT)
        ParseFlag = NOCOMMENT ;
	 
	  ParseIndex++ ;   
	}
	
	*Result += '\0' ;
	return 0 ;
}


int AddGroop(vector <Groop> *Groops, std::string GroopName_, bool ActivityFlag)
{
  //vector <Groop>::iterator Groop_Iter ;
  Groop Buffer ;
  
  int i = 0;
  bool flag = 1 ;
  for (i = 0; i < Groops->size() ; i++)
  {
    
	Buffer = Groops -> at(i) ;
    // Если группа найдена - устанавливаем ActivityFlag
	if (Buffer.GroopName  == GroopName_ ) 
	{
	    Groops -> at(i).IsActive = ActivityFlag ;
		flag = 0 ;
	}	
  }

  // Если группа не найдена - добавляем в конец
  if (flag)
  {
    Buffer.GroopName = GroopName_ ;
    Buffer.Variables.clear() ;
	
	Buffer.IsActive = ActivityFlag ;
    Groops->push_back(Buffer) ;
  }
 
  return 0 ;
}

int InsertVar(vector <Groop> *Groops, std::string VarName, ReportVar::Type type)
{
  int i = 0 ;
  ReportVar Buffer ;
  Buffer.Name   = VarName ;
  Buffer.m_type = type;
  for (i = 0; i < Groops->size(); i++)
  {
    if (Groops -> at(i).IsActive)
	  Groops -> at(i).Variables.push_back(Buffer) ;
  }

  return 0 ;
}


int FindKeyWord(vector <std::string> List, int StartIndex, int& WhatWord)
{
  int i, j;
/*
  std::map<int, ReportVar::Type> reportKeyWords;
  reportKeyWords[2] = ReportVar::WATCH_STATE;
  reportKeyWords[3] = ReportVar::WATCH_PAR;
  reportKeyWords[4] = ReportVar::GET_VALUE;
  reportKeyWords[5] = ReportVar::WATCH_QUANT;
  reportKeyWords[6] = ReportVar::WATCH_VALUE;
*/
  for (i = StartIndex; i < List.size(); i++)
  {
	for (j = 0; j < KEYWORDS_QUANTITY; j++)
	{
		std::string kw = KeyWords[j];
		if (List.at(i) == kw)
		{
			WhatWord = j; //! TODO: завести таблицу преобразований KEYWORDS_QUANTITY -> ReportVar::Type
			return i + 1;
		}
	}
  }
	WhatWord = UNDEFINED;
	return UNDEFINED;
}

int GetParam(vector <Groop> *Groops, vector <std::string> List)
{
  int i, j, k ;
  ReportVar VarBuf ;
  for (i = 0; i < Groops->size(); i++)
  {
	  for (j = 0; j < Groops->at(i).Variables.size(); j++)
	  {
		  VarBuf = Groops->at(i).Variables.at(j) ;
		  for (k = 0; k < List.size(); k++)
		  {
				if(List.at(k) == VarBuf.Name)
				{
				  switch (VarBuf.m_type)
				  {
				  case 4: Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+1)) ;
						  Groops->at(i).Variables.at(j).Parameters.push_back("") ;
						  Groops->at(i).Variables.at(j).Parameters.push_back("") ;
						  Groops->at(i).Variables.at(j).Parameters.push_back("") ;
						  Groops->at(i).Variables.at(j).Parameters.push_back("") ;
						  Groops->at(i).Variables.at(j).Parameters.push_back("") ;
					  break ;
				  default:  Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+1)) ;
							Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+2)) ;
							Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+3)) ;
							Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+4)) ;
							Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+5)) ;
							Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+6)) ;
					  break ;
				  }
				}
		  }
	  }
  }
  return 0 ;
}



