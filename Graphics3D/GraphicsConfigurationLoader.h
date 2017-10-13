/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GRAPHICS_CONFIGURATION_LOADER_H_
#define _GRAPHICS_CONFIGURATION_LOADER_H_

#include <cstdio>
#include <string>
#include "Platform/DataTypes.h"

namespace Graphics
{

	/// This class is used to load configuration data for graphics rendering from disc.
	/** The data that is loaded from disc is used for graphics rendering purposes. */
	class GraphicsConfigurationLoader
	{
	public:
		/** Loads and get graphics configuration data from disc. 
		@param refreshRateHertz [out] The structure of the line of the config file should look like: "Refresh rate (Hertz): 60"
		@param multisamplingCount [out] The line structure after the config line containing refresh rate should look like: "Multisampling count: 1"
		@param multisamplingQuality [out] The line structure after the config line containing multisampling count should look like: "Multisampling quality: 0"
		@param enableVSync [out] The line structure after the config line containing multisampling quality should look like: "VSync: true"
		@param fileName (optional) The path and name of the file containing the configuration data.*/
		static void loadGraphicsConfiguration(uint32 &refreshRateHertz, uint32 &multisamplingCount, uint32 &multisamplingQuality, bool &enableVSync,
			const std::string &fileName = "Configuration/Graphics.cfg");

	private:
		/** Helper function that is used to convert "true" or "false" to a boolean value.
		@param file Characters of this file are read and analyzed. x characters are read whereas x = min(bufferLength, length of current line, characters till eof)
		@param buffer This buffer is used to temporary store read characters that should be equal to "true" or false.
		@param bufferLength This is the maximum number of characters that is read from file and written to buffer.*/
		static bool readBoolean(FILE *file, char *buffer, uint32 bufferLength);
	};
}

#endif // _GRAPHICS_CONFIGURATION_LOADER_H_
