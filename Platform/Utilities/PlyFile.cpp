/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifdef _DEBUG
	#include <iostream>
#endif // _DEBUG
#include <sstream>
#include "Graphics/VerticesDescription.h"
#include "Math/Vector2.h"
#include "Platform/FailureHandling/FileCorruptionException.h"
#include "Platform/Utilities/HelperFunctions.h"
#include "Platform/Utilities/PlyFile.h"

using namespace FailureHandling;
using namespace Graphics;
using namespace Math;
using namespace std;
using namespace Storage;
using namespace Utilities;

const char *PlyFile::DELIMETERS = " \r\n";
const char *PlyFile::HEADER_COMMENT_START = "comment";
const char *PlyFile::HEADER_ELEMENT = "element";
const char *PlyFile::HEADER_ENCODING_START = "format";
const char *PlyFile::HEADER_END_LINE = "end_header";
const char *PlyFile::HEADER_FACE = "face";
const char *PlyFile::HEADER_LIST = "list";
const char *PlyFile::HEADER_PROPERTY = "property";
const char *PlyFile::HEADER_START_LINE = "ply";
const char *PlyFile::HEADER_VERTEX = "vertex";
const char *PlyFile::HEADER_VERTEX_INDICES = "vertex_indices";

PlyFile::PlyFile(const Path &fileName, FileMode mode, bool binaryFile) :
	File(fileName, mode, binaryFile), mName(fileName)
{

}

void PlyFile::loadHeader(VerticesDescription &verticesFormat, FacesDescription *facesFormat)
{
	// temporary variables
	Encoding fileEncoding = ENCODING_BINARY_LITTLE_ENDIAN;
	bool processedFormat = false;

	#ifdef _DEBUG
		cout << "\nReading ply file header." << endl;
	#endif // _DEBUG

	readTextLine(mLineParts, mLine, DELIMETERS);
	if (mLineParts[0] != HEADER_START_LINE)
		throw FileCorruptionException("File format not recognized as ply. Should start with ply", mName);

	readTextLine(mLineParts, mLine, DELIMETERS);
	if (mLineParts.empty())
		throw FileCorruptionException("The ply header is empty!!", mName);

	while (hasLeftData())
	{
		// end of header?
		if (mLineParts[0] == HEADER_END_LINE)
			break;

		// element structure? (e.g., vertex, face)
		if (mLineParts[0] == HEADER_ELEMENT)
		{
			if (mLineParts[1] == HEADER_VERTEX)
				loadVertexStructure(verticesFormat);
			else if (facesFormat && mLineParts[1] == HEADER_FACE)
				loadFaceStructure(*facesFormat);
			continue;
		}

		// format ...?
		if (!processedFormat && mLineParts[0] == HEADER_ENCODING_START)
		{
			// How is the data encoded?
			if (mLineParts[1] == EncodingStrings[ENCODING_ASCII])
				mEncoding = ENCODING_ASCII;
			else if (mLineParts[1] == EncodingStrings[ENCODING_BINARY_LITTLE_ENDIAN])
				mEncoding = ENCODING_BINARY_LITTLE_ENDIAN;
			else if (mLineParts[1] == EncodingStrings[ENCODING_BINARY_BIG_ENDIAN])
				mEncoding = ENCODING_BINARY_BIG_ENDIAN;
			else
				throw FileCorruptionException("Could not determine ply file data encoding", mName);
			processedFormat = true;
		}
		// comment
		else if (mLineParts[0] == HEADER_COMMENT_START)
		{
			#ifdef _DEBUG
				cout << mLine.substr(8) << endl;
			#endif // _DEBUG
		}

		readTextLine(mLineParts, mLine, DELIMETERS);
	}

	#ifdef _DEBUG
		cout << "\nFinished reading of ply file header." << endl;
	#endif // _DEBUG
}

void PlyFile::loadFaceStructure(FacesDescription &structure)
{
	// set encoding
	structure.setEncoding(mEncoding);

	// read number of faces
	uint32 faceCount = Converter::to<uint32>(mLineParts[2]);
	structure.setElementCount(faceCount);

	while (hasLeftData())
	{
		// is there another face property?
		readTextLine(mLineParts, mLine, DELIMETERS);
		if (mLineParts[0] != HEADER_PROPERTY) // property
			break;

		// per face vertex index list?
		if (mLineParts[1] == HEADER_LIST) // list
			loadFaceStructureListProperty(structure);
		else // single element property
			loadFaceStructureSingleProperty(structure);
	}
}

void PlyFile::loadFaceStructureListProperty(FacesDescription &structure)
{
	// property list ...
	assert(mLineParts[0] == HEADER_PROPERTY);
	assert(mLineParts[1] == HEADER_LIST);

	// get list size type from line parts
	const uint32 sizeType = find(ElementsDescription::TYPE_IDENTIFIERS, ElementsDescription::TYPE_COUNT, mLineParts[2]);
	if (sizeType >= ElementsDescription::TYPE_COUNT)
	{
		// unclear data type in list -> um problema grande since the face property type sizes in bytes must be known
		string error = "Could not recognize list size data type in ply header mLine: ";
		error += mLine;
		throw FileCorruptionException(error, mName);
	}
		
	// get list element type from line parts
	const uint32 listElementType = find(ElementsDescription::TYPE_IDENTIFIERS, ElementsDescription::TYPE_COUNT, mLineParts[3]);
	if (listElementType >= ElementsDescription::TYPE_COUNT)
	{
		// unclear data type in list -> um problema grande since the face property type sizes in bytes must be known
		string error = "Could not recognize list element type in ply header mLine: ";
		error += mLine;
		throw FileCorruptionException(error, mName);
	}
		
	// get property semantic, e.g. vertex indices
	uint32 semantic = find(FacesDescription::SEMANTIC_IDENTIFIERS, FacesDescription::SEMANTICS_COUNT, mLineParts[4]);
	if (semantic >= FacesDescription::SEMANTIC_UNKNOWN)
	{
		semantic = FacesDescription::SEMANTIC_UNKNOWN;
		cout << "Found unsupported / unkwon face property semantic " << mLineParts[4] << endl;
	}

	// update faces description
	structure.add((ElementsDescription::TYPES) sizeType, (ElementsDescription::TYPES) listElementType, (FacesDescription::SEMANTICS) semantic);
}

void PlyFile::loadFaceStructureSingleProperty(FacesDescription &structure)
{
	// property ...
	assert(mLineParts[0] == HEADER_PROPERTY);

	// get element type from line parts
	const uint32 type = find(ElementsDescription::TYPE_IDENTIFIERS, ElementsDescription::TYPE_COUNT, mLineParts[1]);
	if (type >= ElementsDescription::TYPE_COUNT)
	{
		// unclear data type -> um problema grande since the face property type size in bytes must be known
		string error = "Could not recognize the type of a single type face property in ply header mLine: ";
		error += mLine;
		throw FileCorruptionException(error, mName);
	}
		
	// get property semantic, e.g. flags
	uint32 semantic = find(FacesDescription::SEMANTIC_IDENTIFIERS, FacesDescription::SEMANTICS_COUNT, mLineParts[2]);
	if (semantic >= FacesDescription::SEMANTIC_UNKNOWN)
	{
		semantic = FacesDescription::SEMANTIC_UNKNOWN;
		cout << "Found unsupported / unkwon semantic for a single type property for faces: " << mLineParts[2] << endl;
	}

	// update faces description
	structure.add((ElementsDescription::TYPES) type, (FacesDescription::SEMANTICS) semantic);
}

void PlyFile::loadVertexStructure(VerticesDescription &structure)
{
	// set encoding
	structure.setEncoding(mEncoding);

	// load vertex count
	uint32 vertexCount = Converter::to<uint32>(mLineParts[2]);
	structure.setElementCount(vertexCount);

	while (hasLeftData())
	{
		// is there another vertex property?
		readTextLine(mLineParts, mLine, DELIMETERS);
		if (mLineParts[0] != HEADER_PROPERTY)
			break;

		// single element vertex property
		// get data type from line parts
		const uint32 type = find(ElementsDescription::TYPE_IDENTIFIERS, ElementsDescription::TYPE_COUNT, mLineParts[1]);
		if (type >= ElementsDescription::TYPE_COUNT)
		{
			// unclear data type in vertex format -> grande problema since the vertex property type size in bytes must be known
			string error = "Could not recognize data type in ply header mLine: ";
			error += mLine;
			throw FileCorruptionException(error, mName);
		}

		// get property semantic, e.g. x-component of normal
		uint32 semantic = find(VerticesDescription::SEMANTIC_IDENTIFIERS, VerticesDescription::SEMANTICS_COUNT, mLineParts[2]);
		if (VerticesDescription::SEMANTIC_UNKNOWN <= semantic)
		{
			semantic = VerticesDescription::SEMANTIC_UNKNOWN;
			cout << "Found unsupported / unkwon vertex property semantic " << mLineParts[2] << endl;
		}

		// update vertices description
		structure.add((ElementsDescription::TYPES) type, (VerticesDescription::SEMANTICS) semantic);
	}
}

void PlyFile::loadTriangles(vector<uint32> &indices, const FacesDescription &facesFormat)
{
	// get format
	const ElementsSyntax &listSizeTypes = facesFormat.getListSizeTypes();
	const ElementsSyntax &types = facesFormat.getTypeStructure();
	const ElementsSemantics &semantics = facesFormat.getSemantics();
	const uint32 faceCount = facesFormat.getElementCount();
	const uint32 propertyCount = facesFormat.getPropertyCount();

	// reserve memory
	const uint32 maxTriangles = faceCount * 3 * 2; // file potentially contains quads
	indices.reserve(maxTriangles);

	// read each face
	for (uint32 faceIdx = 0; faceIdx < faceCount; ++faceIdx)
	{
		// read each face property
		for (uint32 propertyIdx = 0; propertyIdx < propertyCount; ++propertyIdx)
		{
			// some list?
			const ElementsDescription::TYPES listSizeType = listSizeTypes[propertyIdx];
			if (ElementsDescription::TYPE_INVALID == listSizeType)
				readFaceSingleProperty(types[propertyIdx], semantics[propertyIdx]);
			else
				readFaceListProperty(indices, listSizeType, types[propertyIdx], semantics[propertyIdx]);
		}
	}
}

void PlyFile::readFaceListProperty(vector<uint32> &indices,
	const ElementsDescription::TYPES listSizeType, const ElementsDescription::TYPES type, const uint32 semantic)
{
	// read & check list size
	uint32 listSize = (uint32) read<uint32>(listSizeType);
	if (listSize > 4 || listSize < 3)
		throw FileCorruptionException("Unsupported index list size for a face.", mName);

	// read all indices of the face
	for (uint32 listElementIdx = 0; listElementIdx < listSize; ++listElementIdx)
	{
		// read next index with possibly preceeding white space
		if (ENCODING_ASCII == mEncoding)
		{
			char c;
			File::read(&c, 1, 1, 1);
			if (' ' != c && '\0' != c && ',' != c && ';' != c)
				throw FileCorruptionException("Invalid face vertex index list format!", mName);
		}

		const uint32 newVertexIndex = read<uint32>(type);
		
		// simple triangle?
		if (listElementIdx < 3)
		{
			indices.push_back(newVertexIndex);
		}
		else // convert quad to two triangles, keep winding order
		{
			// add second quad triangle (first one was already added)
			const uint32 indexCount = (uint32) indices.size();
			const uint32 index0 = indices[indexCount - 3];
			const uint32 index2 = indices[indexCount - 1];
			const uint32 index3 = newVertexIndex;

			indices.push_back(index0);
			indices.push_back(index2);
			indices.push_back(index3);
		}
	}
}

void PlyFile::readFaceSingleProperty(const ElementsDescription::TYPES type, const uint32 semantic)
{
	// single properties of faces are ignored
	char trash[32];
	read(trash, type);
}

void PlyFile::read(void *destination, const ElementsDescription::TYPES type)
{
	if (ElementsDescription::TYPE_INVALID == type)
		return;
	else if (type >= ElementsDescription::TYPE_INT)
		*((int32 *) destination) = readInt32(mEncoding);
	else if (type >= ElementsDescription::TYPE_UCHAR)
		*((uint8 *) destination) = readByte(mEncoding);
	else if (type >= ElementsDescription::TYPE_FLOAT)
		*((Real *) destination) = (Real) readFloat(mEncoding);
	else
		*((Real *) destination) = (Real) readDouble(mEncoding);
}


void PlyFile::readVertexProperty(Vector3 *color, Vector3 *normal, Vector3 &position, Vector2 *uvCoords,
	Real *confidence, Real *value, uint32 *viewIDs,
	const ElementsDescription::TYPES type, const VerticesDescription::SEMANTICS semantic)
{
	uint8 temp[128];
	Real *tempReal = (Real *) temp;
	uint32 *tempUint32 = (uint32 *) temp;

	switch (semantic)
	{
		case VerticesDescription::SEMANTIC_X: case VerticesDescription::SEMANTIC_Y:	case VerticesDescription::SEMANTIC_Z:
		{
			// read data and directly store it in position
			const Axis axis = (Axis) (semantic - VerticesDescription::SEMANTIC_X);
			position[axis] = read<Real>(type);
			return;
		}

		case VerticesDescription::SEMANTIC_NX: case VerticesDescription::SEMANTIC_NY: case VerticesDescription::SEMANTIC_NZ:
		{
			*tempReal = read<Real>(type);
			if (normal)
			{
				const Axis axis = (Axis) (semantic - VerticesDescription::SEMANTIC_NX);
				(*normal)[axis] = *tempReal;
			}
			return;
		}

		case VerticesDescription::SEMANTIC_R: case VerticesDescription::SEMANTIC_RED: case VerticesDescription::SEMANTIC_DIFFUSE_RED:
		case VerticesDescription::SEMANTIC_G: case VerticesDescription::SEMANTIC_GREEN: case VerticesDescription::SEMANTIC_DIFFUSE_GREEN:
		case VerticesDescription::SEMANTIC_B: case VerticesDescription::SEMANTIC_BLUE: case VerticesDescription::SEMANTIC_DIFFUSE_BLUE:
		{
			const Axis axis = (Axis) ((semantic - VerticesDescription::SEMANTIC_R) / 3);

			switch (type)
			{
				case ElementsDescription::TYPE_DOUBLE: case ElementsDescription::TYPE_FLOAT64:
				case ElementsDescription::TYPE_FLOAT: case ElementsDescription::TYPE_FLOAT32:
				{
					*tempReal = read<Real>(type);
					if (color)
						(*color)[axis] = *tempReal;
					return;
				}
							
				case ElementsDescription::TYPE_UCHAR: case ElementsDescription::TYPE_UINT8:
				case ElementsDescription::TYPE_INT16: case ElementsDescription::TYPE_UINT16:
				case ElementsDescription::TYPE_INT: case ElementsDescription::TYPE_INT32:
				case ElementsDescription::TYPE_UINT: case ElementsDescription::TYPE_UINT32:
				{
					*tempUint32 = read<uint32>(type);
					if (color)
						(*color)[axis] = *tempUint32 / 255.0f;
					return;
				}

				default:
					assert(false);
					throw FileCorruptionException("Unsupported vertex color component type.", mName);
			}
		}

		case VerticesDescription::SEMANTIC_U: case VerticesDescription::SEMANTIC_V:
		{
			*tempReal = read<Real>(type);
			if (uvCoords)
			{
				const Axis axis = (Axis) (semantic - VerticesDescription::SEMANTIC_U);
				(*uvCoords)[axis] = *tempReal;
			}
			return;
		}

		case VerticesDescription::SEMANTIC_CONFIDENCE:
		{
			*tempReal = read<Real>(type);
			if (confidence)
				*confidence = *tempReal;
			return;
		}

		case VerticesDescription::SEMANTIC_VALUE:
		{
			*tempReal = read<Real>(type);
			if (value)
				*value = *tempReal;
			return;
		}

		case VerticesDescription::SEMANTIC_VIEWID0: case VerticesDescription::SEMANTIC_VIEWID1: case VerticesDescription::SEMANTIC_VIEWID2: 
		case VerticesDescription::SEMANTIC_VIEWID3: case VerticesDescription::SEMANTIC_VIEWID4:	case VerticesDescription::SEMANTIC_VIEWID5: 
		case VerticesDescription::SEMANTIC_VIEWID6: case VerticesDescription::SEMANTIC_VIEWID7: case VerticesDescription::SEMANTIC_VIEWID8: 
		case VerticesDescription::SEMANTIC_VIEWID9: case VerticesDescription::SEMANTIC_VIEWID10:
		{
			*tempUint32 = read<uint32>(type);
			if (viewIDs)
				viewIDs[semantic - VerticesDescription::SEMANTIC_VIEWID0] = *tempUint32;
			return;
		}

		case VerticesDescription::SEMANTIC_UNKNOWN:
		{
			// read and discard whatever this is
			*tempReal = read<Real>(type);
			return;
		}

		case VerticesDescription::SEMANTIC_INVALID: case VerticesDescription::SEMANTICS_COUNT:	default:
		{
			throw FileCorruptionException("Could not read vertex property due to unsupported semantic.", mName);
		}
	}
}

void PlyFile::saveTriangleMesh(const Encoding encoding, const bool toSinglePrecision, const uint32 vertexCount, const uint32 indexCount,
	const Vector3 *colors, const Vector3 *normals, const Vector3 *positions,
	const Real *confidences, const Real *values, const uint32 *viewIDs, const uint32 viewsPerVertex,
	const uint32 *indices)
{
	const ElementsDescription::TYPES outputPrecision = (toSinglePrecision ? ElementsDescription::TYPE_FLOAT : ElementsDescription::TYPE_REAL);

	saveTriangleMeshHeader(encoding, outputPrecision,
		vertexCount, indexCount, colors, normals, positions, 
		confidences, values, viewIDs, viewsPerVertex, indices);
	
	saveVertices(encoding, outputPrecision,
		vertexCount, colors, normals, positions, 
		confidences, values, viewIDs, viewsPerVertex);

	saveTriangles(encoding, indexCount, indices);
}

void PlyFile::saveTriangleMeshHeader(const Encoding encoding, const ElementsDescription::TYPES outputPrecision,
	const uint32 vertexCount, const uint32 indexCount, const Vector3 *colors, const Vector3 *normals, const Vector3 *positions,
	const Real *values, const Real *confidences, const uint32 *viewIDs, const uint32 viewsPerVertex,
	const uint32 *indices)
{
	ostringstream stream;

	// ply file identifier
	stream << HEADER_START_LINE << endl;
	
	// encoding
	stream << HEADER_ENCODING_START << " " << EncodingStrings[encoding] << " 1.0" << endl;
	
	// start description of vertices
	stream << HEADER_ELEMENT << " " << HEADER_VERTEX << " " << vertexCount << endl; 
	
	// each vertex has:
	// a position
	assert(positions);
	stream << HEADER_PROPERTY << " " <<
		ElementsDescription::TYPE_IDENTIFIERS[outputPrecision] << " " << VerticesDescription::SEMANTIC_IDENTIFIERS[VerticesDescription::SEMANTIC_X] << endl;
	stream << HEADER_PROPERTY << " " <<
		ElementsDescription::TYPE_IDENTIFIERS[outputPrecision] << " " << VerticesDescription::SEMANTIC_IDENTIFIERS[VerticesDescription::SEMANTIC_Y] << endl;
	stream << HEADER_PROPERTY << " " << 
		ElementsDescription::TYPE_IDENTIFIERS[outputPrecision] << " " << VerticesDescription::SEMANTIC_IDENTIFIERS[VerticesDescription::SEMANTIC_Z] << endl;
	
	// a color?
	if (colors)
	{
		stream << HEADER_PROPERTY << " " <<
			ElementsDescription::TYPE_IDENTIFIERS[ElementsDescription::TYPE_UCHAR] << " " << VerticesDescription::SEMANTIC_IDENTIFIERS[VerticesDescription::SEMANTIC_RED] << endl;
		stream << HEADER_PROPERTY << " " <<
			ElementsDescription::TYPE_IDENTIFIERS[ElementsDescription::TYPE_UCHAR] << " " << VerticesDescription::SEMANTIC_IDENTIFIERS[VerticesDescription::SEMANTIC_GREEN] << endl;
		stream << HEADER_PROPERTY << " " << 
			ElementsDescription::TYPE_IDENTIFIERS[ElementsDescription::TYPE_UCHAR] << " " << VerticesDescription::SEMANTIC_IDENTIFIERS[VerticesDescription::SEMANTIC_BLUE] << endl;
	}
	
	// a normal?
	if (normals)
	{
		stream << HEADER_PROPERTY << " " <<
			ElementsDescription::TYPE_IDENTIFIERS[outputPrecision] << " " << VerticesDescription::SEMANTIC_IDENTIFIERS[VerticesDescription::SEMANTIC_NX] << endl;
		stream << HEADER_PROPERTY << " " <<
			ElementsDescription::TYPE_IDENTIFIERS[outputPrecision] << " " << VerticesDescription::SEMANTIC_IDENTIFIERS[VerticesDescription::SEMANTIC_NY] << endl;
		stream << HEADER_PROPERTY << " " << 
			ElementsDescription::TYPE_IDENTIFIERS[outputPrecision] << " " << VerticesDescription::SEMANTIC_IDENTIFIERS[VerticesDescription::SEMANTIC_NZ] << endl;
	}

	// confidence?
	if (confidences)
	{
		stream << HEADER_PROPERTY << " " <<		
			ElementsDescription::TYPE_IDENTIFIERS[outputPrecision] << " " << VerticesDescription::SEMANTIC_IDENTIFIERS[VerticesDescription::SEMANTIC_CONFIDENCE] << endl;
	}

	// some value?
	if (values)
	{
		stream << HEADER_PROPERTY << " " <<
			ElementsDescription::TYPE_IDENTIFIERS[outputPrecision] << " " << VerticesDescription::SEMANTIC_IDENTIFIERS[VerticesDescription::SEMANTIC_VALUE] << endl;
	}

	// view ID links for vertices?
	if (viewIDs)
	{
		for (uint32 viewIdx = 0; viewIdx < viewsPerVertex; ++viewIdx)
		{
			const uint32 semanticIdx = VerticesDescription::SEMANTIC_VIEWID0 + viewIdx;
			stream << HEADER_PROPERTY << " " <<
				ElementsDescription::TYPE_IDENTIFIERS[ElementsDescription::TYPE_INT] << " " << VerticesDescription::SEMANTIC_IDENTIFIERS[semanticIdx] << endl;
		}
	}
	
	// start description of triangles
	if (indices)
	{
		const uint32 triangleCount = indexCount / 3;
		stream << HEADER_ELEMENT << " " << HEADER_FACE << " " << triangleCount << endl;
		stream << HEADER_PROPERTY << " " << HEADER_LIST << " " <<
			ElementsDescription::TYPE_IDENTIFIERS[ElementsDescription::TYPE_UCHAR] << " " << 
			ElementsDescription::TYPE_IDENTIFIERS[ElementsDescription::TYPE_INT] << " " <<
			HEADER_VERTEX_INDICES << endl;
	}
	
	stream << HEADER_END_LINE << endl;
	
	// write header to file
	string headerString = stream.str();
	write(headerString.c_str(), sizeof(char), headerString.size());
}

void PlyFile::saveVertices(const Encoding encoding, const ElementsDescription::TYPES outputPrecision,
	const uint32 vertexCount, const Vector3 *colors, const Vector3 *normals, const Vector3 *positions,
	const Real *confidences, const Real *values, const uint32 *viewIDs, const uint32 viewsPerVertex)
{
	const bool singlePrecision = ((ElementsDescription::TYPE_FLOAT ==  outputPrecision) || (ElementsDescription::TYPE_FLOAT32 == outputPrecision));

	// get pointers to go through all vertices
	const Vector3 *position = positions;
	const Vector3 *color = colors;
	const Vector3 *normal = normals;
	const Real *confidence = confidences;
	const Real *value = values;
	const uint32 *viewID = viewIDs;
	const char space = ' ';

	// output all vertices
	for (uint32 vertexIdx = 0; vertexIdx < vertexCount; ++vertexIdx)
	{
		// write vertex position, color & normal
		for (Axis axis = AXIS_X; axis <= AXIS_Z; axis = (Axis) (axis + 1))
		{
			const Real temp = (*position)[axis];
			if (singlePrecision)
				writeFloat((float) temp, encoding);
			else
				writeDouble(temp, encoding);

			if (ENCODING_ASCII == encoding)
				write(&space, 1, 1);
		}
		++position;
		
		if (color)
		{
			for (Axis axis = AXIS_X; axis <= AXIS_Z; axis = (Axis) (axis + 1))
			{
				const uint8 temp = (uint8) roundr((*color)[axis] * 255);
				writeUInt8(temp, encoding);

				if (ENCODING_ASCII == encoding)
					write(&space, 1, 1);
			}
			++color;
		}
		
		if (normal)
		{
			for (Axis axis = AXIS_X; axis <= AXIS_Z; axis = (Axis) (axis + 1))
			{
				const Real temp = (*normal)[axis];
				if (singlePrecision)
					writeFloat((float) temp, encoding);
				else
					writeDouble(temp, encoding);

				if (ENCODING_ASCII == encoding)
					write(&space, 1, 1);
			}
			++normal;
		}

		if (confidence)
		{
			const Real temp = *confidence;
			if (singlePrecision)
				writeFloat((float) temp, encoding);
			else
				writeDouble(temp, encoding);

			if (ENCODING_ASCII == encoding)
				write(&space, 1, 1);
			++confidence;
		}

		if (value)
		{
			const Real temp = *value;
			if (singlePrecision)
				writeFloat((float) temp, encoding);
			else
				writeDouble(temp, encoding);

			if (ENCODING_ASCII == encoding)
				write(&space, 1, 1);
			++value;
		}

		if (viewID)
		{
			for (uint32 viewIdx = 0; viewIdx < viewsPerVertex; ++viewIdx)
			{
				writeInt32(*viewID, encoding);
				if (ENCODING_ASCII == encoding)
					write(&space, 1, 1);
				++viewID;
			}
		}
		
		if (ENCODING_ASCII == encoding)
			write("\r\n", 1, 2);
	}
	
	position = NULL;
	color = NULL;
	normal = NULL;
	confidence = NULL;
	value = NULL;
	viewID = NULL;
}

void PlyFile::saveTriangles(const Encoding encoding, const uint32 indexCount, const uint32 *indices)
{
	const uint8 verticesPerTriangle = 3;
	const uint32 triangleCount = indexCount / verticesPerTriangle;
	const uint32 *triangle = indices;
	const char space = ' ';

	// write for each triangle: 3, index0, index1, index2
	for (uint32 triangleIdx = 0; triangleIdx < triangleCount; ++triangleIdx)
	{
		writeUInt8(verticesPerTriangle, encoding);
		if (ENCODING_ASCII == encoding)
			write(&space, 1, 1);

		for (uint32 cornerIdx = 0; cornerIdx < verticesPerTriangle; ++cornerIdx)
		{
			writeInt32(triangle[cornerIdx], encoding);
			if (ENCODING_ASCII == encoding)
				if (cornerIdx < verticesPerTriangle - 1)
					write(&space, 1, 1);
		}
		if (ENCODING_ASCII == encoding)
			write("\r\n", 1, 2);

		triangle += 3;
	}
	
	triangle = NULL;
}
