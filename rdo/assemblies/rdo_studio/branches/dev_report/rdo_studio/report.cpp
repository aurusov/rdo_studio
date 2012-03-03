#include "stdafx.h"
#include "report.h"

using namespace std ;

// Конструктор класса ReportParser
ReportParser::ReportParser( std::string UserString )
{
	StringToParse = UserString;
    ParserStatus = RPS_COMMENTSNOTDELETED;
	CurrentWordIndex = 0;
}
// Функция удаления комментариев
int ReportParser :: DeleteComments() 
{
    int i = 0 ;	
	DeleteCommentsFlagType ParseFlag = DC_NOCOMMENT ;
	while ( i < ( int ) StringToParse.length() )
	{
       switch( StringToParse[i] )
	   {
	  	  case '/':  {
			         if ( StringToParse[i + 1] == '/' && StringToParse[i + 2] != '#' && ParseFlag == DC_NOCOMMENT )
	  				   ParseFlag = DC_LINECOMMENT;
	                 if ( StringToParse[i+1] == '*' && ParseFlag == DC_NOCOMMENT )
					   ParseFlag = DC_LONGCOMMENT;
	                 } 
		             break;
		  case '*':  if ( StringToParse[i+1] == '/' && ParseFlag == DC_LONGCOMMENT )
					 {
					   ParseFlag = DC_ENDLONGCOMMENT ;
					   i++;
					 }
	                 break;
		  case '\n': if ( ParseFlag == DC_LINECOMMENT )
					 ParseFlag = DC_NOCOMMENT ; 
			         break ;
	    }
	  // Записываем новую букву
      if ( ParseFlag == DC_NOCOMMENT )
        StringToParse_wo_Comments  += StringToParse[i]; 
	  if ( ParseFlag == DC_ENDLONGCOMMENT )
        ParseFlag = DC_NOCOMMENT;
	  i++ ;   
	}
	StringToParse_wo_Comments += '\0';
	ParserStatus = RPS_COMMENTSDELETED;
	return 0 ;
}
int ReportParser::StringToList()
{
    int ParseIndex = 0;
	std::string ListBuffer = "";
	// Если комменарии не удалены - то сообщить об ошибке
	if (ParserStatus == RPS_COMMENTSNOTDELETED)
		return 1;
	while( StringToParse_wo_Comments[ParseIndex] != '\0' )
	{
		// Пропускаем пробелы, переносы строки и табуляцию
		while( StringToParse_wo_Comments[ParseIndex] == ' ' || StringToParse_wo_Comments[ParseIndex] == '\n' || StringToParse_wo_Comments[ParseIndex] == 9 || StringToParse_wo_Comments[ParseIndex] == 13 )
			ParseIndex++;
		// Собираем слово
		while( StringToParse_wo_Comments[ParseIndex] != ' ' && StringToParse_wo_Comments[ParseIndex] != '\n' && StringToParse_wo_Comments[ParseIndex] != '\0' && StringToParse_wo_Comments[ParseIndex] != ':' && StringToParse_wo_Comments[ParseIndex] != 9 && StringToParse_wo_Comments[ParseIndex] != 13 )
			ListBuffer += StringToParse_wo_Comments[ParseIndex++];
		// Добавляем непустое слово в список
		if ( ListBuffer != "" )
			ListToParse.push_back( ListBuffer );
		// Очищаем буфер
		ListBuffer = "" ;
		// Если встретили двоеточие - выделить его в отдельное слово
		if ( StringToParse_wo_Comments[ParseIndex] == ':' )
		{
			ListToParse.push_back( ":" );
			ParseIndex++ ;
		}
	}
	ParserStatus = RPS_LISTCREATED;
	return 0;
}

int ReportParser::FindNextKeyWord()
{
  int i, j;
  for ( i = CurrentWordIndex; i < ( int ) ListToParse.size(); i++ )
	for ( j = 0; j < ReportParserKeyWordsQuantity; j++ )
		if ( ListToParse.at(i) == ReportParserKeyWords[j] )
		{
			KeyWordType = ( ReportParser::ReportParserKeyWordType ) j;
			CurrentWordIndex = i + 1;
			return 0; 
		}
  KeyWordType = RPKWT_END ;
  CurrentWordIndex = 0;
  return 1 ;
}

int GroupManager::AddGroup ( std::string GroupName_, bool Activity )
{
Group Buffer ;  
int i = 0;
bool flag = 1;
for ( i = 0; i < ( int ) GroupsVector.size() ; i++ )
{ 
	Buffer = GroupsVector.at( i );
// Если группа найдена - устанавливаем ActivityFlag
	if ( Buffer.GroupName  == GroupName_ ) 
	{
		ActivityPattern.at( i ) = Activity;
		flag = 0 ;
	}	
}
// Если группа  не найдена - добавляем в конец
if (flag)
{
	Buffer.GroupName = GroupName_;
    Buffer.Variables.clear();
	ActivityPattern.push_back( Activity );
	GroupsVector.push_back( Buffer ) ;
} 
return 0 ;
}

int GroupManager::InsertVar( std::string VarName, ReportVar::VarTypeValues VarType_ )
{
int i = 0;
ReportVar Buffer;
Buffer.Name = VarName;
Buffer.VarType = VarType_;
for ( i = 0; i < ( int ) GroupsVector.size(); i++ )
	if ( ActivityPattern.at(i) )
		GroupsVector.at( i ).Variables.push_back( Buffer );
return 0 ;
}

int GroupManager::GetParam()
{
int i, j, k;
ReportVar VarBuf;
// Внешний цикл - проходим группы
for ( i = 0; i < ( int ) GroupsVector.size(); i++ )
{
	// Промежуточный цикл - проходим показатели
	for ( j = 0; j < ( int ) GroupsVector.at( i ).Variables.size(); j++ )
	{
		// Сохраняем в буфер текущий показатель  
		VarBuf = GroupsVector.at( i ).Variables.at( j );
		for ( k = 0; k < ( int ) List.size(); k++ )
		{
			if ( List.at( k ) == VarBuf.Name )
			{
				switch ( VarBuf.VarType )
				{
					// Если показатель имеет тип Get_value то остальные поля заполняем пустыми строками
				case ReportVar::GET_VALUE : GroupsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 1 ) );
						            GroupsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
						            GroupsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
									GroupsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
									GroupsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
									GroupsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
					break ;
					default: GroupsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 1 ) );
							 GroupsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 2 ) );
						     GroupsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 3 ) );
							 GroupsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 4 ) );
							 GroupsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 5 ) );
							 GroupsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 6 ) );
							 
					break ;
			    }
			}
		}
	}
}
return 0;
}

int ChartManager::AddChart ( std::string ChartName_, bool Activity )
{
Chart Buffer ;  
int i = 0;
bool flag = 1;
for ( i = 0; i < ( int ) ChartsVector.size() ; i++ )
{ 
	Buffer = ChartsVector.at( i );
// Если группа найдена - устанавливаем ActivityFlag
	if ( Buffer.ChartName  == ChartName_ ) 
	{
		ActivityPattern.at( i ) = Activity;
		flag = 0 ;
	}	
}
// Если группа  не найдена - добавляем в конец
if (flag)
{
	Buffer.ChartName = ChartName_;
    Buffer.Variables.clear();
	ActivityPattern.push_back( Activity );
	ChartsVector.push_back( Buffer ) ;
} 
return 0 ;
}

int ChartManager::InsertVar( std::string VarName, ReportVar::VarTypeValues VarType_ )
{
int i = 0;
ReportVar Buffer;
Buffer.Name = VarName;
Buffer.VarType = VarType_;
for ( i = 0; i < ( int ) ChartsVector.size(); i++ )
	if ( ActivityPattern.at(i) )
		ChartsVector.at( i ).Variables.push_back( Buffer );
return 0 ;
}

int ChartManager::GetParam()
{
int i, j, k;
ReportVar VarBuf;
// Внешний цикл - проходим группы
for ( i = 0; i < ( int ) ChartsVector.size(); i++ )
{
	// Промежуточный цикл - проходим показатели
	for ( j = 0; j < ( int ) ChartsVector.at( i ).Variables.size(); j++ )
	{
		// Сохраняем в буфер текущий показатель  
		VarBuf = ChartsVector.at( i ).Variables.at( j );
		for ( k = 0; k < ( int ) List.size(); k++ )
		{
			if ( List.at( k ) == VarBuf.Name )
			{
				switch ( VarBuf.VarType )
				{
					// Если показатель имеет тип Get_value то остальные поля заполняем пустыми строками
				case ReportVar::GET_VALUE : ChartsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 1 ) );
						            ChartsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
						            ChartsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
									ChartsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
									ChartsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
									ChartsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
					break ;
					default: ChartsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 1 ) );
							 ChartsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 2 ) );
						     ChartsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 3 ) );
							 ChartsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 4 ) );
							 ChartsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 5 ) );
							 ChartsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 6 ) );
							 
					break ;
			    }
			}
		}
	}
}
return 0;
}

