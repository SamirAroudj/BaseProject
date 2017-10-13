/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _COLLISION_DETECTION_OBLIQUE_CIRCULAR_CONE_H_
#define _COLLISION_DETECTION_OBLIQUE_CIRCULAR_CONE_H_

#include "Math/MathHelper.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Platform/DataTypes.h"

namespace CollisionDetection
{
	/// Oblique (not perpendicular) circular cone which stores precomputed data for quick collision tests
	class ObliqueCircularCone
	{
	public:
		/** Creates a cone which is circular (can conceptually be divided into circular discs) but not perpendicular.
			So it is constructed by moving and growing a first infinitesimally small circular disc (not necessarily perpendicular to the movement direction) to some end position.
			The main axis from the center of the last circular disc to the apex is not necessarily perpendicular to the circular discs (-> oblique cone).
			The cone is circular.
		@param apexPosition This is where the cone originates / where its sharp end is.
		@param endOutsideNormal Set this to define the orientation of the circular discs centered at the main axis from the apex to the end center position.
			This is the normal of the circular disc at getEndCenter() and supposed to point outside at the cone end.
		@param endCenter Set this to where the main cone axis ends (= center of the last & biggest circular disc of the cone).
		@param endRadius Defines the radius of the circular disc at endCenter (radius of last / biggest circular disc of the cone) and thus defines the "width of the cone". 
		@param length Defines how long the view cone is starting from its end going to its apex. Meaning if length = distance(end, apex) -> full cone, length = 0.5 * distance(end, apex) -> half cone ending at end.*/
		ObliqueCircularCone(const Math::Vector3 &apexPosition,
			const Math::Vector3 &endOutsideNormal, const  Math::Vector3 &endCenter, const Real endRadius, const Real length);	
		
		/** Returns true if the cone contains the entered point or false if it is outside of the cone.
		@param p Returns true if p is inside the cone (returns true if p is at the cone border) or false if it is outside of the cone.
		@return Returns true if the cone contains the entered point or false if it is outside of the cone.*/
		bool contains(const Math::Vector3 &p) const;

		/** Returns a tangent vector parallel to the circular discs of the cone/orthogonal to the end outside normal.
			The tangent lies within one of the coordinate system planes and has the length getEndRadius().
		@param ignoredDimension Set this to 0, 1 or 2 to define which dimension is ignored (x, y or z). E.g., ignoredDimension = 2 -> a 2D vector within the xy-plane is returned.
		@return Returns a 2D vector within the coordinate system plane orthogonal to ignoredDimension axis (0 = x, 1 = y, 2 = z) and which is orthogonal to getEndOutsideNormal().
		@see See getEndOutsideNormal(), getEndRadius(). */
		inline Math::Vector2 get2DEndRadiusTangent(const Math::Axis ignoredDimension) const;

		/** Returns a pointer to an array of two vectors representing the minimum and maximum corner of
			the tight axis aligned bounding box around the cone.
		@return Returns a pointer to an array of two vectors representing the AABB minimum [0] and maximum [1]. */
		inline const Math::Vector3 *getAABB() const;

		/** Returns where the cone originates / where its sharp end is.
		@return Returns where the cone originates / where its sharp end is. */
		inline const Math::Vector3 &getApex() const;

		/** Returns the vector from the cone apex to the center of the circular disc at the cone end (= main cone axis which is NOT perpendicular to the end / ground disc of the cone).
		@return Returns the cone main / center axis vector starting at the apex and ending at the last circular disc center. */
		inline const Math::Vector3 &getApexToEnd() const;
		
		/** Returns where the main cone axis ends (= center of the last & biggest circular disc of the cone).
		@return Returns where the main cone axis ends (= center of the last & biggest circular disc of the cone). */
		inline const Math::Vector3 &getEndCenter() const;

		/** Returns the orientation of the circular discs centered at the main axis from the apex to the end center position (= normal of circular disc at getEndCenter()).
			The normal points outside the cone at its end and not inside.
		@Return Returns the normal of all circular discs, including the one at getEndCenter(). It points outside the cone at the end of the cone. */
		inline const Math::Vector3 &getEndOutsideNormal() const;

		/** Returns the radius of the circular disc at mEndCenter (radius of last / biggest circular disc of the cone).
		@return Returns the radius of the circular disc at mEndCenter (radius of last / biggest circular disc of the cone). */
		inline Real getEndRadius() const;

		/** Returns the distence of the main axis from the cone central end to start point. (getStartCenter() - getApex()).getLength().
		@return Returns the distence of the main axis from the cone central end to start point. (getStartCenter() - getApex()).getLength(). */
		inline Real getLength() const;

		/** Returns value = 1.0f / < mApexToEnd, mEndOutsideNormal >.
		@return Returns value = 1.0f / < mApexToEnd, mEndOutsideNormal > */
		inline Real getOne_StartToEndDotOutsideNormal() const;
		
		/** Returns where the cone actually starts which is a factor relative to the main axis between the apex and the end center position.
		@return Returns where the cone actually starts, 0 = at apex, 0.5 = in the middle between getApex() and getEndCenter().
		@see getApex(), getEndCenter() */
		inline Real getRelativeStart() const;

		/** Returns where the cone actually starts. (It can be cut off at the beginning, if not this is equal to getApex()).
		@return Returns where the cone actually starts. (It can be cut off at the beginning, if not this is equal to getApex())*/
		inline const Math::Vector3 &getStartCenter() const;
	
		/** Returns how far p is relatively positioned w.r.t. the cone main axis. 0 = cone apex, 1 = cone end, 0.5 = cone main axis center.
		@param p The point p is projected onto the main axis along the projective circular disc centered at the cone main axis and orthogonal to the getEndOutsideNormal().
		@return Returns how far p is relatively positioned w.r.t. the cone main axis. 0 = cone apex, 1 = cone end, 0.5 = cone main axis center. */
		inline Real getTCompleteCone(const Math::Vector3 &p) const;
					
		/** Returns how far a point is relative to the cut cone start and end based on the t value for the complete cone. (E.g., tCompleteCone = 0.75, cone start = 0.5 -> tCutCone = 0.5). 
		@param tCompleteCone Set this to, see getTCompleteCone().
		@return Returns how far a point is relative to the cut cone start and end based on the t value for the complete cone. (E.g., tCompleteCone = 0.75, cone start = 0.5 -> tCutCone = 0.5). 
		@see getTCompleteCone(). */
		inline Real getTCutCone(const Real tCompleteCone) const;

		/** Returns the volume of the possibly cut cone.
		@return Returns the volume of the possibly cut cone. */
		Real getVolume() const;

		/** Only returns true if a separating axis between this cone and the AABB was found.
			It might be the case that the AABB and the cone are actually not intersecting but false was returned as the separating axis was not found. 
		@param AABBMin Set this to the minimum corner (-x, -y, -z) of the AABB you want to test against this cone.
		@param AABBSize Set this to the edge lengths of the AABB you want to test against this cone. (AABB maximum = AABBMin + AABBSize)
		@return Only returns true if a separating axis between the cone and the AABB was found. False is returned if there can be an intersection. */
		bool haveSeparatingAxis(const Math::Vector3 &AABBMin, const Math::Vector3 &AABBSize) const;

		/** Tests whether there are separating planes parallel to the coordinate axis orthogonalAxis (separatingAxis is orthogonal to it)
			and these planes are between AABB and the cone's apex to end edges.
			This is a 2D test between two edges and the AABB since the planes are parallel to orthogonalAxis which is why one vector dimension is ignored.
		@param AABBCenter Set this to the center position of the axis aligned bounding box you want to test against the cone start to end edges.
		@param AABBHalfSize Set this to the half vector of the AABB storing its half size/extent.
		@param orthogonalAxis Set this to AXIS_X, AXIS_Y or AXIS_Z meaning having a 2D test in the yz-, xz- or xy-coordinate system planes.
		@return Returns true if there is a separating axis orthogonal to the system axis orthogonalAxis between the AABB and one of the cone's start to end edges. */
		bool haveSeparatingAxesIn2DForStartToEnd(const Math::Vector3 &AABBCenter, const Math::Vector3 &AABBHalfSize, const Math::Axis orthogonalAxis) const;
			
		/** Tests whether there are separating planes orthogonal to the end outside normal of the cone behind or in front of the cone.
			The first test checks for a separating plane between the cone start and the AABB and the second test checks for a plane between the cone end circular disc and the AABB.
		@param AABBCenter Set this to the center position of the axis aligned bounding box you want to test against the cone start to end edges.
		@param AABBHalfSize Set this to the half vector of the AABB storing its half size/extent.
		@return Returns true if a separating axis/plane was found otherwise false is returned. */
		bool haveSeparatingAxesAtBackOrFront(const Math::Vector3 &AABBCenter, const Math::Vector3 &AABBHalfSize) const;

		/** Returns true if the normal of the end circular disc is almost perpendicular to the cone main axis meaning the cone has almost no volume.
		@return Returns true if the normal of the end circular disc is almost perpendicular to the cone main axis meaning the cone has almost no volume. */
		inline bool isDegenerated() const;

	private:
		Math::Vector3 mAABB[2];					/// Minimum and maximum corner of the tight AABB around the cone.
		Math::Vector3 mApex;					/// This is where the cone originates / where its sharp end is.
		Math::Vector3 mApexToEnd;				/// Vector from the cone apex to the center of the circular disc at the cone end (= main cone axis which is NOT perpendicular to the end / ground disc of the cone).
		Math::Vector3 mEndCenter;				/// Defines where the main cone axis ends (= center of the last & biggest circular disc of the cone).
		Math::Vector3 mEndOutsideNormal;		/// Defines the orientation of the circular discs centered at the main axis from the apex to the end center position (= normal of circular disc at mEndCenter).
		Math::Vector3 mStartCenter;				/// Defines where the cone actually starts. (It can be cut off at the beginning.)
		
		Real mEndPlaneDistance;					/// = < mEndOutsideNormal, mEndCenter > = signed distance of the plane in which the end circular disc lies w.r.t. the origin
		Real mEndRadius;						/// Defines the radius of the circular disc at mEndCenter (radius of last / biggest circular disc of the cone) and thus defines the "width of the cone".
		Real mLength;							/// Length of the main axis from mStartCenter to mEndcenter.
		Real mOne_StartToEndDotOutsideNormal;	/// = 1.0f / < mApexToEnd, mEndOutsideNormal >
		Real mRelativeStart;					/// Defines where the cone actually starts, 0 = at apex, 0.5 = in the middle between getApex() and getEndCenter().
		Real mStartPlaneDistance;				/// = < -mEndOutsideNormal, mStartCenter > = signed distance of the plane in which the start circular disc lies w.r.t. the origin
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	inline Math::Vector2 ObliqueCircularCone::get2DEndRadiusTangent(const Math::Axis ignoredDimension) const
	{
		// end radius-sized tangent (orthogonal to end normal)
		const Math::Vector2 endNormal = reduceVector(mEndOutsideNormal, ignoredDimension);
		Math::Vector2 endTangent = Math::Vector2(-endNormal.y, endNormal.x);
		endTangent.normalize();
		endTangent *= getEndRadius();

		return endTangent;
	}

	inline const Math::Vector3 *ObliqueCircularCone::getAABB() const
	{
		return mAABB;
	}

	inline const Math::Vector3 &ObliqueCircularCone::getApex() const
	{
		return mApex;
	}

	inline const Math::Vector3 &ObliqueCircularCone::getApexToEnd() const
	{
		return mApexToEnd;	
	}
		
	inline const Math::Vector3 &ObliqueCircularCone::getEndCenter() const
	{
		return mEndCenter;
	}

	inline const Math::Vector3 &ObliqueCircularCone::getEndOutsideNormal() const
	{
		return mEndOutsideNormal;
	}

	inline Real ObliqueCircularCone::getEndRadius() const
	{
		return mEndRadius;
	}

	inline Real ObliqueCircularCone::getLength() const
	{
		return mLength;
	}

	inline Real ObliqueCircularCone::getOne_StartToEndDotOutsideNormal() const
	{
		return mOne_StartToEndDotOutsideNormal;
	}

	inline Real ObliqueCircularCone::getRelativeStart() const
	{
		return mRelativeStart;
	}

	inline const Math::Vector3 &ObliqueCircularCone::getStartCenter() const
	{
		return mStartCenter;
	}

	inline Real ObliqueCircularCone::getTCompleteCone(const Math::Vector3 &p) const
	{
		const Math::Vector3 apexToP = p - getApex();
		const Real tCompleteCone = apexToP.dotProduct(getEndOutsideNormal()) * getOne_StartToEndDotOutsideNormal();
		return tCompleteCone;
	}

	inline Real ObliqueCircularCone::getTCutCone(const Real tCompleteCone) const
	{
		const Real tCutCone = (tCompleteCone - getRelativeStart()) / (1.0f - getRelativeStart());
		return tCutCone;
	}

	inline bool ObliqueCircularCone::isDegenerated() const
	{
		return (Math::EPSILON > mEndOutsideNormal.dotProduct(mApexToEnd));
	}
}

#endif // _COLLISION_DETECTION_OBLIQUE_CIRCULAR_CONE_H_