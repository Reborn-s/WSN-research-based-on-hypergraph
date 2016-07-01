// CArchitecture.cpp : Defines the class behaviors for the application.
/*
 * Project------ CArchitecture
 * Class Name--- CArchitecture.cpp
 * Author------- wxf891201@gmail.com
 * Date--------- 2013-4-8
 * Edition------ 1.0

 * Description-- MFC��APP, ʵ��ͬʱ��ָ���Ķ����׺����ͬ���ļ�

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
#define DISTANCE		28       //200���ڵ㣺37      1000���ڵ㣺50	  400:20    500:42   800:50   1000:60  2000:28
#define TRANS_DISTANCE	   424	//			 43.5				 217.5
#define ENERGY_INITIAL    2      //��ʼ����
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
	if(!hgraphFile)                        //����ļ��Ƿ񴴽��ɹ�
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
	
	//������
	fstream energyFile;
	energyFile.open("F:\\matlab2010a\\install\\work\\Energy.txt",ios::in);
	if(!energyFile)                        //����ļ��Ƿ񴴽��ɹ�
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

	//���Ѿ��Ǵ�ͷ�ڵ���ļ�
	fstream totalClusterHeads;
	totalClusterHeads.open("..//output/hgr/totalClusterHeads.txt",ios::in);
	if(!totalClusterHeads)                        //����ļ��Ƿ񴴽��ɹ�
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

	for(int i=0;i<NODE_NUMBER;i++)       //����ÿ���ڵ���ھӽڵ�����Ȩ��
	{
		int neighborNum=0;
		for(int j=0;j<NODE_NUMBER;j++)
		{
			if(i!=j&&nodes[j].getEnergy()>0)    //�����ڵ㲻����ھӽڵ�
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
	int hedgeNumber=0;		      //������Ŵ�0��ʼ�㣬ÿ�����ߵĵ�һ���ڵ�����������ߵ�Ȩ�غʹ������ߵ����				 
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
					nodes[j].setHedge(hedgeNumber);      //�ڵ�������������
					hedge.push_back(nodes[j]);
					sumDistance+=tempDistance;
				}
			}
		}
		if(hedge.size()==2)     //����ֻ��һ����
			sumDistance=WSN_LENGTH/100.0;
		averDistance=sumDistance/(hedge.size()-1);
		hedge.at(0).setHedgeWeights(averDistance,WSN_LENGTH,nodes[i].getEnergy(),ENERGY_INITIAL);
		totalHedge.push_back(hedge);
		hedgeNumber++;
	}


	//������ͼ�ļ�

	fstream hgraph_file;                     
	hgraph_file.open("..//testcases/hgr/Hgraph.hgr",ios::out);
	if(!hgraph_file)                        //����ļ��Ƿ񴴽��ɹ�
    {
        cout<<"error open"<<endl;
        exit(0);    
    }

	hgraph_file<<totalHedge.size()<<" "<<NODE_NUMBER<<" "<<fmt<<"\n";                       //��ͼ�ļ���һ�У������� ������ Ȩֵ����
	for(int i=0;i<totalHedge.size();i++)
	{
		hgraph_file<<(int)totalHedge[i][0].getHedgeWeights()<<" ";	             //���볬�ߵ�Ȩֵ
		for(int j=1;j<totalHedge[i].size()-1;j++)
		{
			hgraph_file<< totalHedge[i][j].getHgraphNode()+1<<" ";     //w�ļ��еĳ�ͼ�ڵ���Ŵ�1��ʼ
		}
		hgraph_file<<totalHedge[i][totalHedge[i].size()-1].getHgraphNode()+1<<"\n";
		
	}
	for(int k=0;k<NODE_NUMBER-1;k++)
	{
		hgraph_file<<(int)nodes[k].getWeights()<<"\n";
	}
	hgraph_file<<(int)nodes[NODE_NUMBER-1].getWeights();

	hgraph_file.close();

	
	 //�����ڵ������ĸ����ߵ��ļ������ߴ�0��ʼ
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
	

	//����ÿ�������ж��ٽڵ���ļ�
	fstream hedgeNodeNumber_file;					   
	hedgeNodeNumber_file.open("..//output/hgr/HedgeNodeNumber.txt",ios::out);
	if(!hedgeNodeNumber_file)                        
    {
        cout<<"error open"<<endl;
        exit(0);    
    }
	//cout << "һ��" << maxFlag << "������" << endl;
	for (int i = 0; i < totalHedge.size(); i++) {
		//cout<<totalHedge[i].size()-1<<"������ ";
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
	 * �˴�testcases_menu�ļ���ÿ��ǰ�������path: ��Ϊ�����ڸ��к������file����Ŀ���ֿ���
	 * ���β�����û�п��Ƿֿ����������ļ���


	*/

	generateWSN(nodes);
	generateHyperGraph(nodes);

	extern CLog *dataLog;
	dataLog=new CLog();

	fstream			pFileTxt;					// ���ڲ����������в����ļ�·�����Ǹ��ı��ļ�
	string			strFileName;				// �����ļ�(�������в����ļ���·�����ļ���)
	string			testFilePath;				// ÿһ�������ļ���·����
	string			testFileName;
	int			posPath;
	LPTESTCASE		pTestCase;
	
	fstream			pFileDir;
	string			fileName;
	string			fileNameTmp;
	string			filePath;

	vector < string >	ary_fileName;	// testcase�ļ���

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
			testFileName	= testFilePath.substr ( posPath + 5 );		// ��path:֮��ʼ����β���Ӵ���Ϊ·����
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

	//ÿ������֮ǰ�����漸�п������recordForExcel
	ofstream excelFile;
	string	filePathName = "..//recordsForExcel.txt";
	excelFile.open(filePathName.c_str());
	excelFile.close();



	excelFile.open ( filePathName.c_str(), ios::app );   //׷�ӷ�ʽ��
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



