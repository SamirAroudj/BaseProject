/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cassert>
#include <cmath>
#include "CollisionDetection/CollisionDetection.h"
#include "Math/MathHelper.h"
#include "Platform/DataTypes.h"

using namespace CollisionDetection;
using namespace Math;

const uint32 X_INTERVAL = 0x1 << 0;
const uint32 Y_INTERVAL = 0x1 << 1;
const uint32 Z_INTERVAL = 0x1 << 2;

Real CollisionDetection::getDistanceToAABB(const Vector2 &position, const Vector2 &AABBMin, const Vector2 &AABBMax)
{
	// if position is within the Voronoi region of an AABB corner then return the distance to this corner
	Vector2	closestVertex(REAL_MAX, REAL_MAX);
	uint32	insideAxisInterval(0);

	// to the left or right of the AABB?
	if (position.x > AABBMax.x)
		closestVertex.x = AABBMax.x;
	else if (position.x < AABBMin.x)
		closestVertex.x = AABBMin.x;
	else
		insideAxisInterval |= X_INTERVAL;

	// above or under the AABB?
	if (position.y > AABBMax.y)
		closestVertex.y = AABBMax.y;
	else if (position.y < AABBMin.y)
		closestVertex.y = AABBMin.y;
	else
		insideAxisInterval |= Y_INTERVAL;

	if (0 == insideAxisInterval)
		return (position - closestVertex).getLength();

	// the point is in the Voronoi region of some side or inside the AABB
	if (X_INTERVAL == insideAxisInterval) // above or below the top or bottom side
	{
		if (position.y > AABBMax.y)
			return position.y - AABBMax.y;
		else
			return AABBMin.y - position.y;
	}
	else if (Y_INTERVAL == insideAxisInterval)	// to the left or right of the left or right side
	{
		if (position.x > AABBMax.x)
			return position.x - AABBMax.x;
		else
			return AABBMin.x - position.x;
	}

	// the point is inside the AABB
	return 0.0f;
}

Real CollisionDetection::getDistanceToAABB(const Vector3 &position, const Vector3 &AABBMin, const Vector3 &AABBMax)
{
	// if position is within the Voronoi region of an AABB corner then return the distance to this corner
	Vector3	closestVertex(REAL_MAX, REAL_MAX, REAL_MAX);
	uint32	insideAxisInterval(0);

	// to the left or right of the AABB?
	if (position.x > AABBMax.x)
		closestVertex.x = AABBMax.x;
	else if (position.x < AABBMin.x)
		closestVertex.x = AABBMin.x;
	else
		insideAxisInterval |= X_INTERVAL;

	// above or under the AABB?
	if (position.y > AABBMax.y)
		closestVertex.y = AABBMax.y;
	else if (position.y < AABBMin.y)
		closestVertex.y = AABBMin.y;
	else
		insideAxisInterval |= Y_INTERVAL;

	// in front of or behind the AABB?
	if (position.z > AABBMax.z)
		closestVertex.z = AABBMax.z;
	else if (position.z < AABBMin.z)
		closestVertex.z = AABBMin.z;
	else
		insideAxisInterval |= Z_INTERVAL;

	// position is within the Voronoi region of an AABB vertex
	if (0 == insideAxisInterval)
		return (position - closestVertex).getLength();

	// the point is in the Voronoi region of some side or inside the AABB
	if ((X_INTERVAL & Y_INTERVAL) == insideAxisInterval) // before or behind the front or back side?
	{
		if (position.z > AABBMax.z)
			return position.z - AABBMax.z;
		else
			return AABBMin.z - position.z;
	}
	else if ((X_INTERVAL & Z_INTERVAL) == insideAxisInterval) // above or below the top or bottom side
	{
		if (position.y > AABBMax.y)
			return position.y - AABBMax.y;
		else
			return AABBMin.y - position.y;
	}
	else if ((Y_INTERVAL & Z_INTERVAL) == insideAxisInterval)	// to the left or right of the left or right side
	{
		if (position.x > AABBMax.x)
			return position.x - AABBMax.x;
		else
			return AABBMin.x - position.x;
	}

	// the point is inside the AABB
	return 0.0f;
}

Real CollisionDetection::getDistanceToLine(const Vector3 &x, const Vector3 &p0, const Vector3 &p1)
{
	// see Christer Ericson's book Real-Time Collision Detection, second edition, page 544
	const Vector3 v = p1 - p0;
	const Vector3 w = (x - p0);
	const Real temp = w.dotProduct(v);
	const Real lengthSq = w.dotProduct(w) - (temp * temp) / v.dotProduct(v);

	return sqrtr(lengthSq);
}

Real CollisionDetection::getDistanceToLine(Vector3 *basePoint, const Vector3 &x, const Vector3 &p0, const Vector3 &p1)
{
	// see http://geomalgorithms.com/a02-_lines.html "The Parametric Line"
	const Vector3 v = p1 - p0;
	const Vector3 w = (x - p0);
	const Real b = w.dotProduct(v) / v.dotProduct(v);
	const Vector3 temp = v * b;

	// set base point if requested (base point is the projection of x onto the line)
	if (basePoint)
		*basePoint = p0 + temp;

	return (w - temp).getLength();
}

Real CollisionDetection::getDistanceToLineSegment(Vector2 *closestPoint, const Vector2 &x, const Vector2 &p0, const Vector2 &p1)
{
	// see http://geomalgorithms.com/a02-_lines.html "The Parametric Line"
	const Vector2 v = p1 - p0;
	const Vector2 w = x - p0;

	// Is x "in front of p0"?
	const Real dotProduct0 = w.dotProduct(v);
	if (dotProduct0 <= 0)
	{
		if (closestPoint)
			*closestPoint = p0;
		return w.getLength();
	}

	// Is x "behind p1"
	const Real dotProduct1 = v.dotProduct(v);
	if (dotProduct1 <= dotProduct0)
	{
		if (closestPoint)
			*closestPoint = p1;
		return (p1 - x).getLength();
	}

	// set the closest point if requested
	// closest point of line segment(p0, p1) w.r.t. x is between p0 and p1
	const Real b = dotProduct0 / dotProduct1;
	const Vector2 temp = p0 + v * b;
	if (closestPoint)
		*closestPoint = temp;

	return (x - temp).getLength();
}

Real CollisionDetection::getDistanceToLineSegment(Vector3 *closestPoint, const Vector3 &x, const Vector3 &p0, const Vector3 &p1)
{
	// see http://geomalgorithms.com/a02-_lines.html "The Parametric Line"
	const Vector3 v = p1 - p0;
	const Vector3 w = x - p0;

	// Is x "in front of p0"?
	const Real dotProduct0 = w.dotProduct(v);
	if (dotProduct0 <= 0)
	{
		if (closestPoint)
			*closestPoint = p0;
		return w.getLength();
	}

	// Is x "behind p1"
	const Real dotProduct1 = v.dotProduct(v);
	if (dotProduct1 <= dotProduct0)
	{
		if (closestPoint)
			*closestPoint = p1;
		return (p1 - x).getLength();
	}

	// set the closest point if requested
	// closest point of line segment(p0, p1) w.r.t. x is between p0 and p1
	const Real b = dotProduct0 / dotProduct1;
	const Vector3 temp = p0 + v * b;
	if (closestPoint)
		*closestPoint = temp;

	return (x - temp).getLength();
}

bool CollisionDetection::intersectAABBWithSphere(const Vector2 &AABBMin, const Vector2 &AABBMax, const Vector2 sphereCoords, const Real sphereRadius)
{
	// collision tests according to SAT
	// check all voronoi regions of the AABB against the corresponding closest sphere point

	// 4 Voronoi regions of the AABB edges (not exactly the Voronoi regions, these cases cover a larger area, but still correct)
	// sphere on the left of the AABB?
	if (AABBMin.x > sphereCoords.x + sphereRadius)
		return false;
	// sphere on the right of the AABB?
	if (AABBMax.x < sphereCoords.x - sphereRadius)
		return false;
	// sphere below the AABB?
	if (AABBMin.y > sphereCoords.y + sphereRadius)
		return false;
	// sphere above the AABB?
	if (AABBMax.y < sphereCoords.y - sphereRadius)
		return false;

	// 4 Voronoi regions of the AABB vertices
	// closest vertex if if sphere center is truly within a vertex Voronoi region
	Vector2 closestVertex(REAL_MAX, REAL_MAX);
	if (sphereCoords.x > AABBMax.x)
		closestVertex.x = AABBMax.x;
	else if (sphereCoords.x < AABBMin.x)
		closestVertex.x = AABBMin.x;
	else
		return true; // sphere must overlap with AABB

	if (sphereCoords.y > AABBMax.y)
		closestVertex.y = AABBMax.y;
	else if (sphereCoords.y < AABBMin.y)
		closestVertex.y = AABBMin.y;
	else
		return true;  // sphere must overlap with AABB

	// is there a separating axis orthogonal to the connection of the sphere center and the closest AABB vertex?
	const Real distanceSq = (closestVertex - sphereCoords).getLengthSquared();
	return (distanceSq <= sphereRadius * sphereRadius);
}

bool CollisionDetection::intersectAABBWithSphere(const Vector3 &AABBMin, const Vector3 &AABBMax, const Vector3 sphereCoords, const Real sphereRadius)
{
	// collision tests according to SAT
	// check all voronoi regions of the AABB against the corresponding closest sphere point

	// 6 Voronoi regions of the AABB faces and edges (~= merged Voronoi regions, still correct)
	// sphere on the left of the AABB?
	if (AABBMin.x > sphereCoords.x + sphereRadius)
		return false;
	// sphere on the right of the AABB?
	if (AABBMax.x < sphereCoords.x - sphereRadius)
		return false;
	// sphere below the AABB?
	if (AABBMin.y > sphereCoords.y + sphereRadius)
		return false;
	// sphere above the AABB?
	if (AABBMax.y < sphereCoords.y - sphereRadius)
		return false;
	// sphere in front of AABB?
	if (AABBMin.z > sphereCoords.z + sphereRadius)
		return false;
	// sphere behind the AABB?
	if (AABBMax.z < sphereCoords.z - sphereRadius)
		return false;

	// 8 Voronoi regions of the AABB vertices
	// closest vertex if if sphere center is truly within a vertex Voronoi region
	Vector3 closestVertex(sphereCoords);
	if (sphereCoords.x > AABBMax.x)
		closestVertex.x = AABBMax.x;
	else if (sphereCoords.x < AABBMin.x)
		closestVertex.x = AABBMin.x;

	if (sphereCoords.y > AABBMax.y)
		closestVertex.y = AABBMax.y;
	else if (sphereCoords.y < AABBMin.y)
		closestVertex.y = AABBMin.y;

	if (sphereCoords.z > AABBMax.z)
		closestVertex.z = AABBMax.z;
	else if (sphereCoords.z < AABBMin.z)
		closestVertex.z = AABBMin.z;

	// is there a separating axis orthogonal to the connection of the sphere center and the closest AABB vertex?
	const Real distanceSq = (closestVertex - sphereCoords).getLengthSquared();
	return (distanceSq <= sphereRadius * sphereRadius);
}

bool CollisionDetection::intersectAABBWithLineSegment(Real &entryT, Real &exitT, const Vector2 &lineP0, const Vector2 &lineP1, const Vector2 &AABBMin, const Vector2 &AABBMax)
{
	Vector2 direction = lineP1 - lineP0;
	if (!CollisionDetection::intersectAABBWithLine(entryT, exitT, lineP0, direction, AABBMin, AABBMax))
		return false;

	// clamp the returned line intersection point t-values to the line segment ends
	if (entryT < 0.0f)
		entryT = 0.0f;
	if (exitT > 1.0f)
		exitT = 1.0f;

	return (exitT >= entryT);
}

bool CollisionDetection::intersectAABBWithLineSegment(Real &entryT, Real &exitT, const Vector3 &lineP0, const Vector3 &lineP1, const Vector3 &AABBMin, const Vector3 &AABBMax)
{
	Vector3 direction = lineP1 - lineP0;
	if (!CollisionDetection::intersectAABBWithLine(entryT, exitT, lineP0, direction, AABBMin, AABBMax))
		return false;

	// clamp the returned line intersection point t-values to the line segment ends
	if (entryT < 0.0f)
		entryT = 0.0f;
	if (exitT > 1.0f)
		exitT = 1.0f;

	return (exitT >= entryT);
}

bool CollisionDetection::intersectAABBWithLine(Real &entryT, Real &exitT, const Vector2 &lineP0, const Vector2 &lineDir, const Vector2 &AABBMin, const Vector2 &AABBMax)
{
	// intersect line with all 4 box planes
	Vector2 inverseDir(lineDir);
	inverseDir.invert();

	Vector2 tAABBMin = inverseDir.multiply(AABBMin - lineP0);
	Vector2 tAABBMax = inverseDir.multiply(AABBMax - lineP0);

	// re-order intersections to find line entry and exit values (smallest & largest) for each axis
	Vector2 tMin = tAABBMin.minimum(tAABBMax);
	Vector2 tMax = tAABBMin.maximum(tAABBMax);

	// find the t values where the line enters and exits the AABB (largest tmin & smallest tmax)
	entryT	= maximum(tMin.x, tMin.y);
	exitT	= minimum(tMax.x, tMax.y);

	return (exitT >= entryT);
}

bool CollisionDetection::intersectAABBWithLine(Real &entryT, Real &exitT, const Vector3 &lineP0, const Vector3 &lineDir, const Vector3 &AABBMin, const Vector3 &AABBMax)
{
	// intersect ray with all six box planes
	Vector3 inverseDir(lineDir);
	inverseDir.invert();

	Vector3 tAABBMin = inverseDir.multiply(AABBMin - lineP0);
	Vector3 tAABBMax = inverseDir.multiply(AABBMax - lineP0);

	// re-order intersections to find ray entry and exit values (smallest & largest) for each axis
	Vector3 tMin = tAABBMin.minimum(tAABBMax);
	Vector3 tMax = tAABBMin.maximum(tAABBMax);

	// find the t values where the ray enters and exits the AABB (largest tmin & smallest tmax)
	entryT = maximum(tMin.x, tMin.y);
	entryT = maximum(tMin.z, entryT);

	exitT = minimum(tMax.x, tMax.y);
	exitT = minimum(tMax.z, exitT);

	return (exitT >= entryT);
}

bool CollisionDetection::intersectAABBWithTriangle(const Vector3 &AABBMin, const Vector3 &AABBMax,
	const Vector3 &a, const Vector3 &b, const Vector3 &c)
{
	// required stuff
	const Vector3 AABBCenter = (AABBMin + AABBMax) * 0.5f;
	const Vector3 AABBHalfVector = AABBCenter - AABBMin;

	// triangle in coordinate frame centered at AABB center
	const Vector3 v[3] = 	
	{
		a - AABBCenter,
		b - AABBCenter,
		c - AABBCenter
	};

	// AABB faces
	{
		// SA 00: AABB x-axis
		// is the triangle completely on the left or right side of the AABB?
		if (v[0].x < -AABBHalfVector.x && v[1].x < -AABBHalfVector.x &&	v[2].x < -AABBHalfVector.x)
			return false;
		if (v[0].x > AABBHalfVector.x && v[1].x > AABBHalfVector.x && v[2].x > AABBHalfVector.x)
			return false;

		// SA 01: AABB y-axis
		// is the triangle completely below or above the AABB?
		if (v[0].y < -AABBHalfVector.y && v[1].y < -AABBHalfVector.y &&	v[2].y < -AABBHalfVector.y)
			return false;
		if (v[0].y > AABBHalfVector.y && v[1].y > AABBHalfVector.y && v[2].y > AABBHalfVector.y)
			return false;
	
		// SA 02: AABB z-axis
		// is the triangle completely in front or behind the AABB?
		if (v[0].z < -AABBHalfVector.z && v[1].z < -AABBHalfVector.z &&	v[2].z < -AABBHalfVector.z)
			return false;
		if (v[0].z > AABBHalfVector.z && v[1].z > AABBHalfVector.z && v[2].z > AABBHalfVector.z)
			return false;
	}

	// triangle edges
	Vector3 triangleEdges[3];
	triangleEdges[0] = v[2] - v[0];
	triangleEdges[1] = v[1] - v[0];

	// rule out possible intersections based on separating axes theorem (SAT)
	// SA 03: triangle normal direction for triangle face (AABB in front or behind the triangle)
	{
		const Vector3 ortho = triangleEdges[0].crossProduct(triangleEdges[1]);

		const Real centerDistance = fabsr(ortho.dotProduct(v[0])); // d = ||ortho|| * |interval length along SA of AABB center to triangle|
		const Real requiredDistance = ortho.dotProductAbsolute(AABBHalfVector); // d = ||ortho|| * |interval length along SA of worst half vector to closest AABB corner|
		if (centerDistance > requiredDistance)
			return false;
	}

	// SA tests for planes parallel to triangle and AABB edges -> SAs orthogonal to triangle and AABB edges
	{
		const Vector3 AABBEdges[3] =
		{
			Vector3(1.0, 0.0, 0.0),
			Vector3(0.0, 1.0, 0.0),
			Vector3(0.0, 0.0, 1.0)
		};

		Vector3 separatingAxis;
		triangleEdges[2] = v[2] - v[1];

		//SA 04 - SA 12: orthogonal to AABB m-axis and triangle edge n-axis
		for (uint32 triangleEdgeIdx = 0; triangleEdgeIdx < 3; ++triangleEdgeIdx)
		{
			// choose triangle edge to test & set its opposite vertex index
			const Vector3 &triEdge = triangleEdges[triangleEdgeIdx];
			const uint32 oppoVertexIdx = (triangleEdgeIdx + 1) % 3;
			const Vector3 &edgeVertex = v[triangleEdgeIdx];
			const Vector3 &oppoVertex = v[oppoVertexIdx];

			for (uint32 AABBEdgeIdx = 0; AABBEdgeIdx < 3; ++AABBEdgeIdx)
			{
				const Vector3 &cubeEdge = AABBEdges[AABBEdgeIdx];
				separatingAxis = triEdge.crossProduct(cubeEdge);
				
				// lengths of intervals along axis from cube center to test edge & vertex opposite to the test edge
				const Real projectedLengthOppoVertex = separatingAxis.dotProduct(oppoVertex);
				const Real projectedLengthEdge = separatingAxis.dotProduct(edgeVertex);

				// cube is projected onto the interval [-requiredDistance, requiredDistance] along separating axis
				const Real requiredDistance = separatingAxis.dotProductAbsolute(AABBHalfVector);
				
				// triangle on the negative cube side along axis?
				if (projectedLengthOppoVertex < -requiredDistance && projectedLengthEdge < -requiredDistance)
					return false;
				// triangle on the positive cube side along axis?
				if (projectedLengthOppoVertex > requiredDistance && projectedLengthEdge > requiredDistance)
					return false;
			}
		}
	}

	return true;
}

bool CollisionDetection::intersectLineSegments(Vector2 *intersection, Real *alpha, bool *colinear,
	const Vector2 &startA, const Vector2 &endA, const Vector2 &startB, const Vector2 &endB)
{
	// initial results
	if (intersection)
		intersection->set(REAL_MAX, REAL_MAX);
	if (alpha)
		*alpha = REAL_MAX;
	if (colinear)
		*colinear = false;

	// as + l(ae - as) = bs + m(be - bs)
	// (ae - as) = aD
	// (bs - be) = bD
	// (bs - as) = l * aD + m * bD
	// (bs - as) = baD
	// x == cross product (a x b = scalar for a, b = Vector2)
	// I  -> both sides crossed by bD -> baD x bD = l * aD x bD
	// II -> both sides crossed by aD -> baD x aD = m * bD x aD
	// d = aD x bD (if d == 0 -> parallel line directions)
	// I ->  l = baD x bD / aD x bD = baD x bD / d
	// II -> m = baD x aD / bD x aD = aD x baD / d
	const Vector2 baD(startB - startA);
	const Vector2 aD(endA - startA);
	const Vector2 bD(startB - endB);

	// 3 possible cases: colinear, parallel, intersecting in a single point -> within line segements?
	const Real d = aD.crossProduct(bD);

	// line segment directions are not parallel?
	if (fabsr(d) > EPSILON)
	{
		// not parallel - do segments intersect within both segment ranges?
		const Real invD = 1.0f / d;

		const Real l = baD.crossProduct(bD) * invD;
		if (l < 0.0f || l > 1.0f)
			return false;

		const Real m = aD.crossProduct(baD) * invD;
		if (m < 0.0f || m > 1.0f)
			return false;

		// the intersection is within both segments
		if (intersection)
			*intersection = startA + (aD * l);
		if (alpha)
			*alpha = l;
		return true;
	}

	// parallel line segment directions
	const Real colinearity = baD.crossProduct(aD);
	if (fabsr(colinearity) > EPSILON)
		return false; // parallel line segments = parallel lines & some distance between them

	// colinear lines
	if (colinear)
		*colinear = true;

	// overlapping segments?
	// . = inner product
	// bs = as + l * aD <=> bs - as = l * aD <=> baD.aD = l * aD.aD
	// l0 = baD.aD / aD.aD
	// l1 = (be - as).aD / aD.aD
	const Real x  = aD.dotProduct(aD);
	const Real l0 = baD.dotProduct(aD) / x;
	const Real l1 = Vector2(endB - startA).dotProduct(aD) / x;

	// is line segement b completely behind or in front of line segment a? colinear segment without overlap?
	const Real minL = minimum(l0, l1);
	const Real maxL = maximum(l0, l1);
	if (maxL < 0.0f || minL > 1.0f)
		return false;

	// caller is interested in further data?
	if (!intersection && !alpha)
		return true;

	// only return intersection within segment A and not before it
	const Real finalL = (minL > 0.0f ? minL : 0.0f);

	// set intersection data
	if (intersection)
		*intersection = startA + aD * minL;
	if (alpha)
		*alpha = minL;
	return true;
}

bool CollisionDetection::intersectLineWithPlane(Real &t, Vector3 *intersection, bool *parallel,
	const Plane &plane, const Vector3 &p0, const Vector3 &lineDir)
{
	t = -REAL_MAX;
	if (intersection)
		intersection->set(REAL_MAX, REAL_MAX, REAL_MAX);
	if (parallel)
		*parallel = false;

	// derivation
	// 1. Vector3 p = p0 + t * lineDir;
	// 2. n.dotProduct(p) = planeDistance;
	// => n.dotProduct(p0 + t * lineDir) - planeDistance = 0;
	// => n.dotProduct(p0) - planeDistance + t * n.dotProduct(lineDir) = 0;
	// => t = (planeDistance - n.dotProduct(p0)) / n.dotProduct(lineDir)

	const Real cosANormalRay = plane.getNormal().dotProduct(lineDir);
	if (fabsr(cosANormalRay) < EPSILON) // is the ray parallel to the plane?
	{
		// confirm parallelity
		if (parallel)
			*parallel = true;

		// does the ray lie within the plane?
		const Real dRayToPlane = plane.getDistanceToPlane(p0);
		if (fabsr(dRayToPlane) < EPSILON)
		{
			if (intersection)
				*intersection = p0;
			t = 0;
			return true;
		}

		return false;
	}

	// line is not parallel to plane -> there is some intersection at p0 + t * lineDir
	t = -plane.getDistanceToPlane(p0) / cosANormalRay;

	// set intersection point and confirm intersection
	if (intersection)
		*intersection = p0 + lineDir * t;
	return true;
}

bool CollisionDetection::intersectLineWithSphere(Vector3 intersections[2], Real x[2],
	const Vector3 &p0, const Vector3 &lineDir, const Vector3 &sphereCoords, const Real sphereRadius)
{
	const Vector3 sP0 = (p0 - sphereCoords);
	const Real dot = sP0.dotProduct(lineDir);
	const Real sP02 = sP0.getLengthSquared();
	const Real r2 = sphereRadius * sphereRadius;
	const Real d2 = lineDir.getLengthSquared();

	// 0 = x^2 + (2 * dot / ||d||^2) * x + (||sP0||^2 - r^2) / ||d||^2
	// x^2 + p * x + q = 0 <-> x1,2 = -(p/2) +- sqrtr((p/2)^2 - q)
	// p = (2 * dot / ||d||^2)
	const Real p_2 = dot / d2;
	// q = (||sP0||^2 - r^2) / ||d||^2
	const Real q = (sP02 - r2) / d2;

	// no intersection if the root cannot be computed
	const Real underSqrt = p_2 * p_2 - q;
	if (underSqrt < 0.0f)
		return false;

	// user is not interested in intersection points?
	if (!intersections || !x)
		return true;

	// x0, x1
	const Real temp = sqrtr(underSqrt);
	x[0] = -p_2 - temp;
	x[1] = -p_2 + temp;

	// intersection points
	intersections[0] = p0 + lineDir * x[0];
	intersections[1] = p0 + lineDir * x[1];
	return true;
}

bool CollisionDetection::intersectLineWithSphereNormalized(Vector3 intersections[2], Real x[2],
	const Vector3 &p0, const Vector3 &unitLengthLineDir, const Vector3 &sphereCoords, const Real sphereRadius)
{
	const Vector3 sP0 = (p0 - sphereCoords);
	const Real dot = sP0.dotProduct(unitLengthLineDir);
	const Real sP02 = sP0.getLengthSquared();
	const Real r2 = sphereRadius * sphereRadius;

	// 0 = x^2 + (2 * dot) * x + (||sP0||^2 - r^2)
	// x^2 + p * x + q = 0 <-> x1,2 = -(p/2) +- sqrtr((p/2)^2 - q)
	// p = (2 * dot)
	const Real p_2 = dot;
	// q = (||sP0||^2 - r^2)
	const Real q = (sP02 - r2);

	// no intersection if the root cannot be computed
	const Real underSqrt = p_2 * p_2 - q;
	if (underSqrt < 0.0f)
		return false;

	// user is not interested in intersection points?
	if (!intersections || !x)
		return true;

	// x0, x1
	const Real temp = sqrtr(underSqrt);
	x[0] = -p_2 - temp;
	x[1] = -p_2 + temp;

	// intersection points
	intersections[0] = p0 + unitLengthLineDir * x[0];
	intersections[1] = p0 + unitLengthLineDir * x[1];
	return true;
}

uint32 CollisionDetection::intersectLineSegmentWithFilledTriangle(Vector2 intersections[2], Real alphas[2], bool inside[2],
		const Vector2 &s0, const Vector2 &s1, const Vector2 triangle[3])
{
	// first test the complete line going through segment [s0, s1]
	inside[0] = inside[1] = false;
	uint32 count = intersectLineWithTriangle(intersections, alphas, s0, (s1 - s0), triangle);

	// no intersection?
	if (0 == count)
		return 0;

	// no intersection within line segment [s0, s1]?
	// intersection before or after the test line segement
	if ((1 == count && (alphas[0] < 0.0f || alphas[0] > 1.0f)) ||
		(alphas[1] < 0.0f || alphas[0] > 1.0f))
	{
		alphas[0] = alphas[1] = REAL_MAX;
		intersections[0].set(REAL_MAX, REAL_MAX);
		intersections[1].set(REAL_MAX, REAL_MAX);
		return 0;
	}

	// only touching?
	if (1 == count)
		return 1;

	// clamp the intersections to the line segment
	if (alphas[0] < 0.0f)
	{
		alphas[0] = 0.0f;
		intersections[0] = s0;
		inside[0] = true;
	}
	if (alphas[1] > 1.0f)
	{
		alphas[1] = 1.0f;
		intersections[1] = s1;
		inside[1] = true;
	}

	return count;
}

uint32 CollisionDetection::intersectLineWithTriangle(Vector2 intersections[2], Real alphas[2],
	const Vector2 &p0, const Vector2 &lineDir,	const Vector2 triangle[3])
{
	// also see intersectLineSegments
	// initial results: no intersections
	uint32 count = 0;
	alphas[0] = alphas[1] = REAL_MAX;
	intersections[0].set(REAL_MAX, REAL_MAX);
	intersections[1].set(REAL_MAX, REAL_MAX);

	// precalculations
	const Vector2 p0ToS0s[3] = { triangle[0] - p0, triangle[1] - p0, triangle[2] - p0 };
	const Real lineDirDotP0ToS0s[3] =
	{
		lineDir.crossProduct(p0ToS0s[0]),
		lineDir.crossProduct(p0ToS0s[1]),
		lineDir.crossProduct(p0ToS0s[2])
	};

	// s0 + m * (s1 - s0) = p0 + l * lineDir
	// <-> m * (s1 - s0) x lineDir = (p0 - s0) x lineDir + l * lineDir x lineDir
	// <-> m = ((p0 - s0) x lineDir + l * 0) / ((s1 - s0) x lineDir)
	// <-> m = (lineDir x (s0 - p0)) / ((s1 - s0) x lineDir)
	//   l = ((s1 - s0) x (s0 - p0)) / ((s1 - s0) x lineDir)
	for (uint32 idx0 = 0; idx0 < 3 && count < 2; ++idx0)
	{
		// get triangle side idx0
		const uint32 idx1 = (idx0 + 1) % 3;
		const Vector2 &s1 = triangle[idx1];
		const Vector2 &s0 = triangle[idx0];
		const Vector2 &p0ToS0 = p0ToS0s[idx0];

		// denominator = ((s1 - s0) x lineDir), if parallel -> d ~= 0
		const Vector2 s0ToS1(s1 - s0);
		const Real denominator = s0ToS1.crossProduct(lineDir);
		
		// 3 possible cases: colinear, parallel, intersecting in a single point -> witin triangle side line segment?
		// intersecting?
		if (fabsr(denominator) > EPSILON)
		{
			// within line segment?
			const Real invD = 1.0f / denominator;
			const Real m = lineDirDotP0ToS0s[idx0] * invD;
			if (m < 0.0f || m > 1.0f)
				continue;

			// l = ((s1 - s0) x (s0 - p0)) / ((s1 - s0) x lineDir)
			alphas[count] = s0ToS1.crossProduct(p0ToS0) * invD;

			// only touching one triangle corner?
			if (m < EPSILON)
			{
				intersections[0] = s0;
				return 1;
			}

			// try to find the second intersection
			intersections[count] = p0 + lineDir * alphas[count];
			++count;
			continue;
		}
		
		// parallel directions: only parallel or colinear 
		// colinear but some distance between them?
		const Real colinearity = p0ToS0.crossProduct(lineDir);
		if (EPSILON < fabsr(colinearity))
			continue;
		
		// colinear: triangle side is completely on the line (triangle side = overlap area)
		const Axis axis = (lineDir.x > lineDir.y ? AXIS_X : AXIS_Y);
		alphas[0] = p0ToS0[axis] / lineDir[axis];
		alphas[1] = p0ToS0s[idx1][axis] / lineDir[axis];
		intersections[0] = s0;
		intersections[1] = s1;
		count = 2;
		break;
	}

	// order the intersections
	if (1 < count && alphas[0] > alphas[1])
	{
		Utilities::swap(alphas[0], alphas[1]);
		Utilities::swap(intersections[0], intersections[1]);
	}

	return count;
}

bool CollisionDetection::intersectRayWithLineSegment(Vector2 *intersection, bool *colinear,
	const Vector2 &p0, const Vector2 &p1, const Vector2 &rayStart, const Vector2 &rayDir)
{
	// initial results
	if (colinear)
		*colinear = false;
	if (intersection)
		intersection->set(REAL_MAX, REAL_MAX);

	// p0 + l(p1 - p0) = rayStart + m * rayDir
	// (p1 - p0) = segDiff
	// (rayStart - p0) = l * segDiff - m * rayDir
	// (rayStart - p0) = rsp0D
	// x == cross product (a x b = scalar for a, b = Vector2)
	// I  -> both sides crossed by rayDir -> rsp0D x rayDir = l * segDiff x rayDir
	// II -> both sides crossed by segDiff -> rsp0D x segDiff = -m * rayDir x segDiff
	// d = segDiff x rayDir (if d == 0 -> parallel line directions)
	// I ->  l = rsp0D x rayDir / segDiff x rayDir = rsp0D x rayDir / d
	// II -> m = -rsp0D x segDiff / rayDir x segDiff = -rsp0D x segDiff  / -d = rsp0D x segDiff / d
	const Vector2 rsp0D(rayStart.x - p0.x, rayStart.y - p0.y);
	const Vector2 segDiff(p1.x - p0.x, p1.y - p0.y);

	// 3 possible cases: colinear, parallel, intersecting in a single point (next: intersection inside or outside the test elements?)
	const Real d = segDiff.crossProduct(rayDir);

	// line segment direction and ray direction are not parallel?
	if (fabsr(d) > EPSILON)
	{
		// not parallel - intersection within elements?
		const Real invD = 1.0f / d;

		const Real l = rsp0D.crossProduct(rayDir) * invD;
		if (l < 0.0f || l > 1.0f)
			return false;

		const Real m = rsp0D.crossProduct(segDiff) * invD;
		if (m < 0.0f) // not " || m > 1.0f)" as ray with infinite length
			return false;

		// the intersection is within the line segment and within the non-negative ray space
		if (intersection)
			*intersection = rayStart + (rayDir * m);
		return true;
	}

	// parallel directions

	// parallel directions & some distance between ray and line segment?
	if (rsp0D.getLengthSquared() > EPSILON * EPSILON)
		return false;
	const Real colinearity = rsp0D.crossProduct(segDiff);
	if (fabsr(colinearity) > EPSILON)
		return false;

	// colinear ray and line segment
	if (colinear)
		*colinear = true;

	// overlapping?
	// . = inner product
	// p0 = rayStart + m0 * rayDir <=> p0 - rayStart = m0 * rayDir <=> -rsp0D.rayDir = m0 * rayDir.rayDir
	// m0 = -rsp0D.rayDir / rayDir.rayDir
	// m1 = -rsp1D.rayDir / rayDir.rayDir
	const Real temp = rayDir.dotProduct(rayDir);
	const Real m0 = -(rsp0D.dotProduct(rayDir) / temp);
	const Real m1 =  (p1 - rayStart).dotProduct(rayDir) / temp;

	// line segment is behind the ray start?
	if (m0 < 0.0f && m1 < 0.0f)
		return false; // colinear points, but there is no intersection

	// intersection point = the point closest to rayStart which is within the line segment & on positive ray half
	if (intersection)
		if (m0 <= 0.0f || m1 <= 0.0f)
			*intersection = rayStart;
		else
			*intersection = (m0 <= m1 ? p0 : p1);

	return true;
}


bool CollisionDetection::intersectPlaneWithSphere(Vector3 *intersectionCenter, Real *intersectionRadius,
	const Plane &plane,	const Vector3 &sphereCoords, const Real sphereRadius)
{
	// sphere is close enough to plane for an intersection?
	const Real centerDistance = plane.getDistanceToPlane(sphereCoords);
	if (fabsr(centerDistance) > sphereRadius)
		return false;
	
	if (!intersectionCenter)
		return true;
	
	// circle of intersection
	*intersectionCenter = plane.getClosestPoint(sphereCoords);
	if (intersectionRadius)
		*intersectionRadius = sqrtr(sphereRadius * sphereRadius - centerDistance * centerDistance);
	return true;
}

bool CollisionDetection::intersectRayWithPlane(Vector3 *intersection, bool *parallel,
	const Plane &plane, const Vector3 &rayStart, const Vector3 &rayDir)
{
	Real t;
	bool hit = intersectLineWithPlane(t, intersection, parallel, plane, rayStart, rayDir);
	if (hit && t < 0.0f)
	{
		if (intersection)
			intersection->set(REAL_MAX, REAL_MAX, REAL_MAX);
		return false;
	}

	return true;
}

bool CollisionDetection::intersectRayWithTriangle(Vector3 &intersection,
	const bool backfaceCulling, const Vector3 &rayStart, const Vector3 &rayDir,
	const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 *triangleNormal)
{
	// todo rather the implementation of Christer Ericson's Real-Time Collision Detection book, second edition, pages 585-588
	// get triangle normal
	const Vector3 planeNormal = (triangleNormal ? *triangleNormal : (b - a).crossProduct(c - a));

	// backface culling?
	if (backfaceCulling)
		if (rayDir.dotProduct(planeNormal) >= 0.0f)
			return false;

	// triangle - plane test
	const Plane trianglePlane(a, planeNormal, false);
	if (!intersectRayWithPlane(&intersection, NULL, trianglePlane, rayStart, rayDir))
		return false;

	// compute Barycentric coordinates of intersection w.r.t. specified triangle
	Real u, v, w;
	getBarycentric<Vector3>(u, v, w, intersection, a, b, c);

	return (u >= 0.0f && u <= 1.0f) && (v >= 0.0f && v <= 1.0f) && (w >= 0.0f && w <= 1.0f);
}

bool CollisionDetection::isPointInAABB(const Vector2 &p, const Vector2 AABB[2])
{
	return isPointInAABB(p, AABB[0], AABB[1]);
}

bool CollisionDetection::isPointInAABB(const Vector2 &p, const Vector2 &AABBMin, const Vector2 &AABBMax)
{
	return (p.x >= AABBMin.x && p.y >= AABBMin.y &&
			p.x <= AABBMax.x && p.y <= AABBMax.y);
}

bool CollisionDetection::isPointInAABB(const Vector3 &p, const Vector3 AABB[2])
{
	return isPointInAABB(p, AABB[0], AABB[1]);
}

bool CollisionDetection::isPointInAABB(const Vector3 &p, const Vector3 &AABBMin, const Vector3 &AABBMax)
{
	return (p.x >= AABBMin.x && p.x <= AABBMax.x &&
			p.y >= AABBMin.y && p.y <= AABBMax.y &&
			p.z >= AABBMin.z && p.z <= AABBMax.z);
}

bool CollisionDetection::intersectSegmentsOnLine(Vector2 intersections[2], Real alphas[2],
	const Vector2 &startA, const Vector2 &endA,
	const Vector2 &startB, const Vector2 &endB)
{
	// actually 1D problem -> choose an axis
	const Vector2 diff = endA - startA;
	const Axis axis = (diff.x > diff.y ? AXIS_X : AXIS_Y);
	
	// where w.r.t. the line segment A is segment B?
	const Real tStartB = (startB[axis] - startA[axis]) / diff[axis];
	const Real tEndB = (endB[axis] - startA[axis]) / diff[axis];

	// overlap? (A = [0, 1], B = [minB, maxB] w.r.t. segment A)
	const Real minB = minimum(tStartB, tEndB);
	const Real maxB = maximum(tStartB, tEndB);
	if (minB > 1.0f || maxB < 0.0f)
	{
		if (alphas)
			alphas[0] = alphas[1] = REAL_MAX;

		if (intersections)
		{
			intersections[0].set(REAL_MAX, REAL_MAX);
			intersections[1].set(REAL_MAX, REAL_MAX);
		}

		return false;
	}

	// stop here or compute further data
	if (!intersections && !alphas)
		return true;

	// get tStart, tEnd \in [0, 1]
	const Real finalStartT = (minB > 0.0f ? minB : 0.0f);
	const Real finalEndT = (maxB < 1.0f ? maxB : 1.0f);

	// set scalars w.r.t. segment A for overlap borders
	if (alphas)
	{
		alphas[0] = finalStartT;
		alphas[1] = finalEndT;
	}

	// compute intersection border positions
	if (intersections)
	{
		intersections[0] = startA + diff * finalStartT;
		intersections[1] = startA + diff * finalEndT;
	}

	return true;
}