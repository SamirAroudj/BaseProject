/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include <cstdio>
#include <iostream>
#include <regex>
#include <sstream>
#include "Math/MathCore.h"
#include "Platform/MagicConstants.h"
#include "Platform/Platform.h"
#include "Platform/ResourceManagement/MemoryManager.h"
#include "Utilities/Licenser.h"
#include "Utilities/RandomManager.h"
#include "Utilities/RegularExpression.h"
#include "Utilities/SVGLoader.h"

using namespace std;
using namespace Math;
using namespace Storage;
using namespace Utilities;


#ifdef MEMORY_MANAGEMENT
	const uint32 ResourceManagement::DEFAULT_POOL_BUCKET_NUMBER = 5;
	const uint16 ResourceManagement::DEFAULT_POOL_BUCKET_CAPACITIES[DEFAULT_POOL_BUCKET_NUMBER] = { 1024, 1024, 1024, 1024, 1024 };
	const uint16 ResourceManagement::DEFAULT_POOL_BUCKET_GRANULARITIES[DEFAULT_POOL_BUCKET_NUMBER] = { 16, 32, 64, 128, 256 };
#endif // MEMORY_MANAGEMENT

void test(const string &name, bool value)
{
	cout << name << ((value) ? ": V":": X") << endl;
}

#ifdef _REGEX_
	void testIntegers()
	{
		// test RegularExpression::INTEGER
		cout << "RegularExpression::INTEGER" << endl;
		int32		values[]	= { 92, 42, -36, 53, 3 };
		const char	*integers[]	= { "92", "+42", "-036", "bla 53", "bla 3 bla 4" };
		regex		regInt(RegularExpression::INTEGER);

		for (uint32 i = 0; i < 5; ++i)
		{
			// find matches
			match_results<const char *> matches;
			regex_search(integers[i], matches, regInt);

			// convert matches
			int32 result = 0;
			sscanf(matches.str(0).c_str(), "%d", &result);

			// output
			ostringstream stream;
			stream << "text " << integers[i] << " == value " << values[i] << " == result " << result;
			test(stream.str().c_str(), result == values[i]);
		}
	}

void testReals()
{
	// test RegularExpression::Real
	cout << "RegularExpression::REAL" << endl;
	const uint32	COUNT		= 8;
	double			values[]	= { 92,		42.3,	-36.,	.3,		3e10,	1E2, 3e-4,		0.42e-2 };
	const char		*doubles[]	= { "92",	"42.3",	"-36.",	".3",	"3e10",	"1E2", "3e-4",	"0.42e-2" };
	regex			regReal(RegularExpression::REAL);

	for (uint32 i = 0; i < COUNT; ++i)
	{
		float result = FLT_MAX;

		// find matches
		match_results<const char *> matches;
		if (regex_search(doubles[i], matches, regReal))
		{
			// convert matches
			string number = matches.str(0);
			sscanf(number.c_str(), "%f", &result);
		}

		// test
		const Real epsilon = (result > 1e10 ? 10000 : Math::EPSILON);
		bool equal = epsilon > fabsr(result - (Real) values[i]);

		ostringstream stream;
		stream << "text " << doubles[i] << " == value " << values[i] << " == result " << result;
		test(stream.str().c_str(), equal);
	}
}


void testVector2()
{
	// test RegularExpression::VECTOR2
	cout << "RegularExpression::VECTOR2" << endl;
	const uint32	COUNT		= 8;
	double			values[]	= { 92,			42.3,			-36.,			.3,			3e10,			1E2,		3e-4,			0.42e-2 };
	const char		*doubles[]	= { "92, 92",	"42.3 42.3",	"-36.:-36.",	".3, .3",	"3e10,3e10",	"1E2|1E2", "3e-4; 3e-4",	"0.42e-2, 0.42e-2" };
	regex			regRealPair(RegularExpression::REAL_PAIR);

	for (uint32 i = 0; i < COUNT; ++i)
	{
		float results[2] = { FLT_MAX, FLT_MAX };

		// find matches
		match_results<const char *> matches;
		if (regex_search(doubles[i], matches, regRealPair))
		{
			// convert matches
			string n0 = matches.str(1);
			string n1 = matches.str(2);
			sscanf(n0.c_str(), "%f", results);
			sscanf(n1.c_str(), "%f", results + 1);
		}

		// test
		const Real epsilon = (values[i] > 1e10 ? 10000 : Math::EPSILON);
		bool equal = (epsilon > fabsr(results[0] - (Real) values[i])) && (epsilon > fabsr(results[1] - (Real) values[i]));

		ostringstream stream;

		stream << "text " << doubles[i] << " == value " << values[i] << " == result " << results[0] << ", " << results[1];
		test(stream.str().c_str(), equal);
	}
}

void testRegularExpression()
{
	cout << "Test regular expressions." << endl;

	testIntegers();
	testReals();
	testVector2();

	//		cout << "\n";
	//		for(size_t n = 0; n < matches.size(); ++n)
	//			cout << " matches[" << n << "]=[" << matches[n] << "] ";
	//		cout << endl;
}

#endif // _REGEX_

void testSVGLoader()
{
    const uint32 REFINEMENT_COUNT = 5;
    Polybezier<Vector2> *polybeziers = NULL;
    string title  = "Surface";

	const Path fileName(DATA_DIRECTORY "Polybezier00.svg");
    uint32 surfaceCount = SVGLoader::loadPolybeziers(polybeziers, fileName, &title);

    for (uint32 j = 0; j < surfaceCount; ++j)
        for (uint32 i = 0; i < REFINEMENT_COUNT; ++i)
			polybeziers[j].refine();
}

void testRandom()
{
	// create random number generator
	RandomManager *random = new RandomManager();

	cout << "Testing random:\n";
	cout << "getSeed(): " << random->getSeed() << "\n";
	cout << "getUniform(): " << random->getUniform() << "\n";
	cout << "getUniform(3, 42): " << random->getUniform(3, 42) << "\n";
	cout << "getUniform(Vector2(1, 1), Vector2(10, 10)): " << random->getUniform(Vector2(1, 1), Vector2(10, 10)) << "\n";
	cout << "getUniform(Vector3(2, 2, 2), Vector3(4, 4, 4)): " << random->getUniform(Vector3(2, 2, 2), Vector3(4, 4, 4)) << "\n";

	// free random number generator
	delete random;
	random = 0;
}

void testLicenser()
{
	vector<string> fileTypes;
	fileTypes.push_back(".cpp");
	fileTypes.push_back(".h");
	
	Licenser licenser("/gcc/scratch/home/samaroud/Repos/BaseProject/LicenseReference.txt");
	licenser.setLicense("/gcc/scratch/home/samaroud/Repos/BaseProject/", fileTypes);
}

int main(int32 argc, const char **argv)
{
	testLicenser();
	
	vector<string> arguments;
	Utilities::getCommandLineArguments(arguments, argv, argc);

	for (size_t i = 0; i < arguments.size(); ++i)
		cout << arguments[i] << " ";
	cout << endl;

	#ifdef _REGEX_
		testRegularExpression();
	#endif // _REGEX_
    testSVGLoader();
	testRandom();
	
	#ifdef MEMORY_MANAGEMENT
		ResourceManagement::MemoryManager::shutDown();
	#endif // MEMORY_MANAGEMENT

	return 0;
}
