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
		/** Creates a pinhole camera with no distortion, a principle point at (0.5, 0.5), a focal length of 1.0 and a pixel aspect ratio of 1 at the origin looking along the x-axis. */
		PinholeCamera();

		/** Initializes the camera to be at world origin and its projection matrix with the entered values.
		@param orientation Sets the camera orientation according to the entered quaternion which must represent an orientation / rotation.
			Must be a right handed unit quaternion. (length = 1).
		@param positionWS Set this to world space coordiantes of the camera center.
		@param focalLength Set this to the focal length of the camera.
		@param principalPoint Defines the camera center w.r.t. projected 2D coordinates \in [0, 1]^2. (Defines where the camera z-axis hits the image plane. Usually (0.5, 0.5).
		@param aspectRatio Set this to the ratio of pixel width to pixel height of the pinhole camera. aspectRatio = pixel width / pixel height.
		@param distortion Set this to the camera lens distortion parameters. This model uses two parameters.*/
		PinholeCamera(const Math::Quaternion &orientation, const Math::Vector4 &positionWS,
			const Real focalLength, const Math::Vector2 &principalPoint, const Real aspectRatio,
			const Real distortion[2]);

		/** Destroys the camera. */
		virtual ~PinholeCamera();

		/** Computes and returns a matrix P^-1 that
			maps normalized homogenous device coordinates (x, y, z) with z = 1 (after viewport transformation - not in pixels) to non-normalized view space camera ray directions. 
		@return Computes and returns a matrix P^-1 that maps normalized homogenous device coordinates (x, y, z) with z = 1 to non-normalized view space camera ray directions.  */
		virtual Math::Matrix3x3 computeInverseProjectionMatrix() const;

		/** Computes a matrix that converts a homogenous pixel position (px, py, z = 1) \in ([0, viewport width], [0, viewport height], 1) to a
			non-normalized ray direction in world space coordinates.
		@param viewportHeight Set this to the height of the viewport for which you want to have rays (number of pixels in a column).
		@param addPixelCenterOffset If this is set to true then integer pixel coordinates refer to pixel centers since an offset of half a pixel is added to the coordinates.
			If the parameter is set to false then rays for the upper left pixel corner are returned. 
		@return Returns a matrix M which converts homogeneours (3D) pixel coordinates to non-normalized ray directions (||rayDir|| is not necessarily 1) posHPS * M = rayDirWS. */
		virtual Math::Matrix3x3 computeHPSToNNRayDirWS(const uint32 &viewportHeight, const bool addPixelCenterOffset = true) const;
						
		/** todo */
		virtual Math::Matrix3x3 computeInverseViewportMatrix(const uint32 &viewportHeight, const bool addPixelCenterOffset) const;

		/** Computes and returns the matrix which transforms coordinates relative to the world space coordinate system into the pixel coordinate system of this camera for some resolution.
			The pixel coordinates are not normalized! You must perform the perspective division after the matrix has been applied.
		@param viewportHeight Set this to the height of the target viewport (number of pixels in a column).
		@param considerPixelCenterOffset Set this to true if you want to get coordinates refering to pixel centers instead of lower left corners.
			E.g., a point at the lower left camera frustum edge is mapped to the pixel coordinates (0.5, 0.5) if this is set to true (instead of (0, 0)). 
		@return Returns a matrix which transforms world space coordiantes into non normalized pixel coordinates relative to the camera's viewport of resolution imageSize.*/
		virtual Math::Matrix4x4 computeWorldSpaceToPixelSpaceMatrix(const uint32 &viewportHeight, const bool considerPixelCenterOffset = true) const;

		/** Returns camera lens distortion parameters. This model uses two parameters.
		@return Returns an array containing two lens distortion parameters. */
		inline const Real *getDistortion() const;

		/** Returns the focal length of the camera.
		@return The focal length of the camera is returned. */
		inline Real getFocalLength() const;

		/** Returns the principal point of the camera which defines the camera center w.r.t. projected 2D coordinates \in [0, 1]^2. (Defines where the camera z-axis hits the image plane. Usually (0.5, 0.5).
		@return Returns the principal point efines the camera center w.r.t. projected 2D coordinates \in [0, 1]^2. (Defines where the camera z-axis hits the image plane. Usually (0.5, 0.5). */
		inline const Math::Vector2 &getPrincipalPoint() const;
		
		/** Set camera's pixel aspect ratio = pixel width / pixel height.
		@param aspectRatio This is the ratio of pixel width to pixel height w.r.t. the camera image sensor.  Must be positive.*/
		virtual void setAspectRatio(const Real aspectRatio);

		/** Set the lens distortion parameters of this camera.
		@param distortion Set this to the two parameters modeling this camera's distortion.
			{0.0f, 0.0f} represents no distortion.
			todo: better comment*/
		inline void setDistortion(const Real distortion[2]);

		/** Replaces the focal length of the camera. Must be positive.
		@param focalLength Set this to the new focal length value which must be positive. */
		inline void setFocalLength(const Real focalLength);
		
		/** Set a new principal point.
		@param principalPoint Defines the camera center w.r.t. projected 2D coordinates \in [0, 1]^2. (Defines where the camera z-axis hits the image plane. Usually (0.5, 0.5). */
		inline void setPrincipalPoint(const Math::Vector2 &principalPoint);

		/** Sets the corresponding members and updates the internal projection matrix.
		@param focalLength Set this to the new focal length value which must be positive.
		@param aspectRatio This is the ratio of pixel width to pixel height of the camera sensor.  Must be positive.*/
		inline void setProjectionProperties(const Real focalLength, const Real aspectRatio);

	private:		
		Math::Vector2 mPrincipalPoint;	/// Defines the principal point which is used to define the center of the image for mWSToPS.
		Real mDistortion[2];			/// Stores lens distortion parameters. {0.0f, 0.0f} represents no distortion.
		Real mFocalLength;				/// Contains the focal length of the camera.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	inline const Real *PinholeCamera::getDistortion() const
	{
		return mDistortion;
	}

	inline Real PinholeCamera::getFocalLength() const
	{
		return mFocalLength;
	}

	inline const Math::Vector2 &PinholeCamera::getPrincipalPoint() const
	{
		return mPrincipalPoint;
	}

	inline void PinholeCamera::setDistortion(const Real distortion[2])
	{
		mDistortion[0] = distortion[0];
		mDistortion[1] = distortion[1];
	}

	inline void PinholeCamera::setFocalLength(const Real focalLength)
	{
		assert(focalLength > 0.0f);
		mFocalLength = focalLength;
		mProjection = Math::Matrix4x4::createProjectionRealWorld(focalLength, mAspectRatio);
	}
		
	inline void PinholeCamera::setPrincipalPoint(const Math::Vector2 &principalPoint)
	{
		mPrincipalPoint = principalPoint;
	}

	inline void PinholeCamera::setProjectionProperties(const Real focalLength, const Real aspectRatio)
	{
		mFocalLength = focalLength;
		setAspectRatio(aspectRatio);
	}
}

#endif // _PINHOLE_CAMERA_H_
