/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
#include <Windows.h>
#endif // _WINDOWS
#include <GL/gl.h>

#include "Graphics/MagicConstants.h"
#include "Math/Matrix4x4.h"
#include "Graphics/Sprite.h"

using namespace Graphics;
using namespace Math;
using namespace std;
using namespace Utilities;

Sprite::Sprite(const string &textureName, uint32 tileIndex) :
	mTexture(Texture::request(textureName)),
	mPivot(0.0f, 0.0f),
	mPosition(0.0f, 0.0f),
	mScaling(1.0f, 1.0f),
	mAngle(0.0f),
	mDepth(0.0f)
{
	setTileIndex(tileIndex);
}

Sprite::~Sprite()
{
	if (mTexture)
		Texture::release(mTexture);
}

void Sprite::render() const
{
	mTexture->bind();

	// compute the sprite transformation matrix
	Matrix4x4 transformation;
	transformation.m00 = mScaling.x;
	transformation.m11 = mScaling.y;
	transformation.m30 = mScaling.x * -mPivot.x;
	transformation.m31 = mScaling.y * -mPivot.y;

	Matrix4x4 rotationZ = Matrix4x4::createRotationZ(mAngle);

	Matrix4x4 result = transformation * rotationZ;
	result.m30 += mPosition.x;
	result.m31 += mPosition.y;

	const Size2<Real> &tileSize =  mTexture->getTileSize();

	glPushMatrix();
	glMultMatrixr(result.getData());
	
	// simple quad which is transformed
	glBegin(GL_QUADS);			
		// bottom left
		glTexCoord2r(mTileX, mTileY);															
		glVertex3r(-0.5f * SPRITE_WIDTH, -0.5f * SPRITE_HEIGHT, mDepth);

		// bottom right
		glTexCoord2r(mTileX + tileSize[0], mTileY);								
		glVertex3r(0.5f * SPRITE_WIDTH, -0.5f * SPRITE_HEIGHT, mDepth);

		// top right
		glTexCoord2r(mTileX + tileSize[0], mTileY + tileSize[1]);	
		glVertex3r(0.5f * SPRITE_WIDTH, 0.5f * SPRITE_HEIGHT, mDepth);

		// top left
		glTexCoord2r(mTileX, mTileY + tileSize[1]);								
		glVertex3r(-0.5f * SPRITE_WIDTH, 0.5f * SPRITE_HEIGHT, mDepth);
	glEnd();

	glPopMatrix();
}

void Sprite::setTileIndex(uint32 index)
{
	const Size2<Real> &tileSize = mTexture->getTileSize();
	// find coordinates of the indexed tile in the grid layout
	// values are rounded down on purpose
	const uint32 rowLength = static_cast<uint32>(1.0f / tileSize[0]);		
	const uint32 columnLength = static_cast<uint32>(1.0f / tileSize[1]);	
	const uint32 rowIndex = index % rowLength;
	const uint32 columnIndex = index / columnLength;

	mTileX = tileSize[0] * rowIndex;
	mTileY = tileSize[1] * columnIndex;
}

