/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _PARAMETERS_MANAGER_H_
#define _PARAMETERS_MANAGER_H_

#include <map>
#include <string>
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Patterns/Singleton.h"
#include "Platform/DataTypes.h"
#include "Platform/Storage/Path.h"

namespace Platform
{
	/// Manages simple parameters which are described by their simple type, unique name and value which might differ between application runs.
	/** This class is supposed for management of application parameters which are set by the user and might change between application runs.
		All useer defined / user changeable parameters are supposed to be stored at a single point, a specific file which is loaded by this manager in the beginning.
		All other application components might then query this manager for the actual values of their required parameters. */
	class ParametersManager : public Patterns::Singleton<ParametersManager>
	{
	public:
		/** Creates the parameters manager and creates and sets all parameters according to the content of the file parametersFile.
		@param parametersFile Set this to the name of the file containing all parameter descriptions you want to load.
			The file is expected to be encoded in ASCII.
			Further, each parameter is expected to have a single line in the following format:\n
			"type name = value;"\n
			For example:\n
			- "Real foo = 0.35;"
			- "uint32 Platform::windowWidth = 800;"\n
			- "Real Graphics::maxFrameTime = 0.016;"\n
			- "bool Platform::fullscreen = false;"\n
			Each line starting with // is interpreted as comment and thus ignored.
			Only ' ' is a delimeter for token (type, name, =, value) splitting.
		@param nameSpace Parameter names are changed from name to nameSpace::name to avoid naming clashes. nameSpace is ignored if it is empty.*/
		ParametersManager(const Storage::Path &parametersFile, const std::string &nameSpace = "");

		/** Frees resources. */
		~ParametersManager();

		/** Erases all parameters. */
		void clear();
	
		/** bool parameter retrieval function.
		@param parameter Is set to the value of the parameter "bool name" if it exists or false otherwise.
		@param name Set this to the unique bool parameter name to identify the bool parameter the value of which you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(bool &parameter, const std::string &name) const;
	
		/** int8 parameter retrieval function.
		@param parameter Is set to the value of the parameter "int8 name" if it exists or -128 otherwise.
		@param name Set this to the unique int8 parameter name to identify the int8 parameter the value of which you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(int8 &parameter, const std::string &name) const;
		
		/** int16 parameter retrieval function.
		@param parameter Is set to the value of the parameter "int16 name" if it exists or -32768 otherwise.
		@param name Set this to the unique int16 parameter name to identify the int16 parameter the value of which you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(int16 &parameter, const std::string &name) const;
		
		/** int32 parameter retrieval function.
		@param parameter Is set to the value of the parameter "int32 name" if it exists or 0xffffffff otherwise.
		@param name Set this to the unique int32 parameter name to identify the int32 parameter the value of which you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(int32 &parameter, const std::string &name) const;

		/** int64 parameter retrieval function.
		@param parameter Is set to the value of the parameter "int64 name" if it exists or -0xffffffffffffffff otherwise.
		@param name Set this to the unique int64 parameter name to identify the int64 parameter the value of which you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(int64 &parameter, const std::string &name) const;
		
		/** Real parameter retrieval function.
		@param parameter Is set to the value of the parameter "Real name" if it exists or -REAL_MAX otherwise.
		@param name Set this to the unique Real parameter name to identify the Real parameter the value of which you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(Real &parameter, const std::string &name) const;
		
		/** string parameter retrieval function.
		@param parameter Is set to the string in the parameter "string name" if it exists or "" otherwise.
		@param name Set this to the unique string parameter name to identify the string parameter the value of which you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(std::string &parameter, const std::string &name) const;
		
		/** uint8 parameter retrieval function.
		@param parameter Is set to the value of the parameter "uint8 name" if it exists or 0xff otherwise.
		@param name Set this to the unique uint8 parameter name to identify the uint8 parameter the value of which you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(uint8 &parameter, const std::string &name) const;

		/** uint16 parameter retrieval function.
		@param parameter Is set to the value of the parameter "uint16 name" if it exists or 0xffff otherwise.
		@param name Set this to the unique uint16 parameter name to identify the uint16 parameter the value of which you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(uint16 &parameter, const std::string &name) const;

		/** uint32 parameter retrieval function.
		@param parameter Is set to the value of the parameter "uint32 name" if it exists or 0xffffffff otherwise.
		@param name Set this to the unique uint32 parameter name to identify the uint32 parameter the value of which you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(uint32 &parameter, const std::string &name) const;
		
		/** uint64 parameter retrieval function.
		@param parameter Is set to the value of the parameter "uint64 name" if it exists or 0xffffffffffffffff otherwise.
		@param name Set this to the unique uint64 parameter name to identify the uint64 parameter the value of which you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(uint64 &parameter, const std::string &name) const;

		/** Math::Vector2 parameter retrieval function.
		@param parameter Is set to the value of the parameter "Math::Vector2 name" if it exists or Math::Vector2(REAL_MAX, REAL_MAX) otherwise.
		@param name Set this to the unique Math::Vector2 parameter name to identify the Math::Vector2 parameter you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(Math::Vector2 &parameter, const std::string &name) const;

		/** Math::Vector3 parameter retrieval function.
		@param parameter Is set to the value of the parameter "Math::Vector3 name" if it exists or Math::Vector3(REAL_MAX, REAL_MAX, REAL_MAX) otherwise.
		@param name Set this to the unique Math::Vector3 parameter name to identify the Math::Vector3 parameter you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(Math::Vector3 &parameter, const std::string &name) const;

		/** Math::Vector4 parameter retrieval function.
		@param parameter Is set to the value of the parameter "Math::Vector4 name" if it exists or Math::Vector4(REAL_MAX, REAL_MAX, REAL_MAX, REAL_MAX) otherwise.
		@param name Set this to the unique Math::Vector4 parameter name to identify the Math::Vector4 parameter you want to get.
			There might be parameters with the same name but different type.
		@return Returns true if the searched parameter was found and returned accordingly or false if it does not exist and cannot be returned.*/
		bool get(Math::Vector4 &parameter, const std::string &name) const;

		/* Creates and sets all parameters according to the content of the file parametersFile.
			Existing parameters are overwritten.
		@param parametersFile Set this to the name of the file containing all parameter descriptions you want to load.
			The file is expected to be encoded in ASCII.
			Further, each parameter is expected to have a single line in the following format:\n
			"type name = value;"\n
			For example:\n
			- "Real foo = 0.35;"
			- "uint32 Platform::windowWidth = 800;"\n
			- "Real Graphics::maxFrameTime = 0.016;"\n
			- "bool Platform::fullscreen = false;"\n
			Each line starting with // is interpreted as comment and thus ignored.
			Only ' ' is a delimeter for token (type, name, =, value) splitting.
		@param nameSpace All parameter names in the file are changed from <name> to <nameSpace>::<name> to avoid naming clashes.\n
			nameSpace is ignored if it is empty.*/
		void loadFromFile(const Storage::Path &fileName, const std::string &nameSpace = "");

		/** bool parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "bool name" parameter.
		@param name Set this to the unique bool parameter name to identify the bool parameter you want to create or overwrite.
			There might be parameters with the same name but different type.
		@return Returns true if the parameter was created and false if it already existed and thus was overwritten. */
		bool set(const bool &parameter, const std::string &name);

		/** int8 parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "int8 name" parameter.
		@param name Set this to the unique int8 parameter name to identify the int8 parameter you want to create or overwrite.
			There might be parameters with the same name but different type.
		@return Returns true if the parameter was created and false if it already existed and thus was overwritten. */
		bool set(const int8 &parameter, const std::string &name);
		
		/** int16 parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "int16 name" parameter.
		@param name Set this to the unique int16 parameter name to identify the int16 parameter you want to create or overwrite.
			There might be parameters with the same name but different type.
		@return Returns true if the parameter was created and false if it already existed and thus was overwritten. */
		bool set(const int16 &parameter, const std::string &name);

		/** int32 parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "int32 name" parameter.
		@param name Set this to the unique int32 parameter name to identify the int32 parameter you want to create or overwrite.
			There might be parameters with the same name but different type.
		@return Returns true if the parameter was created and false if it already existed and thus was overwritten. */
		bool set(const int32 &parameter, const std::string &name);

		/** int64 parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "int64 name" parameter.
		@param name Set this to the unique int64 parameter name to identify the int64 parameter you want to create or overwrite.
			There might be parameters with the same name but different type.
		@return Returns true if the parameter was created and false if it already existed and thus was overwritten. */
		bool set(const int64 &parameter, const std::string &name);
		
		/** Real parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "Real name" parameter.
		@param name Set this to the unique Real parameter name to identify the Real parameter you want to create or overwrite.
			There might be parameters with the same name but different type.
		@return Returns true if the parameter was created and false if it already existed and thus was overwritten. */
		bool set(const Real &parameter, const std::string &name);

		/** string parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "string name" parameter.
		@param name Set this to the unique string parameter name to identify the string parameter you want to create or overwrite.
			There might be parameters with the same name but different type.
		@return Returns true if the parameter was created and false if it already existed and thus was overwritten. */
		bool set(const std::string &parameter, const std::string &name);
		
		/** uint8 parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "uint8 name" parameter.
		@param name Set this to the unique uint8 parameter name to identify the uint8 parameter you want to create or overwrite.
			There might be parameters with the same name but different type.
		@return Returns true if the parameter was created and false if it already existed and thus was overwritten. */
		bool set(const uint8 &parameter, const std::string &name);
		
		/** uint16 parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "uint16 name" parameter.
		@param name Set this to the unique uint16 parameter name to identify the uint16 parameter you want to create or overwrite.
			There might be parameters with the same name but different type.
		@return Returns true if the parameter was created and false if it already existed and thus was overwritten. */
		bool set(const uint16 &parameter, const std::string &name);
		
		/** uint32 parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "uint32 name" parameter.
		@param name Set this to the unique uint32 parameter name to identify the uint32 parameter you want to create or overwrite.
			There might be parameters with the same name but different type.
		@return Returns true if the parameter was created and false if it already existed and thus was overwritten. */
		bool set(const uint32 &parameter, const std::string &name);
		
		/** uint64 parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "uint64 name" parameter.
		@param name Set this to the unique uint64 parameter name to identify the uint64 parameter you want to create or overwrite.
			There might be parameters with the same name but different type.
		@return Returns true if the parameter was created and false if it already existed and thus was overwritten. */
		bool set(const uint64 &parameter, const std::string &name);

		/** Math::Vector2 parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "Vector2 name" parameter.
		@param name Set this to the unique Vector2 parameter name to identify the Vector2 parameter you want to create or overwrite.
			There might be paramaters with the same name but different type.*/
		bool set(const Math::Vector2 &parameter, const std::string &name);
		
		/** Math::Vector3 parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "Vector3 name" parameter.
		@param name Set this to the unique Vector3 parameter name to identify the Vector3 parameter you want to create or overwrite.
			There might be paramaters with the same name but different type.*/
		bool set(const Math::Vector3 &parameter, const std::string &name);

		/** Math::Vector4 parameter creation / overwriting function.
		@param parameter Set this to the desired value for the "Vector4 name" parameter.
		@param name Set this to the unique Vector4 parameter name to identify the Vector4 parameter you want to create or overwrite.
			There might be paramaters with the same name but different type.*/
		bool set(const Math::Vector4 &parameter, const std::string &name);

	protected:
		/** Loads and stores a parameter from text description.
		@param type Set this to the simple type, e.g., "bool", "int64", "uint8", "Real", see "Platform/DataTypes.h".
		@param name Set this to the variable name, e.g., "foo", "Platform::fullscreen".
		@param value Set this to the actual parameter value, e.g., "true", "3.5", "42". */
		void loadParameter(const std::string &type, const std::string &name, const std::string value);

	public:
		static const char *DELIMETERS;					/// Defines where tokens are split for parameters loading from file.

	private:
		std::map<std::string, bool> mBooleans;			/// Contains all bool parameters uniquely identified by their name.

		// signed integers
		std::map<std::string, int8> mInt8s;				/// Contains all int8 parameters uniquely identified by their name.
		std::map<std::string, int16> mInt16s;			/// Contains all int16 parameters uniquely identified by their name.
		std::map<std::string, int32> mInt32s;			/// Contains all int32 parameters uniquely identified by their name.
		std::map<std::string, int64> mInt64s;			/// Contains all int64 parameters uniquely identified by their name.

		std::map<std::string, Real> mReals;				/// Contains all Real parameters uniquely identified by their name.
		std::map<std::string, std::string> mStrings;	/// Contains all string parameters uniquely identified by their name.
		
		// unsigned integers
		std::map<std::string, uint8> mUint8s;			/// Contains all uint8 parameters uniquely identified by their name.
		std::map<std::string, uint16> mUint16s;			/// Contains all uint16 parameters uniquely identified by their name.
		std::map<std::string, uint32> mUint32s;			/// Contains all uint32 parameters uniquely identified by their name.
		std::map<std::string, uint64> mUint64s;			/// Contains all uint64 parameters uniquely identified by their name.

		// (math) vectors
		std::map<std::string, Math::Vector2> mVector2s; /// Contains all Math::Vector2 parameters uniquely identified by their name.
		std::map<std::string, Math::Vector3> mVector3s; /// Contains all Math::Vector3 parameters uniquely identified by their name.
		std::map<std::string, Math::Vector4> mVector4s; /// Contains all Math::Vector4 parameters uniquely identified by their name.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif // _PARAMETERS_MANAGER_H_