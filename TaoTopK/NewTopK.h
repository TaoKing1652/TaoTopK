#pragma once
#include <iostream>
#include <stdio.h>

using namespace std;


template<class T>
class CNewTopK
{
	struct TOPKDATA
	{
		T*	pValue;
		TOPKDATA* pYoungChildren;
		TOPKDATA* pOlderChildren;
	};

public:
	CNewTopK(int nTopNum);
	~CNewTopK();

	int GetHeapTopK(T* pDataArr, int nSize, int& m_nTopNum);
	int GetHeapTopK(TOPKDATA* pDataArr, int nSize, int& m_nTopNum);
	int GetNewTopK(T* pDataArr, int nSize, int& m_nTopNum);
	int NewTopKSelect(TOPKDATA* pDataArr, int nSize, int& m_nTopNum);

public:
	int		   m_nTopNum;
	TOPKDATA*  m_Data;

private:
	void Clear();
	void HeapAdjust(int nStart, int nLen);
	void BuildHeap(int nLen);
};

template<class T>
CNewTopK<T>::CNewTopK(int nTopNum)
{
	m_nTopNum=nTopNum;
	//m_Data = NULL;
	m_Data = new TOPKDATA[m_nTopNum];
}

template<class T>
CNewTopK<T>::~CNewTopK()
{
	Clear();
}

template<class T>
void CNewTopK<T>::Clear()
{
	if (NULL != m_Data)
	{
		delete[] m_Data;
		m_Data = NULL;
	}
}

//调整小根堆，堆顶为Top K最小
template<class T>
void CNewTopK<T>::HeapAdjust(int nStart, int nLen)
{
	int nMinChild = 0;
	TOPKDATA fTemp;

	while ((2 * nStart + 1) < nLen)
	{
		nMinChild = 2 * nStart + 1;
		if ((2 * nStart + 2) < nLen)
		{
			//比较左子树和右子树,记录最小值的Index
			if (*(m_Data[2 * nStart + 2].pValue) < *(m_Data[2 * nStart + 1].pValue))
			{
				nMinChild = 2 * nStart + 2;
			}
		}

		//change data
		if (*(m_Data[nStart].pValue) > *(m_Data[nMinChild].pValue))
		{
			//交换nStart与nMaxChild的数据
			fTemp = m_Data[nStart];
			m_Data[nStart] = m_Data[nMinChild];
			m_Data[nMinChild] = fTemp;

			//堆被破坏,需要重新调整
			nStart = nMinChild;
		}
		else
		{
			//比较左右孩子均大则堆未破坏,不再需要调整
			break;
		}
	}
}

//建立堆
template<class T>
void CNewTopK<T>::BuildHeap(int nLen)
{
	//将m_Data[0, Len-1]建成小根堆，这里只维护一个小根堆，不排序
	for (int i = nLen / 2 - 1; i >= 0; i--)
	{
		HeapAdjust(i, nLen);
	}
}

//获取前top的k个数
template<class T>
int CNewTopK<T>::GetHeapTopK(T* pDataArr, int nSize, int& m_nTopNum)
{
	if (m_nTopNum > nSize)
		m_nTopNum = nSize;

	//分配空间
	//m_Data = new TOPKDATA[m_nTopNum];
	//if (NULL == m_Data)
	//{
	//	cout << "new operator failed!" << endl;
	//	return -1;
	//}

	//cout << "please wait..." << endl;

	int i = 0;
	//读取前m_nTopNum个数据,注意数据的类型T
	for (i = 0; i < m_nTopNum; i++)
	{
		m_Data[i].pOlderChildren = pDataArr + i;
		m_Data[i].pYoungChildren = NULL;
	}

	if (m_nTopNum >= nSize)
		return 0;


	BuildHeap(m_nTopNum);//建立小顶堆

	for (i = m_nTopNum; i < nDataNum; i++)
	{
		if (*(pDataArr + i) > *(m_Data[0].pOlderChildren))
		{
			//交换并调整堆
			m_Data[0].pOlderChildren = (pDataArr + i);
			HeapAdjust(0, m_nTopNum);
		}
	}

	return 0;
}

//获取前top的k个数
template<class T>
int CNewTopK<T>::GetHeapTopK(TOPKDATA* pDataArr, int nSize, int& m_nTopNum)
{
	if (m_nTopNum > nSize)
		m_nTopNum = nSize;

	//分配空间
	m_Data = new TOPKDATA[m_nTopNum];
	//if (NULL == m_Data)
	//{
	//	cout << "new operator failed!" << endl;
	//	return -1;
	//}

	//cout << "please wait..." << endl;

	int i = 0;
	//读取前m_nTopNum个数据,注意数据的类型T
	for (i = 0; i < m_nTopNum; i++)
	{
		m_Data[i] = *(pDataArr + i);
	}

	if (m_nTopNum >= nSize)
		return 0;


	BuildHeap(m_nTopNum);//建立小顶堆

	for (i = m_nTopNum; i < nSize; i++)
	{
		if (*(pDataArr[i].pValue) > *(m_Data[0].pValue))
		{
			//交换并调整堆
			m_Data[0] = pDataArr[i];
			HeapAdjust(0, m_nTopNum);
		}
	}

	return 0;
}

template<class T>
int CNewTopK<T>::GetNewTopK(T* pDataArr, int nSize, int& m_nTopNum)
{
	TOPKDATA* children = new TOPKDATA[nSize];

	for (int i = 0; i<nSize; i++)
	{
		children[i].pValue = pDataArr+i;
		children[i].pYoungChildren = NULL;
		children[i].pOlderChildren = NULL;
	}

	NewTopKSelect(children, nSize, m_nTopNum);

	if (children != NULL) {
		delete[]children;
	}

	return 0;
}

template<class T>
int CNewTopK<T>::NewTopKSelect(TOPKDATA* pDataArr, int nSize, int& m_nTopNum)
{
	if (nSize>3 * m_nTopNum)
	{
		TOPKDATA* children = NULL;

		int nbsize = nSize / 2;
		if (nSize % 2 == 1)
		{
			nbsize = nSize / 2 + 1;
			children = new TOPKDATA[nbsize];

			children[nSize / 2].pValue = pDataArr[nSize - 1].pValue;
			children[nSize / 2].pOlderChildren = pDataArr[nSize - 1].pOlderChildren;
			children[nSize / 2].pYoungChildren = NULL;
		}
		else
		{
			children = new TOPKDATA[nbsize];
		}

		for (int i = 0; i<nSize / 2; i++)
		{
			if (*(pDataArr[2 * i].pValue)<*(pDataArr[2 * i + 1].pValue))
			{
				children[i].pValue = pDataArr[2 * i + 1].pValue;
				children[i].pOlderChildren = pDataArr+2 * i + 1;
				children[i].pYoungChildren = pDataArr+2 * i;
			}
			else
			{
				children[i].pValue = pDataArr[2 * i].pValue;
				children[i].pOlderChildren = pDataArr+2 * i;
				children[i].pYoungChildren = pDataArr+2 * i + 1;
			}
		}

		NewTopKSelect(children, nbsize, m_nTopNum);

		delete[]children;
	}
	else
	{
		TOPKDATA* pTopDataArr = new TOPKDATA[2*m_nTopNum];
		//get topK
		GetHeapTopK(pDataArr, nSize, m_nTopNum);
		int nDataSize=0;
		while(1)
		{
			nDataSize=0;
			for (int i = 0; i < m_nTopNum; i++)
			{
				if(m_Data[i].pOlderChildren != NULL)
				{
					pTopDataArr[nDataSize]=*m_Data[i].pOlderChildren;
					nDataSize++;
				}
				else
				{
					pTopDataArr[nDataSize].pValue=m_Data[i].pValue;
					pTopDataArr[nDataSize].pOlderChildren=m_Data[i].pOlderChildren;
					pTopDataArr[nDataSize].pYoungChildren=m_Data[i].pYoungChildren;
					nDataSize++;
				}

				if(m_Data[i].pYoungChildren != NULL)
				{
					pTopDataArr[nDataSize]=*m_Data[i].pYoungChildren;
					nDataSize++;
				}
			}

			if(nDataSize<m_nTopNum)
			{
				break;
			}

			if(nDataSize==m_nTopNum)
			{
				GetHeapTopK(pTopDataArr, nDataSize, m_nTopNum);
				break;
			}

			GetHeapTopK(pTopDataArr, nDataSize, m_nTopNum);
		}
	}
	return 0;
}

