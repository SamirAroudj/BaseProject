/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include <cassert>
#include "Graphics/FacesDescription.h"
#include "Platform/FailureHandling/GraphicsException.h"

using namespace std;
using namespace FailureHandling;
using namespace Graphics;

const char *const FacesDescription::SEMANTIC_IDENTIFIERS[FacesDescription::SEMANTICS_COUNT] =
{
	"vertex_indices",
	"flags",
	"unknown",
	"invalid"
};

FacesDescription::FacesDescription(const Encoding encoding, const uint32 faceCount) :
	ElementsDescription(encoding, faceCount)
{
}

void FacesDescription::add(const TYPES type, const SEMANTICS semantic)
{
	mListSizeTypes.push_back(ElementsDescription::TYPE_INVALID);
	ElementsDescription::add(type, semantic);
}

void FacesDescription::add(const TYPES listSizeType, const TYPES listElementType, const SEMANTICS semantic)
{
	mListSizeTypes.push_back(listSizeType);
	ElementsDescription::add(listElementType, semantic);
}
