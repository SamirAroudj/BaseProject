/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cctype>
#include "Platform/ParametersManager.h"
#include "Platform/Storage/File.h"
#include "Utilities/HelperFunctions.h"

using namespace Math;
using namespace Patterns;
using namespace Platform;
using namespace std;
using namespace Storage;
using namespace Utilities;

const char *ParametersManager::DELIMETERS = " \t\r\n";

ParametersManager::ParametersManager(const Path &fileName, const string &nameSpace)
{
	loadFromFile(fileName, nameSpace);
}

void ParametersManager::loadFromFile(const Path &fileName, const string &nameSpace)
{
	const string beginning = (nameSpace.empty() ? "" : nameSpace + "::");
	File file(fileName, File::OPEN_READING, false);
	if (file.errorOccured())
		return;
	
	// load file data
	string textLine;
	vector<string> lineParts;

	while (file.hasLeftData())
	{
		file.readTextLine(textLine);

		// comment?
		const size_t commentStart = textLine.find("//");
		if (string::npos != commentStart)
			textLine = textLine.substr(0, commentStart);
		
		while (!textLine.empty() && isspace(textLine.back()))
			textLine.pop_back();

		if (textLine.empty())
			continue;

		// get tokens / line parts
		Utilities::split(lineParts, textLine, DELIMETERS);
		
		// get tokens: type name = paramAsText;\n	e.g., uint8 foo = 42;
		const string type = lineParts[0];
		string name = lineParts[1];
		if (!beginning.empty())
			name = (beginning + name);

		if (1 != lineParts[2].size() && '=' != lineParts[2][0])
		{
			// todo log this correctly
			//cerr << "ParametersManager(): Invalid parameter format: " << textLine << endl;
			continue;
		}

		// get parameter value
		// everything between '=' and '\n' is supposed to be the parameter value
		const size_t searchStart = lineParts[0].size() + 1 + lineParts[1].size(); // type name
		const size_t valueStart = textLine.find(" = ", searchStart) + 3;
		const size_t valueEnd = textLine.find_last_of(";");
		const string value = textLine.substr(valueStart, valueEnd - valueStart);
		
		loadParameter(type, name, value);
	}
}

void ParametersManager::loadParameter(const std::string &type, const std::string &name, const std::string paramAsText)
{
	// booleans
	if (type == "bool")
		set(convertToBoolean(paramAsText), name);

	// signed integers
	else if (type == "int8")
		set(convert<int8>(paramAsText), name);
	else if (type == "int16")
		set(convert<int16>(paramAsText), name);
	else if (type == "int32")
		set(convert<int32>(paramAsText), name);
	else if (type == "int64")
		set(convert<int64>(paramAsText), name);

	// floating point parameters
	else if (type == "Real")
		set(convert<Real>(paramAsText), name);

	// text
	else if (type == "string")
		set(paramAsText, name);

	// unsigned integers
	else if (type == "uint8")
		set(convert<uint8>(paramAsText), name);
	else if (type == "uint16")
		set(convert<uint16>(paramAsText), name);
	else if (type == "uint32")
		set(convert<uint32>(paramAsText), name);
	else if (type == "uint64")
		set(convert<uint64>(paramAsText), name);

	// (math) vectors
	else if (type == "Vector2")
	{
		Vector2 temp;
		convert<Real>(&temp.x, 2, paramAsText);
		set(temp, name);
	}
	else if (type == "Vector3")
	{
		Vector3 temp;
		convert<Real>(&temp.x, 3, paramAsText);
		set(temp, name);
	}
	else if (type == "Vector4")
	{
		Vector4 temp;
		convert<Real>(&temp.x, 4, paramAsText);
		set(temp, name);
	}
	else
	{
		assert(false); // todo log this
	}
}

ParametersManager::~ParametersManager()
{
	clear();
}

void ParametersManager::clear()
{
	mBooleans.clear();

	mInt16s.clear();
	mInt32s.clear();
	mInt64s.clear();

	mReals.clear();
	mStrings.clear();
		
	mUint8s.clear();
	mUint16s.clear();
	mUint32s.clear();
	mUint64s.clear();
}

bool ParametersManager::get(bool &parameter, const string &name) const
{
	map<string, bool>::const_iterator it = mBooleans.find(name);
	if (it == mBooleans.end())
	{
		parameter = false;
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(int8 &parameter, const string &name) const
{
	map<string, int8>::const_iterator it = mInt8s.find(name);
	if (it == mInt8s.end())
	{
		parameter = -128;
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(int16 &parameter, const string &name) const
{
	map<string, int16>::const_iterator it = mInt16s.find(name);
	if (it == mInt16s.end())
	{
		parameter = -32768;
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(int32 &parameter, const string &name) const
{	
	map<string, int32>::const_iterator it = mInt32s.find(name);
	if (it == mInt32s.end())
	{
		parameter = 0xffffffff;
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(int64 &parameter, const string &name) const
{
	map<string, int64>::const_iterator it = mInt64s.find(name);
	if (it == mInt64s.end())
	{
		parameter = 0xffffffffffffffff;
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(Real &parameter, const string &name) const
{	
	map<string, Real>::const_iterator it = mReals.find(name);
	if (it == mReals.end())
	{
		parameter = -REAL_MAX;
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(string &parameter, const string &name) const
{
	map<string, string>::const_iterator it = mStrings.find(name);
	if (it == mStrings.end())
	{
		parameter = "";
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(uint8 &parameter, const string &name) const
{
	map<string, uint8>::const_iterator it = mUint8s.find(name);
	if (it == mUint8s.end())
	{
		parameter = 0xff;
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(uint16 &parameter, const string &name) const
{
	map<string, uint16>::const_iterator it = mUint16s.find(name);
	if (it == mUint16s.end())
	{
		parameter = 0xffff;
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(uint32 &parameter, const string &name) const
{
	map<string, uint32>::const_iterator it = mUint32s.find(name);
	if (it == mUint32s.end())
	{
		parameter = 0xffffffff;
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(uint64 &parameter, const string &name) const
{
	map<string, uint64>::const_iterator it = mUint64s.find(name);
	if (it == mUint64s.end())
	{
		parameter = 0xffffffffffffffff;
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(Vector2 &parameter, const string &name) const
{
	map<string, Vector2>::const_iterator it = mVector2s.find(name);
	if (it == mVector2s.end())
	{
		parameter.set(REAL_MAX, REAL_MAX);
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(Vector3 &parameter, const string &name) const
{
	map<string, Vector3>::const_iterator it = mVector3s.find(name);
	if (it == mVector3s.end())
	{
		parameter.set(REAL_MAX, REAL_MAX, REAL_MAX);
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::get(Vector4 &parameter, const string &name) const
{
	map<string, Vector4>::const_iterator it = mVector4s.find(name);
	if (it == mVector4s.end())
	{
		parameter.set(REAL_MAX, REAL_MAX, REAL_MAX, REAL_MAX);
		return false;
	}

	parameter = it->second;
	return true;
}

bool ParametersManager::set(const bool &parameter, const string &name)
{
	auto iterator = mBooleans.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

bool ParametersManager::set(const int8 &parameter, const string &name)
{
	auto iterator = mInt8s.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

bool ParametersManager::set(const int16 &parameter, const string &name)
{
	auto iterator = mInt16s.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

bool ParametersManager::set(const int32 &parameter, const string &name)
{
	auto iterator = mInt32s.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

bool ParametersManager::set(const int64 &parameter, const string &name)
{
	auto iterator = mInt64s.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}


bool ParametersManager::set(const Real &parameter, const string &name)
{
	auto iterator = mReals.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

bool ParametersManager::set(const string &parameter, const string &name)
{
	auto iterator = mStrings.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

bool ParametersManager::set(const uint8 &parameter, const string &name)
{
	auto iterator = mUint8s.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

bool ParametersManager::set(const uint16 &parameter, const string &name)
{
	auto iterator = mUint16s.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

bool ParametersManager::set(const uint32 &parameter, const string &name)
{
	auto iterator = mUint32s.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

bool ParametersManager::set(const uint64 &parameter, const string &name)
{
	auto iterator = mUint64s.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

bool ParametersManager::set(const Vector2 &parameter, const string &name)
{
	auto iterator = mVector2s.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

bool ParametersManager::set(const Vector3 &parameter, const string &name)
{
	auto iterator = mVector3s.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

bool ParametersManager::set(const Vector4 &parameter, const string &name)
{
	auto iterator = mVector4s.insert(make_pair(name, parameter));
	if (iterator.second)
		return true;

	iterator.first->second = parameter;
	return false;
}

