/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _COLLISION_DETECTION_PLANE_H_
#define _COLLISION_DETECTION_PLANE_H_

#include <cassert>
#include "Math/MathCore.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Platform/DataTypes.h"

namespace CollisionDetection
{
	/// Represents an infinitely broad and infinitely thin plane.
	/** The plaen is completely defined by its normal (direction perpendicular to all points in the plane) and its signed distance to the origin along its normal. */
	class Plane
	{
	public:
		/** Defines an infinitely large and thin plane by its normal and a point in the plane. 
		@param pointOnPlane Set this to any point in the plane.
		@param normalDirection Set this to normal direction of the plane. Might or might not be normalized depending on parameter normalized.
		@param normalized Set this to true if normalDirection is a unit length vector and set it to false if normalDirection needs to be normalized. */
		inline Plane(const Math::Vector3 &pointOnPlane, const Math::Vector3 &normalDirection, bool normalized);

		/** Defines the plane by means of the Hessian normal form: <p, n> = d <-> <p, n> = <p0, n> <-> <p - p0, n> = 0 with p0 as any point on the plane.
		@parameter unitNormal set this to the vector perpendicular to the plane (with length 1).
		@parameter d Set this to the plane's signed distance to the origin along the normal vector.*/
		inline Plane(const Math::Vector3 &unitNormal, const Real d);
		
		/** Returns the closest point to x which is in the plane.
		@param x Set this to any point you want to get the closest point for (from x along plane normal towards plane).
		@return Returns the point which has the smallest distance to x and which lies in the plane. */
		inline Math::Vector3 getClosestPoint(const Math::Vector3 &x) const;

		/** Returns the signed distance of the plane to the origin along its normal direction.
		@return Returns d = <p0, n> with p0 as any point on the plane. (distance to the plane = <p, n> - d)*/
		inline Real getD() const;

		/** Returns the distance of the point x to the plane (corresponding closest point on the plane).
		@param x Set this to the point coordinates you want to know the distance for w.r.t. the plane.
		@return Returns <planeNormal, x - pointOnPlane> with a unit length plane normal resulting in the signed distance of x to the plane. */
		inline Real getDistanceToPlane(const Math::Vector3 &x) const;

		/** Returns the unit length normal which is perpendicular to the plane.
		@return Returns the unit length normal vector which is orthogonal to the plane. */
		inline const Math::Vector3 &getNormal() const;
		
		/** Defines an infinitely large and thin plane by its normal and a point in the plane. 
		@param pointOnPlane Set this to any point in the plane.
		@param normalDirection Set this to normal direction of the plane. Might or might not be normalized depending on parameter normalized.
		@param normalized Set this to true if normalDirection is a unit length vector and set it to false if normalDirection needs to be normalized. */
		inline void set(const Math::Vector3 &pointOnPlane, const Math::Vector3 &normalDirection, bool normalized);

		/** Defines the plane by means of the Hessian normal form: <p, n> = d <-> <p, n> = <p0, n> <-> <p - p0, n> = 0 with p0 as any point on the plane.
		@parameter unitNormal set this to the vector perpendicular to the plane (with length 1).
		@parameter d Set this to the plane's signed distance to the origin along the normal vector.*/
		inline void set(const Math::Vector3 &unitNormal, const Real d);

	private:
		Math::Vector3 mNormal;	/// Unit length normal vector orthogonal to the plane.
		Real mD;				/// Signed distance of the plane to the origin along its normal vector.
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	inline Plane::Plane(const Math::Vector3 &pointOnPlane, const Math::Vector3 &normalDirection, bool normalized)
	{
		set(pointOnPlane, normalDirection, normalized);
	}

	inline Plane::Plane(const Math::Vector3 &unitNormal, const Real d)
	{
		set(unitNormal, d);
	}

	inline Math::Vector3 Plane::getClosestPoint(const Math::Vector3 &x) const
	{
		return x + mNormal * (mD - x.dotProduct(mNormal));
	}

	inline Real Plane::getD() const
	{
		return mD;
	}

	inline Real Plane::getDistanceToPlane(const Math::Vector3 &x) const
	{
		return x.dotProduct(mNormal) - mD;
	}

	inline const Math::Vector3 &Plane::getNormal() const
	{
		return mNormal;
	}

	inline void Plane::set(const Math::Vector3 &pointOnPlane, const Math::Vector3 &normalDirection, bool normalized)
	{
		#ifdef _DEBUG
			assert(!normalized || normalDirection.hasUnitLength());
		#endif // _DEBUG

		// unit length normal
		mNormal = normalDirection;
		if (!normalized)
			mNormal.normalize();

		// distance to origin of points on the plane
		mD = mNormal.dotProduct(pointOnPlane);
	}

	inline void Plane::set(const Math::Vector3 &unitNormal, const Real d)
	{
		#ifdef _DEBUG
			assert(unitNormal.hasUnitLength());
		#endif // _DEBUG

		mNormal = unitNormal;
		mD = d;
	}

}

#endif // _COLLISION_DETECTION_PLANE_H_