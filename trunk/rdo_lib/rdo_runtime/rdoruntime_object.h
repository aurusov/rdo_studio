#ifndef RDORUNTIME_OBJECT_H
#define RDORUNTIME_OBJECT_H

namespace rdoRuntime {

typedef double RDOValue;

class RDORuntime;

class RDORuntimeObject
{
protected:
	RDORuntime*             runtime;
	const RDORuntimeObject* parent;
	RDORuntimeObject();
	RDORuntimeObject( RDORuntime* _runtime );
	RDORuntimeObject( const RDORuntimeObject* _parent );
	virtual ~RDORuntimeObject();

public:
	void reparent( const RDORuntimeObject* _parent );
};

class RDODelRuntimeObject
{
public:
	RDODelRuntimeObject()          {}
	virtual ~RDODelRuntimeObject() {}
};

} // namespace rdoRuntime

#endif // RDORUNTIME_OBJECT_H
