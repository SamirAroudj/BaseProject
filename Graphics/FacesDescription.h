/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _FACES_DESCRIPTION_H_
#define _FACES_DESCRIPTION_H_

#include "Graphics/ElementsDescription.h"

namespace Graphics
{
	// todo comments
	class FacesDescription : public ElementsDescription
	{
	public:
		enum SEMANTICS
		{
			SEMANTIC_VERTEX_INDICES,
			SEMANTIC_UNKNOWN,
			SEMANTIC_INVALID,
			SEMANTICS_COUNT
		};

	public:
		static const char *const SEMANTIC_IDENTIFIERS[SEMANTICS_COUNT];

	public:
		FacesDescription(const Encoding encoding = ENCODING_BINARY_LITTLE_ENDIAN, const uint32 faceCount = 0);
		
		virtual void add(const TYPES type, const SEMANTICS semantic);
		void add(const TYPES listSizeType, const TYPES listElementType, const SEMANTICS semantic);

		inline const std::vector<ElementsDescription::TYPES> &getListSizeTypes() const;

	private:
		std::vector<TYPES> mListSizeTypes;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline & template function definitions   /////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline const std::vector<ElementsDescription::TYPES> &FacesDescription::getListSizeTypes() const
	{
		return mListSizeTypes;
	}
}

#endif // _FACES_DESCRIPTION_H_
