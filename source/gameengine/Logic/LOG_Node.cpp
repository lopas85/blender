#ifdef WITH_PYTHON

#include "LOG_Node.h"

#include "KX_GameObject.h"

#include "CM_Message.h"

LOG_Node::LOG_Node()
	:m_object(nullptr),
	m_init(false)
{
}

LOG_Node::~LOG_Node()
{
}

std::string LOG_Node::GetName() const
{
	return "LOG_Node";
}

EXP_Value *LOG_Node::GetReplica()
{
	LOG_Node *replica = new LOG_Node(*this);
	replica->ProcessReplica();

	// Subclass the python node.
	PyTypeObject *type = Py_TYPE(GetProxy());
	if (!py_base_new(type, PyTuple_Pack(1, replica->GetProxy()), nullptr)) {
		CM_Error("failed replicate node");
		delete replica;
		return nullptr;
	}

	return replica;
}

void LOG_Node::ProcessReplica()
{
	EXP_Value::ProcessReplica();
	m_object = nullptr;
	m_init = false;
}

KX_GameObject *LOG_Node::GetGameObject() const
{
	return m_object;
}

void LOG_Node::SetGameObject(KX_GameObject *gameobj)
{
	m_object = gameobj;
}

void LOG_Node::Start()
{
	PyObject *ret = PyObject_CallMethod(GetProxy(), "start", "");
	if (PyErr_Occurred()) {
		PyErr_Print();
	}
	Py_XDECREF(ret);
}

void LOG_Node::Update()
{
	if (!m_init) {
		Start();
		m_init = true;
	}

	PyObject *ret = PyObject_CallMethod(GetProxy(), "update", "");
	if (PyErr_Occurred()) {
		PyErr_Print();
	}
	Py_XDECREF(ret);
}

PyObject *LOG_Node::py_node_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	LOG_Node *node = new LOG_Node();

	PyObject *proxy = py_base_new(type, PyTuple_Pack(1, node->GetProxy()), kwds);
	if (!proxy) {
		delete node;
		return nullptr;
	}

	return proxy;
}

PyTypeObject LOG_Node::Type = {
	PyVarObject_HEAD_INIT(nullptr, 0)
	"LOG_Node",
	sizeof(EXP_PyObjectPlus_Proxy),
	0,
	py_base_dealloc,
	0,
	0,
	0,
	0,
	py_base_repr,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	0, 0, 0, 0, 0, 0, 0,
	Methods,
	0,
	0,
	&EXP_PyObjectPlus::Type,
	0, 0, 0, 0, 0, 0,
	py_node_new
};

PyMethodDef LOG_Node::Methods[] = {
	{nullptr, nullptr} // Sentinel
};

PyAttributeDef LOG_Node::Attributes[] = {
	EXP_PYATTRIBUTE_RO_FUNCTION("object", LOG_Node, pyattr_get_object),
	EXP_PYATTRIBUTE_NULL // Sentinel
};

PyObject *LOG_Node::pyattr_get_object(EXP_PyObjectPlus *self_v, const EXP_PYATTRIBUTE_DEF *attrdef)
{
	LOG_Node *self = static_cast<LOG_Node *>(self_v);
	KX_GameObject *gameobj = self->GetGameObject();

	if (gameobj) {
		return gameobj->GetProxy();
	}
	else {
		Py_RETURN_NONE;
	}
}
#endif
