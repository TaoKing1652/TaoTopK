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

//��ȡǰtop��k����
template<class T>
int CTopK<T>::GetTopK(T* pDataArr, int nDataNum, int& nTop)
{
	FILE* fp = NULL;
	T fData;
	int i = 0;

	if (nTop > nDataNum)
		nTop = nDataNum;

	//����ռ�
	m_Data = new T[nTop];
	if (NULL == m_Data)
	{
		cout << "new operator failed!" << endl;
		return -1;
	}

	//cout << "please wait..." << endl;

	//��ȡǰnTop������,ע�����ݵ�����T
	for (i = 0; i < nTop; i++)
	{
		m_Data[i] = *(pDataArr + i);
	}

	BuildHeap(nTop);//����С����

	for (i = nTop; i < nDataNum; i++)
	{
		fData= *(pDataArr + i);
		if (fData > m_Data[0])
		{
			//������������
			m_Data[0] = fData;
			HeapAdjust(0, nTop);
		}
	}

	return 0;
}
//��ȡǰtop��k����
template<class T>
int CTopK<T>::GetTopK(const char* sFile, int& nTop)
{
	FILE* fp = NULL;
	T fData;
	int i = 0;

	//�ж��������
	if ((NULL == sFile) || (nTop <= 0))
	{
		cout << "error parameter" << endl;
		return -1;
	}

	//�������
	Clear();

	//���ļ�
	fp = fopen(sFile, "r");
	if (NULL == fp)
	{
		cout << "open file failed!" << endl;
		return -1;
	}

	//����ռ�
	m_Data = new T[nTop];
	if (NULL == m_Data)
	{
		cout << "new operator failed!" << endl;
		return -1;
	}

	//cout << "please wait..." << endl;

	//��ȡǰnTop������,ע�����ݵ�����T
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

	//���ĸ���С��nTop,��ǰi������
	if (i < nTop)
	{
		nTop = i;
	}
	else
	{
		BuildHeap(nTop);//����С����

		while (EOF != fscanf(fp, "%d", &fData))
		{
			if (fData > m_Data[0])
			{
				//������������
				m_Data[0] = fData;
				HeapAdjust(0, nTop);
			}
		}
	}

	return 0;
}

//����С���ѣ��Ѷ�ΪTop K��С
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
			//�Ƚ���������������,��¼��Сֵ��Index
			if (m_Data[2 * nStart + 2] < m_Data[2 * nStart + 1])
			{
				nMinChild = 2 * nStart + 2;
			}
		}

		//change data
		if (m_Data[nStart] > m_Data[nMinChild])
		{
			//����nStart��nMaxChild������
			fTemp = m_Data[nStart];
			m_Data[nStart] = m_Data[nMinChild];
			m_Data[nMinChild] = fTemp;

			//�ѱ��ƻ�,��Ҫ���µ���
			nStart = nMinChild;
		}
		else
		{
			//�Ƚ����Һ��Ӿ������δ�ƻ�,������Ҫ����
			break;
		}
	}
}

//������
template<class T>
void CTopK<T>::BuildHeap(int nLen)
{
	int i = 0;
	//T nTemp;

	//��m_Data[0, Len-1]����С���ѣ�����ֻά��һ��С���ѣ�������
	for (i = nLen / 2 - 1; i >= 0; i--)
	{
		HeapAdjust(i, nLen);
	}
}
