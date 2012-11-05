#include <rule/variable.h>
#include <QObject>
#include <iostream>
#include <rule/linguisticterm.h>
#include <membership/continuous/membershipfunctionsigm.h>
#include <membership/continuous/membershipfunctiontrap.h>
#include <membership/continuous/membershipfunctiontrian.h>
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
//#include <activation/ruleconnectionmethodandmin.h>


int main( int argc, char* argv[] )
{
	JFuzzyQt* model = new JFuzzyQt();
//	model->setVariable("inputVar1",)
	// set inputVariable
	Variable* inputVar1 = new Variable();
	inputVar1->setName("inputVar1");
	// terms for var
	MembershipFunctionTrap* functionTrap   = new MembershipFunctionTrap(inputVar1, 0,10,20,50);
	MembershipFunction*     functionSigm   = new MembershipFunctionSigm(inputVar1,50,1);
	MembershipFunction*     functionTriang = new MembershipFunctionTrian(inputVar1, 20,50,60);

	LinguisticTerm* term1var1 = new LinguisticTerm(inputVar1,"term1var1",functionTrap  );
	LinguisticTerm* term2var1 = new LinguisticTerm(inputVar1,"term2var1",functionSigm  );
	LinguisticTerm* term3var1 = new LinguisticTerm(inputVar1,"term3var1",functionTriang);

	inputVar1->addLinguisticTerm(term1var1);
	inputVar1->addLinguisticTerm(term2var1);
	inputVar1->addLinguisticTerm(term3var1);
	
	// var2
	Variable* inputVar2 = new Variable();
	inputVar2->setName("inputVar2");
	// terms for var
	MembershipFunctionTrap* functionTrap2   = new MembershipFunctionTrap(inputVar2, 10,15,20,60);
	MembershipFunction*     functionSigm2   = new MembershipFunctionSigm(inputVar2,20,1);
	MembershipFunction*     functionTriang2 = new MembershipFunctionTrian(inputVar2, 0,55,67);

	LinguisticTerm* term1var2 = new LinguisticTerm(inputVar2,"term1var2",functionTrap2  );
	LinguisticTerm* term2var2 = new LinguisticTerm(inputVar2,"term2var2",functionSigm2  );
	LinguisticTerm* term3var2 = new LinguisticTerm(inputVar2,"term3var2",functionTriang2);

	inputVar2->addLinguisticTerm(term1var2);
	inputVar2->addLinguisticTerm(term2var2);
	inputVar2->addLinguisticTerm(term3var2);
	
	// output variable
	Variable* outVar = new Variable();
	outVar->setName("outVar");
	// terms for var
	MembershipFunctionTrap* functionTrap3   = new MembershipFunctionTrap(outVar, 10,15,20,60);
	MembershipFunction*     functionSigm3   = new MembershipFunctionSigm(outVar,20,1);
	MembershipFunction*     functionTriang3 = new MembershipFunctionTrian(outVar, 0,55,67);

	LinguisticTerm* term1varOut = new LinguisticTerm(outVar,"term1varOut",functionTrap3  );
	LinguisticTerm* term2varOut = new LinguisticTerm(outVar,"term2varOut",functionSigm3  );
	LinguisticTerm* term3varOut = new LinguisticTerm(outVar,"term3varOut",functionTriang3);

	outVar->addLinguisticTerm(term1varOut);
	outVar->addLinguisticTerm(term2varOut);
	outVar->addLinguisticTerm(term3varOut);

	std::cout << outVar->isOutputVariable() << std::endl;

	// rule terms for input vars
	RuleTerm* ruleTerm1Var1 = new RuleTerm(model,inputVar1,"term1var1", false);
	RuleTerm* ruleTerm2Var1 = new RuleTerm(model,inputVar1,"term2var1", false);
	RuleTerm* ruleTerm3Var1 = new RuleTerm(model,inputVar1,"term3var1", true );

	RuleTerm* ruleTerm1Var2 = new RuleTerm(model,inputVar2,"term1var2", true );
	RuleTerm* ruleTerm2Var2 = new RuleTerm(model,inputVar2,"term2var2", false);
	RuleTerm* ruleTerm3Var2 = new RuleTerm(model,inputVar2,"term3var2", false);

	// expression example
	RuleExpression* expr1   = new RuleExpression();
	expr1->addTerm1Rule(ruleTerm1Var1);
	expr1->addTerm2Rule(ruleTerm2Var2);
	
	RuleConnectionMethodAndMin* connectMethod = new RuleConnectionMethodAndMin();
	expr1->setRuleConnectionMethod(connectMethod);

	// check expression
	std::string expr1String = expr1->toString().toLocal8Bit().constData();
	std::cout << expr1String << std::endl;

	// from rules->expressions
	RuleExpression* expr2   = new RuleExpression();
	expr2->addTerm1Rule(ruleTerm2Var1);

	RuleExpression* expr3   = new RuleExpression();
	expr2->addTerm1Rule(ruleTerm3Var1);

	RuleExpression* expr4   = new RuleExpression();
	expr2->addTerm1Rule(ruleTerm1Var2);

	RuleExpression* expr5   = new RuleExpression();
	expr2->addTerm1Rule(ruleTerm2Var2);

	RuleExpression* expr6   = new RuleExpression();
	expr2->addTerm1Rule(ruleTerm3Var2);

	// rule terms for output var
	RuleTerm* ruleTerm1OutVar = new RuleTerm(model,outVar,"term1varOut",true );
	RuleTerm* ruleTerm2OutVar = new RuleTerm(model,outVar,"term2varOut",false);
	RuleTerm* ruleTerm3OutVar = new RuleTerm(model,outVar,"term3varOut",true );

	// construct rules
	Rule* rule1 = new Rule(model,"rule1");
	rule1->addAntecedents(expr1);
	rule1->addConsequent(ruleTerm1OutVar);

	Rule* rule2 = new Rule(model,"rule2");
	rule2->addAntecedents(expr2);
	rule2->addConsequent(ruleTerm2OutVar);
	
	Rule* rule3 = new Rule(model,"rule3");
	rule3->addAntecedents(expr3);
	rule3->addConsequent(ruleTerm3OutVar);

	Rule* rule4 = new Rule(model,"rule4");
	rule4->addAntecedents(expr4);
	rule4->addConsequent(ruleTerm1OutVar);

	Rule* rule5 = new Rule(model,"rule5");
	rule5->addAntecedents(expr5);
	rule5->addConsequent(ruleTerm2OutVar);

	Rule* rule6 = new Rule(model,"rule6");
	rule6->addAntecedents(expr6);
	rule6->addConsequent(ruleTerm3OutVar);

	RuleBlock* rb = new RuleBlock();
	rb->addRule(rule1);
	rb->addRule(rule2);
	rb->addRule(rule3);
	rb->addRule(rule4);
	rb->addRule(rule5);
	rb->addRule(rule6);

	RuleAccumulationMethod* ruleAccMethod = new RuleAccumulationMethodNormalisedSum();
	rb->addRuleAccumulationMethod(ruleAccMethod);

	RuleActivationMethod* ruleActMethod = new RuleActivationMethodMin(model);
	rb->addRuleActivationMethod(ruleActMethod);

	// set actual values for model

	DefuzzifierCenterOfGravity* defuzzifier = new DefuzzifierCenterOfGravity();

	FunctBlock* functionBlock = new FunctBlock(model,"FB");
	functionBlock->addVariable("inputVar1",inputVar1);
	functionBlock->addVariable("inputVar2",inputVar2);
	functionBlock->addVariable("outVar",   outVar   );
	functionBlock->addRuleBlock(rb);

	model->setVariable("inputVar1",52);
	model->setVariable("inputVar2",30);
	model->evaluate();
	double result = model->getValue("res");

}