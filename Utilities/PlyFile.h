/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _PLY_FILE_H_
#define _PLY_FILE_H_

#include "Graphics/FacesDescription.h"
#include "Graphics/VerticesDescription.h"
#include "Math/Vector3.h"
#include "Platform/FailureHandling/FileCorruptionException.h"
#include "Platform/Storage/File.h"

namespace Utilities
{
	/// This class should be used for loading and saving of ply files.
	/** The class is used to process ply file data. */
	class PlyFile : public Storage::File
	{
	public:
		/** Opens or creates a specific ply file.
		@param fileName The name and path of the file the access of which is requested.
		@param mode Defines the way the file is used, corresponds to the file mode options of fopen. See FileMode.
		@param binaryFile If binaryFile is true than the file is opened as binary file
						  otherwise it is opened as text file (some characters are treated in a special way.) */
		PlyFile(const Storage::Path &fileName, FileMode mode, bool binaryFile);

		/** todo */
		inline Encoding getEncoding() const;

		/** todo */
		void loadHeader(Graphics::VerticesDescription &verticesFormat, Graphics::FacesDescription *facesFormat = NULL);

		/** todo */
		void loadTriangles(std::vector<uint32> &indices, const Graphics::FacesDescription &facesFormat);

		/** todo */
		template <class T>
		T read(const Graphics::ElementsDescription::TYPES type);

		/** todo */
		void read(void *destination, const Graphics::ElementsDescription::TYPES type);

		void readVertexProperty(Math::Vector3 *color, Math::Vector3 *normal, Math::Vector3 &position, Math::Vector2 *uvCoords,
			Real *confidence, Real *value, uint32 *viewIDs,
			const Graphics::ElementsDescription::TYPES type, const Graphics::VerticesDescription::SEMANTICS semantic);

		/** todo */
		void saveTriangleMesh(const Encoding encoding, const bool toSinglePrecision, const uint32 vertexCount, const uint32 indexCount,
			const Math::Vector3 *colors, const Math::Vector3 *normals, const Math::Vector3 *positions,
			const Real *confidences, const Real *values, const uint32 *viewIDs, const uint32 viewsPerVertex,
			const uint32 *indices);
		
	private:
		/** todo */
		void loadFaceStructure(Graphics::FacesDescription &structure);

		/** todo */
		void loadVertexStructure(Graphics::VerticesDescription &structure);

		/** todo */
		void readFaceProperty(std::vector<uint32> &indices,
			const Graphics::ElementsDescription::TYPES listSizeType, const Graphics::ElementsDescription::TYPES  type, const uint32 semantic);

		/** todo */
		void saveTriangleMeshHeader(const Encoding encoding, const Graphics::ElementsDescription::TYPES outputPrecision,
			const uint32 vertexCount, const uint32 indexCount,
			const Math::Vector3 *colors, const Math::Vector3 *normals, const Math::Vector3 *positions,
			const Real *confidences, const Real *values, const uint32 *viewIDs, const uint32 viewsPerVertex,
			const uint32 *indices);
		
		/** todo */
		void saveVertices(const Encoding encoding, const Graphics::ElementsDescription::TYPES outputPrecision,
			const uint32 vertexCount, const Math::Vector3 *colors, const Math::Vector3 *normals, const Math::Vector3 *positions,
			const Real *confidences, const Real *values, const uint32 *viewIDs, const uint32 viewsPerVertex);
		void saveTriangles(const Encoding encoding, const uint32 indexCount, const uint32 *indices);
		
	private:
		static const char *DELIMETERS;				/// Define where to split ply file lines into parts. E.g. "property float x"\n -> {property,float,x}
		static const char *HEADER_COMMENT_START;	/// Each comment in the header begins like this.
		static const char *HEADER_ELEMENT;			/// Element (e.g. vertex) identifier for ply file headers.
		static const char *HEADER_ENCODING_START;	/// Each format or encoding description begins like this.
		static const char *HEADER_END_LINE;			/// This is what the last ply file header line should contain.
		static const char *HEADER_FACE;				/// Face identifierr. Used for the beginning of the header part concerning the triangles / faces description.
		static const char *HEADER_LIST;				/// List keyword used to state that a list is following.
		static const char *HEADER_PROPERTY;			/// Property identifier. For example, it is used to add a normal x component to a vertex.
		static const char *HEADER_START_LINE;		/// This is always the first line in a ply file.
		static const char *HEADER_VERTEX;			/// Vertex identifier. E.g., it is used to begin the vertex structure description in the header.
		static const char *HEADER_VERTEX_INDICES;	/// Vertex indices identifier. Used to say that vertex indices are in the file.
		
	private:
		std::vector<std::string> mLineParts;	/// Used to split a ply file line into subparts. Subparts can be processed more easily.
		std::string mLine;						/// Used to store a single ply file line.
		Storage::Path mName;					/// Contains the ply file name for possible exceptions.
		Encoding mEncoding;						/// Defines how data is encoded in the disk file, see DataTypes.h::enum Encoding.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline & template function definitions   /////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline Encoding PlyFile::getEncoding() const
	{
		return mEncoding;
	}

	template <class T>
	T PlyFile::read(const Graphics::ElementsDescription::TYPES type)
	{
		switch (type)
		{
			case Graphics::ElementsDescription::TYPE_DOUBLE:
			case Graphics::ElementsDescription::TYPE_FLOAT64:
				return (T) readDouble(mEncoding);

			case Graphics::ElementsDescription::TYPE_FLOAT:
			case Graphics::ElementsDescription::TYPE_FLOAT32:
				return (T) readFloat(mEncoding);

			case Graphics::ElementsDescription::TYPE_UCHAR:
			case Graphics::ElementsDescription::TYPE_UINT8:
				return (T) readUInt8(mEncoding);

			case Graphics::ElementsDescription::TYPE_INT16:
				return (T) readInt16(mEncoding);

			case Graphics::ElementsDescription::TYPE_UINT16:
				return (T) readUInt16(mEncoding);
				
			case Graphics::ElementsDescription::TYPE_INT:
			case Graphics::ElementsDescription::TYPE_INT32:
				return (T) readInt32(mEncoding);

			case Graphics::ElementsDescription::TYPE_UINT:
			case Graphics::ElementsDescription::TYPE_UINT32:
				return (T) readUInt32(mEncoding);

			case Graphics::ElementsDescription::TYPE_INVALID:
			case Graphics::ElementsDescription::TYPE_COUNT:
			default:
				throw FailureHandling::FileCorruptionException("Unsupported ElementsDescription type.", mName);
		}

		return (T) ((int64) (-1));
	}
}

#endif // _PLY_FILE_
