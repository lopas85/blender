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
 * Contributor(s): Tristan Porteries.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#ifndef __RAS_STORAGE_VAO_H__
#define __RAS_STORAGE_VAO_H__

#include "RAS_AttributeArray.h"
#include "RAS_DisplayArray.h"

#include "GPU_glew.h"

class RAS_DisplayArrayLayout;

class RAS_StorageVao
{
public:
	RAS_StorageVao(const RAS_DisplayArrayLayout &layout, RAS_DisplayArrayStorage *arrayStorage,
				   const RAS_AttributeArray::AttribList& attribList);
	~RAS_StorageVao();

	void BindPrimitives();
	void UnbindPrimitives();

private:
	GLuint m_id;
};

#endif  // __RAS_STORAGE_VAO_H__
