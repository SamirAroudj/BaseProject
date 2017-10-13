/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <ctype.h>
#include <cstring>
#include "Math/MathCore.h"
#include "Math/Vector2.h"
#include "Platform/FailureHandling/FileCorruptionException.h"
#include "tinyxml2.h"
#include "Utilities/RegularExpression.h"
#include "Utilities/SVGLoader.h"

using namespace std;
using namespace FailureHandling;
using namespace Math;
using namespace Storage;
using namespace tinyxml2;
using namespace Utilities;

uint32 SVGLoader::loadPolybeziers(Polybezier<Vector2> *&polybeziers, const Path &fileName, const string *title)
{
	// parse markup language SVG document and create polybeziers from found descriptions
	// check polybeziers pointer which will refer to the loaded curves
	assert(!polybeziers);
	if (polybeziers)
	{
		// todo log that polybeziers points to something and won't be overriden for memory leak prevention
		return 0;
	}

	// open &load file
	tinyxml2::XMLDocument svgFile;
	svgFile.LoadFile(fileName.getCString());

	// error check
	bool error = svgFile.Error();
	assert(!error);
	if (error)
		return 0;

	// only SVG 2 is supported
	XMLElement *svg = svgFile.FirstChildElement("svg");
	if (!svg->Attribute("id", "svg2"))
	{
		// todo log that only SVG 2 is supported
		return 0;
	}

	// get document width & height
	Real width	= (Real) svg->DoubleAttribute("width");
	Real height	= (Real) svg->DoubleAttribute("height");
	if (width <= 0 || height <= 0)
	{
		// todo log invalid spatial document size
		return 0;
	}

	// access group element g which contains all paths / curves / graphical stuff
	XMLElement *group = svg->FirstChildElement("g");
	if (!group)
	{
		// todo log that no group element <g> was found in order to access curve data
		return 0;
	}

	// get paths with a title element that matches the title argument of this function
	vector<const char *> descriptions;

	for (XMLElement *path = group->FirstChildElement("path");
		 NULL != path;
		 path = path->NextSiblingElement("path"))
	{
		// filter according to title and child element title of current path
		if (title)
		{
			XMLElement *titleElement = path->FirstChildElement("title");
			if (!titleElement)
				continue;

			XMLNode *temp = titleElement->FirstChild();
			if (!temp)
				continue;

			XMLText *titleText = temp->ToText();
			if (!titleText)
				continue;

			if (0 != title->compare(titleText->Value()))
				continue;
		}

		const char *description = path->Attribute("d");
		if (description)
			descriptions.push_back(description);
	}

	polybeziers	= createPolybeziers(descriptions, fileName);
	return (uint32) (polybeziers ? descriptions.size() : 0);
}

Polybezier<Vector2> *SVGLoader::createPolybeziers(const vector<const char *> &descriptions, const Path &fileName)
{
	vector<Vector2>		points;
	vector<uint32>		degrees;
	size_t				count		 = descriptions.size();
	Polybezier<Vector2>	*polybeziers = new Polybezier<Vector2>[count];

	try
	{
		// load each polybezier
		for (uint32 curveIdx = 0; curveIdx < count; ++curveIdx)
		{
			const char *description = descriptions[curveIdx];
			if (description[0] != 'm' && description[0] != 'M')
				throw FileCorruptionException("Data attribute d of path element does not start" \
					"with m or M to define the Polybezier start point!", fileName);

			// convert description to points & degrees / Polybezier
			Vector2			reference;
			const size_t	DESC_LENGTH	 = strlen(description);
			for (size_t current = 0; current < DESC_LENGTH; )
				current += processCommand(points, degrees, reference, description + current, fileName);

			polybeziers[curveIdx].set(&points[0], &degrees[0], (uint32) degrees.size());
			points.clear();
			degrees.clear();
		}
	}
	catch (FileCorruptionException &exception)
	{
		// todo log exception
		exception;

		// free all polybeziers due to uncertain correctness
		delete [] polybeziers;
		return NULL;
	}

	return polybeziers;
}

uint32 SVGLoader::processCommand(vector<Vector2> &points, vector<uint32> &degrees,
								 Vector2 &reference, const char *commands, const Path &fileName)
{
	vector<Vector2> rawPairs;

	// determine command size
	uint32 commandSize = 1;
	for ( ; '\0' != commands[commandSize]; ++commandSize)
		if (isalpha(commands[commandSize]))
			break;

	// interprete command
	char	commandType		= commands[0];
	uint32	readCharCount	= 1;
	uint32	degree			= 0;

	switch (commandType)
	{
		// move to, (line to)*: (x y)+
		case 'M':
		case 'm':
		{
			// curve start
			if (!points.empty())
				throw FileCorruptionException("Found move command (m or M) in the middle of path attribute d which" \
					"is not allowed as single and connected curves are expected per path element", fileName);

			readCharCount += RegularExpression::getRealPair(reference.x, reference.y, commands + readCharCount,
															false, commandSize - readCharCount);
			points.push_back(reference);

			// additional and implicit line commands?
			degree = 1;
			break;
		}

		// line to: (x y)+
		case 'L':
		case 'l':
			degree = 1;
			break;

		// quadratic Bezier curve: (x1 y1 x y)+
		case 'Q':
		case 'q':
			degree = 2;
			break;

		// cubic Bezier curve: (x1 y1 x2 y2 x y)+
		case 'C':
		case 'c':
			degree = 3;
			break;

		// loop closure: it is a loop curve / closed curve: command has hasn't any parameters
		// command type = Z = z as both work identically and have no parameters
		case 'Z':
		case 'z':
		{
			// not always necessary "some programs (e.g. Inkscape) insert a superfluous 'z' command
			// when the loop closes with a curve with degree > 1, however,
			// if the last command was a line command then an additional closing line is necessary

			// distance check in space [0, 1]^2 to avoid numerical errors
			Real max = points.back().x;
			if (points.back().y > max)
				max = points.back().y;
			if (points[0].x > max)
				max = points[0].x;
			if (points[0].y > max)
				max = points[0].y;
			Vector2 a = points[0] / max;
			Vector2 b = points.back() / max;

			Real distance = (a - b).getLength();
			if (distance > EPSILON)
			{
				degrees.push_back(1);
				points.push_back(points[0]);
			}
			return commandSize;
		}

		default:
			string text	 = "Unimplemented command \"";
			text		+= commandType;
			text		+= "\" in attribute \"d\" of SVG path element during polybezier loading.";
			throw Exception(text);
	}

	// get & convert all coordinate rawPairs of the current command, see above comments
	RegularExpression::getRealPairs(rawPairs, commands + readCharCount, commandSize - readCharCount);
	if (!rawPairs.empty())
	{
		bool lowerCase = (0 != islower(commandType));
		convertPairsToSubCurve(points, degrees, reference, rawPairs, degree, lowerCase);
	}

	return commandSize;
}

void SVGLoader::convertPairsToSubCurve(vector<Vector2> &points, vector<uint32> &degrees,
									   Vector2 &reference, const vector<Vector2> &rawPairs,
									   uint32 degree, bool relativeCoords)
{
	size_t size = rawPairs.size();
	uint32 next = 0;

	// add each found sub curve
	for (size_t i = 0; i < size; i += degree)
	{
		degrees.push_back(degree);

		// add points of a single sub curve
		for (uint32 j = 0; j < degree; ++j, ++next)
		{
			Vector2 v(rawPairs[next]);
			if (relativeCoords)
				v += reference;

			// special case: control points coincide -> the curve actually has a smaller degree
			// e.g. cubic -> quadratic Bezier curve
			Real distanceSq = (points.back() - v).getLengthSquared();
			if (distanceSq < EPSILON * EPSILON)
				--degrees.back();
			else
				points.push_back(v);
		}

		reference = points.back();
	}
}
