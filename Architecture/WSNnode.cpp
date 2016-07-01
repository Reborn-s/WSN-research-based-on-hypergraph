#include "WSNnode.h"

#define HELP 5

WSNnode::WSNnode()
{
	x = 0;
	y = 0;
	trans_distance = 0.0;
	ver_weights = 0.0;
	hedge_weights=0.0;
	hgraph_node = 0;
	coarshgraph_node=0;
	flag_distance=0.0;
	neighbor_num=0;
	energy=0.0;
	isChecked=false;
	oldModule=0;
	newModule=0;
	isClusterHead=false;
	
}

WSNnode::~WSNnode()
{
}

void WSNnode::setX(int s)
{
	x = s;
}

void WSNnode::setY(int s)
{
	y = s;
}

void WSNnode::setTransDistance(double distance)
{
	trans_distance = distance;
}

void WSNnode::setWeights(double e_current,double e_initial,int neighbor_num)
{
	ver_weights=((e_current+HELP)/(e_initial+HELP))*neighbor_num;
}

void WSNnode::setHedgeWeights(double averDistance,int AREA_X,double energy,double iniEnergy)    //待修改
{
	hedge_weights=AREA_X/averDistance*((energy+HELP)/(iniEnergy+HELP));
}

void WSNnode::setHedge(int hedge)
{
	belongToHedge.push_back(hedge);
}

void WSNnode::setHgraphNode(int i)      //.gr超图文件中的节点序号是从1开始算起的，写文件的时候进行更改,这里设置为0
{
	hgraph_node = i;
}

void WSNnode::setCoarsHraphNode(int i)
{
	coarshgraph_node=i;
}

void WSNnode::setFlagDistance(double distance)
{
	flag_distance=distance;
}

void WSNnode::setNeighborNum(int num)
{
	neighbor_num=num;
}

void WSNnode::setEnergy(double e)
{
	energy=e;
}

void WSNnode::setHedgeNumber(int num)
{
	hedge_number=num;
}

void WSNnode::setIsChecked(bool flag)
{
	  isChecked=flag;
}

void WSNnode::setOldModule(int m)
{
	oldModule=m;
}

void WSNnode::setNewModule(int m)
{
	newModule=m;
}

void WSNnode::setModuleHedges(vector<WSNnode> hedge)
{
	moduleHedges.push_back(hedge);
}

void WSNnode::setIsClusterHead(bool flag)
{
	isClusterHead=flag;
}



int WSNnode::getX()
{
	return x;
}

int WSNnode::getY()
{
	return y;
}

int WSNnode::getTransDistance()
{
	return trans_distance;
}


int WSNnode::getWeights()
{
	return (int)ver_weights;
}

int WSNnode::getHedgeWeights()
{
	return (int)hedge_weights;
}

vector<int> WSNnode::getBelongToHedge()
{
	return belongToHedge;
}

int WSNnode::getHgraphNode()
{
	return hgraph_node;
}

int WSNnode::getCoarsHgraphNode()
{
	return coarshgraph_node;
}

double WSNnode::getFlagDistance()
{
	return flag_distance;
}

int WSNnode::getNeighborNum()
{
	return neighbor_num;
}

double WSNnode::getEnergy()
{
	return energy;
}

int WSNnode::getHedgeNumber()
{
	return hedge_number;
}

int WSNnode::getHNodeMap(int coarsNode)
{
	return hgraph_node;
}

bool WSNnode::getIsChecked()
{
	return isChecked;
}

int WSNnode::getOldModule()
{
	return oldModule;
}

int WSNnode::getNewModule()
{
	return newModule;
}

vector<vector<WSNnode>> WSNnode::getModuleHedges()
{
	return moduleHedges;
}

bool WSNnode::getIsClusterHead()
{
	return isClusterHead;
}