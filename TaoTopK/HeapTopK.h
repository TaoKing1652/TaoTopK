#pragma once
#include <iostream>
#include <stdio.h>


using namespace std;

//from https://blog.csdn.net/luxiaoxun/article/details/7796368

template<class T>
class CTopK
{
public:
	CTopK();
	~CTopK();
	T*  m_Data;
	int GetTopK(const char* sFile, int& nTop);
	int GetTopK(T* pDataArr, int nDataNum, int& nTop);
private:
	void Clear();
	void HeapAdjust(int nStart, int nLen);
	void BuildHeap(int nLen);
};

template<class T>
CTopK<T>::CTopK()
{
	m_Data = NULL;
}

template<class T>
CTopK<T>::~CTopK()
{
	Clear();
}

template<class T>
void CTopK<T>::Clear()
{
	if (NULL != m_Data)
	{
		delete[] m_Data;
		m_Data = NULL;
	}
}

//获取前top的k个数
template<class T>
int CTopK<T>::GetTopK(T* pDataArr, int nDataNum, int& nTop)
{
	FILE* fp = NULL;
	T fData;
	int i = 0;

	if (nTop > nDataNum)
		nTop = nDataNum;

	//分配空间
	m_Data = new T[nTop];
	if (NULL == m_Data)
	{
		cout << "new operator failed!" << endl;
		return -1;
	}

	//cout << "please wait..." << endl;

	//读取前nTop个数据,注意数据的类型T
	for (i = 0; i < nTop; i++)
	{
		m_Data[i] = *(pDataArr + i);
	}

	BuildHeap(nTop);//建立小顶堆

	for (i = nTop; i < nDataNum; i++)
	{
		fData= *(pDataArr + i);
		if (fData > m_Data[0])
		{
			//交换并调整堆
			m_Data[0] = fData;
			HeapAdjust(0, nTop);
		}
	}

	return 0;
}
//获取前top的k个数
template<class T>
int CTopK<T>::GetTopK(const char* sFile, int& nTop)
{
	FILE* fp = NULL;
	T fData;
	int i = 0;

	//判断输入参数
	if ((NULL == sFile) || (nTop <= 0))
	{
		cout << "error parameter" << endl;
		return -1;
	}

	//清除操作
	Clear();

	//打开文件
	fp = fopen(sFile, "r");
	if (NULL == fp)
	{
		cout << "open file failed!" << endl;
		return -1;
	}

	//分配空间
	m_Data = new T[nTop];
	if (NULL == m_Data)
	{
		cout << "new operator failed!" << endl;
		return -1;
	}

	//cout << "please wait..." << endl;

	//读取前nTop个数据,注意数据的类型T
	for (i = 0; i < nTop; i++)
	{
		if (EOF != fscanf(fp, "%d", &fData))
		{
			m_Data[i] = fData;
		}
		else
		{
			break;
		}
	}

	//最大的个数小于nTop,求前i个数据
	if (i < nTop)
	{
		nTop = i;
	}
	else
	{
		BuildHeap(nTop);//建立小顶堆

		while (EOF != fscanf(fp, "%d", &fData))
		{
			if (fData > m_Data[0])
			{
				//交换并调整堆
				m_Data[0] = fData;
				HeapAdjust(0, nTop);
			}
		}
	}

	return 0;
}

//调整小根堆，堆顶为Top K最小
template<class T>
void CTopK<T>::HeapAdjust(int nStart, int nLen)
{
	int nMinChild = 0;
	T fTemp;

	while ((2 * nStart + 1) < nLen)
	{
		nMinChild = 2 * nStart + 1;
		if ((2 * nStart + 2) < nLen)
		{
			//比较左子树和右子树,记录最小值的Index
			if (m_Data[2 * nStart + 2] < m_Data[2 * nStart + 1])
			{
				nMinChild = 2 * nStart + 2;
			}
		}

		//change data
		if (m_Data[nStart] > m_Data[nMinChild])
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
void CTopK<T>::BuildHeap(int nLen)
{
	int i = 0;
	//T nTemp;

	//将m_Data[0, Len-1]建成小根堆，这里只维护一个小根堆，不排序
	for (i = nLen / 2 - 1; i >= 0; i--)
	{
		HeapAdjust(i, nLen);
	}
}
