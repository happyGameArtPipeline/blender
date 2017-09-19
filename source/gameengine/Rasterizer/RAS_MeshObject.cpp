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

/** \file gameengine/Rasterizer/RAS_MeshObject.cpp
 *  \ingroup bgerast
 */

#include "DNA_mesh_types.h"

#include "RAS_MeshObject.h"
#include "RAS_MeshUser.h"
#include "RAS_BoundingBoxManager.h"
#include "RAS_Polygon.h"
#include "RAS_IPolygonMaterial.h"
#include "RAS_BucketManager.h"
#include "RAS_DisplayArray.h"
#include "RAS_Deformer.h"

#include "SCA_IScene.h"

#include "MT_Vector3.h"

#include "CM_Message.h"

RAS_MeshObject::RAS_MeshObject(Mesh *mesh, const LayersInfo& layersInfo)
	:m_name(mesh->id.name + 2),
	m_layersInfo(layersInfo),
	m_boundingBox(nullptr),
	m_mesh(mesh)
{
}

RAS_MeshObject::~RAS_MeshObject()
{
	m_polygons.clear();

	for (RAS_MeshMaterial *meshmat : m_materials) {
		delete meshmat;
	}
	m_materials.clear();
}

int RAS_MeshObject::NumMaterials()
{
	return m_materials.size();
}

const std::string RAS_MeshObject::GetMaterialName(unsigned int matid)
{
	RAS_MeshMaterial *mmat = GetMeshMaterial(matid);

	if (mmat)
		return mmat->GetBucket()->GetPolyMaterial()->GetName();

	return "";
}

RAS_MeshMaterial *RAS_MeshObject::GetMeshMaterial(unsigned int matid) const
{
	if (m_materials.size() > matid) {
		return m_materials[matid];
	}

	return nullptr;
}

RAS_MeshMaterial *RAS_MeshObject::GetMeshMaterialBlenderIndex(unsigned int index)
{
	for (RAS_MeshMaterial *meshmat : m_materials) {
		if (meshmat->GetIndex() == index) {
			return meshmat;
		}
	}

	return nullptr;
}

int RAS_MeshObject::NumPolygons()
{
	return m_polygons.size();
}

RAS_Polygon *RAS_MeshObject::GetPolygon(int num)
{
	return &m_polygons[num];
}

std::vector<RAS_Polygon> *RAS_MeshObject::GetPolygons()
{
	return &m_polygons;
}

std::string& RAS_MeshObject::GetName()
{
	return m_name;
}

const std::string RAS_MeshObject::GetTextureName(unsigned int matid)
{
	RAS_MeshMaterial *mmat = GetMeshMaterial(matid);

	if (mmat)
		return mmat->GetBucket()->GetPolyMaterial()->GetTextureName();

	return "";
}

RAS_MeshMaterial *RAS_MeshObject::AddMaterial(RAS_MaterialBucket *bucket, unsigned int index, const RAS_TexVertFormat& format)
{
	RAS_MeshMaterial *meshmat = GetMeshMaterialBlenderIndex(index);

	// none found, create a new one
	if (!meshmat) {
		meshmat = new RAS_MeshMaterial(this, bucket, index, format);
		m_materials.push_back(meshmat);
	}

	return meshmat;
}

RAS_IDisplayArray *RAS_MeshObject::GetDisplayArray(unsigned int matid) const
{
	RAS_MeshMaterial *mmat = GetMeshMaterial(matid);

	if (!mmat)
		return nullptr;

	RAS_IDisplayArray *array = mmat->GetDisplayArray();

	return array;
}

RAS_ITexVert *RAS_MeshObject::GetVertex(unsigned int matid, unsigned int index)
{
	RAS_IDisplayArray *array = GetDisplayArray(matid);

	if (index < array->GetVertexCount()) {
		return array->GetVertex(index);
	}

	return nullptr;
}

RAS_BoundingBox *RAS_MeshObject::GetBoundingBox() const
{
	return m_boundingBox;
}

RAS_MeshUser* RAS_MeshObject::AddMeshUser(void *clientobj, RAS_Deformer *deformer)
{
	RAS_BoundingBox *boundingBox = (deformer) ? deformer->GetBoundingBox() : m_boundingBox;
	RAS_MeshUser *meshUser = new RAS_MeshUser(clientobj, boundingBox);

	for (RAS_MeshMaterial *mmat : m_materials) {
		RAS_DisplayArrayBucket *arrayBucket;
		/* Duplicate the display array bucket and the display array if needed to store
		 * the mesh slot on a unique list (= display array bucket) and use an unique vertex
		 * array (=display array). */
		if (deformer) {
			// The deformer makes use of vertex array, make sure we have our local copy.
			RAS_IDisplayArray *array = mmat->GetDisplayArray()->GetReplica();

			arrayBucket = new RAS_DisplayArrayBucket(mmat->GetBucket(), array, this, mmat, deformer);
			// Make the deformer the owner of the display array (and bucket).
			deformer->AddDisplayArray(array, arrayBucket);
		}
		else {
			arrayBucket = mmat->GetDisplayArrayBucket();
		}

		RAS_MeshSlot *ms = new RAS_MeshSlot(meshUser, arrayBucket);
		meshUser->AddMeshSlot(ms);
	}
	return meshUser;
}

void RAS_MeshObject::EndConversion(RAS_BoundingBoxManager *boundingBoxManager)
{
#if 0
	m_sharedvertex_map.clear(); // SharedVertex
	std::vector<vector<SharedVertex> >   shared_null(0);
	shared_null.swap(m_sharedvertex_map);   /* really free the memory */
#endif

	RAS_IDisplayArrayList arrayList;

	// Construct a list of all the display arrays used by this mesh.
	for (RAS_MeshMaterial *meshmat : m_materials) {
		RAS_IDisplayArray *array = meshmat->GetDisplayArray();
		array->UpdateCache();
		arrayList.push_back(array);

		const std::string materialname = meshmat->GetBucket()->GetPolyMaterial()->GetName();
		if (array->GetVertexCount() == 0) {
			CM_Warning("mesh \"" << m_name << "\" has no vertices for material \"" << materialname
				<< "\". It introduces performance decrease for empty render.");
		}
		else if (array->GetPrimitiveIndexCount() == 0) {
			CM_Warning("mesh \"" << m_name << "\" has no polygons for material \"" << materialname
				<< "\". It introduces performance decrease for empty render.");
		}
	}

	// Construct the bounding box of this mesh without deformers.
	m_boundingBox = boundingBoxManager->CreateMeshBoundingBox(arrayList);
	m_boundingBox->Update(true);
}

const RAS_MeshObject::LayersInfo& RAS_MeshObject::GetLayersInfo() const
{
	return m_layersInfo;
}

void RAS_MeshObject::GenerateAttribLayers()
{
	for (RAS_MeshMaterial *mmat : m_materials) {
		RAS_DisplayArrayBucket *displayArrayBucket = mmat->GetDisplayArrayBucket();
		displayArrayBucket->GenerateAttribLayers();
	}
}
