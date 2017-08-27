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

/** \file gameengine/Rasterizer/RAS_Polygon.cpp
 *  \ingroup bgerast
 */

#ifdef _MSC_VER
#  pragma warning (disable:4786)
#endif

#include "RAS_Polygon.h"
#include "RAS_IDisplayArray.h"

RAS_Polygon::RAS_Polygon(RAS_MaterialBucket *bucket, RAS_IDisplayArray *darray, unsigned int offsets[3],
		bool visible, bool collider, bool twoside)
	:m_bucket(bucket),
	m_darray(darray),
	m_polyflags(0)
{
	m_polyflags |= (visible) ? VISIBLE : 0;
	m_polyflags |= (collider) ? COLLIDER : 0;
	m_polyflags |= (twoside) ? TWOSIDE : 0;

	for (unsigned short i = 0; i < 3; ++i) {
		m_offset[i] = offsets[i];
	}
}

RAS_ITexVert *RAS_Polygon::GetVertex(unsigned short i) const
{
	return m_darray->GetVertex(m_offset[i]);
}

const RAS_TexVertInfo& RAS_Polygon::GetVertexInfo(unsigned short i) const
{
	return m_darray->GetVertexInfo(m_offset[i]);
}

unsigned int RAS_Polygon::GetVertexOffset(unsigned short i) const
{
	return m_offset[i];
}

bool RAS_Polygon::IsVisible() const
{
	return (m_polyflags & VISIBLE) != 0;
}

bool RAS_Polygon::IsCollider() const
{
	return (m_polyflags & COLLIDER) != 0;
}

bool RAS_Polygon::IsTwoside() const
{
	return (m_polyflags & TWOSIDE) != 0;
}

RAS_MaterialBucket *RAS_Polygon::GetMaterial() const
{
	return m_bucket;
}

RAS_IDisplayArray *RAS_Polygon::GetDisplayArray() const
{
	return m_darray;
}
