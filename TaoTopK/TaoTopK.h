#pragma once
#include <iostream>
#include <stdio.h>


using namespace std;

template<class T>
class CTaoTopK
{
	struct TOPKINDEX
	{
		int startidx;
		int valueidx;
		int size;
		T   data;
	};
public:
	CTaoTopK();
	~CTaoTopK();
	TOPKINDEX*  m_Data;
	int GetTopK(T* pDataArr, int nDataNum, int& nTop);

private:
	void Clear();
	void HeapAdjust(int nStart, int nLen);
	void BuildHeap(int nLen);

	void GroupDataMax(T* pDataArr, int startidx, int nSize, TOPKINDEX& topKidx);
	void GroupDataMaxEx(T* pDataArr, int startidx, int nSize, TOPKINDEX& topKidx, T refValue);
};

template<class T>
CTaoTopK<T>::CTaoTopK()
{
	m_Data = NULL;
}

template<class T>
CTaoTopK<T>::~CTaoTopK()
{
	Clear();
}

template<class T>
void CTaoTopK<T>::Clear()
{
	if (NULL != m_Data)
	{
		delete[] m_Data;
		m_Data = NULL;
	}
}

template<class T>
int CTaoTopK<T>::GetTopK(T* pDataArr, int nDataNum, int& nTop)
{
	TOPKINDEX fData;
	int i = 0;

	if (nTop > nDataNum)
		nTop = nDataNum;

	int nGroupSize = sqrt(nDataNum*1.0);
	if (nDataNum/nGroupSize < nTop)
		nGroupSize = nDataNum / nTop;

	m_Data = new TOPKINDEX[nTop];
	if (NULL == m_Data)
	{
		cout << "new operator failed!" << endl;
		return -1;
	}

	int nStartidx = 0;
	for (i = 0; i < nTop; i++)
	{
		if ((nStartidx + nGroupSize) < nDataNum)
		{
			GroupDataMax(pDataArr, nStartidx, nGroupSize, m_Data[i]);
		}
		else
		{
			GroupDataMax(pDataArr, nStartidx, nDataNum- nStartidx, m_Data[i]);
		}
		nStartidx += nGroupSize;
	}

	BuildHeap(nTop);

	int nGroupNum = nDataNum / nGroupSize;
	for (i = nTop; i < nGroupNum; i++)
	{
		//GroupDataMax(pDataArr, nStartidx, nGroupSize, fData);
		GroupDataMaxEx(pDataArr, nStartidx, nGroupSize, fData,m_Data[0].data);
		if (fData.data > m_Data[0].data)
		{
			m_Data[0] = fData;
			HeapAdjust(0, nTop);
		}
		nStartidx += nGroupSize;
	}
	
	if (nDataNum - nStartidx > 0)
	{
		GroupDataMax(pDataArr, nStartidx, nDataNum- nStartidx, fData);
		if (fData.data > m_Data[0].data)
		{
			m_Data[0] = fData;
			HeapAdjust(0, nTop);
		}
	}

	/////////////////////////////////////////////////////
	int nNewSize = 0;
	int nContinueFlag = 1;
	while (nContinueFlag)
	{
		nContinueFlag = 0;
		for (i = 0; i < nTop; i++)
		{
			if (m_Data[i].size > 1)
			{
				nNewSize = m_Data[i].size / 2;
				if (m_Data[i].valueidx >= (m_Data[i].startidx + nNewSize))
				{
					GroupDataMax(pDataArr, m_Data[i].startidx, nNewSize, fData);
					m_Data[i].startidx = (m_Data[i].startidx + nNewSize);
					m_Data[i].size = m_Data[i].size - nNewSize;
					if (fData.data > m_Data[0].data)
					{
						m_Data[0] = fData;
						HeapAdjust(0, nTop);
					}
				}
				else
				{
					GroupDataMax(pDataArr, m_Data[i].startidx + nNewSize, m_Data[i].size - nNewSize, fData);
					m_Data[i].size = nNewSize;
					if (fData.data > m_Data[0].data)
					{
						m_Data[0] = fData;
						HeapAdjust(0, nTop);
					}
				}
				nContinueFlag = 1;
			}
		}
	}

	return 0;
}

template<class T>
void CTaoTopK<T>::GroupDataMax(T* pDataArr, int startidx, int nSize, TOPKINDEX& topKidx)
{
	topKidx.startidx = startidx;
	topKidx.size = nSize;
	T pos = startidx;
	for (int i = startidx+1; i < startidx + nSize; i++)
	{
		if (*(pDataArr + i) > *(pDataArr + pos))
		{
			pos = i;
		}
	}
	topKidx.valueidx = pos;
	topKidx.data = *(pDataArr + topKidx.valueidx);
}

template<class T>
void CTaoTopK<T>::GroupDataMaxEx(T* pDataArr, int startidx, int nSize, TOPKINDEX& topKidx, T refValue)
{
	int bb = startidx;
	for (int i = startidx+1; i < startidx + nSize; i++)
	{
		if (*(pDataArr + i) > refValue)
		{
			bb = i;
			break;
		}
	}
	int pos = bb;
	topKidx.startidx = bb;
	topKidx.size = startidx + nSize-bb;
	for (int i = bb+1; i < startidx + nSize; i++)
	{
		if (*(pDataArr + i) > *(pDataArr + pos))
		{
			pos = i;
		}
	}
	topKidx.valueidx = pos;
	topKidx.data = *(pDataArr + topKidx.valueidx);
}

template<class T>
void CTaoTopK<T>::HeapAdjust(int nStart, int nLen)
{
	TOPKINDEX fTemp;
	int nMinChild = 0;

	while ((2 * nStart + 1) < nLen)
	{
		nMinChild = 2 * nStart + 1;
		if ((2 * nStart + 2) < nLen)
		{
			if (m_Data[2 * nStart + 2].data < m_Data[2 * nStart + 1].data)
			{
				nMinChild = 2 * nStart + 2;
			}
		}

		//change data
		if (m_Data[nStart].data > m_Data[nMinChild].data)
		{
			fTemp = m_Data[nStart];
			m_Data[nStart] = m_Data[nMinChild];
			m_Data[nMinChild] = fTemp;

			nStart = nMinChild;
		}
		else
		{
			break;
		}
	}
}

template<class T>
void CTaoTopK<T>::BuildHeap(int nLen)
{
	for (int i = nLen / 2 - 1; i >= 0; i--)
	{
		HeapAdjust(i, nLen);
	}
}
