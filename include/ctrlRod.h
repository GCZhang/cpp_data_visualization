//************************************
// PARCS output analysis code
// AUTHOR: DOAN QUANG TUYEN
//************************************

#ifndef CTRLROD_H
#define CTRLROD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
#include "line.h"
#include "assembly.h"

//-------------------------------------------------
class tCtrlAssembly{
	public:
		tCtrlAssembly(const Char_t *filenameIn, tAssembly *assemblyIn, float lengthIn);
		~tCtrlAssembly();
		
		tCtrlRod *bank;
		int nb;
		float length;
		
		tAssembly *assembly;
		const Char_t *filename;
		
		int nbCtrlType;
		tCtrlRodPos *ctrlType;
		
		
		bool ctrlBankPosBlock_in;
		bool ctrlBankPosBlock_end;
		int  ctrlBankPosBlock_readLineNb;
		bool ctrlBankPosBlock_readOut;
		
		bool ctrlBankInitPosBlock_in;
		bool ctrlBankInitPosBlock_end;
		int ctrlBankInitPosBlock_readLineNb;
		int ctrlBankPosBlock_headNb;

		void getCtrlRod_config();
		void setCtrlRod_Pos();
		bool getCtrlRod_InitPos(string lineIn);
		void getCtrlRod_Pos(string line);
		
	private:
		tLine *lineData;
		
};


tCtrlAssembly::tCtrlAssembly(const Char_t *filenameIn, tAssembly *assemblyIn, float lengthIn)
{
	cout<<"tCtrlAssembly: create memory"<<endl;
	int i =0;
	length = lengthIn;
	assembly = assemblyIn;
	filename = filenameIn;
	
	bank = new tCtrlRod[MAX_CTRLROD_NB];
	nb =0; 
	for(i=0; i< MAX_CTRLROD_NB; i++){
		bank[i].ID = 0;
		bank[i].i = 0;
		bank[i].j = 0;
		bank[i].length = length;
		bank[i].pos = 0;
	 }
	 ctrlBankInitPosBlock_in = false;
	 ctrlBankInitPosBlock_in = false;
	 ctrlBankInitPosBlock_readLineNb = 0;
	 
	 ctrlBankPosBlock_in = false;
	 ctrlBankPosBlock_end = false;
	 ctrlBankPosBlock_readOut = false;
	 ctrlBankPosBlock_readLineNb = 0;
	 ctrlBankPosBlock_headNb = 0;
	 
	 lineData = new tLine;
	 
	nbCtrlType =0;
	ctrlType = new tCtrlRodPos[MAX_CTRLROD_NB];
	for(i=0; i< MAX_CTRLROD_NB; i++){
		ctrlType[i].ID = -1;
		ctrlType[i].pos = -1.0;
	}
	
	
	
	getCtrlRod_config();
}

tCtrlAssembly::~tCtrlAssembly()
{
	cout<<"tCtrlAssembly: free memory"<<endl;
	delete bank;
	delete lineData;
	delete ctrlType;
}




//get initial position of control rods
bool tCtrlAssembly::getCtrlRod_InitPos(string line)
{ 
	int i=0;
		string begin_block = "Initial control bank position";
		string end_block = "0 means fully inserted";
			if(line.length() > 1 ){
				if (std::string::npos != line.find(begin_block)){
					ctrlBankInitPosBlock_in = true;
					ctrlBankInitPosBlock_end = false;
					ctrlBankInitPosBlock_readLineNb = 0;
				}else if (std::string::npos != line.find(end_block)){
					ctrlBankInitPosBlock_in = false;
					ctrlBankInitPosBlock_end = true;
					cout<<"tCtrlAssembly: nb of control rod Type: "<<nbCtrlType<<endl;
				}else if(ctrlBankInitPosBlock_in){
					int nbData = lineData->readLine_(line);
					if(ctrlBankInitPosBlock_readLineNb > 0){
						for(i = 0; i <nbData; i++){
							if(lineData->line_buf[i].val < 100000000){
								ctrlType[i].ID = i+1;
								ctrlType[i].pos = lineData->line_buf[i].val;
							}
						}
						nbCtrlType = nbData;
					}
					ctrlBankInitPosBlock_readLineNb++;
				} 
			}	
return ctrlBankInitPosBlock_end;
}


//get control rod configuration (position and initial position)---------------------------------------------
void tCtrlAssembly::getCtrlRod_config()
{
	ifstream input; 
	input.open(filename);
	string line;

		std::string begin_block("Control Bank Configuration");
		std::string end_block("Assemblies Belonging to Each Bank");
		
	bool inBlock = false;
	bool searchCtrlRod_InitPos = true;
	
	if(!input.is_open()){
		cout<<"************************************************"<<endl;
		cout<<"tCtrlAssembly: ************ Error opening file !! *******************"<<endl;
		cout<<"tCtrlAssembly: ************ File name incorrect !! *******************"<<endl;
		cout<<"************************************************"<<endl;
		return NULL;
	}	
	
	int i = 0;
	int j = 0;
	int assembly_pos = 0;
	bool isFinished = false;
	while(!input.eof() && !isFinished)
	{
			string tmp;
			getline(input,line);
			if(line.length() > 1 ){
				if (std::string::npos != line.find(begin_block)){
					inBlock = true;
				}else if (std::string::npos != line.find(end_block)){
					inBlock = false;
					if(!searchCtrlRod_InitPos){ //stop reading when the function getCtrlRod_InitPos has been done
						isFinished = true;
					}
				}else if(inBlock){
					// cout<<line<<endl;
					int nbData = lineData->readLine_(line);
					for(i = 0; i <nbData; i++){
						if(int(lineData->line_buf[i].val) >0){
							bank[nb].ID = int(lineData->line_buf[i].val);
							bank[nb].i = assembly->bank[assembly_pos].i;
							bank[nb].j = assembly->bank[assembly_pos].j;
							nb++;
						}
						assembly_pos++;
					}
				} 
			}
			if(searchCtrlRod_InitPos){
				searchCtrlRod_InitPos = !(getCtrlRod_InitPos(line));
			}
	}
	
	
	setCtrlRod_Pos();
	input.close();
	cout<<"tCtrlAssembly: nb of control rod: "<<nb<<endl;
}

//set to control rod bank
void tCtrlAssembly::setCtrlRod_Pos()
{
	int i = 0;
	int j = 0;
	//set position of Control rod
	for(i=0; i<nbCtrlType; i++){
		for(j=0; j< nb; j++){
			if(bank[j].ID == ctrlType[i].ID){
				bank[j].pos = ctrlType[i].pos;
			}
		}
	}
}

// update control rod positions, used for transient calculation
void tCtrlAssembly::getCtrlRod_Pos(string line)
{ 
	int i=0;
		string begin_block = "Core Condition Change Summary";
		string head_block = "Boron";
			if(line.length() > 1 ){
				if (!(std::string::npos == line.find(begin_block))){
					ctrlBankPosBlock_in = true;
					ctrlBankPosBlock_readOut = false;
					ctrlBankPosBlock_readLineNb = 0;
					ctrlBankPosBlock_headNb = 0;
				}else if ( ctrlBankPosBlock_readLineNb == nbCtrlType){
					ctrlBankPosBlock_in = false;
					ctrlBankPosBlock_readOut = true;
					ctrlBankPosBlock_readLineNb = 0;
					ctrlBankPosBlock_headNb = 0;
					
					setCtrlRod_Pos();
				}else if(ctrlBankPosBlock_in){
					if(std::string::npos != line.find(head_block)){
						ctrlBankPosBlock_headNb++;
					}else if(ctrlBankPosBlock_headNb == 2){
						int nbData = lineData->readLine_(line);
							ctrlType[ctrlBankPosBlock_readLineNb].ID = int(lineData->line_buf[1].val);
							ctrlType[ctrlBankPosBlock_readLineNb].pos = lineData->line_buf[nbData-1].val;
							ctrlBankPosBlock_readLineNb++;
					}
				}else{ctrlBankPosBlock_readOut = false;}
				
			}	
}



#ifdef __cplusplus
}
#endif

#endif
