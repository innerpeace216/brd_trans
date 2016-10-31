
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "brdTrans.h"
#include <stdlib.h>
using namespace std;

using namespace brdTrans;
const std::map<std::string,std::vector<std::string>>& brd2asc::Importa(const string& outfilename)
{
   //const char * split = "!";
   int i,j,k;
   
   vector<string> boardline;
   char temps[512];
   
   outfile_mat.clear();
   //ifstream boardfile("E://a//fboard.txt");
   ifstream boardfile(outfilename);
   
   while (boardfile.getline(temps,512,'\n'))
   {
	   boardline.push_back(string(temps)); 
   }
   vector<std::vector<std::string> >  ext_output;
   string temp="";

   vector<string> temp_line;
   
   for (i=0;i<boardline.size();i++)
   {
	   string line = boardline[i];
	   temp_line.clear();
	
	   k = 0;
	   temp = "";
	   for (j=0;j<line.size();j++)
	   {
		
		   if (line[j] == '!')
		   {
			   temp_line.push_back(string(temp));
			   k++;
			   temp = "";
		   }
		   else
			   temp.push_back(line[j]);
		   
	   }
	   ext_output.push_back(temp_line);
	
   }

    
	vector<string> temp_column;
	for(i=0;i<temp_line.size();i++)
	{
		temp_column.clear();
		
		for(j=2;j<ext_output.size();j++)
		{
			temp_column.push_back(ext_output[j][i]);
		}
		outfile_mat.insert(pair<string,vector<string>> (ext_output[0][i],temp_column));
		
	}
     
	
   
return outfile_mat;
}


