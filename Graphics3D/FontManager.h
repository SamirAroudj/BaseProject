/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#include <vector>
#include "Platform/DataTypes.h"
#include "Font.h"
#include "MagicGraphicsConstants.h"
#include "Patterns/Singleton.h"

/// This namespace contains classes providing graphical user interface functionality, such as FontManager. 
namespace GUI
{
	/// This is the central class to load and access Fonts.
	/** Manager for fonts. Fonts are loaded, freed and accessed by means of this class.
		It loads one font upon construction in order to provide at least one font to use.*/
	class FontManager : public Patterns::Singleton<FontManager>
	{
	public:
		/** Creates a FontManager that loads one font during construction.
		@param defaultfaceName (optional) Name of the default face name of the font, e.g. Arial.
		@param defaultFontHeight (optional) The default font height, in logical units, of the font's character cell or character.*/
		FontManager(const std::wstring &defaultfaceName = DEFAULT_FACE_NAME, uint32 defaultFontHeight = DEFAULT_FONT_HEIGHT);

		/** The destructor unloads all remaining fonts. */
		virtual ~FontManager();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Return the font specified by a number. The number of a font is defined by the loading and unloading order of the fonts.
		/// 			That is, its number is equals to the number of previously loaded fonts which still exist.
		/// 			If the font was loaded as third font then its number is 2. If font number 1 is unloaded then font 2 becomes font 1. </summary>
		///
		/// <remarks>	Samir, 19.11.2011. </remarks>
		///
		/// <param name="number">	Number of the font which depends on the calls to loadFont and unloadFont. </param>
		///
		/// <returns>	The font. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const Font &getFont(uint32 number) const { assert(number < mFonts.size()); return *mFonts[number]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the first font found specified by its face name, e.g. Arial.
		/// 			This function is somewhat slower than the getFont function based on a font number due to its comfort. </summary>
		///
		/// <remarks>	Samir, 19.11.2011. </remarks>
		///
		/// <param name="faceName">	The font the face name of which is equals to faceName is searched and returned.
		/// 						A typical face name is Arial. If no proper font is found an assertion fails. </param>
		///
		/// <returns>	The font that is returned has a face name which is equal to faceName, e.g. Arial. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		const Font &getFont(const std::wstring &faceName) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads a font. To retrieve this font its face name or number must be specified.
		/// 			The font's number is equal to the number of fontswhich already exist minus one.
		/// 			If it is the first font manually loaded after the creation of the FontManager which also loads a font,
		/// 			then its number is 1. If font number 0 is unloaded then its number becomes 0. </summary>
		///
		/// <remarks>	Samir, 19.11.2011. </remarks>
		///
		/// <param name="faceName">  	Name of the face. </param>
		/// <param name="fontHeight">	(optional) height of the font. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		const Font &loadFont(const std::wstring &faceName, uint32 fontHeight = 24u);

		/** Unload a font to free resources. 
		@param number The number of the font is equal to the number of previously loaded fonts which still exist and were not unloaded. (default font inclusive). */
		void unloadFont(uint32 number);

		/** Unload a font to free resources. 
		@param faceName The first loaded font which has the font face name faceName is unloaded. */
		void unloadFont(const std::wstring &faceName);

	private:
		/** If a font is unloaded then there is no font with its particular number, gap, anymore.
			Therefore all following fonts' numbers are decreased by one. (The gap in the array containing the fonts is filled.)
		@param gap The font with the number gap was unloaded. That is why all following fonts are moved one field forward in the array containing the fonts. */
		void fillGap(uint32 gap);

		std::vector<Font *> mFonts; /// This data structure contains all Font objects that were loaded and can be used.
	};
}

#endif // _FONT_MANAGER_H_
