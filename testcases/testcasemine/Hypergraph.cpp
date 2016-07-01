#include "Hypergraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

stEdge::stEdge ( LPCLASS pClass )
{
	pVertex		= new CVertex ( pClass->m_cIndex, pClass );
	pNext		= NULL;
}

stEdge::~stEdge ( )
{
	if ( pVertex )
	{
		delete		pVertex;
		pVertex		= NULL;
	}
}

CVertex::CVertex ( int vIndex, LPCLASS pClass )
{
	m_nvIndex		= vIndex;
	m_pvHead		= pClass;
	m_pvNext		= NULL;
	pNext_on_edge	= NULL;
	m_vWeight		= 1;
	//m_vTotal		= 0;
}
CVertex::~CVertex ()
{

}
CTestCase::CTestCase()
{
    
}
void CVertex::outputVertexToFile ( string fileName, LPVERTEX pVertex )
{
	char		cpIndex[128];
	char		cpVertex[128];
	char		cpWeight[128];
	LPVERTEX	pVerTmp;

	ofstream	pFile;
	pFile.open ( fileName.c_str() );

	pVerTmp		= pVertex;
	while ( pVerTmp )
	{
        	#ifdef WIN32
		     sprintf_s ( cpIndex, 128, "%d ", pVerTmp->m_nvIndex );
	        #else
	             sprintf ( cpIndex,"%d",pVerTmp->m_nvIndex );
		#endif
		pFile.write ( cpIndex, (int) strlen ( cpIndex ) );
		#ifdef WIN32			
		     sprintf_s ( cpVertex, 128, "%s ", pVerTmp->m_pvHead->className.c_str() );
		#else
		     sprintf ( cpVertex, "%s ", pVerTmp->m_pvHead->className.c_str() );
		#endif
		pFile.write ( cpVertex, (int) strlen ( cpVertex ) );
		#ifdef WIN32
		     sprintf_s ( cpWeight, 128, "%d ", pVerTmp->m_vWeight );
		#else
		     sprintf( cpWeight, "%d ", pVerTmp->m_vWeight );
		#endif

		pFile.write ( cpWeight, (int) strlen ( cpWeight ) );

		pFile.write ( "\n", 1 );

		pVerTmp		= pVerTmp->m_pvNext;
	}
}

CEdge::CEdge ( int eIndex, LPVERTEX pVertex )
{
	m_neIndex			= eIndex;
	//m_pEHead			= pVertex;
	m_pENext			= NULL;

	m_pvHead_on_edge	= pVertex;
	m_pvTail_on_edge	= m_pvHead_on_edge;
	
	m_eWeight			= 1;
	m_ivCnt				= 1;
	//m_eTotal			= 0;
}
// 这里释放了所有的顶点
CEdge::~CEdge ( )
{
	LPVERTEX	pVerTmp;
	pVerTmp		= m_pvHead_on_edge;
	while ( pVerTmp )
	{
		m_pvHead_on_edge		= pVerTmp->pNext_on_edge;
		delete					pVerTmp;
		pVerTmp					= m_pvHead_on_edge;
	}
}

void CEdge::addVertex ( LPVERTEX pVertex )
{
	if ( pVertex )
	{
		m_pvTail_on_edge->pNext_on_edge		= pVertex;
		m_pvTail_on_edge					= pVertex;
		m_ivCnt ++;
	}
}

void CEdge::outputEdgesToFile ( string fileName, LPEDGE pEdge )
{
	//char		cpIndex[128];
	char		cpEdges[128];
	char		cpWeight[128];
	char		cpVerIndex[128];
	LPEDGE		pEdgeTmp;
	LPVERTEX	pVerTmp;

	ofstream	pFile;
	pFile.open ( fileName.c_str() );

	pEdgeTmp	= pEdge;
	while ( pEdgeTmp )
	{
		/*sprintf_s ( cpIndex, 128, "%d ", pEdgeTmp->m_neIndex );
		pFile.write ( cpIndex, (int) strlen ( cpIndex ) );*/
		#ifdef WIN32
			sprintf_s ( cpWeight, 128, "%d ", pEdgeTmp->m_eWeight );
		#else
			sprintf( cpWeight, "%d ", pEdgeTmp->m_eWeight );
		#endif
		pFile.write ( cpWeight, (int) strlen ( cpWeight ) );
		pVerTmp		= pEdgeTmp->m_pvHead_on_edge;
		while ( pVerTmp )
		{
	
			#ifdef WIN32
				sprintf_s ( cpEdges, 128, "%s ", pVerTmp->m_pvHead->className.c_str() );
			#else
				sprintf ( cpEdges, "%s ", pVerTmp->m_pvHead->className.c_str() );
			#endif
			pFile.write ( cpEdges, (int) strlen ( cpEdges ) );
			#ifdef WIN32
				sprintf_s ( cpVerIndex, 128, "%d ", pVerTmp->m_nvIndex );
			#else
				sprintf ( cpVerIndex, "%d ", pVerTmp->m_nvIndex );
			#endif
			pFile.write ( cpVerIndex, (int) strlen ( cpVerIndex ) );

			pVerTmp		= pVerTmp->pNext_on_edge;
		}
		pFile.write ( "\n", 1 );
		pEdgeTmp		= pEdgeTmp->m_pENext;
	}
}

CHypergraph::CHypergraph (  int index, int iCnt_ver, int iCnt_edge, LPVERTEX pVer, LPEDGE pEdge )
{
	m_Index			= index;
	m_iCnt_Ver		= iCnt_ver;
	m_iCnt_Edge		= iCnt_edge;
	m_pVertex		= pVer;
	m_pEdge			= pEdge;
	m_pGraphNext	= NULL;
}

void CHypergraph::outputHypergraph ( string fileName, LPHYPERGRAPH pGraph )
{
	LPHYPERGRAPH	pGraphTmp;
	LPEDGE			pEdgeTmp;
	LPVERTEX		pVertexTmp;
	LPVERTEX		pVerTmp_2;
	/*int				iCnt_vertex;
	int				iCnt_edges;*/

	char			cpCount [128];
	//char			cpeIndex [128];
	char			cpvIndex [128];
	//char			cpGraph [128];
	char			cpvWeight [128];
	char			cpeWeight [128];

	ofstream		pFile;

	/*iCnt_vertex		= 0;
	iCnt_edges		= 0;*/

	pFile.open ( fileName.c_str() );

	if ( pGraph )
	{
		pGraphTmp	= pGraph;
		while ( pGraphTmp )
		{
			// 输出第一行
			#ifdef WIN32
				sprintf_s ( cpCount, 128, "%d %d ", pGraphTmp->m_iCnt_Ver, pGraphTmp->m_iCnt_Edge );
			#else 
				sprintf ( cpCount, "%d %d ", pGraphTmp->m_iCnt_Ver, pGraphTmp->m_iCnt_Edge );
			#endif
			pFile.write ( cpCount, (int) strlen ( cpCount ) );
			pFile.write ( "\n", 1 );
			// 输出超图细节
			pEdgeTmp	= pGraphTmp->m_pEdge;
			while ( pEdgeTmp )
			{	
				#ifdef WIN32
					sprintf_s( cpeWeight, 128, "%d ", pEdgeTmp->m_eWeight );
				#else
					sprintf( cpeWeight, "%d ", pEdgeTmp->m_eWeight );
				#endif
				pFile.write ( cpeWeight, (int) strlen ( cpeWeight ) );
				pVertexTmp	= pEdgeTmp->m_pvHead_on_edge;
				while ( pVertexTmp )
				{
					#ifdef WIN32
						sprintf_s ( cpvIndex, 128, "%d ", pVertexTmp->m_nvIndex );
					#else
						sprintf ( cpvIndex, "%d ", pVertexTmp->m_nvIndex );
					#endif
					pFile.write ( cpvIndex, (int) strlen ( cpvIndex ) );
					pVertexTmp	= pVertexTmp->pNext_on_edge;
				}
				pFile.write ( "\n", 1 );
				pEdgeTmp	= pEdgeTmp->m_pNext_on_graph;
			}
			// 换行后输出点的权重列表
			pVerTmp_2	= pGraphTmp->m_pVertex;
			while ( pVerTmp_2 )
			{
				#ifdef WIN32
					sprintf_s ( cpvWeight, 128, "%d ", pVerTmp_2->m_vWeight );
				#else
					sprintf ( cpvWeight, "%d ", pVerTmp_2->m_vWeight );
				#endif
				pFile.write ( cpvWeight, (int) strlen ( cpvWeight ) );
				pVerTmp_2		= pVerTmp_2->m_pvNext;
				pFile.write ( "\n", 1 );
			}
			pFile.write ( "\n", 1 );
			pGraphTmp		= pGraphTmp->m_pGraphNext;
		}
	}
}
