//************************************
// PARCS output analysis code
// AUTHOR: DOAN QUANG TUYEN
//************************************

#ifndef TYPE_H
#define TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <iostream>
#include "string.h"
#include <cstring>
#include "math.h"
#include "stdio.h" 
#include <stdlib>

#include "TApplication.h"  
#include "TAttFill.h"
#include "TAttLine.h"
#include "TAxis.h"
#include "TAttMarker.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TStyle.h"
#include "THStack.h"
#include "TColor.h"
#include "TExec.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TPave.h"
#include "TLegend.h"
#include "TMath.h"
#include "TCanvas.h"

#define MAX_COLOR 100 

#define MAX_AXIAL_NODE 100 
#define MAX_AXIAL_POWER 1.7 // 
#define MAX_RADIAL_POWER 2.2 // 

#define MAX_LINEBUF 1000

#define MAX_CORE_DIM_X 17
#define MAX_CORE_DIM_Y 17
#define MAX_ASSEMBLY_NB 300
#define MAX_CTRLROD_NB 17*17
#define MAX_ASSEMBLY_TYPE 10

using namespace std;

typedef struct 
{
  float val;
  int i;
} tLineElement;

typedef struct 
{
  int ID;
  float size;
  float pos;
  float pow;
} tAxialNode;

typedef struct 
{
	int ID;
	int i;
	int j;
	float length;
	float pos;
} tCtrlRod;

typedef struct 
{
	int ID;
	float pos;
} tCtrlRodPos;

typedef enum 
  {
    INNER,
    OUTER
  } tPos;
  
typedef struct 
{
	int ID;
	float pow;
	int i;
	int j;
	tPos pos;
} tRod;

typedef enum 
  {
    VVER,
    AP1000
  } tReactorType;
  
#ifdef __cplusplus
}
#endif

#endif
