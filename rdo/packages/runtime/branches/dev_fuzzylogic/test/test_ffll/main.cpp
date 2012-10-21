

#include <iostream>
//#include <boost/bind.hpp>
//
//#include <boost/test/included/unit_test.hpp>

#include <FFLLAPI.h>
#include <DefuzzVarObj.h>
#include <FFLLBase.h>
#include <FuzzyModelBase.h>
#include <FuzzyVariableBase.h>
#include <FuzzyOutVariable.h>
#include <RuleArray.h>
#include <MemberFuncTrap.h>
#include <MemberFuncTri.h>
#include <MemberFuncSCurve.h>
#include <MemberFuncSingle.h>
#include <FuzzySetBase.h>

int main( int argc, char* argv[] )
{
	FuzzyModelBase* testModel = new FuzzyModelBase();// = new FuzzyModelBase();
	testModel->init(); //create model with empty rule array
	//const Char* c = model->get_model_name();

	testModel->FuzzyModelBase::set_defuzz_method(0); // set DEFUZZ_COG method
	testModel->FuzzyModelBase::set_inference_method(0);
	testModel->FuzzyModelBase::set_composition_method(0);




	//FuzzyModelBase* pmodel = &model;

	std::wstring name1(L"inputVariable1");
	const wchar_t* szInputVariable1 = name1.c_str();

	std::wstring name2(L"outVariable1");
	const wchar_t* szOutVariable1 = name2.c_str();

	std::wstring name3(L"inputVariable2");
	const wchar_t* szInputVariable2 = name3.c_str();

	FuzzyVariableBase* inputVar1 = new FuzzyVariableBase(testModel);// create new inputVar
	int i = inputVar1->init(szInputVariable1, 34.5, 45.3, true);//left x value, right x value? 	// if no name is passed in name it "Variable 0"
//	ASSERT(i == -1);
	FuzzyVariableBase* inputVar2  = new FuzzyVariableBase(testModel);
	int k = inputVar2->init(szInputVariable2, 34.5, 45.3, true);
//	ASSERT(k == -1);
	FuzzyOutVariable* outVariable1 = new FuzzyOutVariable(testModel); // create new outVar
	int j = outVariable1->init(szOutVariable1, 23.4, 56.5, true); //return 0 if success
//	ASSERT(j == -1);

	testModel->add_input_variable(szInputVariable1, 34.5, 45.3, true);
	testModel->add_input_variable(szInputVariable1, 34.5, 45.3, true);
	testModel->add_output_variable(szOutVariable1, 23.4, 56.5, true);

	// set atributes for InputVariable1

	inputVar1->set_dom_array_count(100); // discrete y
	inputVar1->set_x_array_count(100); //discrete x

	inputVar1->set_index(0);

	// FuzzySets for InputVariable1
	FuzzySetBase* set1Var1 = new FuzzySetBase(inputVar1);
	// Set attributes for set1Var1

	FuzzySetBase* set2Var1 = new FuzzySetBase(inputVar1);
	FuzzySetBase* set3Var1 = new FuzzySetBase(inputVar1);

	// FuzzySets for InputVariable2
	FuzzySetBase* set1Var2 = new FuzzySetBase(inputVar2);
	FuzzySetBase* set2Var2 = new FuzzySetBase(inputVar2);
	FuzzySetBase* set3Var2 = new FuzzySetBase(inputVar2);

	// Membership functions for sets in InputVariable1

	MemberFuncTrap* functionTrap1 = new MemberFuncTrap(set1Var1);


	DOMType* arrayDegreeOfMembership = new DOMType[];
//	arrayDegreeOfMembership = (1,2,4,5,6,7,8,9,8,7,6,5,4,3,2,1);


	//FuzzyVariableBase *var = testModel->get_var(OUTPUT_IDX);


	// this code used for automaic create model from api

}