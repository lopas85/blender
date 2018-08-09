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

/** \file gameengine/Rasterizer/RAS_MeshSlot.cpp
 *  \ingroup bgerast
 */

#include "RAS_MeshSlot.h"
#include "RAS_MeshUser.h"
#include "RAS_IPolygonMaterial.h"
#include "RAS_DisplayArray.h"
#include "RAS_DisplayArrayBucket.h"
#include "RAS_DisplayArrayStorage.h"
#include "RAS_MaterialBucket.h"
#include "RAS_Mesh.h"

#ifdef _MSC_VER
#  pragma warning (disable:4786)
#endif

#ifdef WIN32
#  include <windows.h>
#endif // WIN32

static RAS_DummyNodeData dummyNodeData;

// mesh slot
RAS_MeshSlot::RAS_MeshSlot(RAS_MeshUser *meshUser, RAS_DisplayArrayBucket *arrayBucket)
	:m_displayArrayBucket(arrayBucket),
	m_meshUser(meshUser),
	m_batchPartIndex(-1)
{
	ConstructNodes();
}

RAS_MeshSlot::RAS_MeshSlot(const RAS_MeshSlot& other)
	:m_displayArrayBucket(other.m_displayArrayBucket),
	m_meshUser(other.m_meshUser),
	m_batchPartIndex(other.m_batchPartIndex)
{
	ConstructNodes();
}

RAS_MeshSlot::~RAS_MeshSlot()
{
}

void RAS_MeshSlot::ConstructNodes()
{
	if (m_displayArrayBucket->GetBucket()->GetMaterial()->IsText()) {
		m_node[0] = RAS_MeshSlotUpwardNode(this, &dummyNodeData,
				&RAS_MeshSlot::RunNode<false, false, true, true>, nullptr);
	}

	if (m_displayArrayBucket->ApplyMatrix()) {
		m_node[0] = RAS_MeshSlotUpwardNode(this, &dummyNodeData,
				&RAS_MeshSlot::RunNode<false, false, false, true>, nullptr);
		m_node[1] = RAS_MeshSlotUpwardNode(this, &dummyNodeData,
				&RAS_MeshSlot::RunNode<false, true, false, true>, nullptr);
	}
	else {
		m_node[0] = RAS_MeshSlotUpwardNode(this, &dummyNodeData,
				&RAS_MeshSlot::RunNode<false, false, false, false>, nullptr);
		m_node[1] = RAS_MeshSlotUpwardNode(this, &dummyNodeData,
				&RAS_MeshSlot::RunNode<false, true, false, false>, nullptr);
	}
}

RAS_DisplayArrayBucket *RAS_MeshSlot::GetDisplayArrayBucket() const
{
	return m_displayArrayBucket;
}

void RAS_MeshSlot::SetDisplayArrayBucket(RAS_DisplayArrayBucket *arrayBucket)
{
	m_displayArrayBucket = arrayBucket;
}

RAS_MeshUser *RAS_MeshSlot::GetMeshUser() const
{
	return m_meshUser;
}

short RAS_MeshSlot::GetBatchPartIndex() const
{
	return m_batchPartIndex;
}

void RAS_MeshSlot::SetBatchPartIndex(short index)
{
	m_batchPartIndex = index;
}

void RAS_MeshSlot::GenerateTree(RAS_DisplayArrayUpwardNode& root, RAS_UpwardTreeLeafs& leafs, bool polySort)
{
	RAS_MeshSlotUpwardNode& node = m_node[polySort];
	node.SetParent(&root);
	leafs.push_back(&node);
}

template <bool Override, bool PolySort, bool Text, bool ApplyMatrix>
void RAS_MeshSlot::RunNode(const RAS_MeshSlotNodeTuple& tuple)
{
	RAS_ManagerNodeData *managerData = tuple.m_managerData;
	RAS_MaterialNodeData *materialData = tuple.m_materialData;
	RAS_DisplayArrayNodeData *displayArrayData = tuple.m_displayArrayData;
	RAS_Rasterizer *rasty = managerData->m_rasty;
	rasty->SetFrontFace(m_meshUser->GetFrontFace());

	RAS_DisplayArrayStorage *storage = displayArrayData->m_arrayStorage;

#if 0
	if (/*!managerData->m_shaderOverride*/!Override && !Text)
#endif
	/*{*/
		materialData->m_material->ActivateMeshSlot(this, rasty, managerData->m_trans);
#if 0
		if (/*materialData->m_zsort*/PolySort) {
			displayArrayData->m_array->SortPolygons(managerData->m_trans * mt::mat3x4(m_meshUser->GetMatrix()),
			                                        storage->GetIndexMap());
			storage->FlushIndexMap();
		}
	}
#endif

	rasty->PushMatrix();
#if 0
	if (/*materialData->m_text*/Text) {
		rasty->IndexPrimitivesText(this);
	}
	else
#endif
	{
#if 0
		if (/*displayArrayData->m_applyMatrix*/ApplyMatrix)
#endif
		{
			float mat[16];
			rasty->GetTransform(m_meshUser->GetMatrix(), materialData->m_drawingMode, m_meshUser->GetClientObject(), mat);
			rasty->MultMatrix(mat);
		}
		storage->IndexPrimitives();
	}
	rasty->PopMatrix();
}

// No sort
template void RAS_MeshSlot::RunNode<false, false, false, false>(const RAS_MeshSlotNodeTuple& tuple);
template void RAS_MeshSlot::RunNode<true, false, false, false>(const RAS_MeshSlotNodeTuple& tuple);
template void RAS_MeshSlot::RunNode<false, false, false, true>(const RAS_MeshSlotNodeTuple& tuple);
template void RAS_MeshSlot::RunNode<true, false, false, true>(const RAS_MeshSlotNodeTuple& tuple);
// Sort
template void RAS_MeshSlot::RunNode<false, true, false, false>(const RAS_MeshSlotNodeTuple& tuple);
template void RAS_MeshSlot::RunNode<true, true, false, false>(const RAS_MeshSlotNodeTuple& tuple);
template void RAS_MeshSlot::RunNode<false, true, false, true>(const RAS_MeshSlotNodeTuple& tuple);
template void RAS_MeshSlot::RunNode<true, true, false, true>(const RAS_MeshSlotNodeTuple& tuple);
// Text
template void RAS_MeshSlot::RunNode<false, false, true, false>(const RAS_MeshSlotNodeTuple& tuple);
template void RAS_MeshSlot::RunNode<true, false, true, false>(const RAS_MeshSlotNodeTuple& tuple);
