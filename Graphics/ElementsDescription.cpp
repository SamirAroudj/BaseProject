/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include "Graphics/ElementsDescription.h"
#include "Platform/FailureHandling/GraphicsException.h"

using namespace std;
using namespace FailureHandling;
using namespace Graphics;

const char * const ElementsDescription::TYPE_IDENTIFIERS[ElementsDescription::TYPE_COUNT] =
{
	"double", "float64",
	"float", "float32",
	"uchar", "uint8",
	"int16", "uint16",
	"int", "int32",
	"uint", "uint32",
	"invalid"
};

ElementsDescription::ElementsDescription(const Encoding encoding, const uint32 elementCount) :
	mElementCount(elementCount), mEncoding(encoding)
{

}

void ElementsDescription::add(const TYPES type, const uint32 semantic)
{
	if (type >= TYPE_COUNT)
		throw GraphicsException("Invalid type for elements description.", 215123);

	//if (semantic >= SEMANTICS_COUNT)
	//	throw GraphicsException("Invalid semantic for elements description.", 215124);

	mTypes.push_back(type);
	mSemantics.push_back(semantic);
}

