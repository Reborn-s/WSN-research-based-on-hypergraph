
#include "CInput.h"
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <time.h>

using namespace std;

CInput::CInput(const char _fp[],int _npart=0,int _ubfactor=0)
{
    swit = true;
	switOption6 = 0;
    strcpy_s(fp,_fp);
	npart				=   _npart;
	ubfactor			=   _ubfactor;


	/*for(int i=0;i<10000;i++)
		weight[i]=1;
	for(int i=0;i<10000;i++)
		vertice[i]=1;*/
	CInput_readfile();
		//cout << "read success" << endl;

	MakeWeight();

}
CInput::CInput(char _fp[],int _npart,int _ubfactor,int _nruns,int _ctype,int _rtype,int _vcycle,int _reconst,int _dbglvl)
{
    swit = false;
	switOption6 = 0;
    strcpy_s(fp,_fp);
	npart				=   _npart;
	ubfactor			=   _ubfactor;
	nruns				=   _nruns;	
	ctype				=   _ctype;	                     
	rtype				=   _rtype;	
	vcycle				=   _vcycle;
	reconst				=   _reconst;               
	dbglvl				=   _dbglvl;	
	

	CInput_readfile();

	CInput_check();
		
	MakeWeight();
}

CInput::CInput(char _fp[],char _FixFile[],int _npart,int _ubfactor)
{

	swit = true;
	switOption6 = 1;
	strcpy_s(fp,_fp);
	strcpy_s(fp_fixfile,_FixFile);
	npart				=   _npart;
	ubfactor			=   _ubfactor;

	CInput_readfile();
		//cout << "read success" << endl;

	CInput_readFixFile();
		//cout << "read FixFile success" << endl;

	MakeWeight();
}

CInput::CInput(char _fp[],char _FixFile[],int _npart,int _ubfactor,int _nruns,int _ctype,int _rtype,int _vcycle,int _reconst,int _dbglvl){

	swit = false;
	switOption6 = 1;
    strcpy_s(fp,_fp);
	strcpy_s(fp_fixfile,_FixFile);
	npart				=   _npart;
	ubfactor			=   _ubfactor;
	nruns				=   _nruns;	
	ctype				=   _ctype;	                     
	rtype				=   _rtype;	
	vcycle				=   _vcycle;
	reconst				=   _reconst;               
	dbglvl				=   _dbglvl;	

	CInput_readfile();
		//cout << "read success" << endl;

	CInput_readFixFile();
		//cout << "read FixFile success" << endl;

	MakeWeight();


}
//再加一个fix file构造函数
bool CInput::CInput_check()
{
      if(!belongto(npart, 2,nvtxs))
	  {  
		  cout << "npart must be from 2 to " << nvtxs << endl;
          return false;
	  }

      if(!belongto(ubfactor, 1, 49))
	  {
		  cout << "ubfactor must be from 1 to 49 " << endl;
          return false;
	  }

      if(!belongto(nruns, 1, -1))
	  {
		  cout << "npart must be greater or equal to 1" << endl;
		  return false;
	  }

      if(!belongto(ctype, 1, 5))
	  {
		  cout << "ctype must be from 1 to 5 " << endl;
          return false;
	  }

      if(!belongto(rtype, 1, 3))
	  {
		  cout << "rtype must be from 1 to 3 " << endl;
		  return false;
	  }

	  if(!belongto(vcycle, 0, 3))
	  {
		  cout << "vcycle must be from 0 to 3 " << endl;
		  return false;
	  }

	  if(!belongto(reconst, 0, 1))
	  {
		  cout << "reconst must be from 0 to 1 " << endl;
          return false;
	  }

	   if(!belongto(dbglvl, 0, 24))
	   {
		  cout << "reconst must be from 0 to 24 " << endl;
          return false;
	   }
    
	   return true;
}

bool CInput::CInput_readfile()
{
	ifstream file;
	file.open(fp);

	if(!file)   
	{   
        cout << "open file error" << endl;   
		exit(0);   
	} 

	bool empty = true;
	char StrTemp[1024];
	int num[100];

	int ReadNumOfLine = 0;//加上首行（点、边个数）的行数
	int NumOfLineWord = 0;//每行单词个数，第几个单词
	int NumOfLine=1;
	int EptrNum = 0;
	int WordCount  = 0;
	int VerticeNum = 0;
	int j;              //用于循环的变量
	int line_num;

	type = 0;
	
	string str[1024];

	for(j = 0; j < 1024; j++)
		str[j]="";

	if(file != NULL){
		while(!file.eof()){   
			file.getline(StrTemp,1024);
			/*for (int iTemp = 0; iTemp < strlen(temp); iTemp ++)
			{
				cout<< temp[iTemp]<< endl;
			}
			system("pause");
			*/
			if(0 == ReadNumOfLine){
				for(j = 0; j < int(strlen(StrTemp)); j++){//针对本行内容扫描			
					if(StrTemp[j] != ' '){ //如果内容不为空 
						empty=false;//注明本行内容不为空
						while(StrTemp[j]!=' '){//选择某一个单词，将其全部赋给STR[X]
							str[NumOfLineWord]=str[NumOfLineWord] + StrTemp[j];
							//cout << temp[j] << endl;//考虑语法问题
							j++;
							if ( j > int(strlen(StrTemp)))
							{
								break;
							}
						}
						//cout<< k << str[k]<< endl;
						//system("pause");
						NumOfLineWord++;//处理完一个单词
						
					}
					else//两种情况为空，其一，行空，令j=length之后退出，其二，间隔为空，则继续
						continue;//j++自动为j+1}
				}//end for
				if(!empty){	//针对命令行参数分别赋值
					if(2==NumOfLineWord)
					{
						nhedges=atoi(str[0].c_str());
						nvtxs=atoi(str[1].c_str());
					}
					else if(3==NumOfLineWord)
					{
						nhedges=atoi(str[0].c_str());
						nvtxs=atoi(str[1].c_str());
						type=atoi(str[2].c_str());
					}
					else
					{
						cout<<"file error"<<endl;
						return false;
					}
				}
			}
			else{
				for(j=0;j<int(strlen(StrTemp));j++){
					if(StrTemp[j]!=' '){  
						empty=false;
						while(StrTemp[j]!=' '){
							str[NumOfLineWord]=str[NumOfLineWord]+StrTemp[j];
							j++;
							if ( j > int(strlen(StrTemp)))
							{
								break;
							}
						}
						//cout<<k <<":"<< str[k]<< endl;
						//system("pause");
						NumOfLineWord++;
					
					}
					else
						continue;
				}//end for
				if(!empty){  
					line_num=NumOfLineWord;
					while(line_num>0){
						num[line_num-1]=atoi(str[line_num-1].c_str());
						line_num--;
					}
					line_num=NumOfLineWord;
					if(ReadNumOfLine<nhedges+1){
						switch(type){
						case 0:				 // *weight=NULL;
												EptrNum++;
												for(j=0;j<line_num;j++)
												{
													if(num[j]>nvtxs)
													{
														cout<<"file error at line "<<NumOfLine<<endl;
														exit(0);
													}
													else
														eind[WordCount+j]=num[j]-1;
													//cout<< "eind："<< eind[count + j]<<endl;
												}
												WordCount += line_num;
												eptr[EptrNum]=WordCount;
												//cout<< "eptr:"<< eptr[t]<< endl;
												//system("pause");
												break;
						case 10:				//*weight=NULL;
												EptrNum++;
												for(j=0;j<line_num;j++)
												{
													if(num[j]>nvtxs)
													{
														cout<<"file error at line "<<NumOfLine<<endl;
														exit(0);
													}
													else
														eind[WordCount+j]=num[j]-1;
												}
												WordCount += line_num;
												eptr[EptrNum]=WordCount;
												break;
						case 1:				weight[ReadNumOfLine-1]=num[0];
												EptrNum++;
												for(j=1;j<line_num;j++)
												{
													if(num[j]>nvtxs)
													{
														cout<<"file error at line "<<NumOfLine<<endl;
														exit(0);
													}
													else
														eind[WordCount+j-1]=num[j]-1;
												}
												WordCount += line_num-1;
												eptr[EptrNum]=WordCount;
												break;
						case 11:				weight[ReadNumOfLine-1]=num[0];
												EptrNum++;
												for(j=1;j<line_num;j++)
												{
													if(num[j]>nvtxs)
													{
														cout<<"file error at line "<<NumOfLine<<endl;
														exit(0);
													}
													else
														eind[WordCount+j-1]=num[j]-1;
												}
												WordCount += line_num-1;
												eptr[EptrNum]=WordCount;
												break;

						default :				break;
									

						}
					}
					else {
						switch(type)
						{
						case 10 :		vertice[VerticeNum]=num[0];
											VerticeNum++;
											break;
						case 11 :		vertice[VerticeNum]=num[0];
											VerticeNum++;
											break;
						default		 :	break;
						}
					}
				}
			}

			for(j=0;j<NumOfLineWord;j++)
				str[j].clear();
			//重置循环
			
			NumOfLineWord=0;
			ReadNumOfLine++;
			if(empty)
				ReadNumOfLine--;//减去多加的行数
			if(1==ReadNumOfLine)
			{
				eptr=new int[nhedges+1];
				weight=new int[nhedges];
				vertice=new int[nvtxs];
				part = new int[nvtxs];
				eptr[0] = 0;
			
			}
			empty=true;//重置本行内容为空
		}
	}

	eind_length=WordCount;   //eind实际长度
	//cout<< eind[eind_length - 2] <<endl;
	//cout<< eind_length<< endl;
    if(type==0||type==1)
	{
		if(ReadNumOfLine!=nhedges+1)
		{
		cout<<"file error"<<endl; //here! type = 0
		return false;
		}
	}
	else	if(type==10||type==11)
	{
		if(ReadNumOfLine!=nhedges+nvtxs+1)
		{
		cout<<"file error"<<endl;
		return false;
		}
	
	}

	return true;
}

bool CInput::CInput_readFixFile()
{
	ifstream fixfile;
	bool empty = true;
	int NumOFline=1;
	int ReadNumOfLine = 0;//加上首行（点、边个数）的行数
	fixfile.open(fp_fixfile);
	char StrTemp[1024];
	if(!fixfile)   
	{   
        cout << "open fixfile error" << endl;   
		exit(0);   
	} 
	
	if(fixfile != NULL)
	{
		while(!fixfile.eof())
		{   
			fixfile.getline(StrTemp,1024);
			part[ReadNumOfLine]=atoi(StrTemp);  //如果一行有两个数，则忽略第二个数
			if(part[ReadNumOfLine]>=npart)
			{
				cout<<"fixfile error at line "<<NumOFline<<endl;
				exit(0);
			}
			for(int j = 0; j < int(strlen(StrTemp)); j++)
			{//针对本行内容扫描			
					if(StrTemp[j] != ' ')
					{ //如果内容不为空 
						empty=false;
					}
			}
			ReadNumOfLine++;
			NumOFline++;
			if(empty)
				ReadNumOfLine--;
		}
	}
	return	true;

}

int *CInput::get_option()
{
	if(swit)
	{
		option[0]=0;
		option[1]=10;
		option[2]=1;
		option[3]=1;
		option[4]=1;
		option[5]=0;
		option[6]=0;
		option[7]=-1;
		option[8]=0;
	}
	else
	{
		option[0]=1;

		time_t t;
		srand((unsigned int)time(&t));

		option[1]=nruns;
		option[2]=ctype;
		option[3]=rtype;
		option[4]=vcycle;
		option[5]=reconst;
		option[6]=switOption6;//0;
		option[7]=-1;//rand();
		option[8]=dbglvl;
	}
	return option;


}

int CInput::get_nvtxs()
{
	return nvtxs;

}

int CInput::get_nhedges()
{
	return nhedges;

}
int* CInput::get_vwgts()
{
	//for(int i=0;i<nvtxs;i++)
	//	cout<<"vertice["<<i<<"]="<<vertice[i]<<endl;
	return vertice;

}
int* CInput::get_eptr()
{
	return eptr;

}
int* CInput::get_eind()
{
	return eind;

}
int* CInput::get_hewgts()
{
//	for(int i=0;i<nhedges;i++)
//		cout<<"weight["<<i<<"]="<<weight[i]<<endl;
	return weight;

}
int CInput::get_nparts()
{
	//cout << "npart!!!" << npart << endl;
	return npart;

}
int* CInput::get_part()
{
	return part;
}

int CInput::get_ubfactor()
{
	return ubfactor;

}

int CInput::get_eind_length()
{
	return eind_length;
}


bool CInput::belongto(int require,int min,int max)
{	
	if(require>=min||-1==min)
	{
		if(require<=max||-1==max)
			return true;
	}

	return false;
}

void CInput::MakeWeight()
{
	if(0==type)
	{
		//delete []weight;
		//delete []vertice;

		weight = NULL;
		vertice = NULL;
		/*
		cout << weight << endl;
		cout << vertice << endl;
		*/
	}
	if(1==type)
	{	
		vertice = NULL;
	}
	if(10==type)
	{
		weight = NULL;
	}

}


void CInput::print(void)
{
	
	cout<< "*******************************************************************************\n"
		"HypergraphTest , Wuhan University\n"<<endl;

	cout<< "HyperGraph Information ---------------------------------------------------\n"
		" Name: "<< fp<< " #Vtxs:"<< nvtxs<< " #Hedges:"<< nhedges<< 
		" #Parts:"<< npart<< " #UBfactor:"<< setprecision(2)<< double(ubfactor) / 100<< "\n"<< endl;
}

CInput::~CInput()
{	
	delete[] eptr;
	delete []weight;
	delete []vertice;
	delete []part;
}
