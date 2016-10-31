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

/*class Shape
{
public:
	//Shape() {}
	Shape(std::vector<std::vector<std::string>>,int );
	~Shape() {};
	void setID(int t) { ID = t; }
	void setNet(int t) { NetIndex = t; }
	void setLayer(int t) { Layer = t; }
	std::vector<Point2D>& Vertices(){ return Vertices_; }
	std::vector<enum TypeVoid>& LVT() { return VoidTypes; }
	std::vector<std::vector<Point2D>>& LVV() { return VoidVertices; }

private:
	int ID;  
	int NetIndex;  // representing the net this Shape belongs to
	int Layer;  
	std::vector<Point2D> Vertices_;  // 
	//std::vector<string> VoidIDs;  // this is deprecated
	//std::vector<string> VoidTypes;  // "circular", "polygon"
	std::vector<enum TypeVoid> VoidTypes;  // 0:"circular", 1:"polygon"
	std::vector<std::vector<Point2D>> VoidVertices;
	// if circular, first point (vector index 1) is center, second point is (r,0) where r is radius
	// if polygon,  start from index 1; last point is the same as the first point
};

class Symbol {
public:
	//Symbol() {}
	Symbol(std::vector<std::vector<std::string>>,int, std::vector<std::vector<std::string>>);
	~Symbol() {}

private:
	std::string Name;
	std::string PartNumber;
	std::string DeviceType;
	// all vectors have index 0 reserved; all data start from vector index 1
	std::vector<std::string> PinNumbers;
	std::vector<std::string> PinNames;
	std::vector<Point2D> PinLocations;  // relative to the origin of the local coordinates of the symbol
	std::vector<int> Pinstacks;
	std::vector<double> PinRotations;

};*/

}

//void writeCMD(std::string cmd_path);

