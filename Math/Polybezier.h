/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _POLYBEZIER_H_
#define _POLYBEZIER_H_

#include <string>
#include <vector>
#include "Math/BezierCurve.h"
#include "Platform/DataTypes.h"

namespace Math
{
	/// Realizes a connected set of BezierCurve objects where each has a degree of at least 1.
	/** Represents a single, connected curve defined by several Bezier curves, see also BezierCurve.
		It contains a start point p0 where curve c0 starts.
		Each following curve cn starts at the end point of its previous curve cn-1.
		Bezier sub curves can be stright lines (degree 1), quadratic curves (degree 2), cubic Bezier curves (degree 3) etc. */
	template <class T>
	class Polybezier
	{
	public:
		/** Creates an empty Polybezier without sub curves and control polygon points. */
		Polybezier();

		/** Initializes this Polybezier object with a deep copy of the entered values.
		@param points Contains the control polygon points with
			points[0] = curve start, points[1] = intermediate control point, points[last] = curve end.
		@param degrees Contains the degree of each sub curve, e.g. degrees[0] = 1 -> first sub curve is a straight line.
		@param curveCount Defines the size of degrees and number of sub curves of this Polybezier object. */
		Polybezier(T *points, uint32 *degrees, uint32 curveCount);

		/** Creates a deep copy of copy.
		@param copy copy is deeply copied. */
		Polybezier(const Polybezier &copy);

		/** Releases allocated memory. */
		~Polybezier();

		/** Sets this object to rhs or does nothing if this object is rhs.
		@param rhs Defines the new data of this object. */
		Polybezier &operator =(const Polybezier &rhs);

		/** Returns a point of the total control polygon of this Polybezier object.
			The control polygon contains the control points of all sub curves whereas
			point[0] is at the cuve start, point[getControlPolygonSize() - 1] is at the curve end and
			the end point of curve c[n-1] is the start point of curve c[n[.
			This is true for c(curveCount -1) and c0 if the curve is a closed loop.
		@param id Defines what control point to be returned.
			id must be in { 0, 1, ..., getControlPolygonSize() - 1}.
			If id is set to 0 then the control point defining the curve start is returned.
			If id is set to getControlPolygonSize() - 1 then the control point defining the curve end is returned.
		@return Returns the id-th control polygon point.*/
		T &getControlPoint(uint32 id);

		/** Returns a point of the total control polygon of this Polybezier object.
			The control polygon contains the control points of all sub curves whereas
			point[0] is at the cuve start, point[getControlPolygonSize() - 1] is at the curve end and
			the end point of curve c[n-1] is the start point of curve c[n[.
			This is true for c(curveCount -1) and c0 if the curve is a closed loop.
		@param id Defines what control point to be returned.
			id must be in { 0, 1, ..., getControlPolygonSize() - 1}.
			If id is set to 0 then the control point defining the curve start is returned.
			If id is set to getControlPolygonSize() - 1 then the control point defining the curve end is returned.
		@return Returns the id-th control polygon point.*/
		const T &getControlPoint(uint32 id) const;

		/** Returns the array containing all control points of this curve.
			This control polygon contains the control points of all sub curves whereas
			point[0] is at the cuve start, point[getControlPolygonSize() - 1] is at the curve end and
			the end point of curve c[n-1] is the start point of curve c[n[.
			This is true for c(curveCount -1) and c0 if the curve is a closed loop.
		@return Returns the id-th control polygon point.*/
		const T *getControlPolygon() const;

		/** Returns the number of points that define this Polybezier curve.
		@return Returns the number of elements in the returned array of getControlPolygon().*/
		uint32 getControlPolygonSize() const;

		/** Returns the degrees of the Bezier sub curves.
			The size of the returned array is defined by getSubCurveCount().
		@return The returned array contains a degree for each sub curve in the same order as
			these sub curves are defined in the array returned by getControlPolygon().*/
		const uint32 *getCurveDegrees() const;

		/** Returns the number of sub Bezier curves this Polybezier object consists of.
		@return Returns the number of elements in the array returned by getCurveDegrees(). */
		inline uint32 getSubCurveCount() const;

		/** Partionates the Polybezier into parts based on the entered angle threshold.
			Adjacent line segments are not assigned to the same group if the angle between them is greater than the angular threshold.
		@param partition Contains point indices of all points which are the beginnings (inclusive) of the found partition subsets.
			Each point is also the end (inclusive) of its preceeding subset.
			E.g. closed Polybezier, point 2 = subset 1 start = subset 0 end, point 30 = subset 2 start, point 46 = subset 0 start = subset 2 end.
			Subset 0 does not necessarily start with point 0 as the first subset might contain 
		@param maxAngle Adjacent line segments with an angle between them which is greater than maxAngle are not assigned to the same subset of the resulting partition. */
		void partition(std::vector<uint32> &partition, Real maxAngle) const;

		/** Applies De Casteljau to each Bezier sub curve to get a finer control polygon. */
		void refine();

		/** Resets the curve by deleting previous data and deeply coping the entered data.
		@param points Contains the control polygon points with points[0] = curve start, points[1] = intermediate control point, points[last] = curve end.
		@param degrees Contains the degree of each sub curve, e.g. degrees[0] = 1 -> first sub curve is a straight line.
		@param curveCount Defines the size of degrees and number of sub curves of this Polybezier object.*/
		void set(T *points, uint32 *degrees, uint32 curveCount);

	private:
		/** Releases mDegrees and mPoints and sets mCuveCount to 0. */
		void freeData();

		/** Makes a deep copy of points and degrees and forwards mCurveCount.
		@param points These points are copied to mPoints.
		@param degrees Contains a degree for each sub curve in points and is copied to mDegrees.
		@param curveCount Defines the number of elements in degrees.*/
		void initialize(T *points, uint32 *degrees, uint32 curveCount);

		/** Refines every sub curve by applying De Casteljau to it once.
		@param curveCount Set this to the new curve count of the refined result.
		@param pointCount Set this to the new control polygon point count (size of new mPoints) of the refined results. */
		void refineSubCurves(uint32 curveCount, uint32 pointCount);

	private:
		uint32	*mDegrees;		/// Defines the degree of each Bezier curve. (1 = line, 2 = quadratic curve, 3 = cubic curve etc.)
		T		*mPoints;		/// Defines the control polygon of this Polybezier.
								/// Structure: start point, c0, c1, ..., cn with firstPoint(ci) = lastPoint(ci-1) or start point for i = 0
		uint32	mCurveCount;	/// Defines the number of sub curves which make up this Polybezier. This is the size of mDegrees.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	Polybezier<T>::Polybezier() : mDegrees(NULL), mPoints(NULL), mCurveCount(0)
	{

	}

	template <class T>
	inline Polybezier<T>::Polybezier(T *points, uint32 *degrees, uint32 curveCount) : mDegrees(NULL), mPoints(NULL), mCurveCount(0)
	{
		initialize(points, degrees, curveCount);
	}

	template <class T>
	inline Polybezier<T>::Polybezier(const Polybezier &copy) : mDegrees(NULL), mPoints(NULL), mCurveCount(0)
	{
		initialize(copy.mPoints, copy.mDegrees, copy.mCurveCount);
	}

	template <class T>
	inline Polybezier<T>::~Polybezier()
	{
		freeData();
	}

	template <class T>
	inline void Polybezier<T>::freeData()
	{
		delete [] mDegrees;
		delete [] mPoints;

		mDegrees	= NULL;
		mPoints		= NULL;
		mCurveCount	= 0;
	}

	template <class T>
	inline void Polybezier<T>::initialize(T *points, uint32 *degrees, uint32 curveCount)
	{
		// no memory leak? data was freed before?
		assert(!mDegrees);
		assert(!mPoints);
		assert(0 == mCurveCount);

		// process curveCount
		if (0 == curveCount)
			return;
		mCurveCount = curveCount;

		// copy curve degrees and compute total point count
		uint32	pointCount	= 1;
				mDegrees	= new uint32[mCurveCount];

		for (uint32 i = 0; i < mCurveCount; ++i)
		{
			// check
			uint32 degree = degrees[i];
			assert(degree > 0);
			if (0 == degree)
			{
				// todo log this
				freeData();
				return;
			}

			mDegrees[i] = degree;
			pointCount += degree;
		}

		// copy points
		mPoints = new T[pointCount];

		for (uint32 i = 0; i < pointCount; ++i)
			mPoints[i] = points[i];
	}

	template <class T>
	inline Polybezier<T> &Polybezier<T>::operator =(const Polybezier &rhs)
	{
		if (this == &rhs)
			return *this;

		freeData();
		initialize(rhs.mPoints, rhs.mDegrees, rhs.mCurveCount);

		return *this;
	}

	template <class T>
	T &Polybezier<T>::getControlPoint(uint32 id)
	{
		return mPoints[id];
	}

	template <class T>
	const T &Polybezier<T>::getControlPoint(uint32 id) const
	{
		return mPoints[id];
	}

	template <class T>
	inline const T *Polybezier<T>::getControlPolygon() const
	{
		return mPoints;
	}

	template <class T>
	inline uint32 Polybezier<T>::getControlPolygonSize() const
	{
		if (0 == mCurveCount || !mDegrees || !mPoints)
			return 0;

		// each curve uses the last point of the previous curve as its first point since a single Polybezier is a single connected curve
		// the first curve, however, must use the first "lonely" point in mPoints
		uint32 size = 1;
		for (uint32 i = 0; i < mCurveCount; ++i)
			size += mDegrees[i];

		return size;
	}

	template <class T>
	inline const uint32 *Polybezier<T>::getCurveDegrees() const
	{
		return mDegrees;
	}

	template <class T>
	inline uint32 Polybezier<T>::getSubCurveCount() const
	{
		return mCurveCount;
	}

	template <class T>
	inline void Polybezier<T>::refine()
	{
		if (0 == mCurveCount || !mDegrees || !mPoints)
			return;

		// compute new counts
		uint32 curveCount = mCurveCount;
		uint32 pointCount = 1;

		for (uint32 i = 0; i < mCurveCount; ++i)
		{
			uint32	degree		= mDegrees[i];
					pointCount += degree;

			// pure lines are not refined
			if (degree <= 1)
				continue;

			pointCount += degree;
			++curveCount;
		}

		// there is nothing to be refined
		if (curveCount == mCurveCount)
			return;

		refineSubCurves(curveCount, pointCount);
	}

	template <class T>
	inline void Polybezier<T>::refineSubCurves(uint32 curveCount, uint32 pointCount)
	{
		// there is data to be refined?
		assert(mCurveCount > 0);
		assert(mDegrees);
		assert(mPoints);

		// allocate memory for new sub curves
		uint32	*degrees	= new uint32[curveCount];
		T		*points		= new T[pointCount];

		// refine each Bezier curve with degree > 1 using De Casteljau's algorithm
		// each curve uses the last point of the previous curve as start point as a Polybezier is a single connected path
		points[0] = mPoints[0];

		// refine each sub curve which is not a simple line
		for (uint32 curveIdx = 0, degreeIdx = 0, sourcePointIdx = 0, targetPointIdx = 0;
			 curveIdx < mCurveCount;
			 ++curveIdx)
		{
			uint32	degree	= mDegrees[curveIdx];
			const T	*source = mPoints + sourcePointIdx;
			T		*target = points + targetPointIdx;
			
			// simple lines are only copied other curves are refined to two sub curves using De Casteljau
			BezierCurve<T>::refine(target, source, degree + 1, false);
			degrees[degreeIdx++] = degree;
			if (degree > 1)
				degrees[degreeIdx++] = degree;

			targetPointIdx += (degree > 1 ? 2 * degree : degree);
			sourcePointIdx += degree;
		}

		// reset member variables
		freeData();

		mCurveCount = curveCount;
		mDegrees	= degrees;
		mPoints		= points;
	}

	template <class T>
	void Polybezier<T>::set(T *points, uint32 *degrees, uint32 curveCount)
	{
		freeData();
		initialize(points, degrees, curveCount);
	}
}

#endif // _POLYBEZIER_H_
