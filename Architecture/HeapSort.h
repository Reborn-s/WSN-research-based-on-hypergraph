#ifndef HEAPSORT_H
#define HEAPSORT_H
struct ModStruct
{
	int node;
	double dMod;
};
class CHeapSort
{
public:
	bool renew;
	CHeapSort();
	void AdjustHeap(ModStruct *A, int hLen, int i,int *Node_Rank);
	void BuildHeap(ModStruct *A, int hLen,int *Node_Rank);
	void getMax(ModStruct *A,int hLen,int *Node_Rank);
	void HeapSort(ModStruct *A, int aLen,int *Node_Rank);
};

#endif