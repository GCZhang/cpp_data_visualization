//************************************
// PARCS output analysis code
// AUTHOR: DOAN QUANG TUYEN
//************************************

#include "include/type.h"
#include "include/core.h"
#include "include/ctrlRod.h"
#include "include/assembly.h"

void test(const Char_t *filename)
{
		// string path = "output_Data/output_10percent/";
		string path = "output_Data/output_100percent/";
		
		
	TApplication theApp("App", 0, 0);
	
	  gStyle->SetPalette(1); //set color in TH2
 
	  gStyle ->SetCanvasBorderMode(0);
	  gStyle->SetPadBorderMode(0);
	  gStyle ->SetPadColor(kWhite);
	  gStyle ->SetCanvasColor(kWhite);
	  gStyle ->SetTitleColor(kWhite);
	  gStyle ->SetStatColor(kWhite);
	  gStyle ->SetTitleFont(kWhite);
	  gStyle->SetTitleFillColor(0);
	  
		cout<<"----------------------***-----------------"<<endl;
		cout<<"--------------------********-----------------"<<endl;
		cout<<"------------------************---------------"<<endl;
		cout<<"---------------*******************-----------"<<endl;
		cout<<"------------------************---------------"<<endl;
		cout<<"--------------------********-----------------"<<endl;
		cout<<"----------------------***-----------------"<<endl;
		cout<<"PARCS output analysis code"<<endl;
		cout<<"AUTHOR: DOAN QUANG TUYEN"<<endl;
		cout<<"-----------------------------------------------"<<endl;
		cout<<" main program: Start analysis ... "<<endl;
		cout<<"-----------------------------------------------"<<endl;
		int i = 0;
		tCore *Core = new tCore(filename);
			
		ifstream input; 
		input.open(filename);
		
		TCanvas c("Power Distribution","Power Distribution",1200,800);
		c.Divide(3,2);
		TGraph graph;
		int blockNum =0;
		c.cd(4);
		Core->draw_Core();
		
		TCanvas c1("total reactivity/power","total reactivity/power",800,400);
		c1.Divide(2,1);
		
		int count = 0;

		double timeTab[500];	
		double powTab[500];
		double ractTab[500];
		int nbTimeStep;
		bool time_found = false;
		bool power_found = false;
		bool react_found = false;
		
		double power_total = -1;
		double time = -1;
		double reactivity = -1;
			
		
		while(!input.eof())
		{
			string line;
			getline(input,line);
			Core->getCtrlRod_Pos(line);
			Core->getPower_radial(line);
			Core->getPower_axial(line);
					
			 
				if( ((Core->assembly)->getTime(line)) != -1){
					time_found=true;
					time = (Core->assembly)->getTime(line);
				}
				if( ((Core->assembly)->getPower(line)) != -1){
					power_found = true;
					power_total = (Core->assembly)->getPower(line);
				}
				if( ((Core->assembly)->getReactivity(line)) != -1){
					react_found = true;
					reactivity = (Core->assembly)->getReactivity(line);
				}
				if(time_found && power_found && react_found){
										
					timeTab[nbTimeStep] = time;
					powTab[nbTimeStep] = power_total;
					ractTab[nbTimeStep] = reactivity;				
				
					nbTimeStep++;					
					
					time_found = false;
					power_found = false;
					react_found = false;
				}
			
			
			 if(Core->isReadOut() && blockNum < 20){ //this for transient, condition to modify corresponding to each problem
				count++;
				cout<<count<<endl;
				
				stringstream s;
				s<< time;
				string str1 = s.str();
				string toto1 = "Radial Power Distribution at " + str1 + "s";
				c->SetTitle(toto1.c_str());
				
				c.cd(1);
					Core->updateHisto_Core_within_CtrlRodPosition();
				c.cd(2);		
					Core->draw_ROD_outer_AxialNodes();
				c.cd(3);
					Core->draw_ROD_power_axial(&graph);
				c.cd(5);
					Core->draw_ROD_power_radial("lego2");
				c.cd(6);
					Core->draw_ROD_power_radial("colz");
				(Core->Histo)->hist_ROD_power->SetTitle(toto1.c_str());
					 stringstream ss;
					 ss<<blockNum;
					 ss<<'_';
					 ss << time;
					 string str = ss.str();
					 string toto = path + "PowDistr_";
						 toto += str;
						 toto += ".png";
					 c.SaveAs(toto.c_str());
					 blockNum++;
					 
					c.Update();
								
				Core->readOutModeReset();
			} 
			
		}
	
		Core->~Core();
		
		if(nbTimeStep >0){
			c1.cd(1);
				
				TGraph *gr_powTab = new TGraph(nbTimeStep,timeTab,powTab);
				graph ->SetTitle("Phan bo cong suat");
				gr_powTab->Draw("AL*");
			c1.cd(2);
				TGraph *gr_ractTab = new TGraph(nbTimeStep,timeTab,ractTab);
				graph ->SetTitle("Do phan ung");
				gr_ractTab->Draw("AL*");

				string toto = path + "totalPower.png";
				c1.SaveAs(toto.c_str());				
		}
		
	cout<<"---------------------------------------"<<endl;	
	cout<<"Finished"<<endl;	
	cout<<"---------------------------------------"<<endl;
	
	theApp.Run(); 
}
