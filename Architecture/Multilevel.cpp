
#include "Multilevel.h"
#include <iostream>

#define NODE_NUMBER		2000
#define ENERGY_INITIAL    2      //初始能量
#define SINK_X   300
#define SINK_Y   300
#define HELP 5
extern WSNnode nodes[NODE_NUMBER];    //使用Architecture.cpp中的nodes数组
extern vector<vector<WSNnode>> totalHedge;   //使用Architecture.cpp中的totalHedge

CMygraph::CMygraph()
{
	pAllVertices = NULL;
	pAllEdges = NULL;
	fathergraph = NULL;
	nvetes =0;
	nhedges = 0;
	maxHweight = 1;
	fathergraph = NULL;
}
CMygraph::CMygraph(CHypergraph *p)
{
	fathergraph = NULL;
	pAllVertices = (Vnode*)malloc(sizeof(struct Vnode)*(p->nvtxs));
	pAllEdges  = (HEnode*)malloc(sizeof(struct HEnode)*(p->nhedges));
	nvetes = p->nvtxs;
	nhedges = p->nhedges;
	maxHweight = 1;
	for(int m=0;m<p->nvtxs;m++)
	{
		pAllVertices[m].firstarc = NULL;
		pAllVertices[m].nodeNum = 0;
		if(p->vwgts!=NULL)
			pAllVertices[m].Vweight = p->vwgts[m];
	}
	
	for(int m=0;m<p->nhedges;m++)
	{
		pAllEdges[m].firstarc = NULL;
		pAllEdges[m].nodeNum = 0;
		if(p->hewgts!=NULL)
		{
			pAllEdges[m].Hweight = p->hewgts[m];
			maxHweight = maxHweight>p->hewgts[m]?maxHweight:p->hewgts[m];
		}
	}

	int edgecount = 0;
	for(int i=1;i<=p->nhedges;i++)
	{
		pAllEdges[edgecount].Enode = edgecount;
		
		for(int j=p->eptr[i-1];j<p->eptr[i];j++)
		{
			ArcNodeV *newvert = (ArcNodeV*)malloc(sizeof(struct ArcNodeV));
			newvert->node = p->eind[j];
			newvert->nextarc = pAllEdges[edgecount].firstarc;
			pAllEdges[edgecount].firstarc = newvert;
			pAllEdges[edgecount].nodeNum += pAllVertices[p->eind[j]].Vweight;
			ArcNodeH *newedge =(ArcNodeH*)malloc(sizeof(struct ArcNodeH));
			newedge->hedge = edgecount;
			newedge->nextarc = pAllVertices[p->eind[j]].firstarc;
			pAllVertices[p->eind[j]].firstarc = newedge;
			pAllVertices[p->eind[j]].nodeNum +=pAllEdges[edgecount].Hweight;

		}
		edgecount++;
	}

}

CMultilevel::CMultilevel()
{

}

void CMultilevel::doPartition(CHypergraph *p,int *hcut)
{
	//HMETIS_PartRecursive(p->nvtxs, p->nhedges, p->vwgts, \
				p->eptr, p->eind, p->hewgts, p->npart, p->ubfactor, \
				p->inOptions, p->part, hcut);
}

typedef pair<int,double> PAIR_M;
int cmp_M(const PAIR_M& x,const PAIR_M& y)
{
	return x.second>y.second;			
}

void CMultilevel::myPartition(CHypergraph *p,int *hcut)
{
	pInitGraph = new CMygraph(p);
		
	CMygraph *pCoarsenGraph = pInitGraph;
	
	for(int i=0;i<1;i++)
	{
		pCoarsenGraph = coarseningGraph(pCoarsenGraph);        //超边合并程度
	}
	srand((unsigned)time(0));
	
	int *seedmap = new int[pCoarsenGraph->nvetes];
	double modularity = -10000000.0;
	p->is_parted = false;
	map<int,bool> seedexit;
	for(int i=0;i<10&&i<pCoarsenGraph->nvetes;i++)
	{
 		int seed = rand() % pCoarsenGraph->nvetes;
		
		//
		map<int,bool>::iterator seediter;
		seediter = seedexit.find(seed);
		if(seediter!=seedexit.end())
			continue;
		else
		{
			seedexit.insert(pair<int,bool>(seed,true));
		}

		//
		if(seedmap[seed]!=1)
		{
			seedmap[seed] = 1;
			MyHashStruct *pInitVert = pCoarsenGraph->pMyHashForVert[seed].next;
			CMygraph *hisfather = pCoarsenGraph->fathergraph;
			while(hisfather!=NULL)
			{
				if(hisfather->fathergraph!=NULL)
					pInitVert = hisfather->pMyHashForVert[pInitVert->truePos].next;	
				else
					break;
				hisfather = hisfather->fathergraph;
			}
			int *part_res = new int[p->nvtxs];
			subpartition(p,pInitVert->truePos,part_res);
			double cur_modularity = myFM(part_res);
			
			if(cur_modularity>modularity)
			{
				p->Modu = cur_modularity;
				p->is_parted = true;
				modularity = cur_modularity;
				for(int k=0;k<p->nvtxs;k++)
				{
					p->part[k] = part_res[k];
				}
			}
		}
		else
		{
			i--;
			continue;
		}
	}

	if(p->fatherGraph!=NULL)
	{
	//计算分开后的消耗
	CHypergraph* tempGraph=p;
	map<int,int>::iterator iterForMap;
	vector<int> nodeNumber0;
	vector<int> nodeNumber1;
	vector<int> tempNodeNumber0;
	vector<int> tempNodeNumber1;
	while(tempGraph->fatherGraph != NULL)
	{
		if(nodeNumber0.size()==0&&nodeNumber1.size()==0)
		{
			for(int i=0;i<tempGraph->nvtxs;i++)
			{
				iterForMap=tempGraph->mapForFather.find(i);
				if(tempGraph->part[i]==0)
					nodeNumber0.push_back(iterForMap->second);
				if(tempGraph->part[i]==1)
					nodeNumber1.push_back(iterForMap->second);
			}
			tempGraph=tempGraph->fatherGraph;
		}
		else{
			for(int j=0;j<nodeNumber0.size();j++)
			{
				iterForMap=tempGraph->mapForFather.find(nodeNumber0.at(j));
				tempNodeNumber0.push_back(iterForMap->second);
			}
			for(int j=0;j<nodeNumber1.size();j++)
			{
				iterForMap=tempGraph->mapForFather.find(nodeNumber1.at(j));
				tempNodeNumber1.push_back(iterForMap->second);
			}
			nodeNumber0.clear();
			nodeNumber0=tempNodeNumber0;
			tempNodeNumber0.clear();
			nodeNumber1.clear();
			nodeNumber1=tempNodeNumber1;
			tempNodeNumber1.clear();

			tempGraph=tempGraph->fatherGraph;
		}
	}

	int totalX=0;
	int totalY=0;
	for(int k=0;k<nodeNumber0.size();k++)
	{
		int X=nodes[nodeNumber0.at(k)].getX();
		int Y=nodes[nodeNumber0.at(k)].getY();
		totalX+=X;
		totalY+=Y;
	}
	int averageX0=(int)totalX/(nodeNumber0.size());
	int averageY0=(int)totalY/(nodeNumber0.size());
	map<int,double> nodeCenterdistance0;
	for(int i=0;i<nodeNumber0.size();i++)
	{
		int x=nodes[nodeNumber0.at(i)].getX();
		int y=nodes[nodeNumber0.at(i)].getY();
		double centerDistance=sqrt(pow((x-averageX0),2.0)+pow((y-averageY0),2.0));
		double currentEnergy=nodes[nodeNumber0.at(i)].getEnergy();
		nodeCenterdistance0[i]=1/centerDistance+((currentEnergy+HELP)/(ENERGY_INITIAL+HELP));
	}
	vector<PAIR_M> nodes_centerdistance_vec0(nodeCenterdistance0.begin(),nodeCenterdistance0.end());
	sort(nodes_centerdistance_vec0.begin(),nodes_centerdistance_vec0.end(),cmp_M);     //将map按value进行从大到小排序

	WSNnode cHeadNode0=nodes[nodeNumber0.at(nodes_centerdistance_vec0.at(0).first)];
	double innerDistance0=0.0;
	int cHeadX0=cHeadNode0.getX();
	int cHeadY0=cHeadNode0.getY();
	for(int i=0;i<nodeNumber0.size();i++)
	{
		int x=nodes[nodeNumber0.at(i)].getX();
		int y=nodes[nodeNumber0.at(i)].getY();
		innerDistance0+=sqrt(pow((x-cHeadX0),2.0)+pow((y-cHeadY0),2.0));
	}

	double outerDistance0=sqrt(pow((SINK_X-cHeadX0),2.0)+pow((SINK_Y-cHeadY0),2.0));
	double part0Consum=innerDistance0+outerDistance0;

	totalX=0;
	totalY=0;
	for(int k=0;k<nodeNumber1.size();k++)
	{
		int X=nodes[nodeNumber1.at(k)].getX();
		int Y=nodes[nodeNumber1.at(k)].getY();
		totalX+=X;
		totalY+=Y;
	}
	int averageX1=(int)totalX/(nodeNumber1.size());
	int averageY1=(int)totalY/(nodeNumber1.size());
	map<int,double> nodeCenterdistance1;
	for(int i=0;i<nodeNumber1.size();i++)
	{
		int x=nodes[nodeNumber1.at(i)].getX();
		int y=nodes[nodeNumber1.at(i)].getY();
		double centerDistance=sqrt(pow((x-averageX1),2.0)+pow((y-averageY1),2.0));
		double currentEnergy=nodes[nodeNumber1.at(i)].getEnergy();
		nodeCenterdistance1[i]=1/centerDistance+((currentEnergy+HELP)/(ENERGY_INITIAL+HELP));
	}
	vector<PAIR_M> nodes_centerdistance_vec1(nodeCenterdistance1.begin(),nodeCenterdistance1.end());
	sort(nodes_centerdistance_vec1.begin(),nodes_centerdistance_vec1.end(),cmp_M);     //将map按value进行从大到小排序

	WSNnode cHeadNode1=nodes[nodeNumber1.at(nodes_centerdistance_vec1.at(0).first)];
	double innerDistance1=0.0;
	int cHeadX1=cHeadNode1.getX();
	int cHeadY1=cHeadNode1.getY();
	for(int i=0;i<nodeNumber1.size();i++)
	{
		int x=nodes[nodeNumber1.at(i)].getX();
		int y=nodes[nodeNumber1.at(i)].getY();
		innerDistance1+=sqrt(pow((x-cHeadX1),2.0)+pow((y-cHeadY1),2.0));
	}

	double outerDistance1=sqrt(pow((SINK_X-cHeadX1),2.0)+pow((SINK_Y-cHeadY1),2.0));
	double part1Consum=innerDistance1+outerDistance1;

	double partedConsum=part0Consum+part1Consum;

	//计算分开前的消耗
	CHypergraph* before_tempGraph=p;
	map<int,int>::iterator before_iterForMap;
	vector<int> nodeNumber;
	vector<int> tempNodeNumber;
	while(before_tempGraph->fatherGraph != NULL)
	{
		if(nodeNumber.size()==0)
		{
			for(int i=0;i<before_tempGraph->nvtxs;i++)
			{
				before_iterForMap=before_tempGraph->mapForFather.find(i);
				nodeNumber.push_back(before_iterForMap->second);
				
			}
			before_tempGraph=before_tempGraph->fatherGraph;
		}
		else{
			for(int j=0;j<nodeNumber.size();j++)
			{
				before_iterForMap=before_tempGraph->mapForFather.find(nodeNumber.at(j));
				tempNodeNumber.push_back(before_iterForMap->second);
			}
			nodeNumber.clear();
			nodeNumber=tempNodeNumber;
			tempNodeNumber.clear();
			before_tempGraph=before_tempGraph->fatherGraph;
		}
	}

	totalX=0;
	totalY=0;
	for(int k=0;k<nodeNumber.size();k++)
	{
		int X=nodes[nodeNumber.at(k)].getX();
		int Y=nodes[nodeNumber.at(k)].getY();
		totalX+=X;
		totalY+=Y;
	}
	int averageX=(int)totalX/(nodeNumber.size());
	int averageY=(int)totalY/(nodeNumber.size());
	map<int,double> nodeCenterdistance;
	for(int i=0;i<nodeNumber.size();i++)
	{
		int x=nodes[nodeNumber.at(i)].getX();
		int y=nodes[nodeNumber.at(i)].getY();
		double centerDistance=sqrt(pow((x-averageX),2.0)+pow((y-averageY),2.0));
		double currentEnergy=nodes[nodeNumber.at(i)].getEnergy();
		nodeCenterdistance[i]=1/centerDistance+((currentEnergy+HELP)/(ENERGY_INITIAL+HELP));
	}
	vector<PAIR_M> nodes_centerdistance_vec(nodeCenterdistance.begin(),nodeCenterdistance.end());
	sort(nodes_centerdistance_vec.begin(),nodes_centerdistance_vec.end(),cmp_M);     //将map按value进行从大到小排序

	WSNnode cHeadNode=nodes[nodeNumber.at(nodes_centerdistance_vec.at(0).first)];
	double innerDistance=0.0;
	int cHeadX=cHeadNode.getX();
	int cHeadY=cHeadNode.getY();
	for(int i=0;i<nodeNumber.size();i++)
	{
		int x=nodes[nodeNumber.at(i)].getX();
		int y=nodes[nodeNumber.at(i)].getY();
		innerDistance+=sqrt(pow((x-cHeadX),2.0)+pow((y-cHeadY),2.0));
	}

	double outerDistance=sqrt(pow((SINK_X-cHeadX),2.0)+pow((SINK_Y-cHeadY),2.0));
	double unpartConsum=innerDistance+outerDistance;

	if(partedConsum>unpartConsum)
		p->is_parted = false;
	}

	printf("modularity:%f\n",modularity);
}
CMygraph* CMultilevel::getMyGraph()
{
	return pInitGraph;
}
CMygraph* CMultilevel::coarseningGraph(CMygraph *initGraph)
{
	CMygraph *myCoarseningGraph = new CMygraph();
	myCoarseningGraph->fathergraph = initGraph;

	MyHashStruct *pMyHash = (MyHashStruct*)malloc(sizeof(struct MyHashStruct)*(initGraph->maxHweight+1));
	
	for(int i=0;i<=initGraph->maxHweight;i++)
	{
		pMyHash[i].next = NULL;
		pMyHash[i].truePos = -1;
	}
	for(int i=0;i<initGraph->nhedges;i++)
	{		
		MyHashStruct *newone = (MyHashStruct*)malloc(sizeof(struct MyHashStruct));
		newone->truePos = i;
		newone->next = pMyHash[initGraph->pAllEdges[i].Hweight].next;
		pMyHash[initGraph->pAllEdges[i].Hweight].next = newone;
	}

	
	myCoarseningGraph->pMyHashForVert = (MyHashStruct*)malloc(sizeof(struct MyHashStruct)*(initGraph->nvetes+1));

	for(int i=0;i<=initGraph->nvetes;i++)
	{
		myCoarseningGraph->pMyHashForVert[i].next = NULL;
		myCoarseningGraph->pMyHashForVert[i].truePos = -1;
	}

	int vertCount = 0;
	queue<int> newedges_queue;
	int maxweight = 1;
	for(int i=initGraph->maxHweight;i>=0;i--)
	{
		MyHashStruct *p = pMyHash[i].next;
		while(p!=NULL)
		{
			ArcNodeV *myvert = initGraph->pAllEdges[p->truePos].firstarc;
			bool is_allalone = true;
			while(myvert!=NULL)
			{
				map<int,int>::iterator iter;
				iter = myCoarseningGraph->keyTokey.find(myvert->node);
				if(iter!=myCoarseningGraph->keyTokey.end())
				{
					is_allalone = false;
					newedges_queue.push(p->truePos);
					maxweight = maxweight>initGraph->pAllEdges[p->truePos].Hweight?maxweight:initGraph->pAllEdges[p->truePos].Hweight;
					break;
				}
				myvert = myvert->nextarc;
			}
			myvert = initGraph->pAllEdges[p->truePos].firstarc;
			while(myvert!=NULL&&is_allalone)
			{
				MyHashStruct *newone = (MyHashStruct*)malloc(sizeof(struct MyHashStruct));
				newone->truePos = myvert->node;
				newone->next = myCoarseningGraph->pMyHashForVert[vertCount].next;
				myCoarseningGraph->pMyHashForVert[vertCount].next = newone;	
				myCoarseningGraph->keyTokey.insert(pair<int,int>(myvert->node,vertCount));
				myvert = myvert->nextarc;
			}
			if(is_allalone)
				vertCount ++;
			p=p->next;
		}
		
	}

	queue<int> edges_queue;
	while(!newedges_queue.empty())
	{
		int edgenumber = newedges_queue.front();
		edges_queue.push(edgenumber);
		ArcNodeV *myvert = initGraph->pAllEdges[edgenumber].firstarc;
		bool allvertexist=true;
		while(myvert!=NULL)
		{
			map<int,int>::iterator iter;
			iter = myCoarseningGraph->keyTokey.find(myvert->node);
			if(iter==myCoarseningGraph->keyTokey.end())
			{
				myCoarseningGraph->keyTokey.insert(pair<int,int>(myvert->node,vertCount));

				MyHashStruct *newone = (MyHashStruct*)malloc(sizeof(struct MyHashStruct));
				newone->truePos = myvert->node;
				newone->next = myCoarseningGraph->pMyHashForVert[vertCount].next;
				myCoarseningGraph->pMyHashForVert[vertCount].next = newone;
				allvertexist = false;
			}
			myvert=myvert->nextarc;
		}
		if(!allvertexist)
			vertCount ++;
		newedges_queue.pop();
	}

	//还有好多要修改.....
	for(int i=0;i<initGraph->nvetes;i++)
	{
		map<int,int>::iterator iter;
		iter = myCoarseningGraph->keyTokey.find(i);
		if(iter==myCoarseningGraph->keyTokey.end())
		{
			MyHashStruct *newone = (MyHashStruct*)malloc(sizeof(struct MyHashStruct));
			newone->truePos = i;
			newone->next = myCoarseningGraph->pMyHashForVert[vertCount].next;
			myCoarseningGraph->pMyHashForVert[vertCount].next = newone;
			myCoarseningGraph->keyTokey.insert(pair<int,int>(i,vertCount));
			vertCount ++;
		}
	}

	myCoarseningGraph->pAllVertices = (Vnode*)malloc(sizeof(struct Vnode)*vertCount);
	myCoarseningGraph->nvetes = vertCount;
	myCoarseningGraph->pAllEdges = (HEnode*)malloc(sizeof(struct HEnode)*edges_queue.size());
	myCoarseningGraph->nhedges = int(edges_queue.size());
	myCoarseningGraph->maxHweight = maxweight;

	for(int i=0;i<vertCount;i++)
	{
		myCoarseningGraph->pAllVertices[i].firstarc =NULL;
		myCoarseningGraph->pAllVertices[i].node = i;
	}

	for(int i=0;i<int(edges_queue.size());i++)
	{
		myCoarseningGraph->pAllEdges[i].firstarc =NULL;
		myCoarseningGraph->pAllEdges[i].Enode = i;
	}

	int hedgenum = 0;
	
	while(!edges_queue.empty())
	{
		int edgenum = edges_queue.front();
		ArcNodeV *myvert = initGraph->pAllEdges[edgenum].firstarc;
		map<int,bool> NodeExist;
		while(myvert!=NULL)
		{
			map<int,int>::iterator iterFind;
			iterFind = myCoarseningGraph->keyTokey.find(myvert->node);
			if(iterFind!=myCoarseningGraph->keyTokey.end())
			{
				map<int,bool>::iterator exisetiter;
				exisetiter = NodeExist.find(iterFind->second);
				if(exisetiter==NodeExist.end())
				{
					ArcNodeV *newvert = (ArcNodeV*)malloc(sizeof(struct ArcNodeV));
					newvert->node = iterFind->second;
					newvert->nextarc = myCoarseningGraph->pAllEdges[hedgenum].firstarc;
					myCoarseningGraph->pAllEdges[hedgenum].firstarc = newvert;
					myCoarseningGraph->pAllEdges[hedgenum].Hweight = initGraph->pAllEdges[edgenum].Hweight;
					ArcNodeH *newedge = (ArcNodeH*)malloc(sizeof(struct ArcNodeH));
					newedge->hedge = hedgenum;
					newedge->nextarc = myCoarseningGraph->pAllVertices[iterFind->second].firstarc;
					myCoarseningGraph->pAllVertices[iterFind->second].firstarc = newedge;
					
					NodeExist.insert(pair<int,bool>(iterFind->second,true));

				}
			}
			myvert = myvert->nextarc;
		}
		edges_queue.pop();
		hedgenum ++;

	}

	return myCoarseningGraph;
}

void CMultilevel::subpartition(CHypergraph *p, int seed,int *part_res)
{
	queue <int> onepart;
	map<int,bool> vertExist;
	onepart.push(seed);
	vertExist.insert(pair<int,bool>(seed,true));
	part_res[seed] = 0;
	int vertices_num = 1;
	while(!onepart.empty()&&vertices_num<p->nvtxs/2)
	{
		int nowVert = onepart.front();
		ArcNodeH *myedge = pInitGraph->pAllVertices[nowVert].firstarc;
		while(myedge!=NULL)
		{
			ArcNodeV *myvert = pInitGraph->pAllEdges[myedge->hedge].firstarc;
			while(myvert!=NULL)
			{
				map<int,bool>::iterator iterExist;
				iterExist = vertExist.find(myvert->node);
				if(iterExist==vertExist.end())
				{
					onepart.push(myvert->node);
					part_res[myvert->node] = 0;
					vertices_num ++;
					vertExist.insert(pair<int,bool>(myvert->node,true));
					if(vertices_num>=p->nvtxs/2)
						break;
				}
				myvert = myvert->nextarc;
			}
			if(vertices_num>=p->nvtxs/2)
				break;
			myedge = myedge->nextarc;
		}
		onepart.pop();
		int anotherSeed = seed;
		if(onepart.empty()&&vertices_num<p->nvtxs/2)
		{
			map<int,bool>::iterator iterExist;
			anotherSeed = (anotherSeed+1)%p->nvtxs;
			iterExist = vertExist.find(anotherSeed);
			while(iterExist!=vertExist.end())
			{
				anotherSeed = (anotherSeed+1)%p->nvtxs;
				iterExist = vertExist.find(anotherSeed);
			}
			vertExist.insert(pair<int,bool>(anotherSeed,true));
			part_res[anotherSeed] = 0;
			onepart.push(anotherSeed);
			vertices_num ++;
		}
	}
	for(int i=0;i<p->nvtxs;i++)
	{
		if(part_res[i]!=0)
			part_res[i]=1;
	}
}

vector<int> CMultilevel::FindCutNodes(vector<int> cutedges)
{
	map<int,bool> ExitInList;
	vector<int> cutNodes;
	for(int i=0;i<int(cutedges.size());i++)
	{
		int edge = cutedges.at(i);
		ArcNodeV *myvert = pInitGraph->pAllEdges[edge].firstarc;
		while(myvert!=NULL)
		{
			map<int,bool>::iterator iter;
			iter = ExitInList.find(myvert->node);
			if(iter==ExitInList.end())
			{
				cutNodes.push_back(myvert->node);
				ExitInList.insert(pair<int,bool>(myvert->node,true));
			}
			myvert=myvert->nextarc;
		}
	}
	return cutNodes;
}

vector<int> CMultilevel::FindCutEdges(int *part_res)
{
	vector<int> cutedges;
	for(int i=0;i<pInitGraph->nhedges;i++)
	{
		ArcNodeV *myvert = pInitGraph->pAllEdges[i].firstarc;
		int part_num = part_res[myvert->node];
		while(myvert!=NULL)
		{
			if(part_res[myvert->node]!=part_num)
			{
				cutedges.push_back(i);
				break;
			}
			myvert = myvert->nextarc;
		}
	}

	return cutedges;
}

double CMultilevel::OptimalSteps(vector<int> cutNodes,int *part_res,int *Node_Rank)
{
	int fixnum=0;
	map<int,bool> fixed;
	while(fixnum<int(cutNodes.size()))
	{
		double addmodu = 0.0;
		int fix = -1;
		double modularity = ComputeModularity(part_res);
		for(int i=0;i<int(cutNodes.size());i++)
		{
			int node = cutNodes.at(i);
			map<int,bool>::iterator iter;
			iter = fixed.find(node);
			if(iter!=fixed.end())
			{
				continue;
			}
			double cur_modularity = 0.0;
			
			if(part_res[node]==0)
			{
				part_res[node] = 1;
				cur_modularity = ComputeModularity(part_res);
				part_res[node] = 0;
			}
			else if(part_res[node]==1)
			{
				part_res[node] = 0;
				cur_modularity = ComputeModularity(part_res);
				part_res[node] = 1;
			}
			if(cur_modularity -  modularity>addmodu)
			{
				fix = node;
				addmodu = cur_modularity -  modularity;
			}
			if(cur_modularity -  modularity==addmodu)
			{
				if(Node_Rank[node]<Node_Rank[fix])
				{
					fix = node;
					addmodu = cur_modularity -  modularity;
				}
			}
		}

		if(fix==-1)
			return modularity;

		part_res[fix] = part_res[fix]==1?0:1;
		fixed.insert(pair<int,bool>(fix,true));
		fixnum++;

	}

	return ComputeModularity(part_res);
	
}

void CMultilevel::FindZeroNodeCount(vector<int> cutEdges,int *PartZero,int *part_res,Vnode *pCutNodes)  //partzero[i] 代表超边i在的part0的顶点数
{
	for(int i=0;i<int(cutEdges.size());i++)
	{
		int edge = cutEdges.at(i);
		ArcNodeV *myvert = pInitGraph->pAllEdges[edge].firstarc;
		while(myvert!=NULL)
		{
			if(part_res[myvert->node]==0)
				PartZero[edge]++;

			
			ArcNodeH *p = (ArcNodeH*)malloc(sizeof(struct ArcNodeH));
			p->hedge = edge;
			p->nextarc = pCutNodes[myvert->node].firstarc;
			pCutNodes[myvert->node].firstarc = p;
			pCutNodes[myvert->node].nodeNum ++;
			myvert=myvert->nextarc;
		}
	}
}
double CMultilevel::myFM(int *part_res)
{

	vector<int> cutEdges = FindCutEdges(part_res);
	vector<int> cutNodes = FindCutNodes(cutEdges);
	

	int *Node_Rank = new int[pInitGraph->nvetes];
	memset(Node_Rank,-1,pInitGraph->nvetes);
	NodeRank(cutNodes,cutEdges,Node_Rank);

	Vnode *pCutNodes = (Vnode*)malloc(sizeof(struct Vnode)*(pInitGraph->nvetes));
	for(int i=0;i<pInitGraph->nvetes;i++)
	{
		pCutNodes[i].firstarc = NULL;
		pCutNodes[i].node = i;
		pCutNodes[i].nodeNum = 0;
		pCutNodes[i].Vweight = pInitGraph->pAllVertices[i].Vweight;
	}
	int *PartZeroNodeCount = new int[pInitGraph->nhedges];
	for(int i=0;i<pInitGraph->nhedges;i++)
	{
		PartZeroNodeCount[i]=0;
	}
	//memset(PartZeroNodeCount,0,pInitGraph->nhedges);
	FindZeroNodeCount(cutEdges,PartZeroNodeCount,part_res,pCutNodes);

	//如果算梯度方向，是不是可以考虑按照梯度方向增大最大的方向移动，就是假设当次移动确定，考虑下次移动，如果下次移动的增加值比当次移动的增加值大，则考虑当次移动。

	/*一直下坡，直到无坡可下   好像只能做到2^n的复杂度

	
	*/

	///* 每步都选择最优
	//return OptimalSteps(cutNodes,part_res,Node_Rank);
	

	return HillClimbing(part_res,cutNodes,cutEdges,Node_Rank,PartZeroNodeCount,pCutNodes);
	/*
	return HillClimbing(part_res,cutNodes,cutEdges);
	
	return ComputeModularity(part_res);
	*/
}
void CMultilevel::NodeRank(std::vector<int> cutNodes, std::vector<int> cutEdges, int *Node_Rank)
{
	int max = 0;
	for(int i=0;i<int(cutEdges.size());i++)
	{
		max = pInitGraph->pAllEdges[cutEdges.at(i)].nodeNum>max?pInitGraph->pAllEdges[cutEdges.at(i)].nodeNum:max;
	}

	mymap *NodeMap = (mymap*)malloc(sizeof(struct mymap)*(max+1));

	for(int i=0;i<=max;i++)
	{
		NodeMap[i].next = NULL;
	}

	for(int i=0;i<int(cutEdges.size());i++)
	{
		mymap *newone = (mymap*)malloc(sizeof(struct mymap));
		newone->node = cutEdges.at(i);
		newone->next =NodeMap[pInitGraph->pAllEdges[newone->node].nodeNum].next;
		
		NodeMap[pInitGraph->pAllEdges[newone->node].nodeNum].next = newone;
	}
	int RankValue = 0;
	int step = 0;
	map<int,bool> ExitInList;
	while(step<int(cutNodes.size()))
	{
		for(int i=max;i>=0;i--)
		{
			if(NodeMap[i].next == NULL)
				continue;
			mymap *pickone = NodeMap[i].next;
			while(pickone!=NULL)
			{
				ArcNodeV *mynode = pInitGraph->pAllEdges[pickone->node].firstarc;
				while(mynode!=NULL)
				{
					map<int,bool>::iterator iter;
					iter = ExitInList.find(mynode->node);
					if(iter==ExitInList.end())
					{
						ExitInList.insert(pair<int,bool>(mynode->node,true));
						Node_Rank[mynode->node] = RankValue;
						RankValue++;
						step++;
					}
					if(step>=int(cutNodes.size()))
						break;
					mynode = mynode->nextarc;
				}
				if(step>=int(cutNodes.size()))
					break;
				pickone = pickone->next;
			}
			if(step>=int(cutNodes.size()))
				break;
		}
	}

}
double CMultilevel::HillClimbing(int *part_res,vector<int> cutNodes,vector<int> cutEdges,int *Node_Rank,int *PartZeroNodeCount,Vnode *pCutNodes) //Node_Rank 值越小，越优先
{
	int fixnum=0;
	map<int,bool> fixed;
	int iDistance = 20;
	stack<Stone*> Hill;
	Stone *firstStone = (Stone*)malloc(sizeof(struct Stone));
	firstStone->curModu = ComputeModularity(part_res);
	firstStone->node = -1;
	firstStone->isChanged = false;
	Hill.push(firstStone);
	double maxModu=firstStone->curModu;

	//准备用堆排序
	
	int HeapLen = int(cutNodes.size());
	ModStruct *pModularityList = (ModStruct*)malloc(sizeof(struct ModStruct)*cutNodes.size());

	for(int i=0;i<int(cutNodes.size());i++)
	{
		int node = cutNodes.at(i);
		double cur_modularity = 0.0;
			
		if(part_res[node]==0)
		{
			part_res[node] = 1;
			cur_modularity = ComputeModularity(part_res);
			part_res[node] = 0;
		}
		else if(part_res[node]==1)
		{
			part_res[node] = 0;
			cur_modularity = ComputeModularity(part_res);
			part_res[node] = 1;
		}
		pModularityList[i].dMod = cur_modularity;
		pModularityList[i].node = cutNodes.at(i);
	}
	CHeapSort *mySort = new CHeapSort();
	while(fixnum<int(cutNodes.size())&&fixnum<iDistance)
	{
		mySort->getMax(pModularityList,HeapLen,Node_Rank);
		mySort->renew = false;
		fixnum++;
		HeapLen--;
		if(HeapLen==0)
			break;
		bool renew = false;
		ArcNodeH *hereEdge = pCutNodes[pModularityList[HeapLen].node].firstarc;
		while(hereEdge!=NULL)
		{
			if(part_res[pModularityList[HeapLen].node]==0)
			{
				if(PartZeroNodeCount[hereEdge->hedge]==pInitGraph->pAllEdges[hereEdge->hedge].nodeNum)
					renew = true;
				PartZeroNodeCount[hereEdge->hedge]--;
				if(PartZeroNodeCount[hereEdge->hedge]==0)
					renew = true;
			}
			else if(part_res[pModularityList[HeapLen].node]==1)
			{
				if(part_res[pModularityList[HeapLen].node]==0)
					renew = true;
				PartZeroNodeCount[hereEdge->hedge]++;
				if(PartZeroNodeCount[hereEdge->hedge]==pInitGraph->pAllEdges[hereEdge->hedge].nodeNum)
					renew = true;
			}
			hereEdge=hereEdge->nextarc;
		}

		part_res[pModularityList[HeapLen].node] = part_res[pModularityList[HeapLen].node]==1?0:1;
		Stone *oneStone = (Stone*)malloc(sizeof(struct Stone));
		oneStone->curModu = pModularityList[HeapLen].dMod;
		oneStone->node = pModularityList[HeapLen].node;
		oneStone->isChanged = true;
		Hill.push(oneStone);
		maxModu = oneStone->curModu>maxModu?oneStone->curModu:maxModu;
		if(renew)
		{
			for(int i=0;i<HeapLen;i++)
			{
				double cur_modularity = 0.0;
				int node = pModularityList[i].node;
				if(part_res[node]==0)
				{
					part_res[node] = 1;
					cur_modularity = ComputeModularity(part_res);
					part_res[node] = 0;
				}
				else if(part_res[node]==1)
				{
					part_res[node] = 0;
					cur_modularity = ComputeModularity(part_res);
					part_res[node] = 1;
				}
				pModularityList[i].dMod = cur_modularity;
			}
			mySort->renew = true;
		}
	}
    //到这里为止
	
	
	/*

	while(fixnum<cutNodes.size()&&fixnum<iDistance)
	{
		double modu = -10000.0;
		int fix = -1;
		for(int i=0;i<cutNodes.size();i++)
		{
			int node = cutNodes.at(i);
			map<int,bool>::iterator iter;
			iter = fixed.find(node);
			if(iter!=fixed.end())
			{
				continue;
			}
			double cur_modularity = 0.0;
			
			if(part_res[node]==0)
			{
				part_res[node] = 1;
				cur_modularity = ComputeModularity(part_res);
				part_res[node] = 0;
			}
			else if(part_res[node]==1)
			{
				part_res[node] = 0;
				cur_modularity = ComputeModularity(part_res);
				part_res[node] = 1;
			}

			if(cur_modularity==modu)
			{
				if(Node_Rank[node]<Node_Rank[fix])
				{
					fix = node;
					modu = cur_modularity;
				}
			}
			if(cur_modularity>modu)
			{
				fix = node;
				modu = cur_modularity;
			}
			
		}
		if(fix==-1)
			break;

		part_res[fix] = part_res[fix]==1?0:1;
		fixed.insert(pair<int,bool>(fix,true));
		fixnum++;

		Stone *oneStone = (Stone*)malloc(sizeof(Stone));
		oneStone->curModu = modu;
		oneStone->node = fix;
		oneStone->isChanged = true;
		Hill.push(oneStone);
		maxModu = oneStone->curModu>maxModu?oneStone->curModu:maxModu;

	}
	*/
	
	while(!Hill.empty())
	{
		Stone *oneStone = Hill.top();
		Hill.pop();
		if(oneStone->curModu==maxModu)
			break;

		if(oneStone->isChanged)
		{
			part_res[oneStone->node] = part_res[oneStone->node]==0?1:0;
		}
		
	}
	
	return maxModu;
}

void CMultilevel::HillClimbingBigEdgeFirst(int *part_res,vector<int> cutNodes,vector<int> cutEdges)
{
	// 大边优先

	int iDistance = 20;

	int max = 0;
	for(int i=0;i<int(cutEdges.size());i++)
	{
		max = pInitGraph->pAllEdges[cutEdges.at(i)].nodeNum>max?pInitGraph->pAllEdges[cutEdges.at(i)].nodeNum:max;
	}

	mymap *NodeMap = (mymap*)malloc(sizeof(struct mymap)*(max+1));

	for(int i=0;i<=max;i++)
	{
		NodeMap[i].next = NULL;
	}

	for(int i=0;i<int(cutEdges.size());i++)
	{
		mymap *newone = (mymap*)malloc(sizeof(struct mymap));
		newone->node = cutEdges.at(i);
		newone->next =NodeMap[pInitGraph->pAllEdges[newone->node].nodeNum].next;
		
		NodeMap[pInitGraph->pAllEdges[newone->node].nodeNum].next = newone;
	}
	
	queue<int> NodesList;
	int step = 0;
	map<int,bool> ExitInList;
	while(step<iDistance&&step<int(cutNodes.size()))
	{
		for(int i=max;i>=0;i--)
		{
			if(NodeMap[i].next == NULL)
				continue;
			mymap *pickone = NodeMap[i].next;
			while(pickone!=NULL)
			{
				ArcNodeV *mynode = pInitGraph->pAllEdges[pickone->node].firstarc;
				while(mynode!=NULL)
				{
					map<int,bool>::iterator iter;
					iter = ExitInList.find(mynode->node);
					if(iter==ExitInList.end())
					{
						ExitInList.insert(pair<int,bool>(mynode->node,true));
						NodesList.push(mynode->node);
						step++;
					}
					if(step>=iDistance||step>=int(cutNodes.size()))
						break;
					mynode = mynode->nextarc;
				}
				if(step>=iDistance||step>=int(cutNodes.size()))
					break;
				pickone = pickone->next;
			}
			if(step>=iDistance||step>=int(cutNodes.size()))
				break;
		}
	}

	stack<Stone*> Hill;
	Stone *firstStone = (Stone*)malloc(sizeof(struct Stone));
	firstStone->curModu = ComputeModularity(part_res);
	firstStone->node = -1;
	firstStone->isChanged = false;
	Hill.push(firstStone);
	double maxModu = firstStone->curModu;
	while(!NodesList.empty())
	{
		int nownode = NodesList.front();
		NodesList.pop();
		Stone *oneStone = (Stone*)malloc(sizeof(struct Stone));
		part_res[nownode] = part_res[nownode]==0?1:0;
		oneStone->curModu = ComputeModularity(part_res);
		oneStone->node = nownode;
		oneStone->isChanged = true;
		maxModu = oneStone->curModu>maxModu?oneStone->curModu:maxModu;
		Hill.push(oneStone);
	}
	
	while(!Hill.empty())
	{
		Stone *oneStone = Hill.top();
		Hill.pop();
		if(oneStone->isChanged)
		{
			part_res[oneStone->node] = part_res[oneStone->node]==0?1:0;
		}
		if(oneStone->curModu==maxModu)
			break;
	}
	
}
double CMultilevel::ComputeModularity(int *part_res)
{
	double innerEdgeValue[2];
	int innerVertCount[2];
	double cutEdgeValue = 0.0;
	int cutVertCount = 0;
	for(int i=0;i<2;i++)
	{
		innerVertCount[i]=0;
		innerEdgeValue[i] = 0.0;
	}

	map<int,bool> mapForInnerNodeCount;
	map<int,bool> mapForCutNodeCount;
	for(int i=0;i<pInitGraph->nhedges;i++)
	{
		bool cutedge = false;
		ArcNodeV *myvert = pInitGraph->pAllEdges[i].firstarc;
		int part_num = part_res[myvert->node];
		while(myvert!=NULL)
		{
			if(part_res[myvert->node]!=part_num)
			{
				cutedge = true;
				break;
			}
			myvert = myvert->nextarc;
		}
		if(cutedge==false)
		{
			if(pInitGraph->pAllEdges[i].nodeNum!=0)
				innerEdgeValue[part_num] += double(pInitGraph->pAllEdges[i].Hweight) * (1.0-double(1.0/pInitGraph->pAllEdges[i].nodeNum));
			ArcNodeV *pNode = pInitGraph->pAllEdges[i].firstarc;
			while(pNode!=NULL)
			{
				map<int,bool>::iterator myiter;
				myiter  = mapForInnerNodeCount.find(pNode->node);
				if(myiter==mapForInnerNodeCount.end())
				{
					mapForInnerNodeCount.insert(pair<int,bool>(pNode->node,true));
					innerVertCount[part_num] += 1;//pInitGraph->pAllVertices[pNode->node].Vweight;
				}
				pNode = pNode->nextarc;
			}
			//innerVertCount[part_num] += pInitGraph->pAllEdges[i].nodeNum;
		}
		else
		{
			cutEdgeValue += double(pInitGraph->pAllEdges[i].Hweight);
			ArcNodeV *pNode = pInitGraph->pAllEdges[i].firstarc;
			while(pNode!=NULL)
			{
				map<int,bool>::iterator myiter;
				myiter  = mapForCutNodeCount.find(pNode->node);
				if(myiter==mapForCutNodeCount.end())
				{
					mapForCutNodeCount.insert(pair<int,bool>(pNode->node,true));
					cutVertCount ++;
				}
				pNode = pNode->nextarc;
			}
		}
	}
	double modularity = -1000.0;
	double averInner=0.0;
	double standardInner=0.0;
	if(innerVertCount[0]!=0&&innerVertCount[1]!=0)
		//averInner=((innerEdgeValue[0]/innerVertCount[0]) + (innerEdgeValue[1]/innerVertCount[1]))/2;
		//standardInner=(power((innerEdgeValue[0]/innerVertCount[0]-averInner),2.0)+power((innerEdgeValue[1]/innerVertCount[1]-averInner),2.0))/2;
		//modularity = 2.0*((innerEdgeValue[0]/innerVertCount[0]) + (innerEdgeValue[1]/innerVertCount[1])) - (cutEdgeValue/innerVertCount[0]) - (cutEdgeValue/innerVertCount[1])-standardInner;
		modularity = 2.0*((innerEdgeValue[0]/innerVertCount[0]) + (innerEdgeValue[1]/innerVertCount[1])) - (cutEdgeValue/innerVertCount[0]) - (cutEdgeValue/innerVertCount[1]); 

	/*

	for(int i=0;i<pInitGraph->nhedges;i++)
	{
		bool cutedge = false;
		ArcNodeV *myvert = pInitGraph->pAllEdges[i].firstarc;
		int part_num = part_res[myvert->node];
		while(myvert!=NULL)
		{
			if(part_res[myvert->node]!=part_num)
			{
				cutedge = true;
				break;
			}
			myvert = myvert->nextarc;
		}
		if(cutedge==false)
		{
			innerEdgeValue[part_num] += double(pInitGraph->pAllEdges[i].Hweight) * (1.0-double(1.0/pInitGraph->pAllEdges[i].nodeNum));
			innerVertCount[part_num] += pInitGraph->pAllEdges[i].nodeNum;
		}
		else
		{
			cutEdgeValue += double(pInitGraph->pAllEdges[i].Hweight);
			cutVertCount += pInitGraph->pAllEdges[i].nodeNum;
		}
	}

	double modularity = 2.0*((innerEdgeValue[0]/innerVertCount[0]) + (innerEdgeValue[1]/innerVertCount[1])) - (cutEdgeValue/innerVertCount[0]) - (cutEdgeValue/innerVertCount[1]); 
	*/

    return modularity;
 }
