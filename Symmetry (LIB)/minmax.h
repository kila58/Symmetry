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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	FindMinMax
///
/// \brief	Useful class for finding the maximum and minimum of a list of variables.  Chain for
/// 		best effect.  minmax << var1 << var2 << var3 ...
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class FindMinMax
{
private:
	T m_iMin, m_iMax;

	int m_iMinIndex, m_iMaxIndex;
	int m_iIndex;

public:

	FindMinMax()
	{
		this->m_iIndex = 0;
		this->m_iMinIndex = 0;
		this->m_iMaxIndex = 0;
	}

	FindMinMax& operator<< (const T iVal)
	{
		this->m_iIndex++;

		if (iVal > this->m_iMax || !this->m_iMaxIndex)
		{
			this->m_iMax = iVal;
			this->m_iMaxIndex = this->m_iIndex;
		}
			

		if (iVal < this->m_iMin || !this->m_iMinIndex)
		{
			this->m_iMin = iVal;
			this->m_iMinIndex = this->m_iIndex;
		}

		return *this;
	}

	T GetMin(){ return this->m_iMin; }
	T GetMax(){ return this->m_iMax; }
	int GetIndex(){ return this->m_iIndex; }
	int GetMinIndex(){ return this->m_iMinIndex; }
	int GetMaxIndex(){ return this->m_iMaxIndex; }
};