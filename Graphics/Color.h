/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _COLOR_H_
#define _COLOR_H_

#include <cassert>
#include "Math/MathCore.h"
#include "Math/Vector3.h"

namespace Graphics
{
	/// Objects of this class represents a particular color.
	/** Components that specify the color value are Red, Green, Blue and Alpha.
		The desired color can be achieved by combining the Red, Green and Blue components,
		e.g. red = 1.0f and green = 1.0f is equal to light yellow. */
	class Color
	{
	public:
		/** Linearly interpolates between c0 and c1.
			Computes c = c0 * f0 + c1 * f1 with 1.0f = f0 + f1.
		@param c0 Set this to the first color to interpolate between. It is relative to f0.
		@param c1 Set this to the second color to interpolate between. It is relative to (1.0f - f0).
		@param f0 Set this to a interpolation factor in [0, 1] whereas f0 = 0.0f means only c1 is returned and f0 = 1.0f means c0 is returned. 
		@return returns c = c0 * f0 + c1 * f1 with 1.0f = f0 + f1. */
		static Color interpolateLinearly(const Color &c0, const Color &c1, const Real f0);

	public:
		/** Creates opaque black. ([0, 0, 0, 0]) */
		Color();
		
		/** Creates a color based on a red, green, blue and alpha component.
			Reasonable values for each parameter reach from 0.0f to 1.0f.
			See additive color mixing and alpha blending to find out more.
			Sets alpha to 1.0f for an opaque color.
		@param red Specifies how much red the color contains. 0.0f = no red; 1.0f = as much red as possible.
		@param green Specifies how much green the color contains. 0.0f = no green; 1.0f = as much green as possible.
		@param blue Specifies how much blue the color contains. 0.0f = no blue; 1.0f = as much blue as possible. */
		Color(float red, float green, float blue);

		/** Creates a color based on a red, green, blue and alpha component.
			Reasonable values for each parameter reach from 0.0f to 1.0f.
			See additive color mixing and alpha blending to find out more.
		@param red Specifies how much red the color contains. 0.0f = no red; 1.0f = as much red as possible.
		@param green Specifies how much green the color contains. 0.0f = no green; 1.0f = as much green as possible.
		@param blue Specifies how much blue the color contains. 0.0f = no blue; 1.0f = as much blue as possible.
		@param alpha An alpha value of 0.0f means that this color is completely transparent. Specify 1.0f for opaque colors. */
		Color(float red, float green, float blue, float alpha);

		/** Converts the r, g, b (\in [0, 1] subset of R) color values to the hue \in [0, 2 * PI), saturation \in [0, 1] and lightness \in [0, 1] values of the color space HSL.
			This is completely independent of the alpha value of the color.
		@param H This value is set to the hue/tone defined by this object's r, g and b components. It is an angle within the interval [0, 2 * PI), units: radians. 
		@param S This value is set to the saturation/chroma/colorfulness defined by this object's r, g and b components. It is a scalar value within the interval [0, 1].
		@param L This value is set to the lightness/brightness defined by this object's r, g and b components.  It is a scalar value within the intervalu [0, 1].
		@see http://stackoverflow.com/questions/2353211/hsl-to-rgb-color-conversion
			and https://en.wikipedia.org/wiki/HSL_and_HSV */
		void convertToHSL(float &H, float &S, float &L) const;

		/** Converts the entered hue, saturation and lightess values from the HSV color space into r, g, b (\in [0, 1] subset of R) color values in the RGB color space.
			Transparency can be controlled by the fourth component called alpha.
		@param H Set this to the hue/tone of the color to be converted. h must be represented as an angle in radians within the interval [0, 2 * PI].
			Example values are: 0 -> red, 1/3 * PI -> yellow, 2/3 * PI -> green, PI -> cyan, 4/3 * PI -> blue., 5/3 * PI -> magenta
		@param S Set this to the saturation/chroma/colorfulness of the color to be converted. s must be in the interval [0, 1] subset of R with 0 -> shade of gray, 1 -> very colorful/max chroma.
		@param L Set this to the lightness/brightness of the color to be converted. l must be in the interval [0, 1] subset of R with 0 -> black, 1 -> white.
		@param alpha An alpha value of 0.0f means that this color is completely transparent. Specify 1.0f for opaque colors.
		@see http://stackoverflow.com/questions/2353211/hsl-to-rgb-color-conversion
			and https://en.wikipedia.org/wiki/HSL_and_HSV */ 
		static Color createFromHSL(float H, float S, float L, float alpha = 1.0f);

		/** Computes the polar coordinates of normal and converts these values into a hue and a lightness value and uses these values for RGB color computation.
		@param normal The azimuth angle is transformed into a HSL hue and the polar angle is transformed into a HSL lightness value.
		@param saturation Set this to the colorfulness you want. Is forwareded to createFromHSL.convertToHSL. Must be within [0, 1].
		@param alpha An alpha value of 0.0f means that this color is completely transparent. Specify 1.0f for opaque colors.
		@return Returns an RGBA color created from the hue and saturation from the normal as well as the entered alpha and saturation parameter values.
		@see Color::createFromHSL */
		static Color createFromNormal(const Math::Vector3 &normal, const Real saturation = 1.0f, const Real alpha = 1.0f);

		/** Returns how much red is contained by this color. 
		@return Returned values Range from 0.0f - no red to 1.0f as much red as possible. */
		float getRed() const { return mComponents[0]; }

		/** Returns how much green is contained by this color
		@return Returned values Range from 0.0f - no green to 1.0f as much green as possible. */ 	
		float getGreen() const { return mComponents[1]; }

		/** Returns how much blue is contained by this color.
		@return	Returned values Range from 0.0f - no blue to 1.0f as much blue as possible.*/
		float getBlue() const { return mComponents[2]; }

		/** Returns a value indicating the transparance of this color.
		@return Returned values Range from 0.0f - completely transparent to 1.0f completely opaque.*/
		float getAlpha() const { return mComponents[3]; }

		/**Gets the colour components, red, green, blue and alpha.
		@return Returns an array of four floating point values, red, green, blue and alpha in that order.*/
		const float *getComponents() const { return mComponents; }

		/** Sets the values of all color components.
			Reasonable values for each parameter reach from 0.0f to 1.0f.
			See additive color mixing and alpha blending to find out more.
		@param red Specifies how much red the color contains. 0.0f = no red; 1.0f = as much red as possible.
		@param green Specifies how much green the color contains. 0.0f = no green; 1.0f = as much green as possible.
		@param blue Specifies how much blue the color contains. 0.0f = no blue; 1.0f = as much blue as possible.
		@param alpha An alpha value of 0.0f specifies that this color is completely transparent. Specify 1.0f for opaque colors.*/
		void set(float red, float green, float blue, float alpha);

		/** Sets the value of the red component of this color.
		@param red Reasonable values Range from 0.0f - no red to 1.0f as much red as possible. **/
		void setRed(float red) { assert(checkComponent(red)); mComponents[0] = red; }

		/** Sets the value of the green component of this color.
		@param green Reasonable values Range from 0.0f - no green to 1.0f as much green as possible. */
		void setGreen(float green) { assert(checkComponent(green)); mComponents[1] = green; }

		/** Sets the value of the blue component of this color.
		@param blue Reasonable values Range from 0.0f - no blue to 1.0f as much blue as possible. */
		void setBlue(float blue) { assert(checkComponent(blue)); mComponents[2] = blue; }

		
		/** Sets the alpha value of this color to change the transparance.
		@param alpha Reasonable values Range from 0.0f - completely transparent to 1.0f completely opaque. */
		void setAlpha(float alpha) { assert(checkComponent(alpha)); mComponents[3] = alpha; }

	private:
		/** Is used by HSL to RGB color conversion.
		@param p See Color::createFromHSL function.
		@param q See Color::createFromHSL function.
		@param t See Color::createFromHSL function.
		@see createFromHSL implementation
			and http://stackoverflow.com/questions/2353211/hsl-to-rgb-color-conversion
			and https://en.wikipedia.org/wiki/HSL_and_HSV */
		static float helperVariablesToRGB(float p, float q, float t);

	private:
		/** Check the range of the colour components.
		@param red The red component must be between 0.0f and 1.0f.
		@param green The green component must be between 0.0f and 1.0f.
		@param blue The blue component must be between 0.0f and 1.0f.
		@param alpha The alpha component must be between 0.0f and 1.0f. */
		bool checkComponents(float red, float green, float blue, float alpha);

		/** Check the value of a component. Requirement 0.0f <= component <= 1.0f.
		@param componen The component is checked to have a valid range.*/
		bool checkComponent(float component);

		
		float mComponents[4]; /// The components of the colour: red, green, blue and alpha with a range from 0.0f to 1.0f whereas 1.0f means full color or completely opaque.
	};
}

#endif // _COLOR_H_
