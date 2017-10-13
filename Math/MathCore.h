/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _MATH_CORE_H_
#define _MATH_CORE_H_

#include <cmath>

#ifdef _LINUX
	#include <cfloat>
	#include <cstdlib>
#endif // _LINUX

namespace Math
{
	/// Named constants to identify coordinate axes
	enum Axis
	{
		AXIS_X = 0,
		AXIS_Y = 1,
		AXIS_Z = 2,
		AXIS_W = 3,
		AXIS_COUNT
	};
}

// Redefine float and double to be the datatype real. Preprocessor translation depends on the flag DOUBLE_PRECISION.
#ifdef DOUBLE_PRECISION
	typedef double Real;

	// for scanf, printf etc. -> Real in text = %lf
	#define REAL_IT "%lf"

	// OpenGL defines
	#define GL_REAL GL_DOUBLE
	#define GLReal GLdouble

	// OpenGL functions
	#define glColor3r glColor3d
	#define glColor3rv glColor3dv
	#define glColor4r glColor4d
	#define glColor4rv glColor4dv
	#define glRasterPos2r glRasterPos2d
	#define glLightrv glLightdv
	#define glLoadMatrixr glLoadMatrixd
	#define glScaler glScaled
	#define glTranslater glTranslated
	#define glLoadMatrixdr glLoadMatrixd
	#define glMultMatrixr glMultMatrixd
	#define glTexCoord2r glTexCoord2d
	#define glVertex2r glVertex2d
	#define glVertex3r glVertex3d
	#define glVertex2rv glVertex2dv
	#define glVertex3rv glVertex3dv
	#define glNormal3r glNormal3d
	#define glNormal3rv glNormal3dv
	#define glGetRealv glGetDoublev

	// standard floating point defines
	#define REAL_MAX DBL_MAX
	#define REAL_MIN DBL_MIN

	// standard math functions
	#define acosr acos
	#define asinr asin
	#define atanr atan
	#define atan2r atan2
	#define ceilr ceil
	#define cosr cos
	#define expr exp
	#define fabsr fabs
	#define floorr floor
	#define fmaxr fmax
	#define fminr fmin
	#define logr log
	#define log2r log2
	#define powr pow
	#define roundr round
	#define sqrtr sqrt
	#define sinr sin
	#define tanr tan

	// cuda vector types
	#define Real2 double2
	#define Real3 double3
	#define Real4 double4

	#define make_real2 make_double2
	#define make_real3 make_double3
	#define make_real4 make_double4
#else
	typedef float Real;
	
	// for scanf, printf etc. -> Real in text = %f
	#define REAL_IT "%f"

	// OpenGL defines
	#define GL_REAL GL_FLOAT
	#define GLReal GLfloat

	// OpenGL functions
	#define glColor3r glColor3f
	#define glColor3rv glColor3fv
	#define glColor4r glColor4f
	#define glColor4rv glColor4fv
	#define glRasterPos2r glRasterPos2f
	#define glLightrv glLightfv
	#define glLoadMatrixr glLoadMatrixf
	#define glScaler glScalef
	#define glTranslater glTranslatef
	#define glLoadMatrixdr glLoadMatrixf
	#define glMultMatrixr glMultMatrixf
	#define glTexCoord2r glTexCoord2f
	#define glVertex2r glVertex2f
	#define glVertex3r glVertex3f
	#define glVertex2rv glVertex2fv
	#define glVertex3rv glVertex3fv
	#define glNormal3r glNormal3f
	#define glNormal3rv glNormal3fv
	#define glGetRealv glGetFloatv

	// standard floating point defines
	#define REAL_MAX FLT_MAX
	#define REAL_MIN FLT_MIN

	// standard math functions
	#define acosr acosf
	#define asinr asinf
	#define atanr atanf
	#define atan2r atan2f
	#define ceilr ceilf
	#define cosr cosf
	#define expr expf
	#define fabsr fabsf
	#define floorr floorf
	#define fmaxr fmaxf
	#define fminr fminf
	#define logr logf
	#define log2r log2f
	#define powr powf
	#define roundr roundf
	#define sqrtr sqrtf
	#define sinr sinf
	#define tanr tanf

	// cuda vector types
	#define Real2 float2
	#define Real3 float3
	#define Real4 float4

	#define make_real2 make_float2
	#define make_real3 make_float3
	#define make_real4 make_float4
#endif // DOUBLE_PRECISION
	
/// Contains mathematical functionality, such as vectors, matrices, quaternions etc.
/** The purpose of this namespace is to provide basic mathematical functionality. For example there are implementations for vectors, matrices and quaternions.
Functions for converting between euler angles, quaternions and matrices are also contained. */
namespace Math
{
	/// ratio of any circle's circumference to its diameter - I think you don't need explanations about this mathematical constant ;)
	const Real PI = 3.14159265301f;

	/// short define for 2.0f * PI
	const Real TWO_PI = 2.0f * PI;

	/// short define for PI / 2
	const Real HALF_PI = 0.5f * PI;

	/// short define for PI / 3
	const Real THIRD_PI = PI / 3.0f;

	/// short define for 2.0f * PI / 3
	const Real TWO_THIRD_PI = 2.0f * PI / 3.0f;

	/// short define for PI / 4
	const Real QUARTER_PI = 0.25f * PI;

	/// square root of (2 * PI)
	const Real SQRT_TWO_PI = 2.50662827463f;

	/// Euler's number - (e^x)' = e^x
	const Real EULERS_NUMBER = 2.71828182845904523536f;

	/// used to figure out if two real numbers are equal: fabsr(realNumber1 - realNumber2) < EPSILON -> realNumber1 is equal to realNumber2
	const Real EPSILON = 0.000001f;

	/// square root of two
	const Real SQRT_TWO = 1.414213562373095048801688724f;

	/// one divided by square root of two
	const Real ONE_SQRT_TWO = 1.0f / SQRT_TWO;

	/// one third = 1 / 3
	const Real ONE_THIRD = 1.0f / 3.0f;

	/** Returns true if the entered real number is not a number and has an invalid value (with which further calculations are impossible).
	@value Set this to the value you want to check.
	@return Returns false if you can continue calculating stuff with value and returns true if it is an invalid value with which further calculations are not reasonable. */
	inline bool isNaN(const Real value)
	{
		// return (value != value);
		return std::isnan(value);
	}

	/** Returns true if the entered real number represents infinity (with which further calculations are pretty difficult).
	@value Set this to the value you want to check.
	@return Returns true if value represents infinity with which further calculations are pretty difficult. */
	inline bool isInfinity(const Real value)
	{
		return std::isinf(value);
	}
}

#endif // _MATH_CORE_H_
