#include <string>
#include <map>
#include <vector>
using namespace std;

class CClassContent
{
public:
	string defbody;
	/*  在.h内寻找成员变量，先注释掉
		map<string,string> valueTotype; //保存成员变量和类型
	*/
	string fatherclass;  //以空格分割，每个空格后都是一个父类
	CClassContent();
};

class CReadH
{
public:
	string Hname;
	string globalStr;
	vector<string> headfile;
	int classposition;
	//map <string,string> classdef;
	map <string,CClassContent*> classdef;
	CReadH(string name);
	int getclass(string allContent);
	string readallfile(string name);
	string getclassname(int pos,string content);
	string getclassbody(int pos,string content);
	string getfatherclassname(int pos,string content);
	string findGlobal(string content);
	void getheadfile(int classstart,string content);
	map<string,string> findValue(string content);
};