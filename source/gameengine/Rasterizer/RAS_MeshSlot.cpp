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
#include "RAS_TexVert.h"
#include "RAS_IDisplayArray.h"
#include "RAS_IStorageInfo.h"

#ifdef _MSC_VER
#  pragma warning (disable:4786)
#endif

#ifdef WIN32
#  include <windows.h>
#endif // WIN32

static RAS_DummyNodeData dummyNodeData;

// mesh slot
RAS_MeshSlot::RAS_MeshSlot(RAS_MeshUser *meshUser, RAS_DisplayArrayBucket *arrayBucket)
	:m_node(this, &dummyNodeData, std::mem_fn(&RAS_MeshSlot::RunNode), nullptr),
	m_displayArrayBucket(arrayBucket),
	m_pDerivedMesh(nullptr),
	m_meshUser(meshUser),
	m_batchPartIndex(-1)
{
}

RAS_MeshSlot::~RAS_MeshSlot()
{
}

void RAS_MeshSlot::SetDisplayArrayBucket(RAS_DisplayArrayBucket *arrayBucket)
{
	m_displayArrayBucket = arrayBucket;
}

void RAS_MeshSlot::GenerateTree(RAS_DisplayArrayUpwardNode& root, RAS_UpwardTreeLeafs& leafs)
{
	m_node.SetParent(&root);
	leafs.push_back(&m_node);
}

void RAS_MeshSlot::RunNode(const RAS_MeshSlotNodeTuple& tuple)
{
	RAS_ManagerNodeData *managerData = tuple.m_managerData;
	RAS_MaterialNodeData *materialData = tuple.m_materialData;
	RAS_DisplayArrayNodeData *displayArrayData = tuple.m_displayArrayData;
	RAS_Rasterizer *rasty = managerData->m_rasty;
	rasty->SetClientObject(m_meshUser->GetClientObject());
	rasty->SetFrontFace(m_meshUser->GetFrontFace());


	if (!managerData->m_shaderOverride) {
		rasty->ProcessLighting(materialData->m_useLighting, managerData->m_trans);
		materialData->m_material->ActivateMeshSlot(this, rasty);

		RAS_IStorageInfo *storage = displayArrayData->m_storageInfo;
		if (materialData->m_zsort && storage) {
			displayArrayData->m_array->SortPolygons(managerData->m_trans * MT_Transform(m_meshUser->GetMatrix()),
								 storage->GetIndexMap());
			storage->FlushIndexMap();
		}
	}

	rasty->PushMatrix();

	if (materialData->m_text) {
		rasty->IndexPrimitivesText(this);
	}
	else {
		if (displayArrayData->m_applyMatrix) {
			float mat[16];
			rasty->GetTransform(m_meshUser->GetMatrix(), materialData->m_drawingMode, mat);
			rasty->MultMatrix(mat);
		}

		if (m_pDerivedMesh) {
			rasty->IndexPrimitivesDerivedMesh(managerData->m_drawingMode, this);
		}
		else {
			rasty->IndexPrimitives(displayArrayData->m_storageInfo);
		}
	}

	rasty->PopMatrix();
}
