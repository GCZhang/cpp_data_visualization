//************************************
// PARCS output analysis code
// AUTHOR: DOAN QUANG TUYEN
//************************************

#ifndef HISTO_H
#define HISTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
	

//------------------------------------------------
class tHisto{
	public:
		tHisto();
		~tHisto();		
		
		template <class anyType>
		void setHist(TH2F *hist, int i, int j, anyType ID);
		
		template <class anyType>
		void setHistoAxis_XY(anyType* hist);
		
		template <class anyType>
		void setHistoAxis_Z(anyType* hist, const char* title);
		
		template <class anyType>
		void setHistRod(TH2F *hist, int i, int j, anyType val);

		
		Int_t  *colors;
		TH2F **hist_ROD_Assembly;
		TH2F **hist_axial;
		TH2F *hist_ROD_ctrl;
		TH2F *hist_ROD_power;
		TH2F *hist_ROD_outer;
		TGraph axial_power; 
		
		THStack *Stack_Hist_Core;
		THStack *Stack_Hist_Core_with_CtrlRod;
		THStack *Stack_Hist_Core_AxialNode;
		
};




tHisto::tHisto()
{
	cout<<"tHisto: create memory"<<endl;
	int i = 0;
		colors = new Int_t[MAX_COLOR];
	   Double_t tab_Red[3]    = { 0, 0.00, 1.00};
	   Double_t tab_Green[3]  = { 0, 1.00, 0.00};
	   Double_t tab_Blue[3]   = { 1.0, 0.00, 0.00};
	   Double_t tab_Length[3] = { 0, 0.5, 1.0 };
	   Int_t FI = TColor::CreateGradientColorTable(3,tab_Length,tab_Red,tab_Green,tab_Blue,MAX_COLOR);
		for (i=0; i< MAX_COLOR; i++){
			colors[i] = FI + i;
		}  
				
		hist_axial = new TH2F*[MAX_AXIAL_NODE];
		for(i =0; i<MAX_AXIAL_NODE; i++){
			string toto = "hist_axial_";
			toto += i;
			hist_axial[i] = new TH2F(toto.c_str(),toto.c_str(),2*MAX_CORE_DIM_X,-0,MAX_CORE_DIM_X,2*MAX_CORE_DIM_Y,0,MAX_CORE_DIM_Y);
			setHistoAxis_XY(hist_axial[i]);
			setHistoAxis_Z(hist_axial[i],"Height (cm)");
		}
		
		hist_ROD_Assembly = new TH2F*[MAX_ASSEMBLY_TYPE];
		for(i =0; i<MAX_ASSEMBLY_TYPE; i++){
			string toto = "hist_ROD_Assembly_";
			toto += i;
			hist_ROD_Assembly[i] = new TH2F(toto.c_str(),toto.c_str(),2*MAX_CORE_DIM_X,-0,MAX_CORE_DIM_X,2*MAX_CORE_DIM_Y,0,MAX_CORE_DIM_Y);
			setHistoAxis_XY(hist_ROD_Assembly[i]);
			setHistoAxis_Z(hist_ROD_Assembly[i],"Height (cm)");
		} 
				
		hist_ROD_ctrl = new TH2F("hist_ROD_ctrl","Control Bank Configuration",2*MAX_CORE_DIM_X,-0,MAX_CORE_DIM_X,2*MAX_CORE_DIM_Y,0,MAX_CORE_DIM_Y);
		setHistoAxis_XY(hist_ROD_ctrl);
		setHistoAxis_Z(hist_ROD_ctrl,"Height (cm)");
		
		hist_ROD_power = new TH2F("hist_ROD_power","Radial Power Distribution",2*MAX_CORE_DIM_X,-0,MAX_CORE_DIM_X,2*MAX_CORE_DIM_Y,0,MAX_CORE_DIM_Y);
		setHistoAxis_XY(hist_ROD_power);
		setHistoAxis_Z(hist_ROD_power,"Height (cm)");
		hist_ROD_power->SetMaximum(MAX_RADIAL_POWER);
		hist_ROD_outer = new TH2F("hist_ROD_outer","FAs at boundary",2*MAX_CORE_DIM_X,-0,MAX_CORE_DIM_X,2*MAX_CORE_DIM_Y,0,MAX_CORE_DIM_Y);
		setHistoAxis_XY(hist_ROD_outer);
		setHistoAxis_Z(hist_ROD_outer,"Height (cm)");
		
		
		Stack_Hist_Core = new THStack("Stack_Hist_Core","Reactor Core Configuration");	
		Stack_Hist_Core_with_CtrlRod = new THStack("Stack_Hist_Core_with_CtrlRod","Reactor Core Configuration with CtrlRod");	
		Stack_Hist_Core_AxialNode = new THStack("Stack_Hist_Core_AxialNode","Reactor Axial Power Distribution");		
	
}

tHisto::~tHisto()
{
	 cout<<"tHisto: free memory"<<endl;
	
	if(colors!=NULL) {delete colors;}		
	if(hist_axial != NULL){
		for(int i =0; i<MAX_AXIAL_NODE; i++){
			 delete hist_axial[i];
		}
	}
	
	if(this->hist_ROD_Assembly != NULL){
		for(int i =0; i<MAX_ASSEMBLY_TYPE; i++){
		delete hist_ROD_Assembly[i];
		}
	}
	
	if(hist_ROD_power!=NULL) {delete hist_ROD_power;}
	if(hist_ROD_outer!=NULL) {delete hist_ROD_outer;}
	
}

template <class anyType>
void tHisto:: setHist(TH2F *hist, int i, int j, anyType val)
{
		hist-> SetBinContent(i+1  ,2*j+1,val);
		hist-> SetBinContent(i+1+1,2*j+1,val);
		hist-> SetBinContent(i+1  ,2*j+1+1,val);
		hist-> SetBinContent(i+1+1,2*j+1+1,val);
}

template <class anyType>
void tHisto:: setHistRod(TH2F *hist, int i, int j, anyType val)
{
		hist-> SetBinContent(i+1  ,2*j+1,val);
}


template <class anyType>
void tHisto::setHistoAxis_XY(anyType* hist)
{
	hist->GetXaxis()->SetTitle("FAs pitch");
	hist->GetYaxis()->SetTitle("FAs pitch");
	
	hist->GetYaxis()->SetTitleOffset(1.8);
	hist->GetYaxis()->SetTitleSize(0.03);
	hist->GetYaxis()->SetTitleFont(62);
	
	hist->GetXaxis()->SetTitleOffset(1.8);
	hist->GetXaxis()->SetTitleSize(0.03);
	hist->GetXaxis()->SetTitleFont(62);
}

template <class anyType>
void tHisto::setHistoAxis_Z(anyType* hist, const char* title)
{
	hist->SetStats(0);
	hist->GetZaxis()->SetTitle(title);
		
	hist->GetZaxis()->SetTitleOffset(1.8);
	hist->GetZaxis()->SetTitleSize(0.03);
	hist->GetZaxis()->SetTitleFont(62);
}

//------------------------------------------------





#ifdef __cplusplus
}
#endif

#endif
