/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Font.h"
#include "Graphics/GraphicsManager.h"

using namespace GUI;
using namespace std;

Font::Font(const wstring &faceName, uint32 height)
{
	assert(faceName.length() <= 31); // DirectX 10 restriction
	D3DX10_FONT_DESC fontDescription;
	fontDescription.Height = height;
	fontDescription.Width = 0;
	fontDescription.Weight = 0;
	fontDescription.MipLevels = 1;
	fontDescription.Italic = false;
	fontDescription.CharSet = DEFAULT_CHARSET;
	fontDescription.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDescription.Quality = DEFAULT_QUALITY;
	fontDescription.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy_s(fontDescription.FaceName, faceName.c_str());

	D3DX10CreateFontIndirect(Graphics::GraphicsManager::getSingleton().getDevice(), &fontDescription, &mFont);
}

Font::~Font()
{
	mFont->Release();
}


void Font::renderText(const wstring &text, int32 x, int32 y, const Graphics::Color &color, uint32 maxWidth, uint32 maxHeight, int32 numberOfCharacters) const
{
	D3DXCOLOR directXColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + maxWidth;
	rect.bottom = y + maxHeight;

	uint32 format = DT_LEFT | DT_TOP;
	if (maxWidth == 0 || maxHeight == 0)
		format |= DT_NOCLIP;
	else
		format |= DT_WORDBREAK;

	mFont->DrawTextW(NULL, text.c_str(), -1, &rect, format, directXColor);
}


const wstring Font::getFaceName() const
{
	D3DX10_FONT_DESC fontDescription;
	mFont->GetDesc(&fontDescription);
	return fontDescription.FaceName;
}