#pragma once
#ifndef _WSNNODE_H
#define _WSNNODE_H

#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

class WSNnode
{
public:
	WSNnode();
	WSNnode(int hedge_weights);	                    //用来存储超边的权值
	~WSNnode();

	void setX(int s);                //X坐标
	void setY(int s);                //Y坐标
	void setTransDistance(double distance);          //传输距离
	void setWeights(double e_current,double e_initial,int neight_num);                  //节点权值
	void setHedgeWeights(double averDistance,int AREA_X,double energy,double iniEnergy);         //超边权值，存储在第一个节点中
	void setHedge(int hedgeNumber);    //设置此节点属于哪些超边 
	void setHgraphNode(int i);                    //节点序号
	void setCoarsHraphNode(int i);                //设置模块生成的子图中的节点序号
	void setFlagDistance(double distance);                    //距离最近同属一条超边节点的距离
	void setNeighborNum(int num);                  //节点的邻居节点数目
	void setEnergy(double e);                      //节点的剩余能量
	void setHedgeNumber(int num);
	void setIsChecked(bool flag);
	void setOldModule(int module);
	void setNewModule(int module);
	void setModuleHedges(vector<WSNnode> hedge);    //此点所属于的超边中属于此点所在模块的超边们
	void setIsClusterHead(bool flag);


	int getX(); 
	int getY();
	int getTransDistance();
	int getWeights();
	int getHedgeWeights();
	vector<int> getBelongToHedge();
	int getHgraphNode();
	int getCoarsHgraphNode();
	double getFlagDistance();
	int getNeighborNum();
	double getEnergy();
	int getHedgeNumber();
	int getHNodeMap(int coarsNode);    //由被粗化图中的序号得到原始图中的序号
	bool getIsChecked();
	int getOldModule();              //得到去孤之前属于的模块号
	int getNewModule();              //得到去孤之后属于的模块号
	vector<vector<WSNnode>> getModuleHedges();
	bool getIsClusterHead();

private:
	int x;
	int y;
	int trans_distance;
	double ver_weights;	//节点的权值，就是节点的剩余能量，在0到2之间随意分配
	double hedge_weights;   //超边权值
	vector<int> belongToHedge;   //此节点属于哪些超边
	int hgraph_node;    //在超图文件中的表示:节点序号
	int hedge_number;	 //超边序号
	int coarshgraph_node;  //在模块中子图的节点序号
	double flag_distance;   //形成超边时距离最近同属一条超边节点的距离
	int neighbor_num;   //邻居节点数目
	double energy;      //节点的剩余能量
	bool isChecked;     //此条超边是否被遍历过
	int oldModule;        //去孤之前这个节点属于哪一个模块
	int newModule;        //去孤之后这个节点属于哪一个模块
	vector<vector<WSNnode>> moduleHedges;
	bool isClusterHead;

};
	

#endif _WSNNODE_H