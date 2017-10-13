/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _VERTICES_DESCRIPTION_H_
#define _VERTICES_DESCRIPTION_H_

#include "Graphics/ElementsDescription.h"

namespace Graphics
{
	// todo comments
	class VerticesDescription : public ElementsDescription
	{
	public:
		enum SEMANTICS
		{
			SEMANTIC_X, SEMANTIC_Y, SEMANTIC_Z,
			SEMANTIC_NX, SEMANTIC_NY, SEMANTIC_NZ,
			SEMANTIC_R, SEMANTIC_RED, SEMANTIC_DIFFUSE_RED,
			SEMANTIC_G, SEMANTIC_GREEN, SEMANTIC_DIFFUSE_GREEN,
			SEMANTIC_B, SEMANTIC_BLUE, SEMANTIC_DIFFUSE_BLUE,
			SEMANTIC_U, SEMANTIC_V,
			SEMANTIC_CONFIDENCE,
			SEMANTIC_VALUE,
			SEMANTIC_VIEWID0, SEMANTIC_VIEWID1, SEMANTIC_VIEWID2, SEMANTIC_VIEWID3, SEMANTIC_VIEWID4,
			SEMANTIC_VIEWID5, SEMANTIC_VIEWID6, SEMANTIC_VIEWID7, SEMANTIC_VIEWID8, SEMANTIC_VIEWID9, SEMANTIC_VIEWID10,
			SEMANTIC_UNKNOWN,
			SEMANTIC_INVALID,
			SEMANTICS_COUNT
		};
		
	public:
		static const char *const SEMANTIC_IDENTIFIERS[SEMANTICS_COUNT];

	public:
		VerticesDescription(const Encoding encoding = ENCODING_BINARY_LITTLE_ENDIAN, const uint32 vertexCount = 0);
		virtual void add(const TYPES type, const SEMANTICS semantic);
	};
}

#endif // _VERTICES_DESCRIPTION_H_
