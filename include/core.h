//************************************
// PARCS output analysis code
// AUTHOR: DOAN QUANG TUYEN, NGUYEN DINH QUEN, NGUYEN HOANG TU, NGUYEN DUC HA
// Reactor Physics Group, Nuclear Safety Division, VARANS
//************************************

#ifndef CORE_H
#define CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
#include "line.h"
#include "histo.h"
#include "assembly.h"
#include "ctrlRod.h"

using namespace std;


//-------tCore class------------------------------------------
class tCore{
	private:
			tLine *lineData;
			const Char_t *filename;
			tCtrlAssembly *ctrlAssembly;
			
			
	public:
		tCore(const Char_t *filenameIn);
		~tCore();
		
		tHisto *Histo;
		tAssembly *assembly;

		float *pow_axial;
		float *pos_axial;
		
		void readCoreConfig_(const Char_t *filename);
			
		void setHist_Core();
		void setSTackHist_Core();
		void draw_Core();
		
		void setHisto_CtrlRodPosition();	
		void drawHisto_CtrlRodPosition();
		void getCtrlRod_Pos(string line);
		
		void setSTackHist_Core_with_CtrlRod();
		
		void setHisto_ROD_outer();
		void draw_ROD_outer();		
		
		void setHisto_ROD_outer_AxialNodes();
		void draw_ROD_outer_AxialNodes();
		void update_ROD_outer_AxialNodes(float *tabPow, int tabSize,  float tabMaxValue);
		
		void drawHisto_Core_within_CtrlRodPosition();	
		void updateHisto_Core_within_CtrlRodPosition();		
		
		void getPower_radial(string line);
		void setHisto_ROD_power_radial();
		void draw_ROD_power_radial(const char* option);
				
		void getPower_axial(string line);
		void draw_ROD_power_axial(TGraph *toto);
		
		bool readout_pow_axial; 
		bool readout_pow_radial; 
		bool readout_ctrlRod; 
		
		void readOutModeReset();
		bool isReadOut();
		
};

tCore::tCore(const Char_t *filenameIn)
{
	cout<<"tCore: create memory"<<endl;
	int i =0;
	filename = filenameIn;
	
	Histo = new tHisto;
	
	lineData = new tLine;
	
	pow_axial = new float[MAX_AXIAL_NODE];
	pos_axial = new float[MAX_AXIAL_NODE];
	for(int i = 0; i< MAX_AXIAL_NODE; i++){
		pow_axial[i] = 0;
		pos_axial[i] = 0;
	}

	assembly = new tAssembly(filename);
	cout<<"tCore: Total number of Axial Node: "<<assembly->axialNode_Nb<<"  Core height: "<<assembly->height<<" cm"<<endl;
	
	readCoreConfig_(filename);
	
	setHist_Core();
	setSTackHist_Core();
	
	ctrlAssembly = new tCtrlAssembly(filename, assembly,assembly->height);
	
	setHisto_CtrlRodPosition();
	setSTackHist_Core_with_CtrlRod();
	
	setHisto_ROD_outer();
	setHisto_ROD_outer_AxialNodes();
	
	readOutModeReset();
}

tCore::~tCore()
{
	cout<<"tCore: free memory"<<endl;
	delete lineData;
	delete filename;
	delete ctrlAssembly;
	delete Histo;
	delete assembly;
}
//-------------------------------------------------------------------------------------------------------



//-----------read core configuration --------------------------------------------------------------------
void tCore::readCoreConfig_(const Char_t *filename)
{

	ifstream input; 
	input.open(filename);
	string line;

		std::string begin_block("RAD_CONF");
		std::string end_block_vver("GRID_");
		std::string end_block_ap1000("Assembly mesh spacing along x-axis");
		
	bool inBlock = false;
	// bool searchNbAxialNode = true; 

	if(!input.is_open()){
		cout<<"************************************************"<<endl;
		cout<<"************ Error opening file !! *******************"<<endl;
		cout<<"************ File name incorrect !! *******************"<<endl;
		cout<<"************************************************"<<endl;
		return NULL;
	}		
	int i = 0;
	int j = 0;
	int lineID = 0;
	int ref_i =0;
	int min_hist_i = 10000000;
	assembly->nb = 0;
	bool isFinished = false;
	while(!input.eof() && !isFinished)
	{
			string tmp;
			getline(input,line);
			
			tmp = ToUpper(line);
			if(line.length() > 1 ){
				if (std::string::npos != tmp.find(begin_block)){
					inBlock = true;
					lineID =0;
					ref_i = 0;
					cout<<"------------------------------"<<endl;
				}else if ( (std::string::npos != tmp.find(end_block_vver)) ||
							(std::string::npos != line.find(end_block_ap1000))){
					inBlock = false;
					isFinished = true;
					cout<<"------------------------------"<<endl;
				}else if(inBlock){
					cout<<line<<endl;
					int nbData = lineData->readLine_(line);
					int lineLengh = 0;
					if(lineID == 0){ref_i = int(lineData->line_buf[0].i);}
					for(i = 0; i <nbData; i++){
						if(int(lineData->line_buf[i].val) >0){
							lineLengh++;
							assembly->bank[assembly->nb].ID = int(lineData->line_buf[i].val);
							assembly->bank[assembly->nb].i = (lineData->line_buf[i].i - ref_i);
							assembly->bank[assembly->nb].j = lineID;
							assembly->maxAssemblyID = (assembly->bank[assembly->nb].ID > assembly->maxAssemblyID)?assembly->bank[assembly->nb].ID:assembly->maxAssemblyID;
								 bool foundNewAssemblyType = true;
								for(j = 0; j <assembly->nbAssemblyType; j++){
									if(assembly->assemblyType[j] == assembly->bank[assembly->nb].ID){
										foundNewAssemblyType = false;
									}
								}
								if(foundNewAssemblyType){
									assembly->assemblyType[assembly->nbAssemblyType] = assembly->bank[assembly->nb].ID;
									assembly->nbAssemblyType++; 
								} 
									
							if(lineLengh == 1){assembly->bank[assembly->nb].pos = OUTER;} //identify outer assemblies
							int tmp_i = (lineData->line_buf[i].i - ref_i);
							min_hist_i = (min_hist_i < tmp_i)?min_hist_i:tmp_i;
							assembly->nb++;
						}
					}
					assembly->bank[assembly->nb -1].pos = OUTER;
				lineID++;
				} 
			}
	}
	input.close();
	cout<<"-----------------------------------------------------------------"<<endl;
	cout<<"tCore: Total number of Assembly: "<<assembly->nb<<"   Total number of rod type: "<<assembly->nbAssemblyType<<"     max Rod ID: "<<assembly->maxAssemblyID<<endl;
	cout<<"tCore: Total number of Axial Node: "<<assembly->axialNode_Nb<<"  Core height: "<<assembly->height<<" cm"<<endl;
	cout<<"-----------------------------------------------------------------"<<endl;
	
	for(i = 0; i <assembly->nb; i++){
		assembly->bank[i].i -= min_hist_i; 
		assembly->bank[i].j = lineID -1 - assembly->bank[i].j; 
	}
	
	assembly->getOuterAssembly();
}
//------------------------------------------------------------------------------------------------------------








//--------- core configuration -------------------------------------------------------------------------------------------
void tCore:: setHist_Core()
{
	int i=0;
	int j=0;
	for(j = 0; j <assembly->nbAssemblyType; j++){
		for(i = 0; i <assembly->nb; i++){
			if(assembly->bank[i].ID == assembly->assemblyType[j]){
				Histo->setHist(Histo->hist_ROD_Assembly[j], assembly->bank[i].i, assembly->bank[i].j, assembly->height);
			}
		}
		//set colore for different rod type
	//	int colorIdx = int(float((assembly->assemblyType[j]-1)*(MAX_COLOR-1))/(assembly->maxAssemblyID-1)); //red color for max value of rod ID
		int colorIdx = int(float((assembly->assemblyType[j])*(MAX_COLOR-1))/(assembly->maxAssemblyID)); //red color for max value of rod ID
		// int colorIdx = int(float((assembly->maxAssemblyID-assembly->assemblyType[j])*(MAX_COLOR-1))/(assembly->maxAssemblyID-1)); //red color for min value of rod ID
		Histo->hist_ROD_Assembly[j]->SetFillColor(Histo->colors[colorIdx]);	
	}
	
}
void tCore:: setSTackHist_Core()
{
	int j=0;	
	Histo->hist_ROD_Assembly[assembly->nbAssemblyType]->SetFillColor(0);
	(Histo->Stack_Hist_Core)->Add(Histo->hist_ROD_Assembly[assembly->nbAssemblyType]);
	for(j = 0; j <assembly->nbAssemblyType; j++){
		(Histo->Stack_Hist_Core)->Add(Histo->hist_ROD_Assembly[j]);
	}
	
	//init core_with_CtrlRod stack histogram by core configuration
	(Histo->Stack_Hist_Core_with_CtrlRod)->Add(Histo->hist_ROD_Assembly[assembly->nbAssemblyType]);
	for(j = 0; j <assembly->nbAssemblyType; j++){
		(Histo->Stack_Hist_Core_with_CtrlRod)->Add(Histo->hist_ROD_Assembly[j]);
	}
}
void tCore:: draw_Core()
{
	(Histo->Stack_Hist_Core)->Draw();
	Histo->setHistoAxis_XY(Stack_Hist_Core);
}
//-----------------------------------------------------------------------------------------------------------------------






//----------Control Rod configuration --------------------------------------------------------------------------
void tCore::getCtrlRod_Pos(string line)
{
	ctrlAssembly->getCtrlRod_Pos(line);
	if(ctrlAssembly->ctrlBankPosBlock_readOut){
		readout_ctrlRod = true;
	}
}
void tCore:: setHisto_CtrlRodPosition()
{
	int i=0;
	for(i =0; i< ctrlAssembly->nb; i++){
		Histo->setHistRod(Histo->hist_ROD_ctrl, ctrlAssembly->bank[i].i, ctrlAssembly->bank[i].j, 1.0 + ctrlAssembly->bank[i].pos);
	}
}
void tCore:: drawHisto_CtrlRodPosition()
{
	(Histo->hist_ROD_ctrl)->Draw("lego2");
}
//-----------------------------------------------------------------------------------------------------------------









//--------- core configuration with CtrlRod -------------------------------------------------------------
void tCore:: setSTackHist_Core_with_CtrlRod()
{
	(Histo->Stack_Hist_Core_with_CtrlRod)->Add(Histo->hist_ROD_ctrl);
}
void tCore:: drawHisto_Core_within_CtrlRodPosition()
{
	(Histo->Stack_Hist_Core_with_CtrlRod)->Draw();
	Histo->hist_ROD_ctrl->SetFillColor(6);
	(Histo->Stack_Hist_Core_with_CtrlRod)->SetMaximum(2*assembly->height);
	(Histo->Stack_Hist_Core_with_CtrlRod)->SetTitle("Reactor Core Configuration with Control Rod");
	Histo->setHistoAxis_XY(Histo->Stack_Hist_Core_with_CtrlRod);
}
void tCore:: updateHisto_Core_within_CtrlRodPosition()
{
	(Histo->Stack_Hist_Core_with_CtrlRod)->RecursiveRemove(Histo->hist_ROD_ctrl);
	
	setHisto_CtrlRodPosition();
	setSTackHist_Core_with_CtrlRod();
	
	(Histo->Stack_Hist_Core_with_CtrlRod)->Draw();
	
	Histo->hist_ROD_ctrl->SetFillColor(6);
	(Histo->Stack_Hist_Core_with_CtrlRod)->SetMaximum(2*assembly->height);
	(Histo->Stack_Hist_Core_with_CtrlRod)->SetTitle("Reactor Core Configuration with Control Rod");
	Histo->setHistoAxis_XY(Histo->Stack_Hist_Core_with_CtrlRod);
}
//---------------------------------------------------------------------------------------------------------------






//------- outer rod ---------------------------------------------------------------------------------------------
void tCore:: setHisto_ROD_outer()
{
	for(int i = 0; i <assembly->nb; i++){
		if(assembly->bank[i].pos == OUTER){
			Histo->setHist(Histo->hist_ROD_outer, assembly->bank[i].i, assembly->bank[i].j, assembly->height);
		}
	} 
}
void tCore::draw_ROD_outer()
{
	Histo->hist_ROD_outer->Draw("lego2");
}
//---------------------------------------------------------------------------------------------------------------








//-------------- Axial Nodes --------------------------------------------------------------------------------------
void tCore:: setHisto_ROD_outer_AxialNodes()
{
	int i=0;
	int m=0;
	int n=0;
	
	Histo->hist_axial[assembly->axialNode_Nb]->SetFillColor(0);
	Histo->Stack_Hist_Core_AxialNode->Add(Histo->hist_axial[assembly->axialNode_Nb]);
	
	for(i =0; i<assembly->axialNode_Nb; i++){
		for(m =0; m<2*MAX_CORE_DIM_X; m++){
			for( n =0; n<2*MAX_CORE_DIM_Y; n++){
				if(Histo->hist_ROD_outer->GetBinContent(m,n) > 0){
					Histo->hist_axial[i]->SetBinContent(m,n,assembly->axialNode[i].size);
				}
			}
		}
		int idx = int((assembly->axialNode[i].pos/assembly->height)*MAX_COLOR);
		Histo->hist_axial[i]->SetFillColor(Histo->colors[idx]);
		Histo->Stack_Hist_Core_AxialNode->Add(Histo->hist_axial[i]);
	}
}
//update color of outer rod using tab of axial power distribution
void tCore:: update_ROD_outer_AxialNodes(float *tabPow, int tabSize,  float tabMaxValue)
{
	int i =0;
	for(i =0; i<tabSize; i++){
		int idx = int((tabPow[i]/tabMaxValue)*(MAX_COLOR-1));
		Histo->hist_axial[i]->SetFillColor(Histo->colors[idx]);
	}
}
// draw outer fuel assembly within axial nodes -------------------
void tCore:: draw_ROD_outer_AxialNodes()
{	
	for(int i = 0; i< assembly->axialNode_Nb; i++){
		(Histo->Stack_Hist_Core_AxialNode)->RecursiveRemove(Histo->hist_axial[i]);
	}
	update_ROD_outer_AxialNodes(pow_axial, assembly->axialNode_Nb, MAX_AXIAL_POWER);	
	for(int i = 0; i< assembly->axialNode_Nb; i++){
		(Histo->Stack_Hist_Core_AxialNode)->Add(Histo->hist_axial[i]);
	}
	(Histo->Stack_Hist_Core_AxialNode)->Draw();
	Histo->setHistoAxis_XY(Histo->Stack_Hist_Core_AxialNode);
}
//---------------------------------------------------------------------------------------------------------------






//--------get/reset readout mode --------------------
bool tCore::isReadOut()
{
	return (readout_pow_axial && readout_pow_radial &&	readout_ctrlRod); 
}
void tCore::readOutModeReset()
{
	readout_pow_axial = false; 
	readout_pow_radial = false; 
	readout_ctrlRod = false; 
}

//--------get radial power ---------------------------------------------------------------------------------------------------
void tCore::getPower_radial(string line)
{
	assembly->getPower_radial(line);
	if(assembly->assemblyRadialPowerBlock_readOut){
		readout_pow_radial = true;
	}
}
void tCore:: setHisto_ROD_power_radial()
{
	for(int i = 0; i <assembly->nb; i++){
		Histo->setHist(Histo->hist_ROD_power, assembly->bank[i].i, assembly->bank[i].j, assembly->bank[i].pow);
	} 
}
void tCore::draw_ROD_power_radial(const char* option)
{
	setHisto_ROD_power_radial();
	Histo->hist_ROD_power->Draw(option);
}
//---------------------------------------------------------------------------------------------------------------


//--------get axial power ---------------------------------------------------------------------------------------------------
void tCore::getPower_axial(string line)
{
	assembly->getPower_axial(line);
	for(int i = 0; i< assembly->axialNode_Nb; i++){
		pow_axial[i] = assembly->axialNode[i].pow;
		pos_axial[i] = assembly->axialNode[i].pos;
	}
	if(assembly->assemblyAxialPowerBlock_readOut){
		readout_pow_axial = true;
	}
}
void tCore::draw_ROD_power_axial(TGraph *graph)
{
	(*graph) = TGraph(assembly->axialNode_Nb,pow_axial,pos_axial);
	graph ->SetTitle("Axial Power Distribution");
	graph->Draw("AL*");
}
//---------------------------------------------------------------------------------------------------------------



#ifdef __cplusplus
}
#endif

#endif
