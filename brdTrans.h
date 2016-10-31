#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Point2D.h"
#include <map>
//int Importa();
namespace brdTrans{
/*class FileData
{
public:
	std::vector <std::vector <std::string> > Importa();
    std::vector<std::vector<std::string> > getmat(){return mat;};
private:
	std::vector<std::vector<std::string> > mat;
};*/
/*
class brd2asc 
{
public:
	brd2asc(){};
	brd2asc(const std::string& ext_path,const std::string& brdfilename);
	~brd2asc(){};
	const std::map<std::string,std::vector<std::string>> getoutfile_mat() const{return outfile_mat;};//？？？返回类型应当加&吗？？？

private:
	const int writeCMD(const std::string& cmd_path);
	const int Extracta(const std::string& ext_path,const std::string& brdfilename,const std::string& cmd_path);
	const int Importa(const std::string& outfilename);//将excel表格文件中的内容读入map结构的内存；
	std::map<std::string,std::vector<std::string>> outfile_mat;//Importa函数执行后将文件数据存入该变量，即Importa（）返回值；


};*/

class brd2asc 
{
public:
	brd2asc(){};
	brd2asc(const std::string& ext_path,const std::string& brdfilename);
	const std::map<std::string,std::vector<std::string>>& Importa(const std::string& outfilename);//将excel表格文件中的内容读入map结构的内存；
	
	const std::string& getcmd_path() const{return cmd_path;};
	~brd2asc(){};
	

private:
	int writeCMD(const std::string& cmd_path);
	int Extracta(const std::string& ext_path,const std::string& brdfilename,const std::string& cmd_path);
	std::map<std::string,std::vector<std::string>> outfile_mat;//Importa函数执行后将文件数据存入该变量，即Importa（）返回值；
	std::string cmd_path;

};



}

//void writeCMD(std::string cmd_path);

