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

//邻接表存储顶点和超边，通过超边得到其序号和包含顶点数以及所有的顶点指针；通过顶点得到其属于的超边数和第一条超边，再接着得到所有的超边指针
struct ArcNodeH
{
	int hedge;  //指向超边号
	ArcNodeH *nextarc; //包含该顶点下一条超边
};

struct Vnode
{
	int  node; //顶点号
	int  Vweight;
	int  nodeNum; //超边数
	ArcNodeH *firstarc; //包含该顶点的第一条超边  
};
struct ArcNodeV
{
	int node; //指向的顶点号
	ArcNodeV *nextarc; //该超边包含的下个顶点
};

struct HEnode
{
	int Enode; //超边号
	int nodeNum; //超边顶点数
	ArcNodeV *firstarc; //该超边包含的第一个顶点
	int Hweight;
};

struct MyHashStruct
{
	int truePos;
	MyHashStruct *next;   //解决hash冲突
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
