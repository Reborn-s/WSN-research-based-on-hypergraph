
#include "CHypergraph.h"
#include <iostream>
#include <cstdlib>

using namespace std;


CHypergraph::CHypergraph()
{
	coupling = 0;
	cohesion = 0;
	leftSon = NULL;
	rightSon = NULL;
	fatherGraph = NULL;
}
CHypergraph::CHypergraph(CInput* cDataIn)
{
	coupling = 0;
	cohesion = 0;
	leftSon = NULL;
	rightSon = NULL;
	fatherGraph = NULL;
	nvtxs = cDataIn->get_nvtxs();
	nhedges = cDataIn->get_nhedges();
	vwgts = cDataIn->get_vwgts();
	eptr = cDataIn->get_eptr();
	eind = cDataIn->get_eind();
	hewgts = cDataIn->get_hewgts();
	npart = cDataIn->get_nparts();
	ubfactor = cDataIn->get_ubfactor();
	part = cDataIn->get_part();
	inOptions = cDataIn->get_option();
	is_parted = false;
}
CHypergraph::CHypergraph(int n)
{
	nvtxs=n;
	nhedges=0;
	vwgts=NULL;
	eptr=NULL;
	eind=NULL;
	hewgts=NULL;
	is_parted = false;
}


