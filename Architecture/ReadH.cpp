
#include "ReadH.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

CClassContent::CClassContent()
{
	defbody = "";
	fatherclass = "";
}

CReadH::CReadH(string name)
{
	Hname = name;
	globalStr = "";
	classposition = -1;
	string allContent = readallfile(Hname);
	getclass(allContent);
	getheadfile(classposition,allContent);
	globalStr = findGlobal(allContent);
}

void CReadH::getheadfile(int classstart,string content)    //时间复杂度很高，要修改
{
	string mycontent = content.substr(0,classstart+1);
	int pos = int(mycontent.find("#include \""));
	while(pos!=string::npos)
	{
		string filename="";
		pos += int(strlen("#include \""));
		int i = pos;
		while(mycontent.at(i)!='\"')
		{
			filename += mycontent.at(i);
			i++;
		}
		headfile.push_back(filename);
		pos = int(mycontent.find("#include \"",pos));
	}
}

int CReadH::getclass(string allContent)
{
	
	int pos = int(allContent.find("\nclass "));      //先简单这样考虑，要改
	while(pos!=string::npos)
	{
		string classname =  getclassname(int(pos),allContent);
		if(classname!="")
		{
			classposition = classposition==-1?int(pos):classposition;
			string fatherclass = getfatherclassname(int(pos),allContent);
			string classbody  =  getclassbody(int(pos),allContent);
			CClassContent *p = new CClassContent();
			p->fatherclass = fatherclass;
			p->defbody = classbody;
			/*  在.h内寻找成员变量，先注释掉
			p->valueTotype = findValue(classbody);
			*/
			classdef.insert(pair <string,CClassContent*>(classname,p));
		}
		pos = int(allContent.find("\nclass ",pos+strlen("\nclass ")));
	}
	return 1;
}

/*  在.h内寻找成员变量，先注释掉
map<string,string> CReadH::findValue(std::string content)
{
	map<string,string> valuemap;
	int pos = content.find("{");
	
	while(pos!=-1)
	{
		string type="",value="";
		pos += 1;
		if(pos>=content.length())
			break;
		else
		{
			char c=content.at(pos);
			int d = c;
			while((content.at(pos)==' '||content.at(pos)=='\t'||content.at(pos)=='\n'||content.at(pos)=='\r'))
				pos++;
		}
		if(pos>=content.length())
			break;
		while((content.at(pos)>=65&&content.at(pos)<=90)||(content.at(pos)>=97&&content.at(pos)<=122)||(content.at(pos)>=48&&content.at(pos)<=57)||content.at(pos)=='_')
		{
			type+=content.at(pos);
			pos++;
		}

		if(pos>=content.length())
			break;

		while((content.at(pos)==' '||content.at(pos)=='\t'||content.at(pos)=='*'))
			pos++;

		if(pos>=content.length())
			break;

		while(((content.at(pos)>=65&&content.at(pos)<=90)||(content.at(pos)>=97&&content.at(pos)<=122)||(content.at(pos)>=48&&content.at(pos)<=57)||content.at(pos)=='_'))
		{
			value+=content.at(pos);
			pos++;
		}
		if(type!=""&&value!="")
			valuemap.insert(pair<string,string>(value,type));
		pos = content.find(";",pos);
	}
	return valuemap;
}
*/
string CReadH::getfatherclassname(int pos,string content)
{
	string ans = "";
	int i = pos+int(strlen("\nclass"));
	while(i<content.length()&&content.at(i)!='{'&&content.at(i)!=';')
		i++;
	string defline = content.substr(pos+1,i-pos-1);
	
	
	int start = int(defline.find("public",0));
	while(start!=string::npos)
	{
		i = start + int(strlen("public"));
		if(i>=int(defline.length()))
			return "";
		if(defline.at(i)!=' '&&defline.at(i)!='\t')
		{
			start = int(defline.find("public",start+int(strlen("public"))-1));
			continue;
		}
		else
		{
			while(defline.at(i)==' '||defline.at(i)=='\t')
			{
				i++;
				if(i>=int(defline.length()))
					break;
			}
		}
		if(i<int(defline.length()))
		{
			while((defline.at(i)>=65&&defline.at(i)<=90)||(defline.at(i)>=97&&defline.at(i)<=122)||(defline.at(i)>=48&&defline.at(i)<=57)||defline.at(i)=='_')
			{
				ans += defline.at(i);
				i++;
				if(i>=int(defline.length()))
						break;
			}
			ans += ' ';
		}
		
		start = int(defline.find("public",start+strlen("public")-1));
	
	}

	return ans;

}

string CReadH::getclassname(int pos, std::string content)
{
	string ans = "";
	int i = pos+int(strlen("\nclass"));
	while(content.at(i)==' '||content.at(i)=='\t')
		i++;

	for(;(content.at(i)>=65&&content.at(i)<=90)||(content.at(i)>=97&&content.at(i)<=122)||(content.at(i)>=48&&content.at(i)<=57);i++)
	{
		ans += content.at(i);
	}
	if(content.at(i)==';')
		return "";
	
	return ans;
	
}

string CReadH::getclassbody(int pos, std::string content)
{
	int ileft = 0;
	int bodystart = int(content.find("{",pos));
	if(bodystart!=string::npos)
	{
		ileft ++;

		int i;
		for(i = bodystart+1;i<int(content.length());i++)
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

string CReadH::findGlobal(string content)
{
	int leftnum=0;
	string strForGlobal="";

	for(int i=0;i<int(content.length());i++)
	{
		if(content.at(i)=='{')
		{
	
			int posfen;
			for(posfen=int(strForGlobal.length()-1);posfen>=0;posfen--)
			{
				if(strForGlobal.at(posfen)==';')
					break;
			}
			
			strForGlobal.erase(posfen+1,strForGlobal.length()-posfen-1);
			
			leftnum++;
		}
		else if(content.at(i)=='}')
			leftnum--;
		//else if(content.at(i)==';')
		//	start = i+1;

		if(leftnum==0)
		{
			if(content.at(i)!='}')
				strForGlobal += content.at(i);  //append(string(content.at(i)));
		}
		if(leftnum<0)
			break;
	}
	return strForGlobal;
}

string CReadH::readallfile(string name)
{
    FILE * pFile;
    long lSize;
    char * buffer;
    int result;
    
#ifdef WIN32
	errno_t err;
	/* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */ 
	err = fopen_s (&pFile, name.c_str(), "rb" );
    if (err!=0)
    {
        fputs ("File error",stderr);
        return "";
    }
#else
	pFile = fopen (name.c_str(), "rb" );
    if (pFile==NULL)
    {
        fputs ("File error",stderr);
        return "";
    }
#endif

    /* 获取文件大小 */
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    /* 分配内存存储整个文件 */ 
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL)
    {
        fputs ("Memory error",stderr); 
        return "";
    }

    /* 将文件拷贝到buffer中 */
    result = int(fread (buffer,1,lSize,pFile));
    if (result != lSize)
    {
        fputs ("Reading error",stderr);
        return "";
    }
    /* 现在整个文件已经在buffer中，可由标准输出打印内容 */
    //printf("%s", buffer); 

    /* 结束演示，关闭文件并释放内存 */
	string ans = string(buffer);
    fclose (pFile);
    free (buffer);
    return ans;
}
