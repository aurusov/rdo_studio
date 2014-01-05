/*!
  \copyright (c) RDO-Team, 2011
  \file      context.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.06.2010
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_CONTEXT_H_
#define _RDOPARSER_CONTEXT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <string>
#include <boost/any.hpp>
#include <boost/function.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/expression.h"
#include "simulator/compiler/parser/context/stack.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Context
// --------------------------------------------------------------------------------
OBJECT_VIRTUAL(Context)
{
DECLARE_FACTORY(Context);
friend void ContextStack::push(LPContext pContext);
friend void ContextStack::pop_not_safed();

public:
	static const std::string METHOD_GET;
	static const std::string METHOD_TYPE_OF;
	static const std::string METHOD_SET;
	static const std::string METHOD_OPERATOR_BRACKETS;
	static const std::string METHOD_OPERATOR_DOT;

	typedef  boost::function<LPExpression ()> CreateExpressionFunction;

	template <class Function>
	struct FunctionWrapper
	{
		FunctionWrapper(const Function& function);
		Function function;
	};
	typedef  FunctionWrapper<CreateExpressionFunction>  CreateExpression;

	struct Params: public std::map<std::string, boost::any>
	{
		static const std::string IDENTIFIER;

		template <class T>
		T get(const std::string& name) const;

		std::string get(const std::string& name) const;
		std::string identifier() const;

		bool exists(const std::string& name) const;
	};

	class FindResult;
	struct SwitchContext
	{
		friend class FindResult;

		LPContext  context;
		Params     params;

		SwitchContext(const LPContext& context);
		SwitchContext(const LPContext& context, const Params& params);

		operator bool() const;

	private:
		SwitchContext();
	};

	class FindResult
	{
	public:
		FindResult();
		explicit FindResult(const CreateExpression& createExpression);
		explicit FindResult(const SwitchContext& switchContext);

		operator bool() const;

		const CreateExpressionFunction& getCreateExpression() const;
		const SwitchContext& getSwitchContext() const;

	private:
		CreateExpressionFunction createExpression;
		SwitchContext            switchContext;
	};

	template <class T>
	rdo::intrusive_ptr<T> cast();

	template <class T>
	rdo::interface_ptr<T> interface_cast();

	FindResult find(const std::string& method, const Params& params, const RDOParserSrcInfo& srcInfo) const;

protected:
	Context();
	virtual ~Context();

private:
	LPContextStack  m_pContextStack;

	void setContextStack  (CREF(LPContextStack) pContextStack);
	void resetContextStack();
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/context/context.inl"

#endif // _RDOPARSER_CONTEXT_H_
