/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cassert>
#include "CollisionDetection/CollisionDetection.h"
#include "CollisionDetection/ObliqueCircularCone.h"

using namespace CollisionDetection;
using namespace Math;

ObliqueCircularCone::ObliqueCircularCone(const Vector3 &apexPosition, const Vector3 &endOutsideNormal, const Vector3 &endCenter, const Real endRadius, const Real length) :
	mApex(apexPosition),
	mApexToEnd(endCenter - apexPosition),
	mEndCenter(endCenter),
	mEndOutsideNormal(endOutsideNormal),
	mEndRadius(endRadius)
{
	// where does the cone start? how much is cut away around the apex?
	const Real maxConeLength = mApexToEnd.getLength();
	const Real relativeLength = length / maxConeLength;
	if (relativeLength > 1.0f || relativeLength <= 0.0f)
	{
		mLength = maxConeLength;
		mRelativeStart = 0.0f;
	}
	else
	{
		mLength = length;
		mRelativeStart = 1.0f - relativeLength;
	}

	mStartCenter = mApex + mApexToEnd * mRelativeStart;
	
	// temporarily stored variables for quick collision tests
	mOne_StartToEndDotOutsideNormal = 1.0f / mApexToEnd.dotProduct(mEndOutsideNormal);
	mStartPlaneDistance = (-mEndOutsideNormal).dotProduct(mStartCenter);
	mEndPlaneDistance = mEndOutsideNormal.dotProduct(mEndCenter);

	// compute AABB of cone
	{
		// initial AABB
		mAABB[0] = mAABB[1] = mEndCenter;

		// compute the relative AABB [-dX, dX; -dY, dY, -dZ, dZ] of the circular patch at the cone end and start
		const Vector3 &n = mEndOutsideNormal;
		const Real nXSq = n.x * n.x;
		const Real nYSq = n.y * n.y;
		const Real nZSq = n.z * n.z;
	
		const Real lengthNormalXZ = sqrtr(nXSq + nZSq);
		const Real lengthNormalXY = sqrtr(nXSq + nYSq);
		const bool nParallelToY = (EPSILON >= lengthNormalXZ);

		// extend of all radiant tangent (orthogonal to n) vectors w.r.t. corodinate axes -> AABB around tangential unit circle
		Vector3 delta;
		delta.x = (nParallelToY ? 1.0f : 1.0f * (fabsr(n.z) / lengthNormalXZ));
		delta.z = (nParallelToY ? 1.0f : 1.0f * (fabsr(n.x) / lengthNormalXZ));
		delta.y = (nParallelToY ? 0.0f : 1.0f * (fabsr(n.x) / lengthNormalXY));

		// set AABB w.r.t. cone end
		const Vector3 dEnd = delta * mEndRadius;
		mAABB[0] = mAABB[0].minimum(mEndCenter - dEnd);
		mAABB[1] = mAABB[1].maximum(mEndCenter + dEnd);

		// update AABB w.r.t. cone start
		const Real rStart = mEndRadius * mRelativeStart;
		const Vector3 dStart = delta * rStart;
		mAABB[0] = mAABB[0].minimum(mStartCenter - dStart);
		mAABB[1] = mAABB[1].maximum(mStartCenter + dStart);
	}
}

bool ObliqueCircularCone::contains(const Vector3 &p) const
{
	// tCutCone \in [0, 1] = how far is p along the main axis w.r.t. the cut cone starting at getRelativeStart()? (0 -> at start, 1 -> at end)
	const Real tComplete = getTCompleteCone(p); 
	const Real tCutCone = (tComplete - getRelativeStart()) / (1.0f - getRelativeStart());
	if (tCutCone < 0.0f || tCutCone > 1.0f)
		return false;

	// p projected onto main axis & cone radius of that point
	const Vector3 projectedP = getApex() + getApexToEnd() * tComplete;
	const Real r = getEndRadius() * tComplete;

	// is positionWS within PCD and view cone?
	const Real radialDistanceSq = (p - projectedP).getLengthSquared();
	const Real rSq = r * r;
	if (radialDistanceSq > rSq)
		return false;

	return true;
}
		
bool ObliqueCircularCone::haveSeparatingAxis(const Vector3 &AABBMin, const Vector3 &AABBSize) const
{
	// simple AABB test
	const Vector3 AABBMax = AABBMin + AABBSize;
	if (!CollisionDetection::intersectAABBWithAABB(AABBMin, AABBMax, mAABB[0], mAABB[1]))
		return true;

	// precomputed variables for quick separating axis tests
	const Vector3 halfSize = AABBSize * 0.5f;
	const Vector3 AABBCenter = AABBMin + halfSize;

	// check edges for separating axes
	// simpler 2D tests: the separating axis parallel to an AABB and cone edge must be parallel to one of the coordinate system planes 
	// 1. a separating plane parallel to AABB x-axis (and parallel to projected cone border edge e0 or e1)
	// 2. a separating plane parallel to AABB y-axis (and parallel to projected cone border edge e0 or e1)
	// 3. a separating plane parallel to AABB z-axis (and parallel to projected cone border edge e0 or e1)
	for (Axis axisIdx = AXIS_X; axisIdx <= AXIS_Z; axisIdx = (Axis) (axisIdx + 1))
		if (haveSeparatingAxesIn2DForStartToEnd(AABBCenter, halfSize, axisIdx))
			return true;

	// check against cone front and back
	if (haveSeparatingAxesAtBackOrFront(AABBCenter, halfSize))
		return true;
	
	// check AABB corners for separating axes between them and the cone
	for (uint32 cornerIdx = 0; cornerIdx < 8; ++cornerIdx)
	{
		const Vector3 corner = getAABBCorner(AABBMin, AABBSize, cornerIdx);

		// intersection point between main axis and plane through corner and parallel to cone end plane
		const Real t = (corner - mApex).dotProduct(mEndOutsideNormal) * mOne_StartToEndDotOutsideNormal;
		const Vector3 centerP = mApex + mApexToEnd * t;
		const Real coneRadiusAtT = t * getEndRadius();
		
		// squared distance between main axis hit and corner within plane parallel to end plane (-> here the radius is coneRadiusAtT)
		const Vector3 centerPToCorner = corner - centerP;
		const Real distanceSq = centerPToCorner.getLengthSquared();

		// is the corner inside the cone? -> there is no separating axis
		if (t > 0.0f && t < 1.0f)
			if (distanceSq < coneRadiusAtT * coneRadiusAtT)
				return false;

		// get cone edge within reasonable separating plane
		const Real distance = sqrtr(distanceSq);
		const Vector3 coneSurfaceP = centerP + centerPToCorner * (coneRadiusAtT / distance);
		const Vector3 coneEdgeVector = coneSurfaceP - mApex;

		const Vector3 projection = coneEdgeVector * (centerPToCorner.dotProduct(coneEdgeVector) / coneEdgeVector.getLengthSquared());
		const Vector3 testAxis = centerPToCorner - projection;
		
		// test all 8 corners against the testAxis
		const Real scaledCenterDistance = (AABBCenter - coneSurfaceP).dotProduct(testAxis);
		const Real scaledWorstHalfVectorDistance = -(testAxis.dotProductAbsolute(halfSize));
		if (scaledCenterDistance + scaledWorstHalfVectorDistance >= 0.0f)
			return true;
	}

	// no saparating axes was found... (but this does not mean the end of the world ;)
	return false;
}

Real ObliqueCircularCone::getVolume() const
{
	if (isDegenerated())
		return 0.0f;

	// cone heights
	const Real hComplete = getApexToEnd().dotProduct(mEndOutsideNormal);
	const Real hCutPart = (mStartCenter - mApex).dotProduct(mEndOutsideNormal);

	// radii
	const Real rEnd = getEndRadius();
	const Real rStart = rEnd * mRelativeStart;

	// volumes
	const Real volumeCompleteCone = (PI / 3.0f) * rEnd * rEnd * hComplete;
	const Real volumeCutAway = (PI / 3.0f) * rStart * rStart * hCutPart;

	// cut cone volume
	const Real volume = volumeCompleteCone - volumeCutAway;
	return volume;
}

bool ObliqueCircularCone::haveSeparatingAxesIn2DForStartToEnd(const Vector3 &AABBCenter, const Vector3 &halfSize, const Axis axisIdx) const
{
	// actually reduces to a 2D triangle AABB test
	const Vector2 AABBCenter2D = reduceVector(AABBCenter, axisIdx);
	const Vector2 halfSize2D = reduceVector(halfSize, axisIdx);
	const Vector2 end2D = reduceVector(mEndCenter, axisIdx);
	const Vector2 start2D = reduceVector(mApex, axisIdx);
	const Vector2 endTangent = get2DEndRadiusTangent(axisIdx);

	// test triangle edge sideIdx as parallel to the separating plane
	for (uint32 sideIdx = 0; sideIdx < 2; ++sideIdx)
	{
		// edge to one of the cone end corners
		const Vector2 tangent = (0 == sideIdx ? -endTangent : endTangent);
		const Vector2 pEnd = end2D + tangent;
		const Vector2 edge = pEnd - start2D;

		// test separating axis must point outwards of the triangle / same direction as tangent
		const Vector2 testAxis(-edge.y, edge.x);
		const Vector2 outwardTestAxis = (testAxis.dotProduct(tangent) > 0.0f ? testAxis : -testAxis);

		// signed! distance (AABB center to edge) > extend(AABB to edge)?
		const Real scaledWorstDistanceEdge = (AABBCenter2D - pEnd).dotProduct(outwardTestAxis) - halfSize2D.dotProductAbsolute(outwardTestAxis);
		if (scaledWorstDistanceEdge >= 0.0f)
			return true;
	}

	return false;
}

bool ObliqueCircularCone::haveSeparatingAxesAtBackOrFront(const Vector3 &AABBCenter, const Vector3 &halfSize) const
{
	// check front cone plane against all AABB vertices
	const Vector3 &n = mEndOutsideNormal;
	const Real nDotWorstHalfVector = n.dotProductAbsolute(halfSize);
	
	// signed distance(AABB center, back plane) > extend(AABB center to edge)
	const Real distanceToBackPlane = n.dotProduct(AABBCenter) - mEndPlaneDistance;
	if (distanceToBackPlane > nDotWorstHalfVector)
		return true;

	// signed distance(AABB center, front plane) > extend(AABB center to edge)
	const Real distanceToFrontPlane = (-n).dotProduct(AABBCenter) - mStartPlaneDistance;
	if (distanceToFrontPlane > nDotWorstHalfVector)
		return true;

	return false;
}