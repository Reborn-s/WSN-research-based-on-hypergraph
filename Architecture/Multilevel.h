#ifndef MULTILEVEL_H
#define MULTILEVEL_H

#include "hmetis.h"
#include "CHypergraph.h"
#include <ctime>
#include <queue>
#include <vector>
#include <stack>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HeapSort.h"
//#ifdef WIN32
	#pragma comment(lib, "..//gdiplus//lib//libhmetis.lib")
//	#pragma comment(linker, "/NODEFAULTLIB:libc.lib")
//#else
	//#include "..//libhmetis.a"
//#endif

#define CRITICAL 15.4

//�ڽӱ��洢����ͳ��ߣ�ͨ�����ߵõ�����źͰ����������Լ����еĶ���ָ�룻ͨ������õ������ڵĳ������͵�һ�����ߣ��ٽ��ŵõ����еĳ���ָ��
struct ArcNodeH
{
	int hedge;  //ָ�򳬱ߺ�
	ArcNodeH *nextarc; //�����ö�����һ������
};

struct Vnode
{
	int  node; //�����
	int  Vweight;
	int  nodeNum; //������
	ArcNodeH *firstarc; //�����ö���ĵ�һ������  
};
struct ArcNodeV
{
	int node; //ָ��Ķ����
	ArcNodeV *nextarc; //�ó��߰������¸�����
};

struct HEnode
{
	int Enode; //���ߺ�
	int nodeNum; //���߶�����
	ArcNodeV *firstarc; //�ó��߰����ĵ�һ������
	int Hweight;
};

struct MyHashStruct
{
	int truePos;
	MyHashStruct *next;   //���hash��ͻ
};
struct mymap
{
	int node;
	mymap *next;
};

struct Stone
{
	double curModu;
	int node;
	bool isChanged;
};
class CMygraph
{
public:
	Vnode *pAllVertices;
	HEnode *pAllEdges;
	CMygraph *fathergraph;
	map<int,int> keyTokey;
	MyHashStruct *pMyHashForVert;
	int nvetes;
	int nhedges;
	int maxHweight;
	CMygraph();
	CMygraph(CHypergraph *p);

};

class CMultilevel
{
public:
	CMultilevel();
	CMygraph *pInitGraph;
	void doPartition(CHypergraph *p,int *hcut);
	void myPartition(CHypergraph *p,int *hcut);
	void subpartition(CHypergraph *p,int seed,int *part_res);
	CMygraph* coarseningGraph(CMygraph* initGraph);
	CMygraph* PMcoarseningGraph(CMygraph* initGraph);
	double myFM(int *part_res);
	double ComputeModularity(int *part_res);
	void HillClimbingBigEdgeFirst(int *part_res,vector<int> cutNodes,vector<int> cutEdges);
	vector<int> FindCutNodes(vector<int> cutedges);
	vector<int> FindCutEdges(int *part_res);
	double HillClimbing(int *part_res,vector<int> cutNodes,vector<int> cutEdges,int *Node_Rank,int *PartZeroNodeCount,Vnode *pCutNodes);
	double OptimalSteps(vector<int> cutNodes,int *part_res,int *Node_Rank);
	void NodeRank(vector<int> cutNodes,vector<int> cutEdges,int *Node_Rank);
	void FindZeroNodeCount(vector<int> cutEdges,int *PartZero,int *part_res,Vnode *pCutNodes);
	CMygraph* getMyGraph();
};


#endif