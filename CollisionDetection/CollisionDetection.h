/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _COLLISION_DETECTION_H_
#define _COLLISION_DETECTION_H_

#include <cassert>
#include "CollisionDetection/ObliqueCircularCone.h"
#include "CollisionDetection/Plane.h"
#include "Math/MathCore.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Platform/DataTypes.h"

namespace CollisionDetection
{
	/** Computes the distance of the point at position to the axis aligned bounding box (AABB) defined by AABBMin & AABBMax.
	@param position Set this coordinates of a point which is within the same frame as the entered AABB.
	@param AABBMin Set this to the minimum point of the AABB to be tested against the specified position.
	@param AABBMax Set this to the maximum point of the AABB to be tested against the specified position.
	@return Returns the Euclidean distance between the point at position and the closest surface point at the entered AABB.
		Zero is returned if position is inside the AABB. Border regions of the AABB are treated as inside.*/
	Real getDistanceToAABB(const Math::Vector2 &position, const Math::Vector2 &AABBMin, const Math::Vector2 &AABBMax);

	/** Computes the distance of the point at position to the axis aligned bounding box (AABB) defined by AABBMin & AABBMax.
	@param position Set this coordinates of a point which is within the same frame as the entered AABB.
	@param AABBMin Set this to the minimum point of the AABB to be tested against the specified position.
	@param AABBMax Set this to the maximum point of the AABB to be tested against the specified position.
	@return Returns the Euclidean distance between the point at position and the closest surface point at the entered AABB.
		Zero is returned if position is inside the AABB. Border regions of the AABB are treated as inside. */
	Real getDistanceToAABB(const Math::Vector3 &position, const Math::Vector3 &AABBMin, const Math::Vector3 &AABBMax);

	/** Computes the distance of the point x to the infinite line which goes through p0 and p1.
	@param x Set this to the point coordinates you want to know the distance for w.r.t. the entered infinitely long line.
	@param p0 Set this to a point at the infinite line you want to get the distance to w.r.t. x whereas p0 must not be equal to p1.
	@param p1 Set this to a point at the infinite line you want to get the distance to w.r.t. x whereas p1 must not be equal to p0. */
	Real getDistanceToLine(const Math::Vector3 &x, const Math::Vector3 &p0, const Math::Vector3 &p1);

	/** Computes the distance of the point x to the infinite line which goes through p0 and p1.
	@param basePoint If basePoint is not NULL then the vector is set to the point which is at the infinite line and
		base point has the property that the line segment (x, basePoint) is perpendicular to the infinite line. (basePoint \in line(p0, p1) ^ lineSegment(basePoint, x).dotProduct((p1 - p0)) = 0
		basePoint is the projection of x onto the infinite line.
	@param x Set this to the point coordinates you want to know the distance for w.r.t. the entered infinitely long line.
	@param p0 Set this to a point at the infinite line you want to get the distance to w.r.t. x whereas p0 must not be equal to p1.
	@param p1 Set this to a point at the infinite line you want to get the distance to w.r.t. x whereas p1 must not be equal to p0. */
	Real getDistanceToLine(Math::Vector3 *basePoint, const Math::Vector3 &x, const Math::Vector3 &p0, const Math::Vector3 &p1);

	/** Computes the distance of the point x to the line segement between p0 and p1. 
	@param closestPoint If closestPoint is not NULL then the vector is set to the point \in line segment(p0, p1) which has the shortest distance to x.
		This is either p0, p1 or a point between p0 and p1.	
	@param x Set this to the point coordinates you want to know the distance for w.r.t. the entered line segment.
	@param p0 Set this to one of the two end points of the line segment you want to check.
	@param p1 Set this to the other of the two end points of the line segment you want to check. */
	Real getDistanceToLineSegment(Math::Vector2 *closestPoint, const Math::Vector2 &x, const Math::Vector2 &p0, const Math::Vector2 &p1);

	/** Computes the distance of the point x to the line segement between p0 and p1. 
	@param closestPoint If closestPoint is not NULL then the vector is set to the point \in line segment(p0, p1) which has the shortest distance to x.
		This is either p0, p1 or a point between p0 and p1.	
	@param x Set this to the point coordinates you want to know the distance for w.r.t. the entered line segment.
	@param p0 Set this to one of the two end points of the line segment you want to check.
	@param p1 Set this to the other of the two end points of the line segment you want to check. */
	Real getDistanceToLineSegment(Math::Vector3 *closestPoint, const Math::Vector3 &x, const Math::Vector3 &p0, const Math::Vector3 &p1);

	/** Checks two axis aligned bounding boxes (AABBs) for overlap / intersection.
	@param AABB0Min Set this to the minimum coordinates (minimum corner) of the first AABB.
	@param AABB0Max Set this to the maximum coordinates (maximum corner) of the first AABB.
	@param AABB1Min Set this to the minimum coordinates (minimum corner) of the second AABB.
	@param AABB1Max Set this to the maximum coordinates (maximum corner) of the second AABB.
	@return Returns true if there must be an overlap between both boxes or false if separating axis theorem (SAT) says there is a gap between both. */
	inline bool intersectAABBWithAABB(const Math::Vector3 &AABB0Min, const Math::Vector3 &AABB0Max, const Math::Vector3 &AABB1Min, const Math::Vector3 &AABB1Max);

	/** Intersects an infinite long line with an axis aligned bounding box (AABB).
	The implementation is based on http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm.
	@param entryT This is set to the t value of the entry intersection point of the line with the box. (entryT * lineDir + lineP0 = entryPoint)
	@param exitT This is set to the t value of the exit intersection point of the line with the box. (exitT * lineDir + lineP0 = exitPoint)
	@param lineP0 Set this to a point on the line.
	@param lineDir Set this to the direction vector of the line to be tested against the box.
	@param AABBMin Set this to the minimum point of the AABB to be tested against the specified line.
	@param AABBMax Set this to the maximum point of the AABB to be tested against the specified line.
	@return Returns true if the line overlaps/intersects the box. False is returned if the line doesn't intersect the axis aligned bounding box.*/
	bool intersectAABBWithLine(Real &entryT, Real &exitT, const Math::Vector2 &lineP0, const Math::Vector2 &lineDir, const Math::Vector2 &AABBMin, const Math::Vector2 &AABBMax);

	/** Intersects an infinite long line with an axis aligned bounding box (AABB).
	The implementation is based on http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm.
	@param entryT This is set to the t value of the entry intersection point of the line with the box. (entryT * lineDir + lineP0 = entryPoint)
	@param exitT This is set to the t value of the exit intersection point of the line with the box. (exitT * lineDir + lineP0 = exitPoint)
	@param lineP0 Set this to a point on the line.
	@param lineDir Set this to the direction vector of the line to be tested against the box.
	@param AABBMin Set this to the minimum point of the AABB to be tested against the specified line.
	@param AABBMax Set this to the maximum point of the AABB to be tested against the specified line.
	@return Returns true if the line overlaps/intersects the box. False is returned if the line doesn't intersect the axis aligned bounding box.*/
	bool intersectAABBWithLine(Real &entryT, Real &exitT, const Math::Vector3 &lineP0, const Math::Vector3 &lineDir, const Math::Vector3 &AABBMin, const Math::Vector3 &AABBMax);

	/** Intersects a line segment with an axis aligned bounding box (AABB).
	The implementation is based on http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm.
	@param entryT This is set to a value in the interval [0, 1] if there is an intersection.
	entryT is set to the t value of the entry intersection point of the line segment with the box. (entryT * (lineP1 -lineP0) + lineP0 = entryPoint)
	@param exitT This is set to a value in the interval [0, 1] if there is an intersection.
	exitT is set to the t value of the exit intersection point of the line segment with the box. (exitT * (lineP1 -lineP0) + lineP0 = exitPoint)
	@param lineP0 Set this to one end point of the line segment.
	@param lineP1 Set this to the other end point of the line segment.
	@param AABBMin Set this to the minimum point of the AABB to be tested against the specified line segment.
	@param AABBMax Set this to the maximum point of the AABB to be tested against the specified line segment.
	@return Returns true if the line segment intersects the AABB. False is returned if the line segment doesn't intersect the AABB.*/
	bool intersectAABBWithLineSegment(Real &entryT, Real &exitT, const Math::Vector2 &lineP0, const Math::Vector2 &lineP1, const Math::Vector2 &AABBMin, const Math::Vector2 &AABBMax);

	/** Intersects a line segment with an axis aligned bounding box (AABB).
	The implementation is based on http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm.
	@param entryT This is set to a value in the interval [0, 1] if there is an intersection.
	entryT is set to the t value of the entry intersection point of the line segment with the box. (entryT * (lineP1 -lineP0) + lineP0 = entryPoint)
	@param exitT This is set to a value in the interval [0, 1] if there is an intersection.
	exitT is set to the t value of the exit intersection point of the line segment with the box. (exitT * (lineP1 -lineP0) + lineP0 = exitPoint)
	@param lineP0 Set this to one end point of the line segment.
	@param lineP1 Set this to the other end point of the line segment.
	@param AABBMin Set this to the minimum point of the AABB to be tested against the specified line segment.
	@param AABBMax Set this to the maximum point of the AABB to be tested against the specified line segment.
	@return Returns true if the line segment intersects the AABB. False is returned if the line segment doesn't intersect the AABB.*/
	bool intersectAABBWithLineSegment(Real &entryT, Real &exitT, const Math::Vector3 &lineP0, const Math::Vector3 &lineP1, const Math::Vector3 &AABBMin, const Math::Vector3 &AABBMax);

	/** Intersects a ray with an axis aligned bounding box (AABB).
		The implementation is based on http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm.
	@param entryT This is set to the t value of the entry intersection point of the ray with the box. (entryT * rayDir + rayOrigin = entryPoint)
	@param exitT This is set to the t value of the exit intersection point of the ray with the box. (exitT * rayDir + rayOrigin = exitPoint)
	@param rayOrigin Set this to the point where the ray starts.
	@param rayDir Set this to the direction vector of the ray to be tested against the box.
	@param AABBMin Set this to the minimum point of the AABB to be tested against the specified ray.
	@param AABBMax Set this to the maximum point of the AABB to be tested against the specified ray.
	@return Returns true if the ray hits the box.
		False is returned if the ray doesn't intersect the axis aligned bounding box.**/
	inline bool intersectAABBWithRay(Real &entryT, Real &exitT, const Math::Vector2 &rayOrigin, const Math::Vector2 &rayDir, const Math::Vector2 &AABBMin, const Math::Vector2 &AABBMax);

	/** Intersects a ray with an axis aligned bounding box (AABB).
		The implementation is based on http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm.
	@param entryT This is set to the t value of the entry intersection point of the ray with the box. (entryT * rayDir + rayOrigin = entryPoint)
	@param exitT This is set to the t value of the exit intersection point of the ray with the box. (exitT * rayDir + rayOrigin = exitPoint)
	@param rayOrigin Set this to the point where the ray starts.
	@param rayDir Set this to the direction vector of the ray to be tested against the box.
	@param AABBMin Set this to the minimum point of the AABB to be tested against the specified ray.
	@param AABBMax Set this to the maximum point of the AABB to be tested against the specified ray.
	@return Returns true if the ray hits the box.
		False is returned if the ray doesn't intersect the axis aligned bounding box. **/
	inline bool intersectAABBWithRay(Real &entryT, Real &exitT, const Math::Vector3 &rayOrigin, const Math::Vector3 &rayDir, const Math::Vector3 &AABBMin, const Math::Vector3 &AABBMax);

	/** Checks whether there is an intersection between the entered axis aligned bounding box and the sphere.
	@param AABBMin Set this to the coordinates of the lower, left = minimum corner of the AABB.
	@param AABBMax Set this to the coordinates of the upper, right = maximum corner of the AABB.
	@param sphereCoords Set this to the coords of the center point of the sphere.
	@param sphereRadius Set this to the size of the sphere.
	@return Returns true if the sphere and the AABB overlap. */
	bool intersectAABBWithSphere(const Math::Vector2 &AABBMin, const Math::Vector2 &AABBMax, const Math::Vector2 sphereCoords, const Real sphereRadius);

	/** Checks whether there is an intersection between the entered axis aligned bounding box and the sphere.
	@param AABBMin Set this to the coordinates of the lower, left, front = minimum corner of the AABB.
	@param AABBMax Set this to the coordinates of the upper, right, back = maximum corner of the AABB.
	@param sphereCoords Set this to the coords of the center point of the sphere.
	@param sphereRadius Set this to the size of the sphere.
	@return Returns true if the AABB and sphere overlap. */
	bool intersectAABBWithSphere(const Math::Vector3 &AABBMin, const Math::Vector3 &AABBMax, const Math::Vector3 sphereCoords, const Real sphereRadius);
	
	/** Checks whether there is an intersection between the entered axis aligned bounding box and triangle.
	@param AABBMin Set this to the coordinates of the lower, left, front = minimum corner of the AABB.
	@param AABBMax Set this to the coordinates of the upper, right, back = maximum corner of the AABB.
	@param v0 Set this to the coordinates of the 1st triangle vertex/corner.
	@param v1 Set this to the coordinates of the 2nd triangle vertex/corner.
	@param v2 Set this to the coordinates of the 3rd triangle vertex/corner.
	@return Returns true if the AABB and triangle overlap. */
	bool intersectAABBWithTriangle(const Math::Vector3 &AABBMin, const Math::Vector3 &AABBMax,
		const Math::Vector3 &v0, const Math::Vector3 &v1, const Math::Vector3 &v2);

	/** Computes intersection data for two line segments [startA, endA] and [startB, endB].
	@param intersection Set this to a Vector2 variable address to get the intersection point which is closest to startA and within [startA, startB].
		If there is no intersection then the vector is set to (REAL_MAX, REAL_MAX).
		It is ignored if intersection is set to NULL.
	@param alpha Set this to an address to get the intersection distance w.r.t. line segement A: intersection point = startA + alpha * (endA - startA)
		If there is no intersection then the value is set to REAL_MAX.
		It is ignored if alpha is set to NULL.
	@param colinear Is set to true if all 4 line points lie on the same straight line otherwise it is set to false.
		It is ignored if colinear is set to NULL.
	@param startA Set this to the start point of segement A.
	@param endA Set this to the end point of segment A.
	@param startB Set this to the start point of segement B.
	@param endB Set this to the end point of segment B. */
	bool intersectLineSegments(Math::Vector2 *intersection, Real *alpha, bool *colinear,
							   const Math::Vector2 &startA, const Math::Vector2 &endA,
							   const Math::Vector2 &startB, const Math::Vector2 &endB);

	/** Checks for intersection between a line segment defined by two points and an infinite plane.
	@param t If there is an intersection then t is set to fulfill intersection position = p0 + t * lineDir and t \in [0, 1]. (t = 0 -> p0, t = 1 -> p1)
	@param intersection Set this to a Vector3 address to retrieve the intersection position or NULL if you don't need it.
	@param parallel Set this to a bool address to retrieve whether the line and the plane are parallel to each other or NULL if you're not interested.
	@param plane Set this to the infinitely large and thin plane you want to test against the line segement.
	@param p0 Set this to one of the two points of the line segement you want to test for intersection.
	@param p1 Set this to the other of the two points of the line segement you want to test for intersection. */
	inline bool intersectLineSegmentWithPlane(Real &t, Math::Vector3 *intersection, bool *parallel,
		const Plane &plane, const Math::Vector3 &p0, const Math::Vector3 &p1);
	
	/** Tests whether a line segments intersects with a triangle in 2D.
	@param intersections Set this to a pointer to an array for storing up to two ordered intersection points.
		intersections[0] = first intersection, intersections[1] = second intersection w.r.t. the entered segment.
		If the segment starts or ends within the triangle then the corresponding intersection point is set to the line segment start or end respectively.
		The triangle is assumed to be filled. E.g., a segment starting outside the triangle and ending inside the triangle
		has an overlap region starting at the entering position = intersections[0] until the segment end = intersections[1]
		Vectors are set to (REAL_MAX, REAL_MAX) if there is no intersection.
	@param alphas Set this to an address of an array of two real values to get the ordered, relative intersection distances w.r.t. the line segment (0 = s0, 1 = s1):
		intersection points[0,1] = s0 + alphas[0,1] * (s1 - s0) with alphas[0] <= alphas[1] 
		If there is no intersection then the values are set to REAL_MAX.
	@param inside This is set to true if an intersection point is within the triangle, meaning s0 or s1 is inside and one or both are the respective intersection points.
		E.g., if inside[0]: intersections[0] = s0, s0 is within the triangle, if inside[0] && inside[1]: the complete segment is within the triangle and intersections[0] = s0 and intersections[1] = s1.
	@param s0 Set this to the start point of the line segment.
	@param s1 Set this to the end point of the line segment.
	@param triangle Set this to the three corner points of the triangle.
	@return Returns the number of intersections stored in intersections between the line segment and the triangle.
		(0 = no intersection, 1 = touching, 2 = the segment crosses the triangle) */
	uint32 intersectLineSegmentWithFilledTriangle(Math::Vector2 intersections[2], Real alphas[2], bool inside[2],
		const Math::Vector2 &s0, const Math::Vector2 &s1, const Math::Vector2 triangle[3]);

	/** Checks for intersection between an infinite line and an infinite plane.
	@param t If there is an intersection then t is set to fulfill intersection position = p0 + t * lineDir.
	@param intersection Set this to a Vector3 address to retrieve the intersection position or NULL if you don't need it.
	@param parallel Set this to a bool address to retrieve whether the line and the plane are parallel to each other or NULL if you're not interested.
	@param plane Set this to the infinitely large and thin plane you want to test against the line segement.
	@param p0 Set this to any point on the infinite line you want to check for intersection.
	@param lineDir Set this the direction of the infinite line. (= difference of two points on the line). Does not need to have unit length. */
	bool intersectLineWithPlane(Real &t, Math::Vector3 *intersection, bool *parallel,
		const Plane &plane, const Math::Vector3 &p0, const Math::Vector3 &lineDir);

	/** Computes whether there is / are intersections between a sphere and an infinitely long line.
	@param intersections Set this to NULL if you are not interested in the intersection points.
		Set it to a pointer of an array of two vectors which will store the intersection points:
		intersections[0] = p0 + x[0] * lineDir and intersections[1] = p0 + x[1] * lineDir with x[0] < x[1].
		Is only set if this and parameter x are not NULL and if there is an intersection.
	@param x Set this to NULL if you are not interested in the intersection points.
		This is set to the 2 scalar values x[0], x[1] of the intersection points with the properites:
		intersections[0] = p0 + x[0] * lineDir and intersections[1] = p0 + x[1] * lineDir with x[0] < x[1].
		Is only set if this and parameter x are not NULL and if there is an intersection.
	@param p0 Set this to any point on the line you want to test against the sphere.
	@param lineDir Set this the vector representing the direction of the line (must be colinear with the line).
		It does not need to have unit length.
	@param sphereCoords Set this to the center position of the sphere you want to test against the line.
	@param sphereRadius Set this to the radius of the sphere you want to test against the line. */
	bool intersectLineWithSphere(Math::Vector3 intersections[2], Real x[2],
		const Math::Vector3 &p0, const Math::Vector3 &lineDir, const Math::Vector3 &sphereCoords, const Real sphereRadius);

	/** Computes whether there is / are intersections between a sphere and an infinitely long line.
	@param intersections Set this to NULL if you are not interested in the intersection points.
		Set it to a pointer of an array of two vectors which will store the intersection points:
		intersections[0] = p0 + x[0] * lineDir and intersections[1] = p0 + x[1] * lineDir with x[0] < x[1].
		Is only set if this and parameter x are not NULL and if there is an intersection.
	@param x Set this to NULL if you are not interested in the intersection points.
		This is set to the 2 scalar values x[0], x[1] of the intersection points with the properites:
		intersections[0] = p0 + x[0] * lineDir and intersections[1] = p0 + x[1] * lineDir with x[0] < x[1].
		Is only set if this and parameter x are not NULL and if there is an intersection.
	@param p0 Set this to any point on the line you want to test against the sphere.
	@param unitLengthLineDir Set this the vector representing the direction of the line (must be colinear with the line).
		It must have unit length.
	@param sphereCoords Set this to the center position of the sphere you want to test against the line.
	@param sphereRadius Set this to the radius of the sphere you want to test against the line. */
	bool intersectLineWithSphereNormalized(Math::Vector3 intersections[2], Real x01[2],
		const Math::Vector3 &p0, const Math::Vector3 &unitLengthLineDir, const Math::Vector3 &sphereCoords, const Real sphereRadius);

	/** Tests whether a line intersects with a triangle in 2D.
	@param intersections Set this to a pointer to an array for storing up to two ordered intersection points.
		intersections[0] = first intersection, intersections[1] = second intersection w.r.t. lineDir
		Vectors are set to (REAL_MAX, REAL_MAX) if there is no intersection.
	@param alphas Set this to an address of an array of two real values to get the ordered, relative intersection distances w.r.t. the line:
		intersection points[0,1] = p0 + alphas[0,1] * lineDir with alphas[0] <= alphas[1] 
		If there is no intersection then the values are set to REAL_MAX.
	@param p0 Set this to a point on the infinitesimal long line.
	@param lineDir Set this to a vector pointing into the same direction as the infinitesimal long line
	@param triangle Set this to the three corner points of the triangle.
	@return Returns the number of intersections stored in intersections between the line segment and the triangle.
		(0 = no intersection, 1 = touching, 2 = the segment crosses the triangle) */
	uint32 intersectLineWithTriangle(Math::Vector2 intersections[2], Real alphas[2],
		const Math::Vector2 &p0, const Math::Vector2 &lineDir, const Math::Vector2 triangle[3]);

	/** Checks whether there is an intersection between the entered plane and the sphere.
	@param intersectionCenter This is set to the center of the intersection circle in the plane with the sphere if both intersect.
		Set this to NULL if you don't care about this.
	@param intersectionRadius This is set to the center of the intersection circle in the plane with the sphere if both intersect.
		This is only computed and set if intersectionCenter is not NULL.
	@param plane Set this to the infinitely large and thin plane you want to test against the line segement.
	@param sphereCoords Set this to the coords of the center point of the sphere.
	@param sphereRadius Set this to define the size of the sphere.
	@return Returns true if the plane and sphere overlap. */
	bool intersectPlaneWithSphere(Math::Vector3 *intersectionCenter, Real *intersectionRadius,
		const Plane &plane,	const Math::Vector3 &sphereCoords, const Real sphereRadius);

	/** Computes intersection data for a line segment [segmentP0, segmentP1] and a ray starting at rayStart and going into the direction rayDir into infinity.
	@param intersection Set this to a Vector2 variable address to get the intersection point which is closest to rayStart and within [segmentP0, segmentP1].
		If there is no intersection then the vector is set to (REAL_MAX, REAL_MAX).
		It is ignored if intersection is set to NULL.
	@param colinear Is set to true if the line segment and the ray lie on the same straight line otherwise it is set to false.
		It is ignored if colinear is set to NULL.
	@param segmentP0 Set this to the first point of the line segement to be tested.
	@param segmentP1 Set this to the second / other point of the line segement to be tested.
	@param rayStart Set this to the start point from which the ray emits.
	@param rayDir Set this to the direction of the ray. Points with p = rayStart + l * rayDir and l >= 0.0f belong to the ray. */
	bool intersectRayWithLineSegment(Math::Vector2 *intersection, bool *colinear,
		const Math::Vector2 &segmentP0, const Math::Vector2 &segmentP1, const Math::Vector2 &rayStart, const Math::Vector2 &rayDir);

	/** Checks for intersection between a ray and an infinite plane.
	@param t If there is an intersection then t is set to fulfill intersection position = p0 + t * rayDir.
	@param intersection Set this to a Vector3 address to retrieve the intersection position or NULL if you don't need it.
	@param parallel Set this to a bool address to retrieve whether the ray and the plane are parallel to each other or NULL if you're not interested.
	@param plane Set this to the infinitely large and thin plane you want to test against the line segement.
	@param rayStart Set this to the point where the ray originates.
	@param rayDir Set this the direction of the ray. (= difference of two points on the ray). Does not need to have unit length. */
	bool intersectRayWithPlane(Math::Vector3 *intersection, bool *parallel,
		const Plane &plane, const Math::Vector3 &rayStart, const Math::Vector3 &rayDir);

	/** Checks for intersection between a ray and a triangle and returns the intersection point if there the ray hits the triangle..
	@param intersection If the function returns true to report an intersection then this vector is set to the intersection point.
	@param backfaceCulling Set this to true to only report collisions if the triangle is hit by the ray from its front side.
		If it is set to false then intersections are reported no matter if the triangle is hit from the front or back side.
	@param rayStart Set this to the coordinates of the ray origin.
	@param rayDir Set this to the positive direction of the ray. (difference of two points on the ray) Does not need unit length.
	@param a The point set a, b, c defines the triangle to check for intersection.
	@param b The point set a, b, c defines the triangle to check for intersection.
	@param c The point set a, b, c defines the triangle to check for intersection.
	@param triangleNormal Set this to the address of a vector pointing in positive normal direction of the triangle a,b,c if available.
		If it is set to NULL then the triangle normal is computed via the cross product of its tangents.
	@return Returns true if there is an intersection between the ray and the triangle according to the setting of backface culling. */
	bool intersectRayWithTriangle(Math::Vector3 &intersection,
		const bool backfaceCulling, const Math::Vector3 &rayStart, const Math::Vector3 &rayDir,
		const Math::Vector3 &a, const Math::Vector3 &b, const Math::Vector3 &c, const Math::Vector3 *triangleNormal = 0);

	/** Computes the overlap region of two line segments wich are assumed to concide with the same line.
	@param intersections Set this to a Vector2 variable address to get the first and last intersection point of the overlap.
		If there is no intersection then the vectors are set to (REAL_MAX, REAL_MAX).
		It is ignored if intersections is set to NULL.
	@param alphas Set this to an address to get the intersection distance w.r.t. line segement A: intersection points[0,1] = startA + alphas[0,1] * (endA - startA)
		If there is no intersection then the values are set to REAL_MAX.
		It is ignored if alphas is set to NULL.
	@param startA Set this to the start point of segement A.
	@param endA Set this to the end point of segment A.
	@param startB Set this to the start point of segement B.
	@param endB Set this to the end point of segment B. */
	bool intersectSegmentsOnLine(Math::Vector2 intersections[2], Real alphas[2],
		const Math::Vector2 &startA, const Math::Vector2 &endA,
		const Math::Vector2 &startB, const Math::Vector2 &endB);

	/** Returns true if p is within the axis aligned bounding box defined by AABB[0] (minimum) and AABB[1] (maximum).
	@param p This point is tested against the entered AABB.
	@param AABBMin Set this to the minimum (lower left corner [0]) and maximum coordinates (upper right corner [1]) of the AABB.
		Points with these coords are still considered to be in the AABB. (at the border = inside)*/
	bool isPointInAABB(const Math::Vector2 &p, const Math::Vector2 AABB[2]);

	/** Returns true if p is within the axis aligned bounding box defined by AABBMin and AABBMax.
	@param p This point is tested against the entered AABB.
	@param AABBMin Set this to the minimum coordinates (lower left corner) of the AABB. Points with these coords are still considered to be in the AABB. (at the border = inside)
	@param AABBMax Set this to the maximum coordinates (upper right corner) of the AABB. Points with these coords are still considered to be in the AABB. (at the border = inside)*/
	bool isPointInAABB(const Math::Vector2 &p, const Math::Vector2 &AABBMin, const Math::Vector2 &AABBMax);

	/** Returns true if p is within the axis aligned bounding box defined by AABB[0] (minimum) and AABB[1] (maximum).
	@param p This point is tested against the entered AABB.
	@param AABBMin Set this to the minimum (lower left corner [0]) and maximum coordinates (upper right corner [1]) of the AABB.
		Points with these coords are still considered to be in the AABB. (at the border = inside)*/
	bool isPointInAABB(const Math::Vector3 &p, const Math::Vector3 AABB[2]);

	/** Returns true if p is within the axis aligned bounding box defined by AABBMin and AABBMax.
	@param p This point is tested against the entered AABB.
	@param AABBMin Set this to the minimum coordinates (lower left corner) of the AABB. Points with these coords are still considered to be in the AABB. (at the border = inside)
	@param AABBMax Set this to the maximum coordinates (upper right corner) of the AABB. Points with these coords are still considered to be in the AABB. (at the border = inside)*/
	bool isPointInAABB(const Math::Vector3 &p, const Math::Vector3 &AABBMin, const Math::Vector3 &AABBMax);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline bool intersectAABBWithAABB(const Math::Vector3 &AABB0Min, const Math::Vector3 &AABB0Max, const Math::Vector3 &AABB1Min, const Math::Vector3 &AABB1Max)
	{
		if (AABB0Min.x > AABB1Max.x || AABB1Min.x > AABB0Max.x)
			return false;

		if (AABB0Min.y > AABB1Max.y || AABB1Min.y > AABB0Max.y)
			return false;

		if (AABB0Min.z > AABB1Max.z || AABB1Min.z > AABB0Max.z)
			return false;

		return true;
	}

	inline bool intersectAABBWithRay(Real &entryT, Real &exitT, const Math::Vector2 &rayOrigin, const Math::Vector2 &rayDir, const Math::Vector2 &AABBMin, const Math::Vector2 &AABBMax)
	{
		return entryT >= -Math::EPSILON && intersectAABBWithLine(entryT, exitT, rayOrigin, rayDir, AABBMin, AABBMax);
	}

	inline bool intersectAABBWithRay(Real &entryT, Real &exitT, const Math::Vector3 &rayOrigin, const Math::Vector3 &rayDir, const Math::Vector3 &AABBMin, const Math::Vector3 &AABBMax)
	{
		return entryT >= -Math::EPSILON && intersectAABBWithLine(entryT, exitT, rayOrigin, rayDir, AABBMin, AABBMax);
	}

	inline bool intersectLineSegmentWithPlane(Real &t, Math::Vector3 *intersection, bool *parallel,
		const Plane &plane, const Math::Vector3 &p0, const Math::Vector3 &p1)
	{
		intersectLineWithPlane(t, intersection, parallel, plane, p0, p1 - p0);
		return (t >= 0.0f && t <= 1.0f);
	}
}

#endif // _COLLISION_DETECTION_H_
