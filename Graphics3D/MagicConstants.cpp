/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Graphics3D/MagicConstants.h"

using namespace Graphics;
using namespace GUI;
using namespace std;

// standard colours
const Graphics::Color Graphics::BACK_BUFFER_CLEAR_COLOR(0.2f, 0.2f, 0.9f, 1.0f);
const Graphics::Color Graphics::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const Graphics::Color Graphics::BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const Graphics::Color Graphics::CYAN(0.0f, 1.0f, 1.0f, 1.0f);
const Graphics::Color Graphics::GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const Graphics::Color Graphics::MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);
const Graphics::Color Graphics::RED(1.0f, 0.0f, 0.0f, 1.0f);
const Graphics::Color Graphics::YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const Graphics::Color Graphics::WHITE(1.0f, 1.0f, 1.0f, 1.0f);

const Graphics::Color Graphics::DARK_GREEN(102 / 255.0f, 132 / 255.0f, 78 / 255.0f, 1.0f);
const Graphics::Color Graphics::DRY_GRASS(0.8431138f, 0.717647f, 0.564706f, 1.0f);
const Graphics::Color Graphics::GREY_BEACH(0.72549f, 0.658824f, 0.5803922f, 1.0f);
const Graphics::Color Graphics::ROCK(130 / 255.0f, 114 / 255.0f, 144 / 255.0f, 1.0f);

const char *Graphics::PATH_TO_MATERIALS = "Materials/";
const char *Graphics::PATH_TO_MESHES = "Meshes/";
const char *Graphics::PATH_TO_SHADERS = "Shaders/";
const char *Graphics::PATH_TO_TEXTURES = "Graphics/Textures/";

const wstring GUI::DEFAULT_FACE_NAME = L"Arial";