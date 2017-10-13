/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cassert>
#include "FontManager.h"

using namespace GUI;
using namespace std;


FontManager::FontManager(const wstring &defaultfaceName, uint32 defaultFontHeight)
{
	loadFont(defaultfaceName, defaultFontHeight);
}

FontManager::~FontManager()
{
	for(uint32 i = 0; i < mFonts.size(); ++i)
		delete mFonts[i];
}

const Font &FontManager::getFont(const wstring &faceName) const
{
	for(uint32 i = 0; i < mFonts.size(); ++i)
		if (mFonts[i]->getFaceName() == faceName)
			return *mFonts[i];
	assert(false);
	return *mFonts[0];
}


const Font &FontManager::loadFont(const wstring &faceName, uint32 fontHeight)
{
	Font *newFont = new Font(faceName, fontHeight);
	mFonts.push_back(newFont);

	return *mFonts.back();
}


void FontManager::unloadFont(uint32 number)
{
	delete mFonts[number];
	fillGap(number);
}

void FontManager::unloadFont(const std::wstring &faceName)
{
	for(uint32 i = 0; i < mFonts.size(); ++i)
	{
		if (mFonts[i]->getFaceName() == faceName)
		{
			delete mFonts[i];
			fillGap(i);
			return;
		}
	}
}

void FontManager::fillGap(uint32 gap)
{
	for(uint32 i = gap; i < mFonts.size() - 1; ++i)
		mFonts[i] = mFonts[i + 1];
	mFonts.pop_back();
}