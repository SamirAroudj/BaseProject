/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "TextInput.h"

using namespace Input;

TextInput::TextInput() :
	mCursorPosition(0),
	mTextLength(0)
{
	mText[0] = L'\0';
}

void TextInput::clear()
{
	mText[0] = L'\0';
	mCursorPosition = 0;
	mTextLength = 0;
}

void TextInput::moveCursorToTheRight()
{
	if (mCursorPosition < mTextLength)
		++mCursorPosition;
}

void TextInput::moveCursorToTheLeft()
{
	if (mCursorPosition > 0)
		--mCursorPosition;
}

void TextInput::processWideCharacterInput(wchar_t wideCharacter)
{
	if ((wideCharacter < 32) || (wideCharacter == 127))	// a control character which should not be added to the text
		return;

	if (MAX_TEXT_INPUT_LENGTH == mTextLength)
		return;

	if (mCursorPosition == mTextLength)
	{
		mText[mCursorPosition] = wideCharacter;
	}
	else
	{
		for (uint32 i = mTextLength; i > mCursorPosition; --i)
			mText[i] = mText[i - 1];
		mText[mCursorPosition] = wideCharacter;
	}

	++mTextLength;
	++mCursorPosition;
	mText[mTextLength] = L'\0';
}

void TextInput::removeCharacterBeforeCursorPosition()
{
	if (0 == mCursorPosition)
		return;

	for (uint32 i = mCursorPosition - 1; i < mTextLength; ++i)
		mText[i] = mText[i + 1]; // no access violation since mText[mTextLength] == L'\0'
	--mTextLength;
	--mCursorPosition;
}

void TextInput::removeCharacterAtCursorPosition()
{
	if (mTextLength == mCursorPosition)
		return;

	for (uint32 i = mCursorPosition; i < mTextLength; ++i)
		mText[i] = mText[i + 1];
	--mTextLength;
}
