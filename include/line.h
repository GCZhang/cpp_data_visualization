//************************************
// PARCS output analysis code
// AUTHOR: DOAN QUANG TUYEN
//************************************

#ifndef LINE_H
#define LINE_H



#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
	

	

//-----------------------------------------------------------------
class tLine{
	public:
		tLine();
		~tLine();		
		void readLine_(string str);
		tLineElement *line_buf;
		
};


tLine::tLine()
{
	cout<<"tLine: create memory"<<endl;
	int i=0;
	line_buf = new tLineElement[MAX_LINEBUF];
	for(i=0; i< MAX_LINEBUF; i++){
	   line_buf[i].val = 0;
	   line_buf[i].i = 0;
	}
}

tLine::~tLine()
{
	cout<<"tLine: free memory"<<endl;
	delete line_buf;
}

tLine:: readLine_(string str)
{
string getStr; 
int i=0;
int nb = 0;
bool getData = false;
for (i=0; i< int(str.length()); i++)
  {
		if ( str.at(i) != ' ' ){
				getData= true;
				getStr += str.at(i);
				
		}else {
			if(getData ){
				getData= false;
				float tmp = (float)std::atof(getStr.c_str());		
				line_buf[nb].val = tmp;
				line_buf[nb].i = i - int(getStr.length());
				getStr.erase (getStr.begin(), getStr.end());
				nb++;
			}
		} 
		if(i==int(str.length())-1){
			if(getData ){
				getData= false;
				float tmp = (float)std::atof(getStr.c_str());
				line_buf[nb].val = tmp;
				line_buf[nb].i =  i -  int(getStr.length()-1);
				getStr.erase (getStr.begin(), getStr.end());
				nb++;
			}
		} 
  }
 return nb;
}
//---------------------------------------------------------------





#ifdef __cplusplus
}
#endif

#endif
