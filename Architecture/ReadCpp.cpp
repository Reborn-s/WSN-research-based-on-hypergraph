
#include "ReadCpp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
CMethodBody::CMethodBody(string method,string funcbody,int cnt)
{
	body = method;
	next = NULL;
	funcline = funcbody;
	paraCnt = cnt;
}
CReadCpp::CReadCpp(string name)
{
	next = NULL;
	imatch = 0;
	classposition = -1;
	globalStr = "";
	CppName = name;
	getrelation();
	
}

CReadCpp::~CReadCpp()
{	
	map<string,CMethodBody*>::iterator it;
	for(it=cppclass.begin();it!=cppclass.end();++it)
	{
		CMethodBody *p,*q;
		p=it->second;
		while(p!=NULL)
		{
			q=p;
			p=p->next;
			delete q;
		}
	}
}

void CReadCpp::setnext(CReadCpp *p)
{
	next = p;
}
CReadCpp* CReadCpp::getnext()
{
	return next;
}
void CReadCpp::getrelation()
{
	string content = readallfile(CppName);
	
	int classstart = -1;
	int pos = findclasses(0,content);
	while(pos!=string::npos)
	{
		if(classposition != -1 &&classstart == -1)
			classstart = classposition;
		pos = findclasses(pos,content);
	}

	if(classstart!=string::npos)
	{
		getheadfile(classstart,content);
	}
	globalStr = findGlobal(content);

}

int CReadCpp::findclasses(int s,std::string content)    //必须顶格写
{	
	string oneclassname="";
	string oneclassbody="";
	bool isclass;
	int ileft = 0;
	int pos = int(content.find("::",s));
	if(pos!=string::npos)
		isclass = true;
	else
		return int(pos);

	int i = pos+int(strlen("::"));
	if(isclass)
	{
		while(content.at(i)!='\n')
		{
			i++;
		}
		if(content.at(i-1)==';')  //如果该行最后一个字符为分号
			isclass = false;
	}
	i=pos;
	if(isclass)
	{
		while(content.at(i)!='\n')
		{
			i--;
			if(i<0)
				break;
		}
		if(content.at(i+1)==' '||content.at(i+1)=='\t')
			isclass = false;
	}

	if(isclass)
	{
		int j = pos-1;
		while(content.at(j)==' '||content.at(j)=='\t')  //找到::前第一个不为空格的字符
			j--;
		int endpo = j;
		if(endpo<0)
			return int(pos+2);
		while(content.at(j)!=' '&&content.at(j)!='\t'&&content.at(j)!='\n')
		{
			j--;
			if(j==-1)
				return int(pos+2);
		}
		oneclassname = content.substr(j+1,endpo-j);
		string classtmp="";
		for(int mm=0;mm<int(oneclassname.length());mm++)
		{
			char c = oneclassname.at(mm);
			if((c>=65&&c<=90)||(c>=97&&c<=122)||(c>=48&&c<=57)||(c=='_'))
				classtmp += c;
			else
				break;
		}
		oneclassname = classtmp;

		int bodystart = int(content.find("{",pos));
		if(bodystart!=string::npos)
		{
			ileft++;
		}
		else
			return int(bodystart);
		int k;
		for(k=bodystart+1;k<int(content.length());k++)
		{
			if(content.at(k)=='{')
				ileft++;
			else if(content.at(k)=='}')
				ileft--;
			if(ileft == 0)
				break;
		}
		
		int start = pos;
		while(content.at(start)!='\n')
			start--;

		oneclassbody = content.substr(start+1,k-start);

		string funcbody = oneclassbody.substr(oneclassbody.find("::")+2,oneclassbody.find("(")-2-oneclassbody.find("::"));
		for(int i=0;i<funcbody.length();i++)
			if(funcbody.at(i)==' '||funcbody.at(i)=='\t')
				funcbody.erase(i,1);
		
		int paraCnt = 0;
		string strPara = oneclassbody.substr(oneclassbody.find("(")+1,oneclassbody.find(")")-1-oneclassbody.find("("));
		for(int i=0;i<strPara.length();i++)
			if(strPara.at(i)==' '||strPara.at(i)=='\t')
				strPara.erase(i,1);
		if(strPara.length()>2)
			paraCnt++;
		for(int i=0;i<strPara.length();i++)
		{
			if(strPara.at(i)=='{')
				break;
			if(strPara.at(i)==',')
				paraCnt ++;
		}

		map<string ,CMethodBody*>::iterator iter;
		iter = cppclass.find(oneclassname);
		if(iter!=cppclass.end())
		{
			CMethodBody *q = iter->second;
			CMethodBody *p = new CMethodBody(oneclassbody,funcbody,paraCnt);
			while(q->next!=NULL)
				q=q->next;
			p->next = NULL;
			q->next = p;
		}
		else
		{
			CMethodBody *p = new CMethodBody(oneclassbody,funcbody,paraCnt);
			p->next = NULL;
			cppclass.insert(pair<string,CMethodBody*>(oneclassname,p));
		}
		classposition = int(k);
		return int(k);
	}

	return int(pos+2);
}

void CReadCpp::getheadfile(int classstart,string content)    //时间复杂度很高，要修改
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

string CReadCpp::findGlobal(string content)
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

string CReadCpp::readallfile(string name)
{
    FILE * pFile;
    long lSize;
    char * buffer;
    int result;
	/* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */ 
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
