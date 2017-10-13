/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SVG_LOADER_H_
#define _SVG_LOADER_H_

#include <string>
#include <vector>
#include "Math/Polybezier.h"
#include "Math/Vector2.h"
#include "Platform/Storage/Path.h"

namespace Utilities
{
	/// Provides functionality to load data from SVG (scalable vector graphics) files.
	class SVGLoader
	{
	public:
		/** Loads a polybezier array from an SVG file which contains corresponding path elements.
		@param polybeziers The pointer will refer to the allocated array containing all loaded polybeziers upon succuss.
			It is set to NULL otherwise.
		@param fileName Specifies the SVG file containing path elements which describe polybeziers.
		@param title Set this to some text to filter path elements of the specified SVG file.
			Only path elements with child title elements containing text equal to this title parameter are loaded.
			E.g. title = "Polybezier" -> data in "<path ...><title>Polybezier</title></path>" is loaded.
			Set this to NULL to load all path elements.
		@return Returns the number of elements in created polybeziers array or 0 if loading fails. */
		static uint32 loadPolybeziers(Math::Polybezier<Math::Vector2> *&polybeziers,
									  const Storage::Path &fileName, const std::string *title = NULL);

	private:
		/** Creates sub curves (in points & degrees) from loaded raw data.
		@param points This vector is extended by absolute control polygon points as result of the conversion.
		@param degrees Entered degree value is added to degrees for each found sub curve.
		@param reference Set this to the current reference (= current point in SVG path attribute d documentation =
			start point in the beginning). If necessary reference is updated by this function.
		@param rawPairs Set this to the raw coordinate values of the d attribute of the SVG path element.
		@param degree Set this to the degree of the sub curves, e.g. 2 for quadratic Bezier curve commands ("Q", "q").
		@param relativeCoords Set this to true if it is a command containing relative coordinates, (e.g. "l", "q", "c"). */
		static void convertPairsToSubCurve(std::vector<Math::Vector2> &points, std::vector<uint32> &degrees,
										   Math::Vector2 &reference, const std::vector<Math::Vector2> &rawPairs,
										   uint32 degree, bool relativeCoords);

		/** Parses multiple d attributes of SVG path elements (= descriptions) and creates a corresponding Polybezier array.
		@param descriptions Set this to the found path element d attribute texts which contain the sub curve commands.
		@param fileName Set this to the SVG file name for exception handling.
		@return Returns a pointer referring to the created Polybezier objects on success or NULL on failure.
			Allocates as many Polybezier objects as descriptions are entered. */
		static Math::Polybezier<Math::Vector2> *createPolybeziers(const std::vector<const char *> &descriptions,
																  const Storage::Path  &fileName);

		/** Processes a single command of a d attribute in a SVG path element, such as "Q 100,200 200,300" or "Z".
		@param points Is extended by newly found absolute coordinates of the control polygon points of the polybezier.
		@param degrees Is extended by a single degree value for each sub curve found in the current command, e.g "L"->degree=1.
		@param reference Is set to the current reference point which is required for relative coordinates interpretation.
		@param commands Must point to the first character of the command to be processed.
		@param fileName Set this to the SVG file name for possible exception handling.
		@return Returns the command size and can be used to get the address of the next potentially exisiting command.*/
		static uint32 processCommand(std::vector<Math::Vector2> &points, std::vector<uint32> &degrees,
									 Math::Vector2 &reference, const char *commands, const Storage::Path  &fileName);
	};
}

#endif // _SVG_LOADER_H_
