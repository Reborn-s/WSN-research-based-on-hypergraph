#include <string>
#include <map>
using namespace std;
class CReadH
{
public:
	string Hname;
	map <string,string> classdef;
	CReadH(string name);
	int getclass();
	string readallfile(string name);
	string getclassname(int pos,string content);
	string getclassbody(int pos,string content);
};