

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
//	testModel->init(); //create model with empty rule array
	RuleArray* ruleArray = new RuleArray(testModel);
	ruleArray->alloc(10); // number of rules;
//	testModel->get_num_of_rules();
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

	std::wstring nameSet(L"set1Var1");
	wchar_t* wset_name1 = convert_to_wide_char("set1Var1");



	FuzzyVariableBase* inputVar1 = new FuzzyVariableBase(testModel);// create new inputVar
	int i = inputVar1->init(szInputVariable1, 34.5, 45.3, true);//left x value, right x value? 	// if no name is passed in name it "Variable 0"
	inputVar1->add_set(inputVar1->new_set(wset_name1,25,inputVar1,0,30,2));
	
	inputVar1->set_x_array_count(100);
//	inputVar1->set_right_x(25);
//	inputVar1->set_ramp(1,0,0);
	//inputVar1->set_dom_array_count(20);
//	inputVar1->set_left_x(0);
	
	
//	ASSERT(i == -1);
	FuzzyVariableBase* inputVar2  = new FuzzyVariableBase(testModel);
	int k = inputVar2->init(szInputVariable2, 40, 55, true);
//	ASSERT(k == -1);
	FuzzyOutVariable* outVariable1 = new FuzzyOutVariable(testModel); // create new outVar
	int j = outVariable1->init(szOutVariable1, 23.4, 56.5, true); //return 0 if success
//	ASSERT(j == -1);

	testModel->add_input_variable(szInputVariable1, 34.5, 45.3, true);
	testModel->add_input_variable(szInputVariable2, 40, 55, true);
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

	FuzzySetBase* outSet1 = new FuzzySetBase(outVariable1);
	FuzzySetBase* outSet2 = new FuzzySetBase(outVariable1);
	FuzzySetBase* outSet3 = new FuzzySetBase(outVariable1);
	outSet1->set_index(0);
	outSet2->set_index(1);
	outSet2->set_index(2);
	// Membership functions for sets in InputVariable1

	MemberFuncTrap* functionTrap1 = new MemberFuncTrap(set1Var1);
	functionTrap1->init();
	functionTrap1->set_ramp(1,0); // RAMP_NA
	functionTrap1->init();
	functionTrap1->set_node(0,25,0);
	functionTrap1->set_node(1,27,1);  // 4 point for trap
	functionTrap1->set_node(2,30,1);
	functionTrap1->set_node(3,32,0);

	wchar_t* wset_name = convert_to_wide_char("Set1Var1");
//	FuzzySetBase* set = new_set(wset_name, 0, inputVar1, 3, 0, 2);

	functionTrap1->get_start_x();
	functionTrap1->get_end_x();
	functionTrap1->get_node_x(2);
	functionTrap1->get_node_y(2);
	functionTrap1->get_ramp();
	functionTrap1->get_center_x();
//	functionTrap1->get_dom();
//	functionTrap1->get_value();

	int o = testModel->get_input_var_count();

	//if inputVar1 term1 and inputVar2 term1 then outputVar term1
	//if inputVar1 term1 and inputVar2 term2 then outputVar term2
	//if inputVar1 term1 and inputVar2 term3 then outputVar term3
	//if inputVar1 term2 and inputVar2 term1 then outputVar term1
	//if inputVar1 term2 and inputVar2 term2 then outputVar term2
	//if inputVar1 term2 and inputVar2 term3 then outputVar term3
	//if inputVar1 term3 and inputVar2 term1 then outputVar term1
	//if inputVar1 term3 and inputVar2 term2 then outputVar term2
	//if inputVar1 term3 and inputVar2 term3 then outputVar term3

	int oo = testModel->get_input_var_count()+1;
	
	std::string* rule_components = new std::string[oo + 1]; // +1 for output var

	testModel->add_rule(0, outSet1->get_index());
	testModel->add_rule(1, outSet2->get_index());
	testModel->add_rule(2, outSet3->get_index());
	testModel->add_rule(3, outSet1->get_index());
	testModel->add_rule(4, outSet2->get_index());
	testModel->add_rule(5, outSet3->get_index());
	testModel->add_rule(6, outSet1->get_index());
	testModel->add_rule(7, outSet2->get_index());
	testModel->add_rule(8, outSet3->get_index());

	DOMType* arrayDegreeOfMembership = new DOMType[];
//	arrayDegreeOfMembership = (1,2,4,5,6,7,8,9,8,7,6,5,4,3,2,1);


	//FuzzyVariableBase *var = testModel->get_var(OUTPUT_IDX);


	// this code used for automaic create model from api

}