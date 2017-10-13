/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CAMERA_2D_H_
#define _CAMERA_2D_H_

#include "Graphics/MagicConstants.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector2.h"
#include "Viewport.h"
#include "Platform/Window.h"

namespace Graphics
{
	/** Camera can be rotated around the z-Axis and it can be translated along x-axis and y-axis. */
	class Camera2D
	{
	public:
		/** Creates a camear at the specified world position and the specified angle in radians for z-axis rotation.
		@param position The position in world coordinates of the new camera.
		@param angle	The angle of the new camera in radians for z-axis rotation. 
		@param projectionScale Influences view matrix so that the higher projectionScale is the more is visible. zoomFactor must be greater than zero. */
		Camera2D(const Math::Vector2 &position = Math::Vector2(), Real angle = 0.0f, Real projectionScale = ORTHO_PROJECTION_DEFAULT_SCALE);

		/** Destroy the camera. */
		~Camera2D();

		/** There can only be one camera that is active and responsible for the current rendering iteration.
			Sets this camera to be the active camera. */
		void becomeActiveCamera() const;

		/** Computes the view matrix of the camera for transformation from world space to view space.
		@return The returned matrix contains camera rotation and translation for transformation into view space.*/
		inline Math::Matrix4x4 computeViewMatrix() const;

		/** Computes the view matrix of a camera whereas its position and coordinate axes must be specified.
		@param position This is the position of the camera in world space.
		@param right This is the x-axis of the camera in world space.
		@param up This is the y-axis of the camera in world space.
		@return Thre returned matrix contains camera rotation and translation for transformation into view space. */
		static Math::Matrix4x4 computeViewMatrix(
			const Math::Vector2 &position, const Math::Vector2 & right, const Math::Vector2 &up);

		/** Converts a point which is relative to the GUI coordinate system of the viewport of the camera to a point in world space. (see SubGUI for GUI space)
		@param guiSpacePoint This is a point the coordinates of which are relative to GUI space. (see SubGUI for GUI space)
		@return The returned point is the entered point but its coordinates are relative to world space. */
		inline Math::Vector2 convertGUISpaceToWorldSpace(const Math::Vector2 &guiSpacePoint) const;

		/** Converts a point/direction which is relative to camera's view space (not in pixel) to a point/direction in GUI space. (see SubGUI for GUI space)
		@param viewSpaceVector This is a point/direction the coordinates of which are relative to view space.
		@return The returned point/direction is the entered point/direction but its coordinates are relative to GUI space. (see SubGUI for GUI space) */
		inline Math::Vector2 convertViewSpaceToGUISpace(const Math::Vector2 &viewSpaceVector) const;

		/** Converts a point which is relative to camera's view space (not in pixel) to a point in world space.
		@param viewSpacePoint This is a point the coordinates of which are relative to view space.
		@return The returned point is the entered point but its coordinates are relative to world space. */
		Math::Vector2 convertViewSpaceToWorldSpace(const Math::Vector2 &viewSpacePoint) const;

		/** Provides access to the camera which is currently used for rendering. Returns NULL if none is active.
		@return Returns either the currently active camera or NULL if none is active. The returned Camera object is responsible for the view matrix of the current rendering iteration.*/
		inline static const Camera2D *getActiveCamera();

		/** Returns the angle of rotation around the z-Axis in radians.
		@return Returned value is camera's angle around z-Axis in radians.*/
		inline Real getAngle() const;

		/** Returned vector is the position of the camera in world coordinates.
		@return Returned vector is the position of the camera in world coordinates. */
		inline const Math::Vector2 &getPosition() const;

		/** Query current projection scale. If projectionScale is high then a lot is visible. If its value is low then just a little bit is visible.
		@return Current projection scale value is returned. */
		inline Real getProjectionScale() const;

		/** Returned vector is the x-axis of the coordinate system of the camera in world coordinates.
		@return Returned vector is the x-axis of the camera in world coordinates. */
		inline const Math::Vector2 &getRight() const;

		/** Provides access to the Viewport object this Camera instance projects to.
		@return The returned Viewport instance is the target of this Camera object. */
		inline const Viewport &getViewport() const;

		/** Query the distance between the middle of the camera frustum and the bottom edge of the frustum. 
		@return The returned value is relative to the view space of the camera. */
		inline Real getViewSpaceFrustumBottom() const;
		
		/** Query the distance between the middle of the camera frustum and the left edge of the frustum. 
		@return The returned value is relative to the view space of the camera. */
		inline Real getViewSpaceFrustumLeft() const;
		
		/** Query the distance between the middle of the camera frustum and the right edge of the frustum.
		@return The returned value is relative to the view space of the camera. */
		inline Real getViewSpaceFrustumRight() const;
		
		/** Query the distance between the middle of the camera frustum and the top edge of the frustum.
		@return The returned value is relative to the view space of the camera. */
		inline Real getViewSpaceFrustumTop() const;

		/** Returned vector is the y-axis of the coordinate system of the camera in world coordinates.
		@return Returned vector is the y-axis of the camera in world coordinates. */
		inline const Math::Vector2 &getUp() const;

		/** Moves the camera along its right-axis which is a camera orientation relative movement.
		@param distance The camera is moved distance units along its right-axis. */
		void moveSidewards(Real distance);

		/** Moves the camera along its up-axis which is a camera orientation relative movement.
		@param distance The camera is moved distance units along its up-axis. */
		void moveUpwards(Real distance);

		/** Rotates the camera around the z-axis. (negative delta -> clock rotation = mathematical convention)
		@param deltaAngle The rotation angle in radians. */
		inline void rotate(Real deltaAngle);

		/** Set the angle of the camera to specify a new orientation with respect to the z-axis.
		@param angle New angle in radians for z-axis rotation. */
		void setAngle(Real angle);

		/** Places the camera at the specified world coordinates.
		@param position New camera coordinates in world space. */
		inline void setPosition(const Math::Vector2 &position);

		/** Changes the projection matrix to "zoom in" or "zoom out" of the current scene.
		@param projectionScale If projectionScale is high then a lot is visible. If its value is low then just a little bit is visible. */
		inline void setProjectionScale(Real projectionScale);

		/** Defines camera's viewport.
		@param position Contains the coordinates of the lower left corner of camera's viewport. (in pixel)
		@param size Contains width = size[0] and height = size[1] of camera's viewport. (in pixel)*/
		inline void setViewport(const uint32 position[2], const Utilities::ImgSize &size);

	private:
		/** Copy constructor is forbidden.
		@param copy Copy constructor is forbidden. */
		inline Camera2D(const Camera2D &copy);

		/** Assignment operator is forbidden.
		@param rhs Assignment operator is forbidden. */
		inline Camera2D &operator =(const Camera2D &rhs);

	protected:
		static const Camera2D	*msActiveCamera;		///	the camera which is currently used for rendering

		Viewport				mViewport;				/// the view port camera projects into

		Math::Vector2			mPosition;				/// position in world coordinates
		Math::Vector2			mRight;					/// camera coordinate system's right-vector (x-axis)
		Math::Vector2			mUp;					/// camera coordinate system's up-vector	(y-axis)

		Real					mAngle;					/// rotation around z-Axis in radians
		Real					mFrustumScale;			/// Influences projection matrix so that more is visible whith higher values and less is visible when
														/// mFrustumScale is low. mFrustumScale > 0.0f.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline Math::Matrix4x4 Camera2D::computeViewMatrix() const
	{
		return computeViewMatrix(mPosition, mRight, mUp);
	}

	inline Math::Vector2 Camera2D::convertGUISpaceToWorldSpace(const Math::Vector2 &guiSpacePoint) const
	{
		return convertViewSpaceToWorldSpace(guiSpacePoint * mFrustumScale);
	}

	inline Math::Vector2 Camera2D::convertViewSpaceToGUISpace(const Math::Vector2 &viewSpaceVector) const
	{
		return viewSpaceVector / mFrustumScale;
	}

	inline const Camera2D *Camera2D::getActiveCamera()
	{
		return msActiveCamera;
	}

	inline Real Camera2D::getAngle() const
	{
		return mAngle;
	}

	inline const Math::Vector2 &Camera2D::getPosition() const
	{
		return mPosition;
	}

	inline Real Camera2D::getProjectionScale() const
	{
		return mFrustumScale;
	}

	inline const Math::Vector2 &Camera2D::getRight() const
	{
		return mRight;
	}

	inline const Viewport &Camera2D::getViewport() const
	{
		return mViewport;
	}

	inline Real Camera2D::getViewSpaceFrustumBottom() const
	{
		return mFrustumScale * ORTHO_PROJECTION_BOTTOM;
	}

	inline Real Camera2D::getViewSpaceFrustumLeft() const
	{
		return mFrustumScale * ORTHO_PROJECTION_LEFT * mViewport.getAspectRatio();
	}

	inline Real Camera2D::getViewSpaceFrustumRight() const
	{
		return mFrustumScale * ORTHO_PROJECTION_RIGHT * mViewport.getAspectRatio();
	}

	inline Real Camera2D::getViewSpaceFrustumTop() const
	{
		return mFrustumScale * ORTHO_PROJECTION_TOP;
	}

	inline const Math::Vector2 &Camera2D::getUp() const
	{
		return mUp;
	}

	inline void Camera2D::rotate(Real deltaAngle)
	{
		setAngle(mAngle + deltaAngle);
	}

	inline void Camera2D::setPosition(const Math::Vector2 &position)
	{
		mPosition = position;
	}

	inline void Camera2D::setProjectionScale(Real projectionScale)
	{
		mFrustumScale = projectionScale;
	}

	inline void Camera2D::setViewport(const uint32 position[2], const Utilities::ImgSize &size)
	{
		mViewport.set(position, size);
	}

	inline Camera2D::Camera2D(const Camera2D &copy)
	{
		assert(false);
	}

	inline Camera2D &Camera2D::operator =(const Camera2D &rhs)
	{
		assert(false);
		return *this;
	}
}

#endif // _CAMERA_2D_H_
