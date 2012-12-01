#include <rule/variable.h>
#include <QObject>
#include <iostream>
#include <rule/linguisticterm.h>
#include <membership/continuous/membershipfunctionsigm.h>
#include <membership/continuous/membershipfunctiontrap.h>
#include <membership/continuous/membershipfunctiontrian.h>
#include <membership/continuous/membershipfunctiongauss.h>
#include <rule/rule.h>
#include <jfuzzyqt.h>
#include <connection/and/ruleconnectionmethodandmin.h>
#include <iostream>
#include <defuzzifier/defuzzifiercenterofgravity.h>
#include <connection/ruleconnectionmethod.h>
#include <rule/ruleblock.h>
#include <activation/ruleactivationmethod.h>
#include <functblock.h>
#include <accumulation/ruleaccumulationmethodnsum.h>
#include <activation/ruleactivationmethodmin.h>
#include <defuzzifier/defuzzifier.h>
#include <defuzzifier/defuzzifiercenterofgravitysingletons.h>

#include "TestQHash.h"
//#include <activation/ruleconnectionmethodandmin.h>


int main( int argc, char* argv[] )
{
	RuleConnectionMethod *AND = NULL;
	RuleConnectionMethod *OR = NULL;

	JFuzzyQt* model = new JFuzzyQt();
//	model->setVariable("inputVar1",)
	// set inputVariable
	FunctBlock* functionBlock = new FunctBlock(model,"fb");
	Variable *const inputVar1 = new Variable(model, "inputvar1");

//	bool t = functionBlock->addVariable(inputVar1->getName(),inputVar1);

	// terms for var
	MembershipFunctionTrap* functionTrap   = new MembershipFunctionTrap(functionBlock, 0,10,20,50);
	MembershipFunction*     functionTrap11   = new MembershipFunctionTrap(functionBlock,1,8,16,24);
	MembershipFunction*     functionTriang = new MembershipFunctionTrian(functionBlock, 20,50,60);

	LinguisticTerm* term1var1 = new LinguisticTerm(inputVar1,"term1var1",functionTrap  );
	LinguisticTerm* term2var1 = new LinguisticTerm(inputVar1,"term2var1",functionTrap11 );
	LinguisticTerm* term3var1 = new LinguisticTerm(inputVar1,"term3var1",functionTriang);
	
	// var2
	Variable* inputVar2 = new Variable(model,"inputvar2");
	// terms for var2
	MembershipFunctionTrap* functionTrap2   = new MembershipFunctionTrap(functionBlock, 10,15,20,60);
	MembershipFunction*     functionSigm2   = new MembershipFunctionSigm(functionBlock,20,1);
	MembershipFunction*     functionTriang2 = new MembershipFunctionTrian(functionBlock, 0,55,67);

	functionTrap2->estimateUniverse();
	functionSigm2->estimateUniverse();
	functionTriang2->estimateUniverse();

	LinguisticTerm* term1var2 = new LinguisticTerm(inputVar2,"term1var2",functionTrap2  );
	LinguisticTerm* term2var2 = new LinguisticTerm(inputVar2,"term2var2",functionSigm2  );
	LinguisticTerm* term3var2 = new LinguisticTerm(inputVar2,"term3var2",functionTriang2);

	
	// output variable
	Variable* outVar = new Variable(functionBlock,"outvar");
	// terms for outputvar
	MembershipFunctionTrap* functionTrap3   = new MembershipFunctionTrap(model, 10,15,20,60);
	MembershipFunction*     functionSigm3   = new MembershipFunctionSigm(model,20,1);
	MembershipFunction*     functionTriang3 = new MembershipFunctionTrian(model, 0,55,67);

	functionTrap3->estimateUniverse();
	functionSigm3->estimateUniverse();
	functionTriang3->estimateUniverse();

	LinguisticTerm* term1varOut = new LinguisticTerm(outVar,"term1varout",functionTrap3  );
	LinguisticTerm* term2varOut = new LinguisticTerm(outVar,"term2varout",functionSigm3  );
	LinguisticTerm* term3varOut = new LinguisticTerm(outVar,"term3varout",functionTriang3);

	std::cout << outVar->isOutputVariable() << std::endl;

	// rule terms for input vars
	RuleTerm* ruleTerm1Var1 = new RuleTerm(functionBlock,inputVar1,"term1var1", false);
	RuleTerm* ruleTerm2Var1 = new RuleTerm(functionBlock,inputVar1,"term2var1", false);
	RuleTerm* ruleTerm3Var1 = new RuleTerm(functionBlock,inputVar1,"term3var1", true );

	RuleTerm* ruleTerm1Var2 = new RuleTerm(functionBlock,inputVar2,"term1var2", true );
	RuleTerm* ruleTerm2Var2 = new RuleTerm(functionBlock,inputVar2,"term2var2", false);
	RuleTerm* ruleTerm3Var2 = new RuleTerm(functionBlock,inputVar2,"term3var2", false);

	RuleConnectionMethodAndMin* connectMethod = new RuleConnectionMethodAndMin();

	// rule terms for output var
	RuleTerm* ruleTerm1OutVar = new RuleTerm(functionBlock,outVar,"term1varout",true );
	RuleTerm* ruleTerm2OutVar = new RuleTerm(functionBlock,outVar,"term2varout",false);
	RuleTerm* ruleTerm3OutVar = new RuleTerm(functionBlock,outVar,"term3varout",true );

	// ruleblock

	RuleBlock* rb = new RuleBlock("FirstRB");
	RuleAccumulationMethod* ruleAccMethod = new RuleAccumulationMethodNormalisedSum(functionBlock);
	rb->addRuleAccumulationMethod(ruleAccMethod);

	RuleActivationMethod* ruleActMethod = new RuleActivationMethodMin(functionBlock);
	rb->addRuleActivationMethod(ruleActMethod);

	std::string rbString = rb->toString().toLocal8Bit().constData();
	std::cout << rbString << std::endl;

	// construct rules

	Rule* rule1 = new Rule(rb,"rule1");
	RuleExpression* expr1   = new RuleExpression(rule1);
	expr1->setRuleConnectionMethod(connectMethod);
	expr1->addTerm1Rule(ruleTerm1Var1);
	expr1->addTerm2Rule(ruleTerm2Var2);
	rule1->setWeight(0.5);
	rule1->addAntecedents(expr1);
	rule1->addConsequent(ruleTerm1OutVar);
	rule1->setDegreeOfSupport(0.5);
 
	std::string rule1String = rule1->toString().toLocal8Bit().constData();
	std::cout << rule1String << std::endl;

	Rule* rule2 = new Rule(rb,"rule2");
	RuleExpression* expr2   = new RuleExpression(rule2);
	expr2->setRuleConnectionMethod(connectMethod);
	expr2->addTerm1Rule(ruleTerm2Var1);
	expr2->addTerm2Rule(ruleTerm3Var2);
	
	rule2->setWeight(0.5);
	rule2->addAntecedents(expr2);
	rule2->addConsequent(ruleTerm2OutVar);
	rule2->setDegreeOfSupport(0.5);

	std::string rule2String = rule2->toString().toLocal8Bit().constData();
	std::cout << rule2String << std::endl;
	
	Rule* rule3 = new Rule(rb,"rule3");
	RuleExpression* expr3   = new RuleExpression(rule3);
	expr3->setRuleConnectionMethod(connectMethod);
	expr3->addTerm1Rule(ruleTerm3Var1);
	expr3->addTerm2Rule(ruleTerm3Var2);

	rule3->setWeight(0.5);
	rule3->addAntecedents(expr3);
	rule3->addConsequent(ruleTerm3OutVar);
	rule3->setDegreeOfSupport(0.5);

	std::string rule3String = rule3->toString().toLocal8Bit().constData();
	std::cout << rule3String << std::endl;

	Rule* rule4 = new Rule(rb,"rule4");
	RuleExpression* expr4   = new RuleExpression(rule4);
	expr4->setRuleConnectionMethod(connectMethod);
	expr4->addTerm1Rule(ruleTerm1Var2);
	expr4->addTerm2Rule(ruleTerm2Var1);

	rule4->setWeight(0.5);
	rule4->addAntecedents(expr4);
	rule4->addConsequent(ruleTerm1OutVar);
	rule4->setDegreeOfSupport(0.5);

	std::string rule4String = rule4->toString().toLocal8Bit().constData();
	std::cout << rule4String << std::endl;

	Rule* rule5 = new Rule(rb,"rule5");
	RuleExpression* expr5   = new RuleExpression(rule5);
	expr5->setRuleConnectionMethod(connectMethod);
	expr5->addTerm1Rule(ruleTerm2Var2);
	expr5->addTerm2Rule(ruleTerm1Var1);

	rule5->setWeight(0.5);
	rule5->addAntecedents(expr5);
	rule5->addConsequent(ruleTerm2OutVar);
	rule5->setDegreeOfSupport(0.5);

	std::string rule5String = rule5->toString().toLocal8Bit().constData();
	std::cout << rule5String << std::endl;

	Rule* rule6 = new Rule(rb,"rule6");
	RuleExpression* expr6   = new RuleExpression(rule6);
	expr6->setRuleConnectionMethod(connectMethod);
	expr6->addTerm1Rule(ruleTerm3Var2);
	expr6->addTerm2Rule(ruleTerm2Var1);
	
	rule6->setWeight(0.5);
	rule6->addAntecedents(expr6);
	rule6->addConsequent(ruleTerm3OutVar);
	rule6->setDegreeOfSupport(0.5);

	std::string rule6String = rule6->toString().toLocal8Bit().constData();
	std::cout << rule6String << std::endl;

	rb->addRule(*rule1);
	rb->addRule(*rule2);
	rb->addRule(*rule3);
	rb->addRule(*rule4);
	rb->addRule(*rule5);
	rb->addRule(*rule6);

	functionBlock->addVariable(inputVar1->getName(),inputVar1);

	inputVar1->addLinguisticTerm(term1var1);
	inputVar1->addLinguisticTerm(term2var1);
	inputVar1->addLinguisticTerm(term3var1);

	functionBlock->addVariable(inputVar2->getName(),inputVar2);

	inputVar2->addLinguisticTerm(term1var2);
	inputVar2->addLinguisticTerm(term2var2);
	inputVar2->addLinguisticTerm(term3var2);

	functionBlock->addVariable(outVar->getName(),outVar);

	outVar->addLinguisticTerm(term1varOut);
	outVar->addLinguisticTerm(term2varOut);
	outVar->addLinguisticTerm(term3varOut);

	functionBlock->addRuleBlock(rb);
	const Variable *tP1 = functionBlock->getVariables().find(inputVar1->getName()).value();

	model->addFunctionBlock(functionBlock);
	
	// set actual values for model

	Defuzzifier* d = functionBlock->createDefuzzifier("cog");
	functionBlock->setDefuzzifier(outVar->getName(), d);

	model->setVariable("inputvar1",52, functionBlock->getName());
	model->setVariable("inputvar2",30, functionBlock->getName());

	// evaluate model
	model->evaluate();
	double result = model->getValue(outVar->getName());

	std::cout << result << std::endl;
	return 0;
}