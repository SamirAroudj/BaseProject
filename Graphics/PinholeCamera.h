/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _PINHOLE_CAMERA_H_
#define _PINHOLE_CAMERA_H_

#include "Math/Vector2.h"
#include "Graphics/BaseCamera3D.h"

namespace Graphics
{
	/// This virtual camera representation is designed for view and projection matrix computation.
	/** Represents a virtual camera to easily manage view and projection transformation matrices. */
	class PinholeCamera : public BaseCamera3D
	{
	public:
		/** Initializes the camera to be at world origin and its projection matrix with the entered values.
		@param orientation Sets the camera orientation according to the entered quaternion which must represent an orientation / rotation.
			Must be a right handed unit quaternion. (length = 1).
		@param positionWS Set this to world space coordiantes of the camera center.
		@param focalLength Set this to the focal length of the camera.
		@param principlePoint Defines the camera center w.r.t. projected 2D coordinates \in [0, 1]^2. (Defines where the camera z-axis hits the image plane. Usually (0.5, 0.5).
		@param aspectRatio Set this to the ratio of width to height of the images for the pinhole camera. aspectRatio = width in pixels / height in pixels. */
		PinholeCamera(const Math::Quaternion &orientation, const Math::Vector4 &positionWS, const Real focalLength, const Math::Vector2 &principlePoint, const Real aspectRatio);

		/** Destroys the camera. */
		virtual ~PinholeCamera();

		/** Computes and returns a matrix P^-1 that
			maps normalized homogenous device coordinates (x, y, z) with z = 1 (after viewport transformation - not in pixels) to non-normalized view space camera ray directions. 
		@return Computes and returns a matrix P^-1 that maps normalized homogenous device coordinates (x, y, z) with z = 1 to non-normalized view space camera ray directions.  */
		virtual Math::Matrix3x3 computeInverseProjectionMatrix() const;

		/** Computes a matrix that converts a homogenous pixel position (px, py, z = 1) \in ([0, viewport width], [0, viewport height], 1) to a
			non-normalized ray direction in world space coordinates.
		@param imageSize Defines the width ([0]) and height ([1]) of the viewport for which the returned matrix computes ray directions.
		@param addPixelCenterOffset If this is set to true then integer pixel coordinates refer to pixel centers since an offset of half a pixel is added to the coordinates.
			If the parameter is set to false then rays for the upper left pixel corner are returned. 
		@return Returns a matrix M which converts homogeneours (3D) pixel coordinates to non-normalized ray directions (||rayDir|| is not necessarily 1) posHPS * M = rayDirWS. */
		virtual Math::Matrix3x3 computeHPSToNNRayDirWS(const Utilities::ImgSize &imageSize, const bool addPixelCenterOffset = true) const;
						
		/** todo */
		virtual Math::Matrix3x3 computeInverseViewportMatrix(const Utilities::ImgSize &imageSize, const bool addPixelCenterOffset) const;

		/** Computes and returns the matrix which transforms coordinates relative to the world space coordinate system into the pixel coordinate system of this camera for some resolution.
			The pixel coordinates are not normalized! You must perform the perspective division after the matrix has been applied.
		@param imageSize Defines the width ([0]) and height ([1]) of the viewport for which the returned matrix computes pixel coordinates.
		@param considerPixelCenterOffset Set this to true if you want to get coordinates refering to pixel centers instead of lower left corners.
			E.g., a point at the lower left camera frustum edge is mapped to the pixel coordinates (0.5, 0.5) if this is set to true (instead of (0, 0)). 
		@return Returns a matrix which transforms world space coordiantes into non normalized pixel coordinates relative to the camera's viewport of resolution imageSize.*/
		virtual Math::Matrix4x4 computeWorldSpaceToPixelSpaceMatrix(const Utilities::ImgSize &imageSize, const bool considerPixelCenterOffset = true) const;

		/** Returns the focal length of the camera.
		@return The focal length of the camera is returned. */
		inline Real getFocalLength() const;

		/** Returns the principle point of the camera which defines the camera center w.r.t. projected 2D coordinates \in [0, 1]^2. (Defines where the camera z-axis hits the image plane. Usually (0.5, 0.5).
		@return Returns the principle point efines the camera center w.r.t. projected 2D coordinates \in [0, 1]^2. (Defines where the camera z-axis hits the image plane. Usually (0.5, 0.5). */
		inline const Math::Vector2 &getPrinciplePoint() const;
		
		/** Set camera's image plane aspect ratio, aspect ratio = width / height.
		@param aspectRatio This is the ratio of width to height of the camera image plane.  Must be positive.*/
		virtual void setAspectRatio(const Real aspectRatio);

		/** Replaces the focal length of the camera. Must be positive.
		@param focalLength Set this to the new focal length value which must be positive. */
		inline void setFocalLength(const Real focalLength);
		
		/** Set a new principle point.
		@param principlePoint Defines the camera center w.r.t. projected 2D coordinates \in [0, 1]^2. (Defines where the camera z-axis hits the image plane. Usually (0.5, 0.5). */
		inline void setPrinciplePoint(const Math::Vector2 &principlePoint);

		/** Sets the corresponding members and updates the internal projection matrix.
		@param focalLength Set this to the new focal length value which must be positive.
		@param aspectRatio This is the ratio of width to height of the camera image plane.  Must be positive.*/
		inline void setProjectionProperties(const Real focalLength, const Real aspectRatio);

	private:		
		Math::Vector2 mPrinciplePoint;	/// Defines the principle point which is used to define the center of the image for mWSToPS.
		Real mFocalLength;				/// Contains the focal length of the camera.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	inline Real PinholeCamera::getFocalLength() const
	{
		return mFocalLength;
	}

	inline const Math::Vector2 &PinholeCamera::getPrinciplePoint() const
	{
		return mPrinciplePoint;
	}

	inline void PinholeCamera::setFocalLength(const Real focalLength)
	{
		assert(focalLength > 0.0f);
		mFocalLength = focalLength;
		mProjection = Math::Matrix4x4::createProjectionRealWorld(focalLength, mAspectRatio);
	}
		
	inline void PinholeCamera::setPrinciplePoint(const Math::Vector2 &principlePoint)
	{
		mPrinciplePoint = principlePoint;
	}

	inline void PinholeCamera::setProjectionProperties(const Real focalLength, const Real aspectRatio)
	{
		mFocalLength = focalLength;
		setAspectRatio(aspectRatio);
	}
}

#endif // _PINHOLE_CAMERA_H_
