/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _BASE_CAMERA_3D_H_
#define _BASE_CAMERA_3D_H_

#include "Graphics/BaseCamera3D.h"
#include "Math/Matrix4x4.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Platform/DataTypes.h"
#include "Utilities/Size2.h"

namespace Graphics
{
	/// This virtual camera representation is designed for view and projection matrix computation.
	/** Represents a virtual camera to easily manage view and projection transformation matrices. */
	class BaseCamera3D
	{
	public:
		/** Computes the local coordinate axes of a camera based on a specified position, target to look at and a vector which points upwards.
		@param xAxis This is set to a unit length vector in direction of the local x-axis of the camera. (pointing sidewards / strife direction)
		@param yAxis This is set to a unit length vector in direction of the local y-axis of the camera. (pointing upwards to the camera top)
		@param zAxis This is set to a unit length vector in direction of the local z-axis of the camera which is set to the direction of v = target - position.
		@param positionWS Set this to world space coordiantes of the camera center.
		@param targetWS Set this to the world space coordinates of the point at which the camera looks.
		@param up Set this to a vector pointing upwards. This is ususally the y-axis. */
		static void computeLocalAxes(Math::Vector3 &xAxis, Math::Vector3 &yAxis, Math::Vector3 &zAxis,
			const Math::Vector4 &positionWS, const Math::Vector4 &targetWS, const Math::Vector3 &up = Math::Vector3(0.0f, 1.0f, 0.0f));

	public:
		/** Initializes the camera with the entered projection properties.
			It is initially placed at the origin.
		@param projection Defines the projection properties of the camera. See Matrix4x4::create... functions.
		@param aspectRatio This is the ratio of width to height of the camera's image plane. Must be positive.*/
		BaseCamera3D(const Math::Matrix4x4 &projection, const Real aspectRatio);

		/** Destroys the camera. */
		virtual ~BaseCamera3D();

		/** Computes and returns a matrix P^-1 that
			maps normalized homogenous device coordinates (x, y, z) with z = 1 (after viewport transformation - not in pixels) to non-normalized view space camera ray directions. 
		@return Computes and returns a matrix P^-1 that maps normalized homogenous device coordinates (x, y, z) with z = 1 to non-normalized view space camera ray directions.  */
		virtual Math::Matrix3x3 computeInverseProjectionMatrix() const = 0;

		/** Compuates a matrix V^-1 which maps view space to world space coordinates, pVS * V^-1 = pWS.
		@return Returns a matrix V^-1 which maps view space to world space coordinates, pVS * V^-1 = pWS. */
		Math::Matrix4x4 computeInverseViewMatrix() const;
		
		/** Compuates a matrix R^-1 which maps view space directions to world space directions, dVS * R^-1 = dWS.
		@return Returns a matrix R^-1 which maps view space directions to world space directions, dVS * R^-1 = dWS. */
		Math::Matrix3x3 computeInverseRotationMatrix() const;

		/** Computes a matrix that converts a homogenous pixel position (px, py, z = 1) \in ([0, viewport width], [0, viewport height], 1) to a
			non-normalized ray direction in world space coordinates.
		@param imageSize Defines the width ([0]) and height ([1]) of the viewport for which the returned matrix computes ray directions.
		@param addPixelCenterOffset If this is set to true then integer pixel coordinates refer to pixel centers since an offset of half a pixel is added to the coordinates.
			If the parameter is set to false then rays for the upper left pixel corner are returned. 
		@return Returns a matrix M which converts homogeneours (3D) pixel coordinates to non-normalized ray directions (||rayDir|| is not necessarily 1) posHPS * M = rayDirWS. */
		virtual Math::Matrix3x3 computeHPSToNNRayDirWS(const Utilities::ImgSize &imageSize, const bool addPixelCenterOffset = true) const = 0;
				
		/** Computes and returns the matrix which transforms coordinates relative to the world space coordinate system into the pixel coordinate system of this camera for some resolution.
			The pixel coordinates are not normalized! You must perform the perspective division after the matrix has been applied.
		@param imageSize Defines the width ([0]) and height ([1]) of the viewport for which the returned matrix computes pixel coordinates.
		@param considerPixelCenterOffset Set this to true if you want to get coordinates refering to pixel centers instead of lower left corners.
			E.g., a point at the lower left camera frustum edge is mapped to the pixel coordinates (0.5, 0.5) if this is set to true (instead of (0, 0)). 
		@return Returns a matrix which transforms world space coordiantes into non normalized pixel coordinates relative to the camera's viewport of resolution imageSize.*/
		virtual Math::Matrix4x4 computeWorldSpaceToPixelSpaceMatrix(const Utilities::ImgSize &imageSize, const bool considerPixelCenterOffset = true) const = 0;
		
		/** Returns the aspect ratio (width / height) used for the projection matrix of the camera. (ratio of image plane width to image plane height)
		@return Returns the aspect ratio (width / height) used for the projection matrix of the camera.*/
		inline Real getAspectRatio() const;

		/** Returns the current orientation of this camera which is a unit quaternion.
		@return The returned quaternion represents the roation from identity to the current orientation of the camera. */
		const Math::Quaternion &getOrientation() const { return mOrientation; }

		/** Get the camera origin which is the projection center.
		@return Returns th origin of the camera which is the projection center. (usually in world coordinates)*/
		const Math::Vector4 &getPosition() const { return mPosition; }

		/** Returns camera projection matrix.
		@return Returns the projection matrix to map from view space to clipping coordinates which are mapped to normalized device coordinates by perspective division done by the graphics card. */
		const Math::Matrix4x4 &getProjectionMatrix() const { return mProjection; }

		/** Obtain the matrix to map from world to to view space of this camera.
		@return The returned view matrix maps coords from world to view space. (vectorVS = vectorWS * mView)*/
		const Math::Matrix4x4 &getViewMatrix() const { return mView; }

		/** Creates view matrix depending on entered camera position, the target point the camera is looking at and some up-vector defining where the camera top is.
		@param positionWS Defines the origin (projection center) of the camera.
		@param targetWS Defines the point at which the camera is directly looking and which is projected to the screen center.
		@param upWS A 3D vector defining where the top of the camera is. Must not be colinear to the vector between target and position. */
		void lookAt(const Math::Vector4 &positionWS, const Math::Vector4 &targetWS, const Math::Vector3 &upWS);

		/** Creates view matrix depending on entered camera position, the target point the camera is looking at and some up-vector defining where the camera top is.
			@param position Defines the origin (projection center) of the camera.
			@param target Defines the point at which the camera is directly looking and which is projected to the screen center.
			@param up A 3D vector defining where the top of the camera is. Must not be colinear to the vector between target and position. */
		void lookAt(const Math::Vector3 &positionWS, const Math::Vector3 &targetWS, const Math::Vector3 &upWS);

		/** Moves the camera along the positive direction of its z-Axis.
		@param distance The camera is moved distance floating point units along the positive direction of its z-Axis. */
		void moveForwards(Real distance);

		/** Moves the camera along the positive direction of its x-Axis.
		@param distance The camera is moved distance floating point units along the positive direction of its x-Axis. */
		void moveSidewards(Real distance);

		/** Moves the camera along the positive direction of its y-Axis.
		@param distance The camera is moved distance floating point units along the positive direction of its y-Axis. */
		void moveUpwards(Real distance);

		/** Camera makes a pitch movement which means that it rotates around its x-Axis.
		@param deltaAngle Defines how far the camera pitches in radians. */
		void pitch(Real deltaAngle);

		/** Camera makes a roll which means that it rotates around its z-Axis.
		@param deltaAngle Defines how far the camera rolls in radians. */
		void roll(Real deltaAngle);

		/** Sets the aspect ratio of the camera image plane width to its height.
		@param aspectRatio Set this to the ratio of the image plane width to its height. Must be positive. */
		virtual void setAspectRatio(const Real aspectRatio) = 0;

		/** Sets the camera orientation according to the entered quaternion which must represent an orientation / rotation.
		@param newOrientation Must be a unit quaternion. (newOrientation must be a normalized 4D vector).*/
		void setOrientation(const Math::Quaternion &newOrientation);

		/** Sets the camera orientation according to the entered rotation matrix which must represent an orientation / rotation / right handed coordinate system.
		@param newOrientation Must be a right handed orthonormal matrix. (3 unit orthogonal unit length vectors).*/
		void setOrientation(const Math::Matrix3x3 &newRightHandedAxes);

		/** Sets the camera to some world space position.
		@param xWS Set this to the new camera world space x-coordinate.
		@param xWS Set this to the new camera world space y-coordinate.
		@param xWS Set this to the new camera world space z-coordinate.
		@param xWS Set this to the new camera world space w-coordinate. */
		void setPosition(const Real xWS, const Real yWS, const Real zWS, const Real wWS = 1);

		/** Completely sets the definition of how the camera maps view space to image plane pixel coordinates.
		@param P Set this to the projection matrix you want to use for this camera.
		@see Take a look at setProjectionProperties and class Math::Matrix4x4 which provides projection matrix creation functions:
			Math::Matrix4x4::createOrthoLHDirectX,
			Math::Matrix4x4::createOrthoRHDirectX,
			Math::Matrix4x4::createProjectionLHDirectX,
			Math::Matrix4x4::createProjectionRHDirectX,
			Math::Matrix4x4::createProjectionFovLHDirectX,
			Math::Matrix4x4::createProjectionFovRHDirectX,
			Math::Matrix4x4::createProjectionRealWorld */
		void setProjectionMatrix(const Math::Matrix4x4 &P);

		/** Camera makes a yaw movement which means that it rotates around its y-Axis or the standard y-axis.
		@param deltaAngle Defines how far the camera yaws in radians.
		@param fixedYawAxis Set this to true to rotate the camera around the standard y-axis or false to rotate it around its own y-axis. */
		void yaw(Real deltaAngle, bool fixedYawAxis = true);

	protected:
		/** Efficiently recalculates view matrix based on internal camera orientation and position. */
		void recalculateViewMatrix();

		/** Efficiently recalculates view matrix based on internal camera position and entered axes wich are the standard axes rotated by mOrientation.
		@param rotatedXAxis Set this to the x-axis rotated by mOrientation.
		@param rotatedYAxis Set this to the y-axis rotated by mOrientation. 
		@param rotatedZAxis Set this to the z-axis rotated by mOrientation. */
		void recalculateViewMatrix(const Math::Vector3 &rotatedXAxis, const Math::Vector3 &rotatedYAxis, const Math::Vector3 &rotatedZAxis);

	protected:
		Math::Matrix4x4	mProjection;	/// projection matrix to map from view space to clipping coordinates (which are mapped to normalized device coordinates by perspective division done by the graphics card)
		Math::Matrix4x4	mView;			/// view matrix maps coords from world to view space (vectorVS = vectorWS * mView)

		Math::Vector4 mPosition;		/// origin of the camera = projection center (usually in world coordinates)
		Math::Quaternion mOrientation;	/// orientation definition = rotation applied to standard orientation
										/// (standard orientation means rotation of view matrix = identity matrix)
		Real mAspectRatio;				/// ratio of camera screen width to height, aspect ratio (screen width / screen height)
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline Real BaseCamera3D::getAspectRatio() const
	{
		return mAspectRatio;
	}
}

#endif // _BASE_CAMERA_3D_H_
