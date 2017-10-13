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

namespace Graphics
{
	/// Objects of this class represents a particular color.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Components that specify the color value are Red, Green, Blue and Alpha.
	///				The desired color can be achieved by combining the Red, Green and Blue components,
	///				e.g. red = 1.0f and green = 1.0f is equal to light yellow. </summary>
	///
	/// <remarks>	Samir, 18.11.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Color
	{
	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a color based on a red, green, blue and alpha component.
		/// 			Reasonable values for each parameter reach from 0.0f to 1.0f.
		/// 			See additive color mixing and alpha blending to find out more. </summary>
		///
		/// <remarks>	Samir Aroudj, 18.11.2011. </remarks>
		///
		/// <param name="red">  	Specifies how much red the color contains. 0.0f = no red; 1.0f = as much red as possible. </param>
		/// <param name="green">	Specifies how much green the color contains. 0.0f = no green; 1.0f = as much green as possible. </param>
		/// <param name="blue"> 	Specifies how much blue the color contains. 0.0f = no blue; 1.0f = as much blue as possible. </param>
		/// <param name="alpha">	An alpha value of 0.0f means that this color is completely transparent. Specify 1.0f for opaque colors. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Color(Real red, Real green, Real blue, Real alpha);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns how much red is contained by this color. </summary>
		///
		/// <remarks>	Samir Aroudj, 18.11.2011. </remarks>
		///
		/// <returns>	Returned values Range from 0.0f - no red to 1.0f as much red as possible. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Real getRed() const { return mComponents[0]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns how much green is contained by this color. </summary>
		///
		/// <remarks>	Samir Aroudj, 18.11.2011. </remarks>
		///
		/// <returns>	Returned values Range from 0.0f - no green to 1.0f as much green as possible. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Real getGreen() const { return mComponents[1]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns how much blue is contained by this color. </summary>
		///
		/// <remarks>	Samir Aroudj, 18.11.2011. </remarks>
		///
		/// <returns>	Returned values Range from 0.0f - no blue to 1.0f as much blue as possible. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Real getBlue() const { return mComponents[2]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns a value indicating the transparance of this color. </summary>
		///
		/// <remarks>	Samir Aroudj, 18.11.2011. </remarks>
		///
		/// <returns>	Returned values Range from 0.0f - completely transparent to 1.0f completely opaque. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Real getAlpha() const { return mComponents[3]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the colour components, red, green, blue and alpha. </summary>
		///
		/// <remarks>	Samir, 01.12.2011. </remarks>
		///
		/// <returns>	Returns an array of four Realing point values, red, green, blue and alpha in that order. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const Real *getComponents() const { return mComponents; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the values of all color components.
		/// 			Reasonable values for each parameter reach from 0.0f to 1.0f.
		/// 			See additive color mixing and alpha blending to find out more. </summary>
		///
		/// <remarks>	Samir Aroudj, 18.11.2011. </remarks>
		///
		/// <param name="red">  	Specifies how much red the color contains. 0.0f = no red; 1.0f = as much red as possible. </param>
		/// <param name="green">	Specifies how much green the color contains. 0.0f = no green; 1.0f = as much green as possible. </param>
		/// <param name="blue"> 	Specifies how much blue the color contains. 0.0f = no blue; 1.0f = as much blue as possible. </param>
		/// <param name="alpha">	An alpha value of 0.0f specifies that this color is completely transparent. Specify 1.0f for opaque colors. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void set(Real red, Real green, Real blue, Real alpha);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the value of the red component of this color. </summary>
		///
		/// <remarks>	Samir, 18.11.2011. </remarks>
		///
		/// <param name="red">	Reasonable values Range from 0.0f - no red to 1.0f as much red as possible. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setRed(Real red) { assert(checkComponent(red)); mComponents[0] = red; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the value of the green component of this color. </summary>
		///
		/// <remarks>	Samir, 18.11.2011. </remarks>
		///
		/// <param name="green">	Reasonable values Range from 0.0f - no green to 1.0f as much green as possible. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setGreen(Real green) { assert(checkComponent(green)); mComponents[1] = green; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the value of the blue component of this color. </summary>
		///
		/// <remarks>	Samir, 18.11.2011. </remarks>
		///
		/// <param name="blue">	Reasonable values Range from 0.0f - no blue to 1.0f as much blue as possible. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setBlue(Real blue) { assert(checkComponent(blue)); mComponents[2] = blue; }

		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the alpha value of this color to change the transparance. </summary>
		///
		/// <remarks>	Samir, 18.11.2011. </remarks>
		///
		/// <param name="alpha">	Reasonable values Range from 0.0f - completely transparent to 1.0f completely opaque. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setAlpha(Real alpha) { assert(checkComponent(alpha)); mComponents[3] = alpha; }

	private:

#ifdef _DEBUG
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Check the range of the colour components. </summary>
		///
		/// <remarks>	Samir, 01.12.2011. </remarks>
		///
		/// <param name="red">  	The red component must be between 0.0f and 1.0f. </param>
		/// <param name="green">	The green component must be between 0.0f and 1.0f. </param>
		/// <param name="blue"> 	The blue component must be between 0.0f and 1.0f. </param>
		/// <param name="alpha">	The alpha component must be between 0.0f and 1.0f. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool checkComponents(Real red, Real green, Real blue, Real alpha);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Check the value of a component. Requirement 0.0f <= component <= 1.0f. </summary>
		///
		/// <remarks>	Samir, 01.12.2011. </remarks>
		///
		/// <param name="component">	The component is checked to have a valid range. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool checkComponent(Real component);
#endif // _DEBUG

		/// <summary> The components of the colour: red, green, blue and alpha with a range from 0.0f to 1.0f whereas 1.0f means full color or completely opaque</summary>
		Real mComponents[4];
	};
}

#endif // _COLOR_H_
