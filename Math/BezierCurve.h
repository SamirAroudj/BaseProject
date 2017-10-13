/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _BEZIER_CURVE_H_
#define _BEZIER_CURVE_H_

#include <cassert>
#include "Platform/DataTypes.h"

namespace Math
{
	/// Implements Bezier curves of a degree of at least one (line) for base types T like Vector2, Vector3, Real etc.
	template <class T>
	class BezierCurve
	{
	public:
		/** Copies entered points to create a simple Bezier curve of degree pointCount - 1.
		@param points These points define the control polygon of the Bezier curve.
			The resulting curve starts in points[0] and ends in points[pointCount -1] and is curved by the points inbetween.
		@param pointCount Set this to the size of the array points. */
		BezierCurve(T *points, uint32 pointCount);

		/** Creates a deep copy of copy.
		@param copy The new curve is gets it data from copy. */
		BezierCurve(const BezierCurve &copy);

		/** Releases allocated memory. */
		~BezierCurve();

		/** If this object is not rhs then the data of this object is freed and recreated from rhs.
		@param rhs Sets the values of this polybezier object. */
		BezierCurve &operator =(const BezierCurve &rhs);

		/** Computes the coordinates of the curve at t.
		@param t Defines where the curve is evaluated.
			The parameter t must be inside the interval [0, 1].*/
		T evaluate(Real t) const;

		/** Computes the coordinates of a Bezier curve at t.
		@param t Defines where the curve is evaluated.
			The parameter t must be inside the interval [0, 1].
		@param points Defines the control polygon of the Bezier curve (start point, ..., end point).
		@param pointCount Defines the number of element in the control polygon array called points. */
		static T evaluate(Real t, T *points, uint32 pointCount);

		/** Computes the first derivative of this Bezier curve at t.
		@param t Defines where the first derivative is computed.
			The parameter t must be in the interval [0, 1]. */
		T evaluateDerivative(Real t) const;

		/** Returns the degree of the Bezier curve. (= control polygon size - 1).
		@return Returns the degree of the Bezier curve. (= control polygon size - 1). */
		uint32 getDegree() const;

		/** Applies De Casteljau to this Bezier curve and creates refined points as new control polygon.
		@param target Set this to an array large enough to hold the new and refined control polygon.
			(new point count = old count + degree)
		@param source Set this to the control polygon to be refined.
		@param pointCount Set this to the number of points in source.
		@param CopyFirstPoint Set this to true to set target[0] to source[0]. Otherwise this copy operation is skipped.
			Is useful if target[0] already contains a vaild starting point.
		@param t Controls the interpolation to determine the new control polygon points. Usually 0.5f is a good value.
			De Casteljau is then always evaluated at Bezier sub part f(t).
			New control polygon mid point will be at evaluate(t). */
		static void refine(T *target, const T *source, uint32 pointCount, bool copyFirstPoint, Real t = 0.5f);

	private:
		/** Checks that t is inside the interval [0, 1].
		@param t Returns true if t is inside the interval [0, 1].
		@return Returns true if t is inside the interval [0, 1]. */
		static bool checkT(Real t);

		/** Releases mPoints and sets mPointCount to 0. */
		void freeData();

		/** Makes a deep copy of points and takes forwards pointCount but does not release any memory.
			Use freeData before initialize if necessary.
		@param points These points are copied to mPoints.
		@param pointCount Defines the number of elements in points. */
		void initialize(T *points, uint32 pointCount);

	private:
		T		*mPoints;		/// Contains the control polygon of the Bezier curve which goes through the first and last point in mPoints
		uint32	mPointCount;	/// Defines the size of mPoints / number of points in the array
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	inline BezierCurve<T>::BezierCurve(T *points, uint32 pointCount) : mPoints(NULL), mPointCount(0)
	{
		initialize(points, pointCount);
	}

	template <class T>
	inline BezierCurve<T>::BezierCurve(const BezierCurve<T> &copy) : mPoints(NULL), mPointCount(0)
	{
		initialize(copy.mPoints, copy.mPointCount);
	}

	template <class T>
	inline BezierCurve<T>::~BezierCurve()
	{
		freeData();
	}


	template <class T>
	inline BezierCurve<T> &BezierCurve<T>::operator =(const BezierCurve &rhs)
	{
		if (this == &rhs)
			return *this;

		freeData();
		initialize(rhs.mPoints, rhs.mPointCount);
	}

	template <class T>
	inline void BezierCurve<T>::freeData()
	{
		delete [] mPoints;

		mPoints		= NULL;
		mPointCount	= 0;
	}

	template <class T>
	inline void BezierCurve<T>::initialize(T *points, uint32 pointCount)
	{
		assert(pointCount >= 2);
		if (pointCount < 2)
		{
			// todo log this;
			return;
		}

		mPointCount	= pointCount;
		mPoints		= new T[mPointCount];

		for (uint32 i = 0; i < mPointCount; ++i)
			mPoints[i] = points[i];
	}

	template <class T>
	inline T BezierCurve<T>::evaluate(Real t) const
	{
		if (!checkT(t))
			return T();
		return evaluate(t, mPoints, mPointCount);
	}

	template <class T>
	inline T BezierCurve<T>::evaluate(Real t, T * points, uint32 count)
	{
		if (!checkT(t))
			return T();

		// evaluate Bezier curve at t depending on its degree / point count
		switch (count)
		{
			// simple line / linear interpolation
			case 2:
				// P0 + t(P1 - P0)
				return points[0]  + (points[1] - points[0]) * t;

			// quadratic Bezier curve
			case 3:
			{
				// (1 - t)^2 * P0 + 2(1 - t) * t * P1 + t^2P2
				Real oneT = 1.0f - t;
				return points[0] * (oneT * oneT) + points[1] * (2.0f * oneT * t) + points[2] * t * t;
			}

			// cubic Bezier curve
			case 4:
			{
				Real oneT	= 1.0f - t;
				Real oneTSq	= oneT * oneT;
				Real tSq	= t * t;

				// (1 - t)^3 * P0 + 3 * (1 - t)^2 * t * P1 + 3 * (1 - t) * t^2 * P2 + t^3 * P3
				T	p  = points[0] * oneT * oneTSq;
					p += points[1] * (3.0f * oneTSq * t);
					p += points[2] * (3.0f * oneT * tSq);
					p += points[3] * t * tSq;
				return p;
			}

			default:
				// higher order Bezier curve is computed recursively
				return evaluate(t, points, count - 1) * (1.0f - t) + evaluate(t, points + 1, count - 1) * t;
		}
	}

	template <class T>
	T inline BezierCurve<T>::evaluateDerivative(Real t) const
	{
		if (!checkT(t))
			return T();

		// get first derivative of Bezier curve at t depending on its point count / degree
		switch (mPointCount)
		{
			// simple line / linear interpolation
			case 2:
				// (P1 - P0)
				return mPoints[1] - mPoints[0];

			// quadratic Bezier curve
			case 3:
			{
				// 2(1 - t)(P1 - P0) + 2t(P2 - P1)
				Real oneT = 1.0f - t;
				return (mPoints[1] - mPoints[0]) * 2.0f * oneT + (mPoints[2] - mPoints[1]) * 2.0f * t;
			}

			// cubic Bezier curve
			case 4:
			{
				Real oneT	= 1.0f - t;
				Real oneTSq	= oneT * oneT;
				Real tSq	= t * t;

				// 3(1 - t)^2 * (P1 - P0) + 6(1 - t)t(P2 - P1) + 3t^2(P3 - P2)
				T	p  = (mPoints[1] - mPoints[0]) * 3 * oneTSq;
					p += (mPoints[2] - mPoints[1]) * 6 * oneT * t;
					p += (mPoints[3] - mPoints[2]) * 3 * tSq;
			}

			// higher order Bezier curve is computed recursively
			default:
				uint32 m = mPointCount - 1;
				return (evaluate(t, mPoints + 1, m) - evaluate(t, mPoints, m)) * getDegree();
		}
	}

	template <class T>
	inline bool BezierCurve<T>::checkT(Real t)
	{
		// t must be in [0, 1]
		assert(t >= 0.0f);
		assert(t <= 1.0f);

		if (t < 0.0f)
		{
			t = 0.0f;
			// todo log this
			return false;
		}

		if (t > 1.0f)
		{
			t = 1.0f;
			// todo log this
			return false;
		}

		return true;
	}

	template <class T>
	inline uint32 BezierCurve<T>::getDegree() const
	{
		return mPointCount - 1;
	}

	template <class T>
	void BezierCurve<T>::refine(T *target, const T * source, uint32 pointCount, bool copyFirstPoint, Real t)
	{
		// a Bezier curve of degree 1 (= line or more complex) should be refined at least
		// the programmer using this function doesn't understand or misuses this function otherwise
		assert(pointCount > 1);
		if (1 >= pointCount)
		{
			// todo log this
			return;
		}

		if (copyFirstPoint)
			target[0] = source[0];

		// purely linear curves are not refined but copied only
		if (2 == pointCount)
		{
			target[1] = source[1];
			return;
		}

		// De Casteljau's algorithm for refinement to get "inner" points of a curve which is quadratic or more complex
		Real	oneT		= 1.0f - t;
		uint32	lastIdx		= pointCount - 1;
		target[2 * lastIdx]	= source[lastIdx];

		// first round of linear interpolations is different as source is accessed
		for (uint32 i = 0; i < lastIdx; ++i)
			target[i + 1] = source[i] * oneT + source[i + 1] * t;

		// following linear interpolation rounds are done in target array only
		while (pointCount > 2)
		{
			target[2 * lastIdx - 1] = target[lastIdx]; // save last point for second sub curve

			--pointCount;
			++target;
			lastIdx = pointCount - 1;
			T previous(target[0]);

			for (uint32 i = 0; i < lastIdx; ++i)
			{
				T &next			= target[i + 1];
				T newPoint		= previous * oneT + next * t;
				previous		= next;
				target[i + 1]	= newPoint;
			}
		}
	}
}

#endif // _BEZIER_CURVE_H_
