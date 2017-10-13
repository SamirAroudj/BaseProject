/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include <cassert>
#include "Graphics/VerticesDescription.h"
#include "Platform/FailureHandling/GraphicsException.h"

using namespace std;
using namespace FailureHandling;
using namespace Graphics;

const char *const VerticesDescription::SEMANTIC_IDENTIFIERS[VerticesDescription::SEMANTICS_COUNT] =
{
	"x", "y", "z",
	"nx", "ny", "nz",
	"r", "red", "diffuse_red",
	"g", "green", "diffuse_green",
	"b", "blue", "diffuse_blue",
	"u", "v",
	"confidence",
	"value",
	"viewID0", "viewID1", "viewID2", "viewID3", "viewID4", "viewID5", "viewID6", "viewID7", "viewID8", "viewID9", "viewID10",
	"unknown",
	"invalid"
};

VerticesDescription::VerticesDescription(const Encoding encoding, const uint32 vertexCount) :
	ElementsDescription(encoding, vertexCount)
{

}


void VerticesDescription::add(const TYPES type, const SEMANTICS semantic)
{
	ElementsDescription::add(type, semantic);
}
