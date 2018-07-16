#ifndef __LOG_NODE_H__
#define __LOG_NODE_H__

#ifdef WITH_PYTHON

#include "EXP_Value.h"

class KX_GameObject;

class LOG_Node : public EXP_Value
{
	Py_Header

private:
	KX_GameObject *m_object;
	bool m_init;

public:
	LOG_Node();
	virtual ~LOG_Node();

	virtual std::string GetName() const;
	virtual EXP_Value *GetReplica();

	void ProcessReplica();

	KX_GameObject *GetGameObject() const;
	void SetGameObject(KX_GameObject *gameobj);

	void SetStartArgs(PyObject *args);

	void Start();
	void Update();	

	static PyObject *py_node_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

	// Attributes
	static PyObject *pyattr_get_object(EXP_PyObjectPlus *self_v, const EXP_PYATTRIBUTE_DEF *attrdef);
};

#endif  // WITH_PYTHON

#endif  // __LOG_NODE_H__
