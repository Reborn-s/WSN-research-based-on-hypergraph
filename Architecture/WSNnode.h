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
	WSNnode(int hedge_weights);	                    //�����洢���ߵ�Ȩֵ
	~WSNnode();

	void setX(int s);                //X����
	void setY(int s);                //Y����
	void setTransDistance(double distance);          //�������
	void setWeights(double e_current,double e_initial,int neight_num);                  //�ڵ�Ȩֵ
	void setHedgeWeights(double averDistance,int AREA_X,double energy,double iniEnergy);         //����Ȩֵ���洢�ڵ�һ���ڵ���
	void setHedge(int hedgeNumber);    //���ô˽ڵ�������Щ���� 
	void setHgraphNode(int i);                    //�ڵ����
	void setCoarsHraphNode(int i);                //����ģ�����ɵ���ͼ�еĽڵ����
	void setFlagDistance(double distance);                    //�������ͬ��һ�����߽ڵ�ľ���
	void setNeighborNum(int num);                  //�ڵ���ھӽڵ���Ŀ
	void setEnergy(double e);                      //�ڵ��ʣ������
	void setHedgeNumber(int num);
	void setIsChecked(bool flag);
	void setOldModule(int module);
	void setNewModule(int module);
	void setModuleHedges(vector<WSNnode> hedge);    //�˵������ڵĳ��������ڴ˵�����ģ��ĳ�����
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
	int getHNodeMap(int coarsNode);    //�ɱ��ֻ�ͼ�е���ŵõ�ԭʼͼ�е����
	bool getIsChecked();
	int getOldModule();              //�õ�ȥ��֮ǰ���ڵ�ģ���
	int getNewModule();              //�õ�ȥ��֮�����ڵ�ģ���
	vector<vector<WSNnode>> getModuleHedges();
	bool getIsClusterHead();

private:
	int x;
	int y;
	int trans_distance;
	double ver_weights;	//�ڵ��Ȩֵ�����ǽڵ��ʣ����������0��2֮���������
	double hedge_weights;   //����Ȩֵ
	vector<int> belongToHedge;   //�˽ڵ�������Щ����
	int hgraph_node;    //�ڳ�ͼ�ļ��еı�ʾ:�ڵ����
	int hedge_number;	 //�������
	int coarshgraph_node;  //��ģ������ͼ�Ľڵ����
	double flag_distance;   //�γɳ���ʱ�������ͬ��һ�����߽ڵ�ľ���
	int neighbor_num;   //�ھӽڵ���Ŀ
	double energy;      //�ڵ��ʣ������
	bool isChecked;     //���������Ƿ񱻱�����
	int oldModule;        //ȥ��֮ǰ����ڵ�������һ��ģ��
	int newModule;        //ȥ��֮������ڵ�������һ��ģ��
	vector<vector<WSNnode>> moduleHedges;
	bool isClusterHead;

};
	

#endif _WSNNODE_H