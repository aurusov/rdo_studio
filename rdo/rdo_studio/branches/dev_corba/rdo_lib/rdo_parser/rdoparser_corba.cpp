#include "pch.h"
#include "rdoparser_corba.h"
#include "rdortp.h"
#include <rdo_resources.h>
#include <rdoruntime_object.h>
#include "RDOCorba.hh"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef CORBA_ENABLE

#define file1 "C:\\RTP.txt"
#define file2 "C:\\RSS.txt"

namespace rdoParse
{

static CORBA::Object_ptr getObjectReference(CORBA::ORB_ptr orb)
{
	CosNaming::NamingContext_var rootContext;

	try {
		
		// Obtain a reference to the root context of the Name service:
		CORBA::Object_var obj;

		obj = orb->resolve_initial_references("NameService");
		
		// Narrow the reference returned.
		rootContext = CosNaming::NamingContext::_narrow(obj);


		if( CORBA::is_nil(rootContext) ) {
			cerr << "Failed to narrow the root naming context." << endl;
			return CORBA::Object::_nil();
		}
	}
	catch (CORBA::NO_RESOURCES&) {
		cerr << "Caught NO_RESOURCES exception. You must configure omniORB "
		<< "with the location" << endl
		<< "of the naming service." << endl;
		return 0;
	}
	catch(CORBA::ORB::InvalidName& ex) {
		// This should not happen!
		cerr << "Service required is invalid [does not exist]." << endl;
		return CORBA::Object::_nil();
	}
	
	// Create a name object, containing the name test/context:
	CosNaming::Name name;
	name.length(2);
	name[0].id = (const char*) "test"; // string copied
	name[0].kind = (const char*) "my_context"; // string copied
	name[1].id = (const char*) "RDO1";
	name[1].kind = (const char*) "Object";
	
	// Note on kind: The kind field is used to indicate the type
	// of the object. This is to avoid conventions such as that used
	// by files (name.type -- e.g. test.ps = postscript etc.)
	try {
		
		// Resolve the name to an object reference.
		return rootContext->resolve(name);
	}
	catch(CosNaming::NamingContext::NotFound& ex) {
		// This exception is thrown if any of the components of the
		// path [contexts or the object] aren’t found:
		cerr << "Context not found." << endl;
	}
	catch(CORBA::TRANSIENT& ex) {
		cerr << "Caught system exception TRANSIENT -- unable to contact the "
		<< "naming service." << endl
		<< "Make sure the naming server is running and that omniORB is "
		<< "configured correctly." << endl;
	}
	catch(CORBA::SystemException& ex) {
		cerr << "Caught a CORBA::" << ex._name()
		<< " while using the naming service." << endl;
		return 0;
	}

	return CORBA::Object::_nil();
}

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRTP
// ----------------------------------------------------------------------------
void RDOParserCorbaRTP::parse()
{
	// Тут надо запросить все типы ресурсов у парного РДО,
	// вызвав с помощью корбы некий метод, который вернёт кучу структур
	// с описанием RTP и насоздавать этих типов

	try {
		
		int argc = 0;
		//char *argv[];

		CORBA::ORB_var orb = CORBA::ORB_init(argc, NULL);
		CORBA::Object_var obj = getObjectReference(orb);

		rdoParse::RDOCorba_var rdocorbaref = rdoParse::RDOCorba::_narrow(obj);


		CORBA::Long rtp_count = 0;
		rdoParse::RDOCorba::GetRTP_var tmp_rtp = rdocorbaref->getRDORTPlist( rtp_count );
		rdoParse::RDOCorba::GetRTP_var my_rtpList ( tmp_rtp );

		FILE *f1;
		f1=fopen(file1,"w");
		for (CORBA::Long i = 0; i < my_rtpList->length(); i++)
		{
			fprintf(f1,"\nИнформация о типе ресурса №%d:\n", i+1);
			fprintf(f1,"   Имя типа ресурса №%d: %s \n", i+1, my_rtpList[i].m_name);
						
		
			if (my_rtpList[i].m_type==rdoParse::RDOCorba::TypeRTP::rt_permanent)
				fprintf(f1,"   Вид типа ресурса: tr_permanent\n");
			else
				fprintf(f1,"   Вид типа ресурса: tr_temporary\n");

				
			for (CORBA::Long j = 0; j < my_rtpList[i].m_param_count; j++)
			{
				fprintf(f1,"\n   Информация о параметре №%d:\n", j+1);
				fprintf(f1,"      Имя параметра:  %s \n", my_rtpList[i].m_param[j].m_name );
					
				switch (my_rtpList[i].m_param[j].m_type )
				{
				case rdoParse::RDOCorba::TypeParam::int_type:{
						fprintf(f1,"      Тип параметра: integer\n");

						if ( my_rtpList[i].m_param[j].m_diap_int == 1 )
						{
							fprintf(f1,"      Минимальное значение: %d \n", my_rtpList[i].m_param[j].m_min_int );
							fprintf(f1,"      Максимальное значение: %d \n", my_rtpList[i].m_param[j].m_max_int );
						}
						if ( my_rtpList[i].m_param[j].m_default_int_ch == 1 )
						{
							fprintf(f1,"      Значение по умолчанию: %d \n", my_rtpList[i].m_param[j].m_default_int );
						}
						break;
					}
					case rdoParse::RDOCorba::TypeParam::double_type:{
						fprintf(f1,"      Тип параметра: real\n");

						if ( my_rtpList[i].m_param[j].m_diap_double == 1 )
						{
							fprintf(f1,"      Минимальное значение: %f \n", my_rtpList[i].m_param[j].m_min_double );
							fprintf(f1,"      Максимальное значение: %f \n", my_rtpList[i].m_param[j].m_max_double );
						}
						if ( my_rtpList[i].m_param[j].m_default_double_ch == 1 )
						{
							fprintf(f1,"      Значение по умолчанию: %f \n", my_rtpList[i].m_param[j].m_default_double );
						}
						break;
					}
					case rdoParse::RDOCorba::TypeParam::enum_type:{
						fprintf(f1,"      Тип параметра: enum\n");
						fprintf(f1,"      Значения параметра:");
											
						CORBA::Long k = 0;
				
						while ( k!= my_rtpList[i].m_param[j].m_var_enum_count )
						{
							fprintf(f1,"\t%s", my_rtpList[i].m_param[j].m_var_enum[k].pd_data );

							//***************************************
							//fprintf(f1,"\t 12345");
							k++;
						}
						
						fprintf(f1,"\n");
						
						if ( my_rtpList[i].m_param[j].m_default_enum_ch == 1 )
						{
							fprintf(f1,"\n      Значение по умолчанию: %s \n", my_rtpList[i].m_param[j].m_default_enum );
						}
						break;
					}
					default: break;
				}
			}
		}
				
		fclose(f1);


	//*************************************************************
		CORBA::Long rss_count = 0;
		
		rdoParse::RDOCorba::GetRSS_var tmp_rss = rdocorbaref->getRDORSSPlist( rss_count );
		
		rdoParse::RDOCorba::GetRSS_var my_rssList ( tmp_rss );

		FILE *f2;
		f2=fopen(file2,"w");
		
		for (CORBA::Long i = 0; i < my_rssList->length(); i++)
		{
			
			fprintf(f2,"\nИнформация о ресурсе №%d:\n\n", i+1);
			fprintf(f2,"Имя ресурса: %s/ Тип ресурса: %s\n", my_rssList[i].m_name, my_rssList[i].m_type);
			
			for (CORBA::Long j = 0; j < my_rssList[i].m_param_count; j++)
			{
				switch (my_rssList[i].m_param[j].m_type)
				{
				case rdoParse::RDOCorba::TypeParam::int_type:{
				
					fprintf(f2,"  Значение параметра: %s = %d\n", my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_int);
					
					break;
					}
				case rdoParse::RDOCorba::TypeParam::double_type:{

					fprintf(f2,"  Значение параметра: %s = %f\n", my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_double);

					break;
					}
				case rdoParse::RDOCorba::TypeParam::enum_type:{

					fprintf(f2,"  Значение параметра: %s = %s\n", my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_enum);

					break;
					}
				default: break;
				}
			}
		}
		
		fclose(f2);

	//*************************************************************

		orb->destroy();
		
	}	
	catch(CORBA::TRANSIENT&) {
		cerr << "Caught system exception TRANSIENT -- unable to contact the "
		<< "server." << endl;
	}
	catch(CORBA::SystemException& ex) {
		cerr << "Caught a CORBA::" << ex._name() << endl;
	}
	catch(CORBA::Exception& ex) {
		cerr << "Caught CORBA::Exception: " << ex._name() << endl;
	}
	catch(omniORB::fatalException& fe) {
		cerr << "Caught omniORB::fatalException:" << endl;
		cerr << " file: " << fe.file() << endl;
		cerr << " line: " << fe.line() << endl;
		cerr << " mesg: " << fe.errmsg() << endl;
	}
	
	
	
/*

	//Создаем и добавляем копии собранных типов ресурсов
	
	// Получили список всех описанных типов ресурсов
	rdoMBuilder::RDOResTypeList rtpList( m_parser );
	
	for (CORBA::Long i = 0; i < rtp_count; i++)
	{

		// Создаем новый тип ресурса
		rdoMBuilder::RDOResType rtp( my_rtpList[i].m_name.in() );
		// Наполняем его параметрами

		for (CORBA::Long j = 0; j < params_count[i]; j++)
		{
			switch (my_rtpList[i].m_param[j].m_type ){
				case rdoParse::RDOCorba::TypeParam::int_type:{
			
					unsigned int fl = 0;

					if ( my_rtpList[i].m_param[j].m_diap_int == 1 &&  my_rtpList[i].m_param[j].m_default_int_ch == 1) 
						fl = 1;
					else
					{
						if ( my_rtpList[i].m_param[j].m_diap_int == 1 )		
							fl = 2;
						else	
						{
							if ( my_rtpList[i].m_param[j].m_default_int_ch = 1 )
								fl = 3;
						}
					}

					switch (fl){
						case 0:{//Нет ни диапазона, ни значения по умолчанию
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_int) );
							break;
						}
   						case 1:{//Есть и диапазон, и значение по умолчанию
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , my_rtpList[i].m_param[j].m_min_int , my_rtpList[i].m_param[j].m_max_int , my_rtpList[i].m_param[j].m_default_int ) );
							break;
						}
						case 2:{//Есть только дипапазон значений
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , my_rtpList[i].m_param[j].m_min_int , my_rtpList[i].m_param[j].m_max_int ) );
							break;
						}
   						case 3:{//Есть толко значение по умолчанию
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_int , my_rtpList[i].m_param[j].m_default_int ) );
							break;
						}
						default: break;
					}
					break;
				}
				case rdoParse::RDOCorba::TypeParam::double_type:{
			
					unsigned int fl = 0;

					if ( my_rtpList[i].m_param[j].m_diap_double == 1 &&  my_rtpList[i].m_param[j].m_default_double_ch == 1) 
						fl = 1;
					else
					{
						if ( my_rtpList[i].m_param[j].m_diap_double == 1 )		
							fl = 2;
						else	
						{
							if ( my_rtpList[i].m_param[j].m_default_double_ch = 1 )
								fl = 3;
						}
					}

					switch (fl){
						case 0:{//Нет ни диапазона, ни значения по умолчанию
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_real) );
							break;
						}
   						case 1:{//Есть и диапазон, и значение по умолчанию
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , my_rtpList[i].m_param[j].m_min_double , my_rtpList[i].m_param[j].m_max_double , my_rtpList[i].m_param[j].m_default_double ) );
							break;
						}
						case 2:{//Есть только дипапазон значений
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , my_rtpList[i].m_param[j].m_min_double , my_rtpList[i].m_param[j].m_max_double ) );
							break;
						}
   						case 3:{//Есть толко значение по умолчанию
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_real , my_rtpList[i].m_param[j].m_default_double ) );
							break;
						}
						default: break;
					}
					break;
				}
				case rdoParse::RDOCorba::TypeParam::enum_type:{
		

					//Пока не заморачиваюсь

					break;
				}
				default: break;
			}
		
		}

		//Добавляем к существующим
		if ( rtpList.append( rtp ) )
		{
			// Добавили успешно
		}
		else
		{
			// Неудача, возможно, тип с таким именем уже есть
			//parser->error( info, rdo::format("Ошибка создания типа ресурса: %s", rtp_name.c_str()) );
	
		}
	}

*/

/*



	// Количество полученных типов ресурсов
	unsigned int rtp_count = 1;

	// Получили список всех типов ресурсов
	rdoMBuilder::RDOResTypeList rtpList( m_parser );

	for ( unsigned int i = 0; i < rtp_count; i++ )
	{
		// Создали новый тип ресурса
		rdoMBuilder::RDOResType rtp( "MyRTP" );
		// Наполнили его параметрами
		rtp.m_params.append( rdoMBuilder::RDOResType::Param("p1", rdoRuntime::g_int) );
		rtp.m_params.append( rdoMBuilder::RDOResType::Param("p2", rdoRuntime::g_int) );
		// Добавили его к списку существующих
		if ( rtpList.append( rtp ) )
		{
			// Добавили успешно
			unsigned int fl = 0;

		}
		else
		{
			// Неудача, возможно, тип с таким именем уже есть
		}
	}

*/

}

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRSS
// ----------------------------------------------------------------------------
void RDOParserCorbaRSS::parse()
{
	// Тут надо запросить все ресурсы у парного РДО

	// Количество полученных ресурсов
	unsigned int rss_count = 1;

	// Получили список всех типов ресурсов
	rdoMBuilder::RDOResTypeList rtpList( m_parser );
	// Получили список всех ресурсов
	rdoMBuilder::RDOResourceList rssList( m_parser );

	for ( unsigned int i = 0; i < rss_count; i++ )
	{
		// Создали новый ресурс
		rdoMBuilder::RDOResource rss( rtpList["Парикмахеры"], "MyRSS1" );
		// Заполнили его параметры
		rss["длительность_max"] = 174;
		// Добавляем его к списку существующих
		if ( rssList.append<rdoParse::RDORSSResource>( rss ) )
		{
			// Добавили успешно
		}
		else
		{
			// Неудача
		}
		// Пример проверки ресурса на существование
		if ( rssList.exist("qqq") )
		{
		}
	}
}

} // namespace rdoParse

#endif // CORBA_ENABLE