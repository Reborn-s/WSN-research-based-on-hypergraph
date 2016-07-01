#include "ReadH.h"
#include <stdio.h>
#include <stdlib.h>

CReadH::CReadH(string name)
{
	Hname = name;
	getclass();
}

int CReadH::getclass()
{
	string allContent = readallfile(Hname);
	int pos = allContent.find("\nclass ");      //先简单这样考虑，要改
	while(pos!=string::npos)
	{
		string classname =  getclassname(pos,allContent);
		string classbody  =  getclassbody(pos,allContent);
		classdef.insert(pair <string,string>(classname,classbody));
		pos = allContent.find("\nclass ",pos+strlen("\nclass "));
	}
	return 1;
}

string CReadH::getclassname(int pos, std::string content)
{
	string ans = "";
	int i = pos+strlen("\nclass");
	while(content.at(i)==' '||content.at(i)=='\t')
		i++;

	for(;(content.at(i)>=65&&content.at(i)<=90)||(content.at(i)>=97&&content.at(i)<=122)||(content.at(i)>=48&&content.at(i)<=57);i++)
	{
		ans += content.at(i);
	}
	
	return ans;
	
}

string CReadH::getclassbody(int pos, std::string content)
{
	int ileft = 0;
	int bodystart = content.find("{");
	if(bodystart!=string::npos)
	{
		ileft ++;

		int i;
		for(i = bodystart+1;i<content.length();i++)
		{
			if(content.at(i)=='{')
				ileft++;
			else if(content.at(i)=='}')
				ileft--;

			if(ileft == 0)
				break;
		}
		string ans = content.substr(pos,i-pos+1);
		return ans;
	}
	return "";
}

string CReadH::readallfile(string name)
{
    FILE * pFile;
    long lSize;
    char * buffer;
    size_t result;
    
    /* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */ 
	pFile = fopen (name.c_str(), "rb" );
    if (pFile==NULL)
    {
        fputs ("File error",stderr);
        exit (1);
    }

    /* 获取文件大小 */
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    /* 分配内存存储整个文件 */ 
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL)
    {
        fputs ("Memory error",stderr); 
        exit (2);
    }

    /* 将文件拷贝到buffer中 */
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize)
    {
        fputs ("Reading error",stderr);
        exit (3);
    }
    /* 现在整个文件已经在buffer中，可由标准输出打印内容 */
    //printf("%s", buffer); 

    /* 结束演示，关闭文件并释放内存 */
	string ans = string(buffer);
    fclose (pFile);
    free (buffer);
    return ans;
}
