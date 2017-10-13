/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CAMERA_3D_H_
#define _CAMERA_3D_H_

#include "Graphics/BaseCamera3D.h"

namespace Graphics
{
	/// This virtual camera representation is designed for view and projection matrix computation.
	/** Represents a virtual camera to easily manage view and projection transformation matrices. */
	class Camera3D : public BaseCamera3D
	{
	public:
		/** Returns the currently active camera or NULL if there is none.
		@return Returns the camera that is currently active or NULL if none is currently set. */
		static Camera3D *getActiveCamera();

	public:
		/** Initializes the camera to be at world origin and its projection matrix with the entered values.
		@param fovY This is the vertical opening angle of the camera view frustum along its y-Axis in radians. Must be positive.
		@param aspectRatio This is the ratio of width to height of the camera's image plane. Must be positive.
		@param zNear Defines the distance between projection center (= camera origin) and beginning of the view frustum (= near view frustum or clipping plane) in floating point units.
					 Must be positive as it is interpreted as absolute value.
					 Everything in front of zNear plane (lower absolute value) is not visible by this camera.
		@param zFar Defines the distance between projection center (= camera origin) and the end of the view frustum (= far view frustum or clipping plane) in floating point units.
					Must be positive since it is interpreted as absolute value..
					Everything behind zFar plane (larger absolute value) is not visible by this camera. */
		Camera3D(Real fovY, Real aspectRatio, Real zNear, Real zFar);

		/** Destroys the camera. */
		virtual ~Camera3D();
		
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
			
		/** Computes and returns the matrix which transforms coordinates relative to the world space coordinate system into the pixel coordinate system of this camera for some resolution.
			The pixel coordinates are not normalized! You must perform the perspective division after the matrix has been applied.
		@param imageSize Defines the width ([0]) and height ([1]) of the viewport for which the returned matrix computes pixel coordinates.
		@param considerPixelCenterOffset Set this to true if you want to get coordinates refering to pixel centers instead of lower left corners.
			E.g., a point at the lower left camera frustum edge is mapped to the pixel coordinates (0.5, 0.5) if this is set to true (instead of (0, 0)). 
		@return Returns a matrix which transforms world space coordiantes into non normalized pixel coordinates relative to the camera's viewport of resolution imageSize.*/
		virtual Math::Matrix4x4 computeWorldSpaceToPixelSpaceMatrix(const Utilities::ImgSize &imageSize, const bool considerPixelCenterOffset = true) const;

		/** Returns the absolute distance (w.r.t the camera center) of the far clipping plane used for the projection matrix of the camera.
		@param Returns the absolute distance (w.r.t the camera center) of the far clipping plane used for the projection matrix of the camera.*/
		inline Real getFarClippingPlane() const;

		/** Returns the angular extent of this View object along the y-axis. (field of view w.r.t. to the cameras up axis).
		@return Returns the angular extent of this View object along the y-axis. (field of view w.r.t. to the cameras up axis). */
		inline Real getFoVY() const;

		/** Returns the absolute distance (w.r.t the camera center) of the near clipping plane used for the projection matrix of the camera.
		@param Returns the absolute distance (w.r.t the camera center) of the near clipping plane used for the projection matrix of the camera.*/
		inline Real getNearClippingPlane() const;

		/** This camera becomes the active camera so that it can be obtained by getActiveCamera(). */
		inline void setAsActiveCamera();

		/** Set camera's image plane aspect ratio, aspect ratio = width / height.
		@param aspectRatio This is the ratio of width to height of the camera image plane.  Must be positive.*/
		virtual void setAspectRatio(const Real aspectRatio);

		/** Recomputes the camera projection matrix according to the entered data
		@param fovY This is the vertical opening angle of the camera view frustum along its y - Axis in radians.Must be positive.
		@param aspectRatio This is the ratio of width to height of the camera's image plane. Must be positive.
		@param zNear Defines the distance between projection center(= camera origin) and beginning of the view frustum(= near view frustum or clipping plane) in floating point units.
					 Must be positive.
					 Everything in front of zNear plane is not visible by this camera.
		@param zFar Defines the distance between projection center(= camera origin) and the end of the view frustum(= far view frustum or clipping plane) in floating point units.
					Must be positive.
					Everything behind zFar plane is not visible by this camera.*/
		void setProjectionProperties(Real fovY, Real aspectRatio, Real zNear, Real zFar);

	private:
		static Camera3D *msActiveCamera; /// points to the currently active camera or is set to NULL

	private:
		Real mFoVY;			/// vertical opening angle of the camera view frustum along its y-Axis in radians
		Real mZFar;			/// absolute distance between projection center (= camera origin) and the end of the view frustum (= far view frustum or clipping plane) in floating point units
		Real mZNear;		/// absolute distance between projection center (= camera origin) and beginning of the view frustum (= near view frustum or clipping plane) in floating point units
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline Camera3D *Camera3D::getActiveCamera()
	{
		return msActiveCamera;
	}

	inline Real Camera3D::getFarClippingPlane() const
	{
		return mZFar;
	}

	inline Real Camera3D::getFoVY() const
	{
		return mFoVY;
	}

	inline Real Camera3D::getNearClippingPlane() const
	{
		return mZNear;
	}

	inline void Camera3D::setAsActiveCamera()
	{
		msActiveCamera = this;
	}
}

#endif // _CAMERA_3D_H_
