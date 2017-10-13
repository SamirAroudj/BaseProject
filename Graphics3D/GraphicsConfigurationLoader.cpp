/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/ExceptionHandling/CorruptedFileException.h"
#include "Platform/File.h"
#include "MagicConstants.h"
#include "GraphicsConfigurationLoader.h"

using namespace ExceptionHandling;
using namespace Graphics;
using namespace Platform;
using namespace std;

void GraphicsConfigurationLoader::loadGraphicsConfiguration(uint32 &refreshRateHertz, uint32 &multisamplingCount, uint32 &multisamplingQuality, bool &enableVSync, const string &fileName)
{
	char buffer[FILE_READING_BUFFER_SIZE];
	const string firstGraphicsOption("Refresh rate (Hertz): ");
	File file(fileName, File::OPEN_READING, false);

	do	// find the first line in the file which contains information for the graphics manager
	{
		if (feof(&file.getHandle()))
			throw CorruptedFileException("Graphics configuration file does not contain the line: \"Refresh rate (Hertz): %u \".", fileName);
		fgets(buffer, FILE_READING_BUFFER_SIZE, &file.getHandle());
	}while(0 != strncmp(buffer, "Refresh rate (Hertz): ", 22));

	sscanf_s(buffer, "Refresh rate (Hertz): %u\n", &refreshRateHertz);
	fscanf_s(&file.getHandle(), "Multisampling count: %u\n", &multisamplingCount);
	fscanf_s(&file.getHandle(), "Multisampling quality: %u\n", &multisamplingQuality);
	fscanf_s(&file.getHandle(), "VSync: ");
	enableVSync = readBoolean(&file.getHandle(), buffer, FILE_READING_BUFFER_SIZE);
}

bool GraphicsConfigurationLoader::readBoolean(FILE *file, char *buffer, uint32 bufferLength)
{
	assert(!feof(file));
	fgets(buffer, bufferLength, file);
	if (buffer[0] == 't' && buffer[1] == 'r' && buffer[2] == 'u' && buffer[3] == 'e')
		return true;

	return false;
}