/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _MATH_HELPER_H_
#define _MATH_HELPER_H_

#include <cassert>
#include "Math/KernelsInterpolationsWeightings.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Platform/DataTypes.h"

namespace Math
{
	class Matrix3x3;
	class Matrix4x4;
	class Quaternion;
	
	/** Clamps a value to an interval.
	@param value This value is clamped to be inside [minValue, maxValue].
	@param maxValue The returned value is at most equal to maxValue.
	@param minValue Thre returned value is at least equal to minValue.
	@return The returned value is restricted to [maxValue, minValue] or not changed if it is already in the interval. */
	template <class T>
	inline T clamp(T value, T maxValue, T minValue);

	/** Computes the determinant of a 2x2 matrix.
	@param m00 Element at row 1 and column 1.
	@param m01 Element at row 1 and column 2.
	@param m10 Element at row 2 and column 1.
	@param m11 Element at row 2 and column 2.
	@returns The determinant of the specified 2x2 matrix, returns m00 * m11 - m10 * m01. */
	inline Real computeDeterminant(Real m00, Real m01, Real m10, Real m11);

	/** Computes 0.5f * omega. omega is the oriented solid angle of the oriented triangle t(a, b, c) seen from point p.
	@param p Set this to the point from which the triangle is seen to compute the half solid angle of it.
	@param a Set this to the first point of the oriented triangle t(a, b, c).
	@param b Set this to the second point of the oriented triangle t(a, b, c).
	@param c Set this to the third point of the oriented triangle t(a, b, c).
	@return Returns 0.5f * omega, wheras omega is the oriented solid angle of the oriented triangle t(a, b, c) seen from point p.
		So if x = a - p, y = b - p, z = c - p then the returned value is
		atan2(det([x y z]), (||x||*||y||*||z|| + <x, y>*||z|| + <y, x>*||z|| + <z, x>*||y||))
	@note See "Robust Inside-Outside Segmentation Using Generalized Winding Numbers.", 2013, ACM Trans. Graph.,
	Jacobson, Alec and Kavan, Ladislav and Sorkine-Hornung, Olga*/
	Real computeHalfSolidAngle(const Vector3 &p, const Vector3 &a, const Vector3 &b, const Vector3 &c);

	/** Computes the oriented angle omega which is the oriented angle of the oriented  line(a, b) seen from point p.
	@param p Set this to the point from which the line is seen to compute its projection as angle.
	@param a Set this to the first point of the oriented line(a, b).
	@param b Set this to the second point of the oriented line(a, b).
	@return Returns the oriented angle omega which is the oriented angle of the oriented  line(a, b) seen from point p
		So if x = a - p, y = b - p then the returned value is
		atan2(det[x y], <x, y>) = atan2(x.x*y.y - x.y*y.x, x.x * y.x + x.y * y.y)
	@note See "Robust Inside-Outside Segmentation Using Generalized Winding Numbers.", 2013, ACM Trans. Graph.,
	Jacobson, Alec and Kavan, Ladislav and Sorkine-Hornung, Olga*/
	Real computeSolidAngle(const Vector2 &p, const Vector2 &a, const Vector2 &b);

	/** Computes the area of the triangle by means of the cross product: 1/2 * ||((v1 - v0) x (v2 - v0))||.
	@param v0 Set this to the 2D coordinates of the first vertex of the triangle.
	@param v1 Set this to the 2D coordinates of the second vertex of the triangle.
	@param v2 Set this to the 2D coordinates of the third vertex of the triangle.
	@return Returns the area of the triangle. */
	inline Real computeTriangleArea(const Vector2 &v0, const Vector2 &v1, const Vector2 &v2);

	/** Computes the area of the triangle by means of the cross product: 1/2 * ||((v1 - v0) x (v2 - v0))||.
	@param v0 Set this to the 2D coordinates of the first vertex of the triangle.
	@param v1 Set this to the 2D coordinates of the second vertex of the triangle.
	@param v2 Set this to the 2D coordinates of the third vertex of the triangle.
	@return Returns the area of the triangle. */
	inline Real computeTriangleArea(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2);

	/** Computes the normal of the triangle by means of the cross product: n = normalize((v1 - v0) x (v2 - v0)).
	@param n Is set to the unit length normal of the triangle defined by v0, v1, v2. If you look at the triangle and the vertices are in CCW order then the normal points towards you. (right hand rule)
	@param v0 Set this to the 3D coordinates of the first vertex of the triangle. If you look at the triangle and the vertices are in CCW order then the normal points towards you. (right hand rule)
	@param v1 Set this to the 3D coordinates of the second vertex of the triangle. If you look at the triangle and the vertices are in CCW order then the normal points towards you. (right hand rule)
	@param v2 Set this to the 3D coordinates of the third vertex of the triangle. If you look at the triangle and the vertices are in CCW order then the normal points towards you. (right hand rule) */
	inline void computeTriangleNormal(Vector3 &n, const Vector3 &v0, const Vector3 &v1, const Vector3 &v2);

	/** Returns (degrees / 180.0) * Math::PI.
	@param degrees Returns (degrees / 180.0) * Math::PI.
	@return Returns (degrees / 180.0) * Math::PI.*/
	inline Real convertDegreesToRadians(const Real degrees);

	/** todo */
	Matrix4x4 createMatrix4x4FromMatrix3x3(const Matrix3x3 &m);

	/** Creates a 3x3 rotation matrix from a unit quaternion representing a rotation.
	@param q q must be a unit quaterion to enable rotation to a rotation represented by a matrix.
	@return Returns a 3x3 matrix containing the rotation represented by the entered unit quaternion q. */
	Matrix3x3 createMatrix3x3FromQuaternion(const Quaternion &q);

	/** Creates a 4x4 rotation matrix from a unit quaternion representing a rotation.
	@param q q must be a unit quaterion to enable rotation to a rotation represented by a matrix.
	@return Returns a 4x4 matrix containing the rotation represented by the entered unit quaternion q. */
	Matrix4x4 createMatrix4x4FromQuaternion(const Quaternion &q);

	// todo comment
	Quaternion createQuaternionFromMatrix(const Matrix3x3 &m);
	Quaternion createQuaternionFromMatrix(const Matrix4x4 &m);
	Quaternion createQuaternionFromAxes(const Vector3 x, const Vector3 y, const Vector3 z);
	
	/** todo */
	Math::Vector3 getAABBCorner(const Math::Vector3 &AABBMin, const Real AABBSize, const uint32 cornerIdx);

	/** todo */
	Math::Vector3 getAABBCorner(const Math::Vector3 &AABBMin, const Math::Vector3 &AABBmax, const uint32 cornerIdx);

	/** Computes the limited deviation of angle w.r.t. meanAngle. (unit: radians, deviation \in [-PI, PI])
	@param angle Set this to a normalized angle within the interval [0, 2 * PI). (unit: radians)
	@param meanAngle Set this to a normalized angle within the interval [0, 2 * PI). (unit: radians)
	@return The signed difference (angle - meanAngle) between angle and mean angle is returned whereas
		The difference is mapped to the interval [-PI, PI]. E.g. deviation(PI/4, 0) = PI/4, deviation(5/3 * PI, 0) = -1/3 PI, deviation(0, 5/3 * PI) = 1/3 PI. */
	inline Real getAngularDeviation(Real angle, const Real meanAngle);

	/** Computes the difference of two normalized angles within the intervals [0, Math::TWO_PI) whereas the maximum difference is always Math::PI.
	E.g. getAngularDistance(Math::QUARTER_PI, 7/4 * Math::PI) returns Math::HALF_PI.
	@param radians0 Set this to a normalized angle (unit: radians) within the interval [0, Math::TWO_PI) \subset of R.
	@param radians1 Set this to a normalized angle (unit: radians) within the interval [0, Math::TWO_PI) \subset of R.*/
	inline Real getAngularDistance(const Real radians0, const Real radians1);

	/** Computes the barycentric coordinates u, v, w of the point p w.r.t. the corresponding triangle vertices a, b, c.
		If p is not within the triangle then the barycentric coordinates of the projection of p onto the plane of the triangle are returned.
	@param u This is p's coordinate w.r.t the triangle vertex a.
	@param v This is p's coordinate w.r.t the triangle vertex b.
	@param w This is p's coordinate w.r.t the triangle vertex c.
	@param p Set this to the point for which you want to compute the Barycentric coordinates w.r.t. the triangle a, b, c.
	@param a This is the triangle vertex a which is associated with the Barycentric coordinate u.
	@param b This is the triangle vertex b which is associated with the Barycentric coordinate v.
	@param c This is the triangle vertex c which is associated with the Barycentric coordinate w.*/
	template <class T>
	inline void getBarycentric(Real &u, Real &v, Real &w,
		const T &p, const T &a, const T &b, const T &c);

	/** Computes proper scale factors for x and y view space vector components for pinhole camera projection matrices depending on the entered aspect ratio.
		The coordinates of the larger camera image plane side are only scaled by the focal length and the coordinates of the smaller side are additionally stretched according to aspect ratio.
		E.g., if width >= height (= aspectRatio >= 1.0f) then x is scaled by f and y is scaled by f * aspect ratio.
	@param sX This scale factor is set to the m00 matrix entry for the pinhole projection matrix.
	@param sY This scale factor is set to the m11 matrix entry for the pinhole projection matrix.
	@param focalLength Set this to the focalLength with which the target image was captured.
	@param aspectRatio Set this to the ratio of the image plane width to its height. Must be positive. */
	void getXYProjectionScaleFactors(Real &sX, Real &sY, const Real focalLength, const Real aspectRatio);

	/** Returns the larger value of a and b or b if a == b.
	@param a a is returned if it is greater than b.
	@param b b is returned if it is greater than or equal to a.
	@return Returns the larger value of a and b or b if a == b.*/
	inline Real maximum(Real a, Real b);

	/** Returns the index of the maximum value in values. Runtime class: O(n)
	@param values Set this to some Real values for which you want to find the maximum.
	@param count Set this to the number of elements in values that shall be considered.
	@return Returns the index of the maximum value in the provided array. */
	inline uint32 maximum(Real *values, const uint32 count);

	/** Returns the smaller value of a and b or b if a == b.
	@param a a is returned if it is smaller than b.
	@param b b is returned if it is smaller than or equal to a.
	@return Returns the larger value of a and b or b if a == b.*/
	inline Real minimum(Real a, Real b);

	/** Normalize an angle in radian measure to a value between 0.0f and two PI.
	@param radian The entered angle in radian measure is normalized to the same angle, but returned value will be between 0.0f and two PI.
	@return Returns the same angle in radian measure and normalized, that is the returned value is between 0.0f and two PI.*/
	Real normalizeAngle(Real radian);

	/** Computes the next largets 32bit representable power of two for n. E.g. f(0) = 0, f(3) = 4, f(2) = 2.
	@param n Set this to a number the next larget power of two can be represented using 32 bit.
	@return Returns a power of two k = 2^i with k >= n. Except for 0 which returns 0.*/
	uint32 power2RoundUp(uint32 n);

	/** Removes a component from a 3D vector and returns a 2D vector with the two left components in the original order. 
	@param v This vector is reduced by removal of one of its components.
	@param doomedIdx Set it to DIMENSION_X, DIMENSION_Y or DIMENSION_Z to remove the x, y or z component of the vector.
	@return The returned vector has two components of v in their original order. */
	inline Vector2 reduceVector(const Vector3 &v, const Axis doomedIdx);

	/** Converts the entered Cartesian coordinates to the corresponding spherical coordinates.
	@param azimuthAngle This is set to the angle in the "ground" plane which contains the x- and z-axis. The returned angle is within the interval [0, Math::TWO_PI). \n
		azimuthAngle = 0 -> x-axis, azimuthAngle = Math::PI -> z-axis
	@param polarAngle This is set to the angle which is relative to the y-axis (the upwards coordinate axis). The returned angle is within the interval [0, Math::PI]. \n
		polarAngle = 0 -> y-axis, plarAngle = Math::HALF_PI -> resulting vector lies in the ground x-z-plane, polarAngle = Math::PI -> -y-axis
	@param cartesian Set this to the Cartesian coordinates you want to convert.
	@param length Set this to the length of the vector if you already know it. length is used if it is set to a positive value otherwise the length of cartesian is computed and used.
	@return Returns the length of the vector cartesian or the parameter value length if it was set to a positive value. */
	Real transformCartesianToSpherical(Real &azimuthAngle, Real &polarAngle, const Vector3 &cartesian, Real length = 0.0f);

	/** Converts the entered spherical coordinates to the same coordinates but in the Cartesian system.
	@param cartesian Is filled with the values which correspond to the entered spherical coordinates.
	@param azimuthAngle Set this to the angle in the "ground" plane which contains the x- and z-axis. \n
		azimuthAngle = 0 -> x-axis, azimuthAngle = Math::PI -> z-axis
	@param polarAngle Set this to the angle which is relative to the y-axis (the upwards coordinate axis).	\n
		polarAngle = 0 -> y-axis, plarAngle = Math::HALF_PI -> resulting vector lies in the ground x-z-plane, polarAngle = Math::PI -> -y-axis
	@param r The unit length vector defined by the azimuth and polar angle is scaled by r. */
	void transformSphericalToCartesian(Vector3 &cartesian, const Real azimuthAngle, const Real polarAngle, const Real r);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	inline T clamp(T value, T maxValue, T minValue)
	{
		assert(maxValue >= minValue);

		if (value > maxValue)
			value = maxValue;
		if (value < minValue)
			value = minValue;

		return value;
	}

	inline Real computeDeterminant(Real m00, Real m01, Real m10, Real m11)
	{
		return m00 * m11 - m10 * m01;
	}

	inline Real computeTriangleArea(const Vector2 &v0, const Vector2 &v1, const Vector2 &v2)
	{
		return 0.5f * fabsr((v1 - v0).crossProduct(v2 - v0));
	}

	inline Real computeTriangleArea(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2)
	{
		return 0.5f * (v1 - v0).crossProduct(v2 - v0).getLength();
	}

	inline void computeTriangleNormal(Vector3 &n, const Vector3 &v0, const Vector3 &v1, const Vector3 &v2)
	{
		n = (v1 - v0).crossProduct(v2 - v0);
		n.normalize();
	}
	
	inline Real convertDegreesToRadians(const Real degrees)
	{
		return (degrees / 180.0f) * Math::PI;
	}

	inline Real getAngularDeviation(Real angle, const Real meanAngle)
	{
		assert(angle >= 0.0f);
		assert(angle < Math::TWO_PI + Math::EPSILON);
		assert(meanAngle >= 0.0f);
		assert(meanAngle < Math::TWO_PI + Math::EPSILON);

		while (true)
		{
			Real diff = angle - meanAngle;
			if (fabsr(diff) <= Math::PI)
				return diff;
			angle += (angle > meanAngle ? -Math::TWO_PI : Math::TWO_PI);
		}
	}

	inline Real getAngularDistance(const Real radians0, const Real radians1)
	{
		assert(radians0 >= 0.0f);
		assert(radians0 < Math::TWO_PI + Math::EPSILON);
		assert(radians1 >= 0.0f);
		assert(radians1 < Math::TWO_PI + Math::EPSILON);

		// maximum difference == PI as it is a circular property
		Real diff = fabsr(radians0 - radians1);
		if (diff > Math::PI)
			diff = Math::TWO_PI - diff;

		// return checked value
		assert(diff >= 0.0f);
		assert(diff < Math::TWO_PI);
		return diff;
	}

	template <class T>
	inline void getBarycentric(Real &u, Real &v, Real &w,
		const T &p, const T &a, const T &b, const T &c)
	{
		// See http://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
		// For similar stuff using Cramer's rule, see Christer Ericson's book Real-Time Collision Detection, second edition, 585-588
		// build and solve linear system of equations for u, v, w
		const T v0 = b - a;
		const T v1 = c - a;
		const T v2 = p - a;

		// v2 = v * v0 + w * v1 + t * (v0 x v1) -> column major matrix A = [(v0 x v1) (v0) (v1)] * column vector [t v w] = v2
		// -> apply Cramer's rule to get this
		const Real d00 = v0.dotProduct(v0);
		const Real d01 = v0.dotProduct(v1);
		const Real d11 = v1.dotProduct(v1);
		const Real d20 = v2.dotProduct(v0);
		const Real d21 = v2.dotProduct(v1);

		// Cramer's rule:
		// t = det(A0) / det(A)
		// v = det(A1) / det(A)
		// w = det(A2) / det(A)
		// with Ai = A but column i replaced by solution vector v2
		const Real invDenominator = 1.0f / (d00 * d11 - d01 * d01);

		v = (d11 * d20 - d01 * d21) * invDenominator;
		w = (d00 * d21 - d01 * d20) * invDenominator;
		u = 1.0f - v - w;
	}

	inline Real maximum(Real a, Real b)
	{
		return (a > b ? a : b);
	}

	inline uint32 maximum(Real *values, const uint32 count)
	{
		uint32	maxIdx	= -1;
		Real	maximum	= -REAL_MAX;

		for (uint32 i = 0; i < count; ++i)
		{
			if (maximum > values[i])
				continue;

			maxIdx	= i;
			maximum	= values[i];
		}

		return maxIdx;
	}

	inline Real minimum(Real a, Real b)
	{
		return (a < b ? a : b);
	}

	inline Vector2 reduceVector(const Vector3 &v, const Axis doomedIdx)
	{
		switch (doomedIdx)
		{
			case AXIS_X: return Vector2(v.y, v.z);
			case AXIS_Y: return Vector2(v.x, v.z);
			case AXIS_Z: return Vector2(v.x, v.y);
			default: assert(false); return Vector2(REAL_MAX, REAL_MAX);
		}
	}
}

#endif // _MATH_HELPER_H_
