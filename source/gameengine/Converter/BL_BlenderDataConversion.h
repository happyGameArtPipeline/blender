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

/** \file BL_BlenderDataConversion.h
 *  \ingroup bgeconv
 */

#ifndef __BL_BLENDERDATACONVERSION_H__
#define __BL_BLENDERDATACONVERSION_H__

#include "RAS_MeshObject.h"
#include "KX_PhysicsEngineEnums.h"
#include "SCA_IInputDevice.h"

struct Material;
struct Mesh;
struct DerivedMesh;
struct Object;
struct Main;
class RAS_Rasterizer;
class RAS_ICanvas;
class RAS_MeshMaterial;
class KX_KetsjiEngine;
class KX_Scene;
class BL_BlenderSceneConverter;

struct BL_MeshMaterial
{
	RAS_IDisplayArray *array;
	RAS_MaterialBucket *bucket;
	bool visible;
	bool twoside;
	bool collider;
	bool wire;
};

RAS_MeshObject *BL_ConvertMesh(Mesh *mesh, Object *lightobj, KX_Scene *scene, BL_BlenderSceneConverter& converter, bool libloading);
void BL_ConvertDerivedMeshToArray(DerivedMesh *dm, Mesh *me, const std::vector<BL_MeshMaterial>& mats,
		const RAS_MeshObject::LayersInfo& layersInfo, std::vector<RAS_Polygon> *polygons);

void BL_ConvertBlenderObjects(Main *maggie,
							  KX_Scene *kxscene,
							  KX_KetsjiEngine *ketsjiEngine,
							  e_PhysicsEngine physics_engine,
							  RAS_Rasterizer *rendertools,
							  RAS_ICanvas *canvas,
							  BL_BlenderSceneConverter& sceneconverter,
							  bool alwaysUseExpandFraming,
							  bool libloading=false);

SCA_IInputDevice::SCA_EnumInputs ConvertKeyCode(int key_code);

#endif  /* __BL_BLENDERDATACONVERSION_H__ */
