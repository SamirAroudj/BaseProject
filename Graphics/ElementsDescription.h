/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _ELEMENTS_DESCRIPTION_H_
#define _ELEMENTS_DESCRIPTION_H_

#include <string>
#include <vector>
#include "Platform/DataTypes.h"

namespace Graphics
{
	class ElementsDescription;

	// todo comments
	class ElementsDescription
	{
	public:
		enum TYPES
		{
			TYPE_DOUBLE, TYPE_FLOAT64,
			TYPE_FLOAT, TYPE_FLOAT32,
			TYPE_UCHAR, TYPE_UINT8,
			TYPE_INT16, TYPE_UINT16,
			TYPE_INT, TYPE_INT32,
			TYPE_UINT, TYPE_UINT32,
			TYPE_INVALID,
			TYPE_COUNT
		};

		// Define TYPE_REAL
		#ifdef DOUBLE_PRECISION
			static const TYPES TYPE_REAL = TYPE_FLOAT64;
		#else
			static const TYPES TYPE_REAL = TYPE_FLOAT32;
		#endif

	public:
		static const char * const TYPE_IDENTIFIERS[TYPE_COUNT];

	public:
		ElementsDescription(const Encoding encoding = ENCODING_BINARY_LITTLE_ENDIAN, const uint32 elementCount = 0);

		virtual void add(const TYPES type, const uint32 semantic);

		inline Encoding getEncoding() const;
		inline const std::vector<uint32> &getSemantics() const;
		inline const std::vector<TYPES> &getTypeStructure() const;
		inline uint32 getElementCount() const;

		inline void setElementCount(const uint32 elementCount);
		inline void setEncoding(const Encoding encoding);

	protected:
		std::vector<uint32> mSemantics;	/// Describes the intended usage / semantic of an element.
		std::vector<TYPES> mTypes;		/// Describes the syntax, in particular, types of an element.
		uint32 mElementCount;			/// Defines how many properties each element has for which this description is used.
		Encoding mEncoding;				/// Defines how integers are encoded.
	};

	typedef std::vector<uint32> ElementsSemantics;
	typedef std::vector<ElementsDescription::TYPES> ElementsSyntax;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline & template function definitions   /////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline Encoding ElementsDescription::getEncoding() const
	{
		return mEncoding;
	}

	inline const std::vector<uint32> &ElementsDescription::getSemantics() const
	{
		return mSemantics;
	}

	inline const std::vector<ElementsDescription::TYPES> &ElementsDescription::getTypeStructure() const
	{
		return mTypes;
	}

	inline uint32 ElementsDescription::getElementCount() const
	{
		return mElementCount;
	}

	inline void ElementsDescription::setElementCount(const uint32 elementCount)
	{
		mElementCount = elementCount;
	}

	inline void ElementsDescription::setEncoding(const Encoding encoding)
	{
		mEncoding = encoding;
	}
}

#endif // _LEMENTS_DESCRIPTION_H_
