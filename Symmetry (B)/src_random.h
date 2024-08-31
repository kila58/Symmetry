/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#pragma once

#define IA 16807
#define IM 2147483647
#define IQ 127773
#define IR 2836
#define NDIV (1+(IM-1)/NTAB)
#define AM (1.0/IM)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

struct SrcGuassRandom
{
	int seed;
	int y, v[32];
};

class SrcRandom
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline static int SrcRandom::GenerateRandomNumber(SrcGuassRandom& rnd)
	///
	/// \brief	Generates a random number.  Reimplemented source function.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static int GenerateRandomNumber(SrcGuassRandom& rnd)
	{
		int j;
		int k;

		if (rnd.seed <= 0 || !rnd.y)
		{
			if (-(rnd.seed) < 1)
				rnd.seed = 1;
			else
				rnd.seed = -(rnd.seed);

			for (j = 39; j >= 0; j--)
			{
				k = (rnd.seed) / 127773;
				rnd.seed = 16807 * (rnd.seed - k * 127773) - 2836 * k;
				if (rnd.seed < 0)
					rnd.seed += 2147483647;
				if (j < 32)
					rnd.v[j] = rnd.seed;
			}
			rnd.y = rnd.v[0];
		}
		k = (rnd.seed) / 127773;
		rnd.seed = 16807 * (rnd.seed - k * 127773) - 2836 * k;
		if (rnd.seed < 0)
			rnd.seed += 2147483647;
		j = rnd.y / 67108864;

		if (j >= 32 || j < 0)
			j = (j % 32) & 0x7fffffff;

		rnd.y = rnd.v[j];
		rnd.v[j] = rnd.seed;

		return rnd.y;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline static float SrcRandom::RandomFloat(SrcGuassRandom& rnd, float flLow, float flHigh)
	///
	/// \brief	Random float between flLow and flHigh.  Reimplemented source function.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static float RandomFloat(SrcGuassRandom& rnd, float flLow, float flHigh)
	{
		float fl = (4.65661e-010) * GenerateRandomNumber(rnd);
		if (fl > (1.0 - 1.2e-7))
		{
			fl = (1.0 - 1.2e-7);
		}
		return (fl * (flHigh - flLow)) + flLow;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline static const Vector SrcRandom::CalculateSpread(int iSeed, const Vector &vecSpread,
	/// 	const Vector &vecShotDirection)
	///
	/// \brief	Calculates spread for a bullet using a seed.  Reimplemented
	/// 		CShotManipulator::ApplySpread function.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	inline static const Vector CalculateSpread(int iSeed, const Vector &vecSpread, const Vector &vecShotDirection)
	{
		float x, y, z;

		Vector right, up;
		VectorVectors(vecShotDirection, right, up);

		SrcGuassRandom rnd = { -iSeed, 0 };

		do
		{
			x = RandomFloat(rnd, -1, 1) * 0.5f + RandomFloat(rnd, -1, 1) * (1 - 0.5f);
			y = RandomFloat(rnd, -1, 1) * 0.5f + RandomFloat(rnd, -1, 1) * (1 - 0.5f);
			z = x*x + y*y;
		} while (z > 1);

		return vecShotDirection + x * vecSpread.x * right + y * vecSpread.y * up;
	}

};