/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _RANDOM_MANAGER_H_
#define _RANDOM_MANAGER_H_

#include <limits>
#include <random>
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Patterns/Singleton.h"
#include "Platform/DataTypes.h"

namespace Utilities
{
	/// RandomManager realizes a deterministic pseudo random number generator.
	class RandomManager : public Patterns::Singleton<RandomManager>
	{
	public:
		/** Creates and initializes this object with a seed value to enable seeded and therefore controllable pseudo random number generation.
		@param seedValue Set this to some seed value. The same seed value always leads to the same sequence of generated random number results.
			Use this value to control your randomness. */
		RandomManager(uint32 seedValue = RANDOM_SEED);

		/** Draws a random floating point number from a normal / Gaussian distribution.
		@param distribution This defines the distribution, e.g. its mean and variance.
		@return The returned random floating point number was drawn from the entered Gaussian distribution which accurately defines the probability of created random floating point values.*/
		inline Real getNormal(std::normal_distribution<Real> &distribution);

		/** Returns a random floating point value in the range [0, 1] which was drawn from a uniform distribution.
		@return The returned value is in the range [0, 1] and was drawn from a uniform distribution.*/
		inline Real getUniform();

		/** Returns a random floating point value in the range defined by the entered uniform distribution.
		@param distribution Set this to a uniform real distribution with a specific lower and upper bound to be reused.
		@return The returned value is in the defined by the entered uniform distribution.*/
		inline Real getUniform(std::uniform_real_distribution<Real> &distribution);

		/** Draws a random number from a uniform distribution which lies in the interval [min, max].
		@param min This defines the minimal possible value. Return value >= min.
		@param max This defines the maximal possible value. Return value <= max.
		@return The returned value is in the range [min, max] and was drawn from a uniform distribution.*/
		inline Real getUniform(Real min, Real max);

		/** Draws two random numbers from a uniform distribution which are in their corresponding per component intervals [min, max].
		@param min This defines the minimal possible value per component. Return value[x] >= min[x].
		@param max This defines the maximal possible value per component. Return value[x] <= max[x].
		@return The returned vector components are in their corresponding ranges [min, max] and all were drawn from a uniform distribution.*/
		inline Math::Vector2 getUniform(const Math::Vector2 &min, const Math::Vector2 &max);

		/** Draws three random numbers from a uniform distribution which are in their corresponding per component intervals [min, max].
		@param min This defines the minimal possible value per component. Return value[x] >= min[x].
		@param max This defines the maximal possible value per component. Return value[x] <= max[x].
		@return The returned vector components are in their corresponding ranges [min, max] and all were drawn from a uniform distribution.*/
		inline Math::Vector3 getUniform(const Math::Vector3 &min, const Math::Vector3 &max);

		/** Draws four random numbers from a uniform distribution which are in their corresponding per component intervals [min, max].
		@param min This defines the minimal possible value per component. Return value[x] >= min[x].
		@param max This defines the maximal possible value per component. Return value[x] <= max[x].
		@return The returned vector components are in their corresponding ranges [min, max] and all were drawn from a uniform distribution.*/
		inline Math::Vector4 getUniform(const Math::Vector4 &min, const Math::Vector4 &max);

		/** Returns the seed value which was used to initialize the random number generator to support deterministic pseudo random numbers.
		@param Returns the seed value with which the random number generator was initialized.*/
		inline uint32 getSeed() const;

		/** Sets the seed and thus restarts deterministic pseudo random number sequence generation based on the entered seed.
		@param seed This seed value essentially defines the sequence of numbers that is created by this generator. */
		void setSeed(uint32 seed);

	public:	
		static const uint32 RANDOM_SEED = 42;	/// This seed is used by RandomManager for deterministic pseudo random number generation.
		
	private:
		std::mt19937_64	mGenerator;				/// Creates pseudo-random numbers based on an initial seed.
		uint32			mSeed;					/// This seed value was used to initialize this object and determines what random numbers are generated.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline Real RandomManager::getNormal(std::normal_distribution<Real> &distribution)
	{
		return distribution(mGenerator);
	}

	inline Real RandomManager::getUniform()
	{
		std::uniform_real_distribution<Real> distribution(0.0f, 1.0f);
		return distribution(mGenerator);
	}

	inline Real RandomManager::getUniform(std::uniform_real_distribution<Real> &distribution)
	{
		return distribution(mGenerator);
	}

	inline Real RandomManager::getUniform(Real min, Real max)
	{
		std::uniform_real_distribution<Real> distribution(min, max);
		return distribution(mGenerator);
	}

	inline Math::Vector2 RandomManager::getUniform(const Math::Vector2 &min, const Math::Vector2 &max)
	{
		return  Math::Vector2(getUniform(min.x, max.x),
							  getUniform(min.y, max.y));
	}

	inline Math::Vector3 RandomManager::getUniform(const Math::Vector3 &min, const Math::Vector3 &max)
	{
		return  Math::Vector3(getUniform(min.x, max.x),
							  getUniform(min.y, max.y),
							  getUniform(min.z, max.z));
	}

	inline Math::Vector4 RandomManager::getUniform(const Math::Vector4 &min, const Math::Vector4 &max)
	{
		return Math::Vector4(getUniform(min.x, max.x),
							 getUniform(min.y, max.y),
							 getUniform(min.z, max.z),
							 getUniform(min.w, max.w));
	}

	inline uint32 RandomManager::getSeed() const
	{
		return mSeed;
	}
}

#endif // _RANDOM_MANAGER_H_
