/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _FONT_H_
#define _FONT_H_

#include <string>
#include "Color.h"
#include "D3DX10.h"
#include "Platform/DataTypes.h"
#include "MagicGraphicsConstants.h"

namespace GUI
{
	///Objects of this class can be used to draw text on the screen.
	class Font
	{
	public:

		/** Creates a font with the face name faceName, e.g. Arial and a particular font height.
		@param faceName Set this to the name of the font's face, e.g. Arial.
		@param height The font height, in logical units, of the font's character cell or character.*/
		Font(const std::wstring &faceName, uint32 height = DEFAULT_FONT_HEIGHT);

		/** Frees requested resources. */
		~Font();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draw text the look of which is specified by this font.
		/// 			The text is drawn in the top left corner of the rectangle specified by the x, y, width and height parameters.
		/// 			If width or height is 0 than the text is not clipped otherwise it is clipped to the specified rectangle. </summary>
		///
		/// <remarks>	Samir, 19.11.2011. </remarks>
		///
		/// <param name="text">				 	This is the text which will be drawn. </param>
		/// <param name="x">				 	The x coordinate of the upper left corner of the rectangle in which the text is drawn. </param>
		/// <param name="y">				 	The y coordinate of the upper left corner of the rectangle in which the text is drawn. </param>
		/// <param name="color">			 	The color of the text to be drawn. </param>
		/// <param name="maxWidth">			 	(optional) The maximum width of the text. If width and height are not zero than the text is not drawn outside the specified rectangle. </param>
		/// <param name="maxHeight">		 	(optional) The maximum height of the text. If width and height are not zero than the text is not drawn utside the specified rectangle. </param>
		/// <param name="numberOfCharacters">	(optional) numberOfCharacters defines how many characters of the text are drawn. Set it to -1 to draw all characters until '\0' is reached. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void renderText(const std::wstring &text, int32 x, int32 y, const Graphics::Color &color, uint32 maxWidth = 0, uint32 maxHeight = 0, int32 numberOfCharacters = -1) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of the font's face, e.g. Arial. </summary>
		///
		/// <remarks>	Samir, 19.11.2011. </remarks>
		///
		/// <returns>	The name of the face of the font, e.g. Arial, which was specified during loading is returned. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		const std::wstring getFaceName() const;

	private:

		/// <summary> The DirectX10 representation of a font. </summary>
		ID3DX10Font *mFont;
	};
}

#endif // _FONT_H_
