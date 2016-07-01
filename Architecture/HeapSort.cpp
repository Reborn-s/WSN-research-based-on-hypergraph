
#include "HeapSort.h"
CHeapSort::CHeapSort()
{
	renew = true;
}
void CHeapSort::AdjustHeap(ModStruct *A, int hLen, int i,int *Node_Rank)
{
     int left = 2*i+1;  //节点i的左孩子
     int right = 2*i+2; //节点i的右孩子节点
     int largest = i;
     double tempMod;
	 int tempNode;
  
     while(left < hLen || right < hLen)
     {
		if (left < hLen && A[largest].dMod <= A[left].dMod)
        {
			if(A[largest].dMod == A[left].dMod)
			{
				if(Node_Rank[A[largest].node]>Node_Rank[A[left].node])
					largest = left;
			}
			else
				largest = left;
        }
         
		if (right < hLen && A[largest].dMod <= A[right].dMod)
        {
			if(A[largest].dMod == A[right].dMod)
			{
				if(Node_Rank[A[largest].node]>Node_Rank[A[right].node])
					largest = right;
			}
			else
				largest = right;
        }
        if (i != largest)   //如果最大值不是父节点
        {
			 tempMod = A[largest].dMod; //交换父节点和和拥有最大值的子节点交换
			 tempNode = A[largest].node;
			 A[largest].dMod = A[i].dMod;
			 A[largest].node = A[i].node;
			 A[i].dMod = tempMod;
			 A[i].node = tempNode;
             i = largest;         //新的父节点，以备迭代调堆
             left = 2*i+1;  //新的子节点
             right = 2*i+2;
         }
         else
         {
            break;
         }
     }
}
void CHeapSort::BuildHeap(ModStruct *A, int hLen,int *Node_Rank)
{
	int i;
    int begin = hLen/2 - 1;  //最后一个非叶子节点
    for (i = begin; i >= 0; i--)
    {
        AdjustHeap(A, hLen, i,Node_Rank);  
    }
}
void CHeapSort::HeapSort(ModStruct *A, int aLen,int *Node_Rank)
{
	/*
	int hLen = aLen;
    int temp;
    BuildHeap(A, hLen);      //建堆 
    while (hLen > 1)
    {
        temp = A[hLen-1];    //交换堆的第一个元素和堆的最后一个元素
        A[hLen-1] = A[0];
        A[0] = temp;
        hLen--;        //堆的大小减一
        AdjustHeap(A, hLen, 0);  //调堆
	}
	*/
}
void CHeapSort::getMax(ModStruct *A, int hLen,int *Node_Rank)
{
	double tempMod;
	int tempNode;
	if(renew==true)
	{
		BuildHeap(A, hLen,Node_Rank);      //建堆 
		tempMod = A[hLen-1].dMod;    //交换堆的第一个元素和堆的最后一个元素
		tempNode = A[hLen-1].node;
		A[hLen-1].dMod = A[0].dMod;
		A[hLen-1].node = A[0].node;
		A[0].dMod = tempMod;
		A[0].node = tempNode;
		//hLen--;        //堆的大小减一
		//AdjustHeap(A, hLen, 0,Node_Rank);  //调堆
		
	}
	else
	{
		/*
		tempMod = A[hLen-1].dMod;    //交换堆的第一个元素和堆的最后一个元素
		tempNode = A[hLen-1].node;
		A[hLen-1].dMod = A[0].dMod;
		A[hLen-1].node = A[0].node;
		A[0].dMod = tempMod;
		A[0].node = tempNode;
		hLen--;        //堆的大小减一
		*/
		AdjustHeap(A, hLen, 0,Node_Rank);  //调堆
	}
}