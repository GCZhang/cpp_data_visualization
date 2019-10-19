//************************************
// PARCS output analysis code
// AUTHOR: DOAN QUANG TUYEN, NGUYEN DINH QUEN, NGUYEN HOANG TU, NGUYEN DUC HA
// Reactor Physics Group, Nuclear Safety Division, VARANS
//************************************

#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
#include "line.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

//-------------------------------------------------
class tAssembly{
	private:
		tLine *lineData;
		
		int assemblyRadialPowerBlock_readLineNb;
		bool assemblyRadialPowerBlock_in;
		
		bool assemblyAxialPowerBlock_in;
		bool assemblyAxialPowerBlock_readOut;
		
	public:
		int assemblyRadialPowerBlock_readAssemblyNb;
		tAssembly(const Char_t *filenameIn);
		~tAssembly();
		
		const Char_t *filename;
		
		tRod *bank;
		float *bank_tmp;
		int nb;
		tAxialNode *axialNode;
		int axialNode_Nb;
		int axialNode_Nb_tmp;
		float height;
		
		
		tReactorType reactorType;
		int maxAssemblyID;
		int nbAssemblyType;
		int *assemblyType;
		
		void getOuterAssembly();
		void getNode_axial(const Char_t *filename);
		void getPower_radial(string line);
		void getPower_axial(string line);
		float getPower(string line);
		float getTime(string line);
		float getReactivity(string line);
		bool assemblyRadialPowerBlock_readOut;
};

tAssembly::tAssembly(const Char_t *filenameIn)
{
	cout<<"tAssembly: create memory"<<endl;
	int i =0;
	
	filename = filenameIn;
	
	bank_tmp = new float[MAX_ASSEMBLY_NB];
	bank = new tRod[MAX_ASSEMBLY_NB];
	nb =0; 
	for(i=0; i< MAX_ASSEMBLY_NB; i++){
		bank_tmp[i] = 0;
		bank[i].ID = 0;
		bank[i].pow = 0;
		bank[i].i = 0;
		bank[i].j = 0;
		bank[i].pos = INNER;
	}
	
	height = 0;
	nbAssemblyType =0;
	maxAssemblyID = -1;
	assemblyType = new int[MAX_ASSEMBLY_TYPE];
	for(i=0; i< MAX_ASSEMBLY_TYPE; i++){
		assemblyType[i] = -1;
	}
	
	axialNode_Nb = 0;
	axialNode_Nb_tmp =0;
	axialNode = new tAxialNode[MAX_AXIAL_NODE];
	for(i=0; i< MAX_AXIAL_NODE; i++){
		axialNode[i].ID = 0;
		axialNode[i].size = 0;
		axialNode[i].pos = 0;
		axialNode[i].pow = 0;
	} 
	
	
	 lineData = new tLine;
	 assemblyRadialPowerBlock_readAssemblyNb = 0;
	 assemblyRadialPowerBlock_readLineNb = 0;
	 assemblyRadialPowerBlock_in = false;
	 assemblyRadialPowerBlock_readOut = false;
	 
	assemblyAxialPowerBlock_in = false;
	assemblyAxialPowerBlock_readOut = false;
		
	getNode_axial(filename);
	
}

tAssembly::~tAssembly()
{
	cout<<"tAssembly: free memory"<<endl;
	delete bank;
	delete assemblyType;
	delete pow_axial;
	delete pos_axial;
	delete lineData;
	delete bank_tmp;
}





// get total power ------------------------------------------------------------
float tAssembly::getReactivity(string line)
{ 
	float Reactivity;
	bool found = false;
	int blank = 0;
	bool readOut = false;
	
	string getStr;
		string begin_line = "Reactivity($):";
		if(line.length() > 1 && std::string::npos != line.find(begin_line)){
			readOut = true;
			lineData->readLine_(line);
			for (int i=lineData->line_buf[0].i; i<int(line.length()); i++)
			{
				if ( line.at(i) == ':' ){
					cout<<"toto"<<endl;
					found = true;
				}
				if(found){
					blank++;		
				}
				if(blank >= 2 && ( line.at(i) != ' ' )){
					getStr += line.at(i);
				}
			}
			Reactivity = (float)(std::atof(getStr.c_str()));
			cout<<"Reactivity"<<": "<<Reactivity<<"    ("<<blank<<")"<<endl;	
		}
 if(readOut) {
	return Reactivity;
 }else{
	return -1;
 }
}
//---------------------------------------------------------------------------------------------

// get total power ------------------------------------------------------------
float tAssembly::getPower(string line)
{ 
	float power_total;
	bool found = false;
	int blank = 0;
	bool readOut = false;
	string getStr;
		string begin_line = "Core Power Level";
		if(line.length() > 1 && std::string::npos != line.find(begin_line)){
			readOut = true;
			lineData->readLine_(line);
			for (int i=lineData->line_buf[0].i; i<int(line.length()); i++)
			{
				if ( line.at(i) == ':' ){
					found = true;
				}
				if(found){
					blank++;		
				}
				if(blank >= 2){
					getStr += line.at(i);
				}
			}
			power_total = (float)(std::atof(getStr.c_str()));
			cout<<"Power Level"<<": "<<power_total<<"    ("<<getStr<<")"<<endl;	
		}
 if(readOut) {
	return power_total;
 }else{
	return -1;
 }
}
//---------------------------------------------------------------------------------------------




// get time  ------------------------------------------------------------
float tAssembly::getTime(string line)
{ 
float time = 0;
	bool found = false;
	bool stop = false;
	int blank = 0;
	bool readOut = false;
		string begin_line = "At Simulation Time";
		if(line.length() > 1 && std::string::npos != line.find(begin_line)){
			readOut = true;
			lineData->readLine_(line);
			string getStr; 
			for (int i=lineData->line_buf[0].i; i<int(line.length()); i++)
			{
				if ( line.at(i) == '=' ){
					found = true;
				}
				if(found){
					blank++;		
				}
				if(line.at(i) == '('){
					stop = true;
				}
				if(blank >= 2 && (!stop)){
					getStr += line.at(i);
				}
			}
			time =  (float)(std::atof(getStr.c_str()));
			cout<<"time"<<": "<<time<<"s    ("<<getStr<<")"<<endl;	
		}
 if(readOut) {
	return time;
 }else{
	return -1;
  }
}
//---------------------------------------------------------------------------------------------





// get radial power distribution ------------------------------------------------------------
void tAssembly::getPower_radial(string line)
{ 
	int i=0;
		string begin_block = "Assembly Power Distribution";
		string head_block = "#.####   box power";
		if (line.length() > 1 && std::string::npos != line.find(begin_block)){
			assemblyRadialPowerBlock_in = true;
			assemblyRadialPowerBlock_readOut = false;
			assemblyRadialPowerBlock_readLineNb =0;
			assemblyRadialPowerBlock_readAssemblyNb = 0;
		}else if (line.length() < 2 && assemblyRadialPowerBlock_in && assemblyRadialPowerBlock_readAssemblyNb > 0){
			assemblyRadialPowerBlock_in = false;
			assemblyRadialPowerBlock_readOut = true;
			int cp = 0;
			if(reactorType == AP1000){//special case for AP1000
				for(i = 0; i <nb; i++){
					if(bank[i].ID != 1){
						bank[i].pow = bank_tmp[cp];
						bank_tmp[cp] = 0;
						cp++;
					}
				}
			}
		}else if(line.length() > 1 &&  assemblyRadialPowerBlock_in && (std::string::npos == line.find(head_block))){
			if(assemblyRadialPowerBlock_readLineNb > 0){
			int nbData = lineData->readLine_(line);	
				if(!(lineData->line_buf[1].val == 0 && lineData->line_buf[2].val == 0 && lineData->line_buf[3].val == 0)){
					if(reactorType == AP1000){//special case for AP1000
						for(i = 1; i <nbData; i++){
							bank_tmp[assemblyRadialPowerBlock_readAssemblyNb] = lineData->line_buf[i].val;
							assemblyRadialPowerBlock_readAssemblyNb++;
						}
					}else{
						for(i = 1; i <nbData; i++){
							bank[assemblyRadialPowerBlock_readAssemblyNb].pow = lineData->line_buf[i].val;
							assemblyRadialPowerBlock_readAssemblyNb++;
							}
						}		
					}else{
						assemblyRadialPowerBlock_readOut = false;
						assemblyRadialPowerBlock_readLineNb = 0;
						break;
					}
			}
			assemblyRadialPowerBlock_readLineNb++;
		}else{assemblyRadialPowerBlock_readOut = false;} 
}
//---------------------------------------------------------------------------------------------




// get nb axial node ------------------------------------------------------------
void tAssembly::getNode_axial(const Char_t *filename)
{ 
	int i=0;
	
	ifstream input; 
	input.open(filename);
	string line;
	if(!input.is_open()){
		cout<<"********************tAssembly::getNode_axial(...)******"<<endl;
		cout<<"************ Error opening file !! *******************"<<endl;
		cout<<"************ File name incorrect !! *******************"<<endl;
		cout<<"************************************************"<<endl;
		return NULL;
	}		
		string begin_block = "Axial Mesh Structure";
		string head_block1 = "Plane";
		string head_block2 = "Number";
		
		bool Block_in = false;
		bool isFinished = false;
		
		while(!input.eof() && !isFinished){
			getline(input,line);
				if (line.length() > 1 && (std::string::npos != line.find(begin_block))){
					Block_in = true;
					axialNode_Nb = 0;
					height = 0;
				}else if (int(line.length()) < 2 && Block_in){
					Block_in = false;
					isFinished = true;
				}else if(line.length() > 1 && Block_in && (std::string::npos == line.find(head_block1)) && (std::string::npos == line.find(head_block2))){
								lineData->readLine_(line);
								axialNode[axialNode_Nb].ID = float(lineData->line_buf[0].val);
								axialNode[axialNode_Nb].size = float(lineData->line_buf[1].val);
								axialNode[axialNode_Nb].pos = float(lineData->line_buf[2].val);
								height += float(lineData->line_buf[1].val);
								axialNode_Nb++;
				}
		}
 	input.close();
}


// get axial power distribution ------------------------------------------------------------
void tAssembly::getPower_axial(string line)
{ 
	string begin_block = "Axial Power Distribution";
	string head_block1 = "Distance";
	string head_block2 = "from Bottom";
		if (line.length() > 1 && std::string::npos != line.find(begin_block)){
				assemblyAxialPowerBlock_in = true;
				assemblyAxialPowerBlock_readOut = false;
				axialNode_Nb_tmp = 0;
		}else if (int(line.length()) < 2 && assemblyAxialPowerBlock_in && axialNode_Nb_tmp > 0){
				assemblyAxialPowerBlock_in = false;
				assemblyAxialPowerBlock_readOut = true;
		}else if(line.length() > 1 && assemblyAxialPowerBlock_in &&
			(std::string::npos == line.find(head_block1)) &&(std::string::npos == line.find(head_block2))){
				lineData->readLine_(line);
				axialNode[axialNode_Nb_tmp].pow = float(lineData->line_buf[1].val);
				axialNode_Nb_tmp++;
		}else{assemblyAxialPowerBlock_readOut = false;} 
}
//---------------------------------------------------------------------------------------------





// ---- get position of outer assembly----------------------------------------------------------
void tAssembly::getOuterAssembly()
{
	//identify residual outer assemblies
	int i =0;
	int j =0;
	int adj = 0;
	for(i = 0; i <nb; i++){
		for(j = i; j <nb; j++){
			if(bank[i].i == bank[j].i){
				if(TMath::Abs(bank[j].j - bank[i].j) == 1){
					adj++;
				}
			}
		}
	}
	if(adj == 0){
		reactorType = VVER;
		cout<<"---------------------------"<<endl;
		cout<<"tAssembly:**********  Reactor Type: VVER ***********"<<endl;
		cout<<"---------------------------"<<endl;
	}else{
		reactorType = AP1000;
		cout<<"---------------------------"<<endl;
		cout<<"tAssembly:**********  Reactor Type: AP1000 ***********"<<endl;
		cout<<"---------------------------"<<endl;
	}
	
	int nbNeigbour =0;
	if(reactorType == VVER){//find outer assembly for vver
		for(i = 0; i <nb; i++){
			if(bank[i].pos != OUTER){
				nbNeigbour =0;
				for(j = 0; j <nb; j++){
					if((TMath::Abs(bank[j].i - bank[i].i) == 1) &&
						(TMath::Abs(bank[j].j - bank[i].j) == 1) )
					{
							nbNeigbour++;
					}
				}
			if(nbNeigbour < 4){bank[i].pos = OUTER;}	
			}
		}
	}else if (reactorType == AP1000){ //find outer assembly for AP1000
		for(i = 0; i <nb; i++){
			if(bank[i].pos != OUTER){
				nbNeigbour =0;
				for(j = 0; j <nb; j++){
					if((TMath::Abs(bank[j].i - bank[i].i) == 0) &&
						(TMath::Abs(bank[j].j - bank[i].j) == 1) )
					{
							nbNeigbour++;
					}
				}
			if(nbNeigbour < 2){bank[i].pos = OUTER;}	
			}
		}
	}
	
}


#ifdef __cplusplus
}
#endif

#endif
