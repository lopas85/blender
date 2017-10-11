/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file gameengine/Ketsji/KX_VertexProxy.cpp
 *  \ingroup ketsji
 */

#ifdef WITH_PYTHON

#include "KX_VertexProxy.h"
#include "KX_Mesh.h"
#include "RAS_Vertex.h"
#include "RAS_IDisplayArray.h"

#include "KX_PyMath.h"

#include "EXP_ListWrapper.h"

#include <boost/format.hpp>

PyTypeObject KX_VertexProxy::Type = {
	PyVarObject_HEAD_INIT(nullptr, 0)
	"KX_VertexProxy",
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
	&EXP_Value::Type,
	0, 0, 0, 0, 0, 0,
	py_base_new
};

PyMethodDef KX_VertexProxy::Methods[] = {
	{"getXYZ", (PyCFunction)KX_VertexProxy::sPyGetXYZ, METH_NOARGS},
	{"setXYZ", (PyCFunction)KX_VertexProxy::sPySetXYZ, METH_O},
	{"getUV", (PyCFunction)KX_VertexProxy::sPyGetUV1, METH_NOARGS},
	{"setUV", (PyCFunction)KX_VertexProxy::sPySetUV1, METH_O},

	{"getUV2", (PyCFunction)KX_VertexProxy::sPyGetUV2, METH_NOARGS},
	{"setUV2", (PyCFunction)KX_VertexProxy::sPySetUV2, METH_VARARGS},

	{"getRGBA", (PyCFunction)KX_VertexProxy::sPyGetRGBA, METH_NOARGS},
	{"setRGBA", (PyCFunction)KX_VertexProxy::sPySetRGBA, METH_O},
	{"getNormal", (PyCFunction)KX_VertexProxy::sPyGetNormal, METH_NOARGS},
	{"setNormal", (PyCFunction)KX_VertexProxy::sPySetNormal, METH_O},
	{nullptr, nullptr} //Sentinel
};

EXP_Attribute KX_VertexProxy::Attributes[] = {
	EXP_ATTRIBUTE_RW_FUNCTION("x", pyattr_get_x, pyattr_set_x),
	EXP_ATTRIBUTE_RW_FUNCTION("y", pyattr_get_y, pyattr_set_y),
	EXP_ATTRIBUTE_RW_FUNCTION("z", pyattr_get_z, pyattr_set_z),

	EXP_ATTRIBUTE_RW_FUNCTION("r", pyattr_get_r, pyattr_set_r),
	EXP_ATTRIBUTE_RW_FUNCTION("g", pyattr_get_g, pyattr_set_g),
	EXP_ATTRIBUTE_RW_FUNCTION("b", pyattr_get_b, pyattr_set_b),
	EXP_ATTRIBUTE_RW_FUNCTION("a", pyattr_get_a, pyattr_set_a),

	EXP_ATTRIBUTE_RW_FUNCTION("u", pyattr_get_u, pyattr_set_u),
	EXP_ATTRIBUTE_RW_FUNCTION("v", pyattr_get_v, pyattr_set_v),

	EXP_ATTRIBUTE_RW_FUNCTION("u2", pyattr_get_u2, pyattr_set_u2),
	EXP_ATTRIBUTE_RW_FUNCTION("v2", pyattr_get_v2, pyattr_set_v2),

	EXP_ATTRIBUTE_RW_FUNCTION("XYZ", pyattr_get_XYZ, pyattr_set_XYZ),
	EXP_ATTRIBUTE_RW_FUNCTION("UV", pyattr_get_UV, pyattr_set_UV),
	EXP_ATTRIBUTE_RW_FUNCTION("uvs", pyattr_get_uvs, pyattr_set_uvs),

	EXP_ATTRIBUTE_RW_FUNCTION("color", pyattr_get_color, pyattr_set_color),
	EXP_ATTRIBUTE_RW_FUNCTION("colors", pyattr_get_colors, pyattr_set_colors),
	EXP_ATTRIBUTE_RW_FUNCTION("normal", pyattr_get_normal, pyattr_set_normal),

	EXP_ATTRIBUTE_NULL //Sentinel
};

float KX_VertexProxy::pyattr_get_x()
{
	return m_vertex.GetXYZ()[0];
}

float KX_VertexProxy::pyattr_get_y()
{
	return m_vertex.GetXYZ()[1];
}

float KX_VertexProxy::pyattr_get_z()
{
	return m_vertex.GetXYZ()[2];
}

float KX_VertexProxy::pyattr_get_r()
{
	return m_vertex.GetColor(0)[0] / 255.0f;
}

float KX_VertexProxy::pyattr_get_g()
{
	return m_vertex.GetColor(0)[1] / 255.0f;
}

float KX_VertexProxy::pyattr_get_b()
{
	return m_vertex.GetColor(0)[2] / 255.0f;
}

float KX_VertexProxy::pyattr_get_a()
{
	return m_vertex.GetColor(0)[3] / 255.0f;
}

float KX_VertexProxy::pyattr_get_u()
{
	return m_vertex.GetUv(0)[0];
}

float KX_VertexProxy::pyattr_get_v()
{
	return m_vertex.GetUv(0)[1];
}

float KX_VertexProxy::pyattr_get_u2()
{
	return (m_vertex.GetFormat().uvSize > 1) ? m_vertex.GetUv(1)[0] : 0.0f;
}

float KX_VertexProxy::pyattr_get_v2()
{
	return (m_vertex.GetFormat().uvSize > 1) ? m_vertex.GetUv(1)[1] : 0.0f;
}

mt::vec3 KX_VertexProxy::pyattr_get_XYZ()
{
	return m_vertex.xyz();
}

mt::vec2 KX_VertexProxy::pyattr_get_UV()
{
	return mt::vec2(m_vertex.GetUv(0));
}

unsigned int KX_VertexProxy::py_get_uvs_size()
{
	return m_vertex.GetFormat().uvSize;
}

PyObject *KX_VertexProxy::py_get_uvs_item(unsigned int index)
{
	const mt::vec2 uv = mt::vec2(m_vertex.GetUv(index));
	return PyObjectFrom(uv);
}

bool KX_VertexProxy::py_set_uvs_item(unsigned int index, PyObject *item)
{
	mt::vec2 uv;
	if (!PyVecTo(item, uv)) {
		return false;
	}

	m_vertex.SetUV(index, uv);
	m_array->NotifyUpdate(RAS_IDisplayArray::UVS_MODIFIED);

	return true;
}

EXP_ListWrapper *KX_VertexProxy::pyattr_get_uvs()
{
	return (new EXP_ListWrapper<KX_VertexProxy, &KX_VertexProxy::py_get_uvs_size, &KX_VertexProxy::py_get_uvs_item,
				&KX_VertexProxy::py_set_uvs_item>(self_v));
}

unsigned int KX_VertexProxy::py_get_colors_size()
{
	return m_vertex.GetFormat().colorSize;
}

PyObject *KX_VertexProxy::py_get_colors_item(unsigned int index)
{
	const unsigned char *rgba = m_vertex.GetColor(index);
	mt::vec4 color(rgba[0], rgba[1], rgba[2], rgba[3]);
	color /= 255.0f;
	return PyObjectFrom(color);
}

bool KX_VertexProxy::py_set_colors_item(unsigned int index, PyObject *item)
{
	mt::vec4 color;
	if (!PyVecTo(item, color)) {
		return false;
	}

	m_vertex.SetColor(index, color);
	m_array->NotifyUpdate(RAS_IDisplayArray::COLORS_MODIFIED);

	return true;
}

EXP_ListWrapper *KX_VertexProxy::pyattr_get_colors()
{
	return (new EXP_ListWrapper(self_v,
	                            ((KX_VertexProxy *)self_v)->GetProxy(),
	                            nullptr,
	                            kx_vertex_proxy_get_colors_size_cb,
	                            kx_vertex_proxy_get_colors_item_cb,
	                            nullptr,
	                            kx_vertex_proxy_set_colors_item_cb))->NewProxy(true);
}

mt::vec4 KX_VertexProxy::pyattr_get_color()
{
	const unsigned char *colp = m_vertex.GetColor(0);
	mt::vec4 color(colp[0], colp[1], colp[2], colp[3]);
	return color / 255.0f;
}

mt::vec3 KX_VertexProxy::pyattr_get_normal()
{
	return mt::vec3(m_vertex.GetNormal());
}

void KX_VertexProxy::pyattr_set_x(float value)
{
	mt::vec3 pos(m_vertex.GetXYZ());
	pos.x = value;
	m_vertex.SetXYZ(pos);
	m_array->NotifyUpdate(RAS_IDisplayArray::POSITION_MODIFIED);
}

void KX_VertexProxy::pyattr_set_y(float value)
{
	mt::vec3 pos(m_vertex.GetXYZ());
	pos.y = value;
	m_vertex.SetXYZ(pos);
	m_array->NotifyUpdate(RAS_IDisplayArray::POSITION_MODIFIED);
}

void KX_VertexProxy::pyattr_set_z(float value)
{
	mt::vec3 pos(m_vertex.GetXYZ());
	pos.z = value;
	m_vertex.SetXYZ(pos);
	m_array->NotifyUpdate(RAS_IDisplayArray::POSITION_MODIFIED);
}

void KX_VertexProxy::pyattr_set_u(float value)
{
	mt::vec2 uv = mt::vec2(m_vertex.GetUv(0));
	uv.x = value;
	m_vertex.SetUV(0, uv);
	m_array->NotifyUpdate(RAS_IDisplayArray::UVS_MODIFIED);
}

void KX_VertexProxy::pyattr_set_v(float value)
{
	mt::vec2 uv = mt::vec2(m_vertex.GetUv(0));
	uv.y = value;
	m_vertex.SetUV(0, uv);
	m_array->NotifyUpdate(RAS_IDisplayArray::UVS_MODIFIED);
}

void KX_VertexProxy::pyattr_set_u2(float value)
{
	if (GetVertex().GetFormat().uvSize > 1) {
		mt::vec2 uv = mt::vec2(m_vertex.GetUv(1));
		uv.x = value;
		m_vertex.SetUV(1, uv);
		m_array->NotifyUpdate(RAS_IDisplayArray::UVS_MODIFIED);
	}
}

void KX_VertexProxy::pyattr_set_v2(float value)
{
	if (GetVertex().GetFormat().uvSize > 1) {
		mt::vec2 uv = mt::vec2(m_vertex.GetUv(1));
		uv.y = value;
		m_vertex.SetUV(1, uv);
		m_array->NotifyUpdate(RAS_IDisplayArray::UVS_MODIFIED);
	}
}

// Union used to modify only one color channel (char) of a color stored on a integer.
union ColorUnion {
	unsigned int coli;
	unsigned char colc[4];
};

void KX_VertexProxy::pyattr_set_r(float value)
{
	ColorUnion col{m_vertex.GetRawColor(0)};
	col.colc[0] = (unsigned char)(value * 255.0f);
	m_vertex.SetColor(0, col.coli);
	m_array->NotifyUpdate(RAS_IDisplayArray::COLORS_MODIFIED);
}

void KX_VertexProxy::pyattr_set_g(float value)
{
	ColorUnion col{m_vertex.GetRawColor(0)};
	col.colc[1] = (unsigned char)(value * 255.0f);
	m_vertex.SetColor(0, col.coli);
	m_array->NotifyUpdate(RAS_IDisplayArray::COLORS_MODIFIED);
}

void KX_VertexProxy::pyattr_set_b(float value)
{
	ColorUnion col{m_vertex.GetRawColor(0)};
	col.colc[2] = (unsigned char)(value * 255.0f);
	m_vertex.SetColor(0, col.coli);
	m_array->NotifyUpdate(RAS_IDisplayArray::COLORS_MODIFIED);
}

void KX_VertexProxy::pyattr_set_a(float value)
{
	ColorUnion col{m_vertex.GetRawColor(0)};
	col.colc[3] = (unsigned char)(value * 255.0f);
	m_vertex.SetColor(0, col.coli);
	m_array->NotifyUpdate(RAS_IDisplayArray::COLORS_MODIFIED);
}

void KX_VertexProxy::pyattr_set_XYZ(const mt::vec3& value)
{
	m_vertex.SetXYZ(value);
	m_array->NotifyUpdate(RAS_IDisplayArray::POSITION_MODIFIED);
}

void KX_VertexProxy::pyattr_set_UV(const mt::vec2& value)
{
	m_vertex.SetUV(0, value);
	m_array->NotifyUpdate(RAS_IDisplayArray::UVS_MODIFIED);
}

bool KX_VertexProxy::pyattr_set_uvs(PyObject *value, const EXP_Attribute *attrdef)
{
	if (PySequence_Check(value)) {
		for (int i = 0; i < PySequence_Size(value) && i < m_vertex.GetFormat().uvSize; ++i) {
			mt::vec2 vec;
			if (EXP_ConvertFromPython(PySequence_GetItem(value, i), vec)) {
				m_vertex.SetUV(i, vec);
			}
			else {
				attrdef->PrintError((boost::format("list[%d] was not a vector") % i).str().c_str());
				return false;
			}
		}

		m_array->NotifyUpdate(RAS_IDisplayArray::UVS_MODIFIED);
		return true;
	}
	return false;
}

void KX_VertexProxy::pyattr_set_color(const mt::vec4& value)
{
	m_vertex.SetColor(0, value);
	m_array->NotifyUpdate(RAS_IDisplayArray::COLORS_MODIFIED);
}

bool KX_VertexProxy::pyattr_set_colors(PyObject *value, const EXP_Attribute *attrdef)
{
	if (PySequence_Check(value)) {
		for (int i = 0; i < PySequence_Size(value) && i < m_vertex.GetFormat().colorSize; ++i) {
			mt::vec4 vec;
			if (EXP_ConvertFromPython(PySequence_GetItem(value, i), vec)) {
				m_vertex.SetColor(i, vec);
			}
			else {
				attrdef->PrintError((boost::format("list[%d] was not a vector") % i).str().c_str());
				return false;
			}
		}

		m_array->NotifyUpdate(RAS_IDisplayArray::COLORS_MODIFIED);
		return true;
	}
	return false;
}

void KX_VertexProxy::pyattr_set_normal(const mt::vec3& value)
{
	m_vertex.SetNormal(value);
	m_array->NotifyUpdate(RAS_IDisplayArray::NORMAL_MODIFIED);
}

KX_VertexProxy::KX_VertexProxy(RAS_IDisplayArray *array, RAS_Vertex vertex)
	:m_vertex(vertex),
	m_array(array)
{
}

KX_VertexProxy::~KX_VertexProxy()
{
}

RAS_Vertex& KX_VertexProxy::GetVertex()
{
	return m_vertex;
}

RAS_IDisplayArray *KX_VertexProxy::GetDisplayArray()
{
	return m_array;
}

// stuff for cvalue related things
std::string KX_VertexProxy::GetName() const
{
	return "vertex";
}

// stuff for python integration
PyObject *KX_VertexProxy::PyGetXYZ()
{
	return PyObjectFrom(m_vertex.GetXYZ());
}

PyObject *KX_VertexProxy::PySetXYZ(PyObject *value)
{
	mt::vec3 vec;
	if (!PyVecTo(value, vec)) {
		return nullptr;
	}

	m_vertex.SetXYZ(vec);
	m_array->NotifyUpdate(RAS_IDisplayArray::POSITION_MODIFIED);
	Py_RETURN_NONE;
}

PyObject *KX_VertexProxy::PyGetNormal()
{
	return PyObjectFrom(m_vertex.GetNormal());
}

PyObject *KX_VertexProxy::PySetNormal(PyObject *value)
{
	mt::vec3 vec;
	if (!PyVecTo(value, vec)) {
		return nullptr;
	}

	m_vertex.SetNormal(vec);
	m_array->NotifyUpdate(RAS_IDisplayArray::NORMAL_MODIFIED);
	Py_RETURN_NONE;
}

PyObject *KX_VertexProxy::PyGetRGBA()
{
	const unsigned int rgba = m_vertex.GetRawColor(0);
	return PyLong_FromLong(rgba);
}

PyObject *KX_VertexProxy::PySetRGBA(PyObject *value)
{
	if (PyLong_Check(value)) {
		int rgba = PyLong_AsLong(value);
		m_vertex.SetColor(0, rgba);
		m_array->NotifyUpdate(true);
		Py_RETURN_NONE;
	}
	else {
		mt::vec4 vec;
		if (PyVecTo(value, vec)) {
			m_vertex.SetColor(0, vec);
			m_array->NotifyUpdate(RAS_IDisplayArray::COLORS_MODIFIED);
			Py_RETURN_NONE;
		}
	}

	PyErr_SetString(PyExc_TypeError, "vert.setRGBA(value): KX_VertexProxy, expected a 4D vector or an int");
	return nullptr;
}

PyObject *KX_VertexProxy::PyGetUV1()
{
	return PyObjectFrom(m_vertex.GetUv(0));
}

PyObject *KX_VertexProxy::PySetUV1(PyObject *value)
{
	mt::vec2 vec;
	if (!PyVecTo(value, vec)) {
		return nullptr;
	}

	m_vertex.SetUV(0, vec);
	m_array->NotifyUpdate(RAS_IDisplayArray::UVS_MODIFIED);
	Py_RETURN_NONE;
}

PyObject *KX_VertexProxy::PyGetUV2()
{
	return (m_vertex.GetFormat().uvSize > 1) ? PyObjectFrom(m_vertex.GetUv(1)) : PyObjectFrom(mt::zero2);
}

PyObject *KX_VertexProxy::PySetUV2(PyObject *args)
{
	mt::vec2 vec;
	if (!PyVecTo(args, vec)) {
		return nullptr;
	}

	if (m_vertex.GetFormat().uvSize > 1) {
		m_vertex.SetUV(1, vec);
		m_array->NotifyUpdate(RAS_IDisplayArray::UVS_MODIFIED);
	}
	Py_RETURN_NONE;
}

#endif // WITH_PYTHON
