//============================================================================
// Name        : TaoTopK.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <assert.h>
#include <time.h>

#include "HeapTopK.h"
#include "TaoTopK.h"

using namespace std;

#define N 10000000
#define K 100					///Top K,KµÄÈ¡Öµ


int main()
{
	time_t t;
	srand((unsigned)time(&t));

	int nX = 0;
	int temp = 0;
	int* a = new int[N];
	for (int i = 0; i<N; i++)
	{
		a[i] = i;
	}
	//for (int i = 0; i<N; i++)
	//{
	//	nX = (100+rand()) % N;
	//	temp = a[i];
	//	a[i] = a[nX];
	//	a[nX] = temp;
	//}

	int nNum = K;
	int nLayerNum=1;
	clock_t startTime,endTime;

	CTopK<int> objHeapTopSum;
	startTime = clock();
	for(int i=0; i<10;i++)
		objHeapTopSum.GetTopK(a, N, nNum);
	endTime = clock();
	cout << "\n HeapTopK run clock num: " << 1000.0*(endTime-startTime) / CLOCKS_PER_SEC << endl;
	
	CTaoTopK<int> objTaoTopSum;
	startTime = clock();
	for(int i=0; i<10;i++)
		objTaoTopSum.GetTopK(a, N, nNum);
	endTime = clock();
	cout << "\n TaoTopK run clock num: " << 1000.0*(endTime - startTime) / CLOCKS_PER_SEC << endl;

	//int vmax=0;
	//startTime = clock();
	//for(int i=0; i<N;i++)
	//{
	//	if(*(a+i)>vmax)
	//	vmax=a[i];
	//}
	//endTime = clock();
	//cout << "\n == run clock num: " << 1000.0*(endTime - startTime) / CLOCKS_PER_SEC << endl;

	long long vv = 0;
	long long sum = 0;
	for (int i = 0; i < nNum; i++)
	{
		vv = objTaoTopSum.m_Data[i].data + K - N+1;
		sum += vv;
	}
	cout << "\n TaoTopK top:" << nNum <<" sum:"<<sum<< endl;
	
	delete[]a;


	return 0;
}
