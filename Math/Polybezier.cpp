/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include "Math/Polybezier.h"
#include "Math/Vector2.h"

//using namespace Math;
using namespace std;

namespace Math
{
	template <>
	void Math::Polybezier<Vector2>::partition(vector<uint32> &partition, Real maxAngle) const
	{
		partition.clear();

		// check that there are enough line segments
		const uint32 pointCount	= getControlPolygonSize();
		if (2 > pointCount)
			return;

		// closed Polybeziers are possible
		// does a subset contain the last and the first line segment? yes -> first subset does not start with the first line segment
		bool subset0NotAt0 = true;
		const Vector2 &firstPoint = mPoints[0];
		const Vector2 &lastPoint = mPoints[pointCount - 1];

		// closed Polybezier / a loop?
		Real distance = (lastPoint - firstPoint).getLength();
		if (distance > EPSILON)
			subset0NotAt0 = false;

		// angle(last segment, first segment) < max angle?
		if (subset0NotAt0)
		{
			const Vector2 firstSegment = mPoints[1] - mPoints[0];
			const Vector2 lastSegment = mPoints[pointCount - 1] - mPoints[pointCount - 2];
			const Real angle = firstSegment.getAngle(lastSegment);
			subset0NotAt0 = (angle < maxAngle);
		}

		// last & first segment belong to different subsets
		if (!subset0NotAt0)
			partition.push_back(0);

		// find adjecent segements which belong to 2 different subsets
		Vector2 segment0 = mPoints[1] - mPoints[0];
		Vector2 segment1 = Vector2();
		Vector2 start = mPoints[1];

		assert(segment0.getLengthSquared() > EPSILON * EPSILON);

		for (uint32 pointIdx = 2; pointIdx < pointCount; ++pointIdx)
		{
			const Vector2 &end = mPoints[pointIdx];

			segment1 = end - start;
			if (segment1.getLengthSquared() <= EPSILON * EPSILON)
				continue;
			assert(segment1.getLengthSquared() > EPSILON * EPSILON);

			// start a new partition with segment1?
			const Real angle = segment0.getAngle(segment1);
			if (angle > maxAngle)
				partition.push_back(pointIdx - 1);

			// prepare next iteration
			start = end;
			segment0 = segment1;
		}

		// finish last subset which contains last and first segment
		if (subset0NotAt0)
		{
			assert(pointCount > 2);

			if (partition.empty())
				partition.push_back(0);
			//else
			//	partition.push_back(partition[0]);
		}
	}
}
