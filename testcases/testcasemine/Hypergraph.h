/*
 * Project------ CArchitecture
 * Class Name--- Hypergraph.h
 * Author------- wxf891201@gmail.com
 * Date--------- 2013-4-8
 * Edition------ 1.0

 * Description-- 超图建模。遍历CSimplex类中得到的classes链表，生成顶点。遍历类间关系得到超边。从CSimplex类中得到的关系生成边，根据顶点和超边建立超图
 *		

 * Change Log:
 *		Date-----
 *		Staff----
 *		Edition--
 *		Content--
*/
#ifndef	HYPERGRAPH_H
#define	HYPERGRAPH_H

#include "Simplex.h"
#include <string.h>
struct stEdge
{
	LPVERTEX	pVertex;				// 某条边上点
	LPSTEDGE	pNext;					// 下一个有多个顶点的边

	stEdge ( LPCLASS pClass );
	~stEdge ( );
};

class CVertex
{
public:
	int			m_nvIndex;				// 点的索引
	LPCLASS		m_pvHead;			 
	LPVERTEX	m_pvNext;				// 用于连接所有点的下一指针
	LPVERTEX	pNext_on_edge;			// 超边上的下一个顶点

	int			m_vWeight;				// 顶点权重，默认为1
	//int			m_vTotal;				// 顶点个数

public:
	CVertex ( int vIndex, LPCLASS pClass );
	~CVertex ( );

	void outputVertexToFile ( string fileName, LPVERTEX pVertex );
};
class TestCase
{
    int a;
    int b;
};
class CEdge
{
public:
	int			m_neIndex;				// 边的索引
	int			m_ivCnt;				// 一条边上点的计数

	LPVERTEX	m_pvHead_on_edge;		// 边上点的链表头指针
	LPVERTEX	m_pvTail_on_edge;		// 尾指针

	//LPEDGE		m_pEHead;
	LPEDGE		m_pENext;				// 用于链接每条边的下一指针
	LPEDGE		m_pNext_on_graph;		// 用于链接超图中的下一条超边

	int			m_eWeight;				// 边的权重，默认为1
	//int			m_eTotal;				// 超边条数

public:
	CEdge ( int eIndex, LPVERTEX pVertex );
	~CEdge ( );

	void	addVertex ( LPVERTEX pVertex );
	void	outputEdgesToFile ( string fileName, LPEDGE pEdge );
};

class CHypergraph
{
public:
	LPVERTEX		m_pVertex;
	LPEDGE			m_pEdge;
	int				m_Index;
	int				m_iCnt_Ver;
	int				m_iCnt_Edge;

	LPHYPERGRAPH	m_pGraphNext;

public:
	CHypergraph ( int index, int iCnt_ver, int iCnt_edge, LPVERTEX pVer, LPEDGE pEdge );
	~CHypergraph ( ) { }

	void	outputHypergraph ( string fileName, LPHYPERGRAPH pGraph );
};

#endif


