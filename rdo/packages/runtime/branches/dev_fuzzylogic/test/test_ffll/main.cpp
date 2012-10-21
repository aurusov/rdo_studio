
#include <iostream>



#include <FFLLAPI.h>
#include <FFLLBase.h>
#include <FuzzyModelBase.h>
#include <FuzzyVariableBase.h>
#include <FuzzyOutVariable.h>

int main( int argc, char* argv[] )
{


// создаем пустую модель
//
//int model_ent = ffll_new_model(); // создается ModelContainer (empty), model = null;
//// may init by *ffll_load_fcl_file()*
//model_ent.init(); // init the model

FuzzyModelBase model;// = new FuzzyModelBase();
model = FuzzyModelBase();
model.init();

FuzzyModelBase* pmodel = &model;

std::wstring name(L"Steve Nash");
const wchar_t* szName = name.c_str();


FuzzyVariableBase* inputVar1 = new FuzzyVariableBase(pmodel);
//FuzzyVariableBase* inputVar2 = new FuzzyVariableBase(model);
int i = inputVar1->init(szName, 34.5, 45.3, true);//left x value, right x value? 	// if no name is passed in name it "Variable 0"
//int k = inputVar2->init("Variable2", true);
return 0;

}