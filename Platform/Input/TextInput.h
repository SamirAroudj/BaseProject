/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TEXT_INPUT_H_
#define _TEXT_INPUT_H_

#include "Platform/DataTypes.h"
#include "Platform/Input/MagicConstants.h"

namespace Input
{
	/** Class for representation of user's text input. */
	class TextInput
	{
	public:
		/** Creates the text input representation. */
		TextInput();

		/** Clears the text input to its blank/initial state.
			Text length and cursor position are set to zero and the text becomes an empty string. */
		void clear();

		/** Obtains the text a user has entered so far.
		@return Returns either an empty string or the text entered by a user so far.*/
		const wchar_t *getText() const { return mText; }

		/** Obtains the cursor position.
			If the cursor is drawn then it should be rendered in front of the character which is located at the position of the cursor. 
		@return Returns the cursor position. (0 <= cursor position <= text length) */
		uint32 getCursorPosition() const { return mCursorPosition; }

		/** Obtains the length of the text the user has entered so far.
			One is added to the return value to include '\0'.
		@return Returns the number of characters a of the current text entered by a user plus one to include the appended '\0'. */
		uint32 getTextLength() const { return mTextLength + 1; }

		/** Moves the cursor to the right.
			That is, its position is incremented, unless its position is equal to the length of the text. */
		void moveCursorToTheRight();

		/** Moves cursor to the left.
			That is, its position is decremented, unless its position is zero.*/
		void moveCursorToTheLeft();

		/** Process the wide character input. Control characters are ignored.
			Wide characters are inserted at the cursor position and all characters with index >= cursor position are moved to the right.
		@param wideCharacter The wide character is inserted at the cursor position if it is not a control character. */
		void processWideCharacterInput(wchar_t wideCharacter);

		/** Removes the character before the cursor position.
			This is equals to pressing the backspace key and could be executed when a user presses backspace. */
		void removeCharacterBeforeCursorPosition();

		/** Removes the character at the cursor position.
			This equals to pressing the delete key and could be executed when a user presses delete. */
		void removeCharacterAtCursorPosition();

	private:
		wchar_t	mText[MAX_TEXT_INPUT_LENGTH + 1];	/// Stores the text the user has entered.
		uint32	mCursorPosition;					/// The cursor position indicates where the characters are inserted and what characters are removed.
		uint32	mTextLength;						/// Length of the text. This is number of characters a user has entered.
													/// Therefore, a returned string has the length mTextLength + 1 since L'\0' is appended.
	};
}

#endif // _TEXT_INPUT_H_
