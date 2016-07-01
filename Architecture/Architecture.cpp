// CArchitecture.cpp : Defines the class behaviors for the application.
/*
 * Project------ CArchitecture
 * Class Name--- CArchitecture.cpp
 * Author------- wxf891201@gmail.com
 * Date--------- 2013-4-8
 * Edition------ 1.0

 * Description-- MFC的APP, 实现同时打开指定的多个后缀名相同的文件

 * Change Log:
 *		Date-----
 *		Staff----
 *		Edition--
 *		Content--
 */


#include "Architecture.h"
#include "WSNnode.h"
#include <fstream>
#include <sstream> 
#include <cmath>
#include "CLog.h"


#define WSN_LENGTH		600
#define WSN_WIDTH			600
#define NODE_NUMBER		2000
#define NEIGHBOR_DISTANCE	22	 
#define DISTANCE		28       //200个节点：37      1000个节点：50	  400:20    500:42   800:50   1000:60  2000:28
#define TRANS_DISTANCE	   424	//			 43.5				 217.5
#define ENERGY_INITIAL    2      //初始能量
#define fmt                 11

void generateHyperGraph(WSNnode* nodes);
void generateWSN(WSNnode* nodes);

WSNnode nodes[NODE_NUMBER];
vector<vector<WSNnode>> totalHedge;

#ifdef WIN32
	#ifdef __cplusplus
	extern "C" 
	#endif
	FILE _iob[3] = {__iob_func()[0], __iob_func()[1], __iob_func()[2]};
#endif


CArchitectureApp::CArchitectureApp()
{
	m_pMgrJob = NULL;

}

// CArchitectureApp message handlers
void* loopJob(void* pParam)
{
	CMgrJob* loopjob;
	loopjob = (CMgrJob*)pParam;
//	CLogNote("MGRJOB THREAD LOOPJOB");
	return loopjob->dwLoopWork();
}

CLog *dataLog;
void *CLogThread(void* pParam)
{
	CLog *CLogwork;
	CLogwork = (CLog *)pParam;
	return CLogwork->loop();

}

void* StopThread(void* pParam)
{
	CMgrJob* stopjob;
	stopjob = (CMgrJob*)pParam;
	
	if(stopjob!=NULL)
	{
		stopjob->setLoopWorking();
	}
	return NULL;
}

void* IOThread(void* pParam)
{
	CMgrJob* IOjob;
	IOjob = (CMgrJob*)pParam;
//	CLogNote("MGRJOB THREAD LOOPJOB");
	return IOjob->dwLoopIOWork();
}

void CArchitectureApp::stopThreads()
{
	CThread*			cpStopThread;
	cpStopThread		= new CThread( StopThread, m_pMgrJob );
}

void generateWSN(WSNnode* nodes)
{
	fstream hgraphFile;
	hgraphFile.open("..//testcases/hgr/NodeAxis.txt",ios::in);
	if(!hgraphFile)                        //检查文件是否创建成功
    {
        cout<<"error open"<<endl;
        exit(0);    
    }

	char line[1024]={0};
	char energyLine[1024]={0};
	char clusterHeadLine[1024]={0};
	int nodeX=0;
	int nodeY=0;
	int number=0;
	double energy=0.0;
	int hgraphNode=0;

	while(!hgraphFile.eof())
	{
		hgraphFile.getline(line,sizeof(line));
		stringstream word(line);
		word>>nodeX;
		word>>nodeY;
		WSNnode node;
		node.setX(nodeX);
		node.setY(nodeY);
		//node.setEnergy(ENERGY_INITIAL);
		node.setTransDistance(TRANS_DISTANCE);
		node.setHgraphNode(number);
		nodes[number] = node;
		
		number++;
		if(number>=NODE_NUMBER)
		{
			break;
		}
	}
	hgraphFile.close();
	number=0;
	
	//读能量
	fstream energyFile;
	energyFile.open("F:\\matlab2010a\\install\\work\\Energy.txt",ios::in);
	if(!energyFile)                        //检查文件是否创建成功
    {
        cout<<"error open"<<endl;
        exit(0);    
    }
	while(!energyFile.eof())
	{
		energyFile.getline(energyLine,sizeof(energyLine));
		stringstream energyWord(energyLine);
		energyWord>>energy;
		nodes[number].setEnergy(energy);
		number++;
		if(number>=NODE_NUMBER)
		{
			break;
		}
	}
	energyFile.close();
	number=0;

	//读已经是簇头节点的文件
	fstream totalClusterHeads;
	totalClusterHeads.open("..//output/hgr/totalClusterHeads.txt",ios::in);
	if(!totalClusterHeads)                        //检查文件是否创建成功
    {
        cout<<"error open"<<endl;
        exit(0);    
    }
	while(!totalClusterHeads.eof())
	{
		totalClusterHeads.getline(clusterHeadLine,sizeof(clusterHeadLine));
		stringstream clusterHeadWord(clusterHeadLine);
		clusterHeadWord>>hgraphNode;
		nodes[hgraphNode].setIsClusterHead(true);
		number++;
		if(number>=NODE_NUMBER)
		{
			break;
		}
	}
	totalClusterHeads.close();

	for(int i=0;i<NODE_NUMBER;i++)       //设置每个节点的邻居节点数和权重
	{
		int neighborNum=0;
		for(int j=0;j<NODE_NUMBER;j++)
		{
			if(i!=j&&nodes[j].getEnergy()>0)    //死亡节点不算进邻居节点
			{  
				double distance=sqrt(pow((nodes[i].getX()-nodes[j].getX()),2.0)+pow((nodes[i].getY()-nodes[j].getY()),2.0));
				if(distance<NEIGHBOR_DISTANCE)
				{
					neighborNum++;
				}
			}
		}
		nodes[i].setNeighborNum(neighborNum);
		nodes[i].setWeights(nodes[i].getEnergy(),ENERGY_INITIAL,nodes[i].getNeighborNum());

	}

}

void generateHyperGraph(WSNnode* nodes)
{	
	int hedgeNumber=0;		      //超边序号从0开始算，每条超边的第一个节点包含此条超边的权重和此条超边的序号				 
	for(int i=0;i<NODE_NUMBER;i++)
	{
		double sumDistance=0.0;
		double averDistance=0.0;
		vector<WSNnode> hedge;
		WSNnode hedge_node;
		hedge_node.setHedgeNumber(hedgeNumber);
		hedge.push_back(hedge_node);
		nodes[i].setHedge(hedgeNumber);
		hedge.push_back(nodes[i]);
		for(int j=0;j<NODE_NUMBER;j++)
		{
			if(j!=i)
			{
				double tempDistance=sqrt(pow((nodes[i].getX()-nodes[j].getX()),2.0)+pow((nodes[i].getY()-nodes[j].getY()),2.0));
				if(tempDistance<DISTANCE)
				{
					nodes[j].setHedge(hedgeNumber);      //节点属于哪条超边
					hedge.push_back(nodes[j]);
					sumDistance+=tempDistance;
				}
			}
		}
		if(hedge.size()==2)     //超边只有一个点
			sumDistance=WSN_LENGTH/100.0;
		averDistance=sumDistance/(hedge.size()-1);
		hedge.at(0).setHedgeWeights(averDistance,WSN_LENGTH,nodes[i].getEnergy(),ENERGY_INITIAL);
		totalHedge.push_back(hedge);
		hedgeNumber++;
	}


	//创建超图文件

	fstream hgraph_file;                     
	hgraph_file.open("..//testcases/hgr/Hgraph.hgr",ios::out);
	if(!hgraph_file)                        //检查文件是否创建成功
    {
        cout<<"error open"<<endl;
        exit(0);    
    }

	hgraph_file<<totalHedge.size()<<" "<<NODE_NUMBER<<" "<<fmt<<"\n";                       //超图文件第一行：超边数 顶点数 权值参数
	for(int i=0;i<totalHedge.size();i++)
	{
		hgraph_file<<(int)totalHedge[i][0].getHedgeWeights()<<" ";	             //输入超边的权值
		for(int j=1;j<totalHedge[i].size()-1;j++)
		{
			hgraph_file<< totalHedge[i][j].getHgraphNode()+1<<" ";     //w文件中的超图节点序号从1开始
		}
		hgraph_file<<totalHedge[i][totalHedge[i].size()-1].getHgraphNode()+1<<"\n";
		
	}
	for(int k=0;k<NODE_NUMBER-1;k++)
	{
		hgraph_file<<(int)nodes[k].getWeights()<<"\n";
	}
	hgraph_file<<(int)nodes[NODE_NUMBER-1].getWeights();

	hgraph_file.close();

	
	 //创建节点属于哪个超边的文件，超边从0开始
	fstream nodeToHedge_file;					
	nodeToHedge_file.open("..//output/hgr/NodeToHedge.txt",ios::out);
	if(!nodeToHedge_file)                        
    {
        cout<<"error open"<<endl;
        exit(0);    
    }
	for(int i=0;i<NODE_NUMBER;i++)
	{
		for(int j=0;j<nodes[i].getBelongToHedge().size()-1;j++)
		{
			nodeToHedge_file<<nodes[i].getBelongToHedge().at(j)<<" ";
		}
		nodeToHedge_file<<nodes[i].getBelongToHedge().at(nodes[i].getBelongToHedge().size()-1)<<"\n";

	}
	nodeToHedge_file.close();
	

	//创建每个超边有多少节点的文件
	fstream hedgeNodeNumber_file;					   
	hedgeNodeNumber_file.open("..//output/hgr/HedgeNodeNumber.txt",ios::out);
	if(!hedgeNodeNumber_file)                        
    {
        cout<<"error open"<<endl;
        exit(0);    
    }
	//cout << "一共" << maxFlag << "条超边" << endl;
	for (int i = 0; i < totalHedge.size(); i++) {
		//cout<<totalHedge[i].size()-1<<"个顶点 ";
		hedgeNodeNumber_file<<totalHedge[i].size()-1<<"\n";
		//for (int j = 1; j < totalHedge[i].size(); j++) {
		//	cout << "(" << totalHedge[i][j].getX() << "," << totalHedge[i][j].getY() << ") ";
		//}
		//cout << endl; 
	}
	
	hedgeNodeNumber_file.close();
	//system("pause");

}

void CArchitectureApp::OnParserOpenfiles()
{
	/*
	 * 此处testcases_menu文件中每行前面加上了path: 是为了与在该行后面加上file等条目区分开，
	 * 本次测试中没有考虑分开给出测试文件名


	*/

	generateWSN(nodes);
	generateHyperGraph(nodes);

	extern CLog *dataLog;
	dataLog=new CLog();

	fstream			pFileTxt;					// 用于操作存有所有测试文件路径的那个文本文件
	string			strFileName;				// 配置文件(包含所有测试文件的路径及文件名)
	string			testFilePath;				// 每一个测试文件的路径及
	string			testFileName;
	int			posPath;
	LPTESTCASE		pTestCase;
	
	fstream			pFileDir;
	string			fileName;
	string			fileNameTmp;
	string			filePath;

	vector < string >	ary_fileName;	// testcase文件名

	strFileName		= "..//testcases_menu.txt";
	testFilePath	= "";
	testFileName	= "";
	posPath			= 0;
	pTestCase		= NULL;
	fileNameTmp		= "";
	filePath		= "";
	//_CrtSetBreakAlloc(4577);
	pFileTxt.open ( strFileName.c_str() ) ;
	while ( getline ( pFileTxt, testFilePath ) )
	{
		posPath	= int(testFilePath.find ( "path:" ));
		if ( -1 != posPath )
		{
			testFileName	= testFilePath.substr ( posPath + 5 );		// 从path:之后开始到结尾的子串均为路径名
			ary_fileName.push_back(testFileName);
			//pTestCase		= new CTestCase ( m_TestIndex, testFileName );
			//pTestCase->doParsing ( pTestCase );
			//m_pMgrJob->addJob(pTestCase);
			//m_pTestcase->doModeling ( m_pTestcase ); 

			//m_pTestcase->m_pNext_test	= m_pTestHead;
			//m_pTestHead					= m_pTestcase;

			//m_TestIndex	++;
		}		
	}
	pFileTxt.close ( );
	m_pMgrJob	= new CMgrJob ( );
	for ( int i=0; i < int(ary_fileName.size()); i++ )
	{
		pTestCase		= new CTestCaseCpp ( int(i), ary_fileName.at(i));
		m_pMgrJob->addIOJob(pTestCase);
	}


// added by loyoen 

	//每次运行之前，下面几行可以清空recordForExcel
	ofstream excelFile;
	string	filePathName = "..//recordsForExcel.txt";
	excelFile.open(filePathName.c_str());
	excelFile.close();



	excelFile.open ( filePathName.c_str(), ios::app );   //追加方式打开
	excelFile.write ( "file name \t hcut \t coupling \t cohesion \t time(\\ms) \n", int(strlen("file name \t hcut \t coupling \t cohesion \t time(\\ms) \n")) );
	excelFile.close();
	CThread*			cpInputJobThread;
	CThread*			cpDealThread;
	CThread*			cpDealThread2;
	CThread*			cpCLogThread;
	//CThread*			cpStopThread;
	
	cpCLogThread = new CThread(CLogThread,dataLog);
	cpInputJobThread	= new CThread ( IOThread, m_pMgrJob );
	cpDealThread		= new CThread ( loopJob, m_pMgrJob );
	cpDealThread2		= new CThread ( loopJob, m_pMgrJob );
	//cpStopThread		= new CThread( StopThread, m_pMgrJob );

	cpInputJobThread->wait4ThisThread();
	cpDealThread->wait4ThisThread();
	cpDealThread2->wait4ThisThread();

	delete cpInputJobThread;
	delete cpDealThread;
	delete cpDealThread2;
	delete m_pMgrJob;

    sleep(2);
#ifndef WIN32
    cpDealThread->wait4ThisThread();
    sleep(2);
#endif

}



