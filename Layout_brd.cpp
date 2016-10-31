#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "brdTrans.h"
#include "Layout10_18.h"

using namespace brdTrans;
using namespace std;
using namespace psiv;

enum TypeShape str2enum(string str)
{
	if(str=="CIRCLE")
	    return Circular;
	else if(str=="")
		return Na;
	else
		return Polygon;

}
bool DouEual(double a,double b)
{
	return abs(a-b)<0.0001;
}

void Padstack::setDrillDiameter(double dia,double pos,double neg)
{
	DblMNM drill_diameter_(dia-neg,dia,dia+pos);
}
void Padstack::setPadOffset(double x, double y,int flag)
{
	Point2D temp;
	temp.setXY(x,y);
	if(flag==1)
	{
		PadOffset.clear();
		PadOffset.push_back(temp);
	}
	else
		PadOffset.push_back(temp);
}
void Padstack::setTypeShape(enum TypeShape shape,int layer_temp)
{
	if(layer_temp==1)
	{
		pad_types_.clear();
		pad_types_.push_back((enum TypeShape)-2);//index为0的那层设为无效TypeShape；
		pad_types_.push_back(shape);
	}
	else
	{
		pad_types_.push_back(shape);
	}
}
void Symbol::setPinCenter(double x,double y,int flag)
{
	Point2D temp;
	temp.setXY(x,y);
	if(flag==1)
	{
		pin_centers_.clear();
		pin_centers_.push_back(temp);
	}
	else
		pin_centers_.push_back(temp);
}
void Part::setPinCenter(double x,double y,int flag)
{
	Point2D temp;
	temp.setXY(x,y);
	if(flag==1)
	{
		pin_centers_.clear();
		pin_centers_.push_back(temp);
	}
	else
		pin_centers_.push_back(temp);
}

Layout::Layout(const std::string& ext_path,const std::string& brdfilename)
{
	//----------------------------转换前准备----------------------------------------------------------------------
	brd2asc ascii(ext_path,brdfilename);//生成12个excel文件（即ASCII文件）；
	//每个out.txt文件的内容先存成map形式方便转换时查找；其中temp_map每处理一个out文件时循环利用；
	map<string,vector<string>> temp_map,temp_map_alt;
	string outfilename;
	vector<string> net_list_temp;
	//----------------------------通过flayer获取layer层数数据-----------------------------------------------------
	outfilename=ascii.getcmd_path()+"flayer.txt";
	temp_map=ascii.Importa(outfilename);//把flayer信息存到临时变量map里；
	map<string,vector<string>>::iterator iter;
	iter=temp_map.begin();
	int layer_sum;
	layer_sum=(iter->second.size()-1)/2;
	//----------------------------translate "class Padstack"------------------------------------------------------

	outfilename=ascii.getcmd_path()+"paddef.txt";
	temp_map=ascii.Importa(outfilename);//把paddef信息存到临时变量map里；
	outfilename=ascii.getcmd_path()+"fcpad.txt";
	temp_map_alt=ascii.Importa(outfilename);//把fcpad信息存到另一个替补临时变量map里；

	map<string,vector<string>>::iterator iter_padname,iter_num,iter_typepad,iter_layer,iter_padwidth,iter_padhght,iter_padshape,iter_drillpos,iter_drillneg,iter_xoff,iter_yoff;
	iter_padname=temp_map.find("PAD_NAME");//查找name_，返回迭代器；
	iter_num=temp_map.find("REC_NUMBER");
	iter_layer=temp_map.find("LAYER");
	iter_padwidth=temp_map.find("PADWIDTH");
	iter_padhght=temp_map.find("PADHGHT");
	iter_padshape=temp_map.find("PADSHAPE1");
	iter_drillpos=temp_map.find("APADHGHT");
	iter_drillneg=temp_map.find("APADXOFF");
	iter_xoff==temp_map.find("PADXOFF");
	iter_yoff==temp_map.find("PADYOFF");
	int i,lines,layer_temp;
	lines=iter_padname->second.size();//paddef中记录的pad元素的个数（即数据行数）；

	Padstack padstack_temp;//临时存放每一个pad对象；
	/*padstack_temp.setName(iter_padname->second[0]);//写入第一个pad对象的name；
	if(iter_padwidth->second[0]!="0")
	{
		padstack_temp.setLayerStart(1);
	}*/

	

	for(i=0;i<lines;i++)
	{
		if(iter_num->second[i]=="1")
		{
			layer_temp=1;
			if(i!=0)
				padstacks_.push_back(padstack_temp);//将上一个pad对象push进padstacks_；

			padstack_temp.setName(iter_padname->second[i]);//写入下一个pad对象的name；
			if(iter_padwidth->second[i]!="0")
	        {
		        padstack_temp.setLayerStart(1);
				
				padstack_temp.setTypeShape(str2enum(iter_padshape->second[i]),layer_temp);
				padstack_temp.setPadOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()),layer_temp);
				//处理padshape。。。
	        }
			else
			{
				padstack_temp.setLayerStart(0);//否则top层无pad即layer_start_=0；
				padstack_temp.setTypeShape(Na,layer_temp);
				padstack_temp.setPadOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()),layer_temp);
			    //处理padshape。。。
			}
		}
		else if(iter_num->second[i]=="3")
		{
			layer_temp=2;
			if(iter_padwidth->second[i]!="0")
	        {
				padstack_temp.setTypeShape(str2enum(iter_padshape->second[i]),layer_temp);
				padstack_temp.setPadOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()),layer_temp);
				//处理padshape。。。
	        }
			else
			{
				padstack_temp.setLayerStop(1);
				padstack_temp.setTypeShape(Na,layer_temp);
				padstack_temp.setPadOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()),layer_temp);
			    //处理padshape。。。
			}
		}
		else if(atoi(iter_num->second[i].c_str())>3&&atoi(iter_num->second[i].c_str())<=layer_sum)
		{
			//说明属于layer1-layerN层以及GND层之间；
			layer_temp=atoi(iter_num->second[i].c_str())-1;
			if(iter_padwidth->second[i]!="0")
	        {
				padstack_temp.setTypeShape(str2enum(iter_padshape->second[i]),layer_temp);
				padstack_temp.setPadOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()),layer_temp);
				//处理padshape。。。
	        }
			else
			{
				padstack_temp.setTypeShape(Na,layer_temp);
				padstack_temp.setPadOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()),layer_temp);
			    //处理padshape。。。
			}
		}
		else if(atoi(iter_num->second[i].c_str())==layer_sum+1)
		{
			//说明在bottom层；
			layer_temp=layer_sum;
			if(iter_padwidth->second[i]!="0")
			{
				padstack_temp.setLayerStop(layer_temp);
				padstack_temp.setTypeShape(str2enum(iter_padshape->second[i]),layer_temp);
				padstack_temp.setPadOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()),layer_temp);
				//处理padshape。。。
			}
			else
			{
				padstack_temp.setTypeShape(Na,layer_temp);
				padstack_temp.setPadOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()),layer_temp);
			    //处理padshape。。。
			}
		}
		else if(atoi(iter_num->second[i].c_str())==layer_sum+8)//
		{
			//说明在drill层，处理drill的数据；
			padstack_temp.setDrillDiameter(atof(iter_padshape->second[i].c_str()),atof(iter_drillpos->second[i].c_str()),atof(iter_drillneg->second[i].c_str()));
			padstack_temp.setDriallOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()));
		}
	}
	
	//----------------------------translate "class Pin"------------------------------------------------------ 
	
	outfilename=ascii.getcmd_path()+"fpin.txt";
	temp_map=ascii.Importa(outfilename);//把fpin信息存到map里；

	map<string,vector<string>>::iterator iter_pinname,iter_netname,iter_refdes,iter_rota,iter_x,iter_y,iter_sub;
	iter_pinname=temp_map.find("PIN_NAME");//查找pin_name_，返回迭代器；
	iter_refdes=temp_map.find("REFDES");
	iter_netname=temp_map.find("NET_NAME");
	iter_rota=temp_map.find("PIN_ROTATION");
	iter_x=temp_map.find("PIN_X");
	iter_y=temp_map.find("PIN_Y");
	iter_sub=temp_map.find("SUBCLASS");
	lines=iter_pinname->second.size();//fpin中记录的pin元素的个数（即数据行数）；

	Pin pin_temp;//临时存放每一个pin对象；
	//先将第一行数据写入对象；
	pin_temp.setPinName(iter_pinname->second[0]);
	pin_temp.setRefDes(iter_refdes->second[0]);
	int pin_id=1;
	pin_temp.setID(pin_id);//pin的id_从1开始记录；
	int net_index=1;
	pin_temp.setNetIndex(net_index);//net_list的index从1开始；
	net_list_temp.push_back(iter_netname->second[0]);//net_list中存入第一个net;
	net_list_temp.push_back(iter_pinname->second[0]);//net_list中存入第一个net里的第一个pin；
	vector<vector<string>> net_list_;
	vector<string> net_temp;
	net_list_.push_back(net_temp);
	//pad_exist_需再确认一下set函数的具体代码；
	pin_temp.setRotation(atof(iter_rota->second[0].c_str()));
	double pin_x,pin_y;
	pin_temp.setLocation(atof(iter_x->second[0].c_str()),atof(iter_y->second[0].c_str()));
	std::vector<int> padexist(layer_sum+1,0);
	if(iter_sub->second[0]=="TOP" ||iter_sub->second[0]=="PASTEMASK_TOP"||iter_sub->second[0]=="SOLDERMASK_TOP")
	{
		padexist[1]=1;
	}
	else if(iter_sub->second[0]=="BOTTOM" ||iter_sub->second[0]=="SOLDERMASK_BOTTOM"||iter_sub->second[0]=="PASTEMASK_BOTTOM")
	{
		padexist[layer_sum]=1;
	}
	
	for(i=1;i<lines;i++)
	{
		pin_x=atof(iter_x->second[i].c_str());
		pin_y=atof(iter_y->second[i].c_str());
		
		if(DouEual(pin_x,pin_temp.getLocation().getX())&&DouEual(pin_y,pin_temp.getLocation().getY()))//写一个doubleequal函数比较两个double；
		{
			//处理pad_exit；
			if(iter_sub->second[i]=="TOP" ||iter_sub->second[i]=="PASTEMASK_TOP"||iter_sub->second[i]=="SOLDERMASK_TOP")
	        {
				padexist[1]=1;
	        }
			else if(iter_sub->second[i]=="BOTTOM" ||iter_sub->second[i]=="SOLDERMASK_BOTTOM"||iter_sub->second[i]=="PASTEMASK_BOTTOM")
			{
				padexist[layer_sum]=1;
			}
			else if(iter_sub->second[i]=="PWR")
				padexist[2]=1;
			else if(iter_sub->second[i]=="GND")
				padexist[layer_sum-1]=1;
			else
			{
				char lastch=iter_sub->second[i].back();
				padexist[2+lastch-'0']=1;
			}
		}
		else
		{
			pin_temp.setPadExist(padexist);
			padexist=std::vector<int>(layer_sum+1,0);
			pins_.push_back(pin_temp);//进入下一个pin对象前将该pin对象push进vector<Pin> pins_中；写入Layout class；
			pin_temp.setLocation(pin_x,pin_y);
			pin_temp.setRotation(atof(iter_rota->second[i].c_str()));
			pin_temp.setPinName(iter_pinname->second[i]);
			pin_temp.setID(++pin_id);
			pin_temp.setRefDes(iter_refdes->second[i]);

			//处理net_index;
			if(net_list_temp[0]==iter_netname->second[i])
			{
				pin_temp.setNetIndex(net_index);
				net_list_temp.push_back(iter_pinname->second[i]);
			}
			else
			{
				net_list_temp.clear();
				net_list_temp.push_back(iter_netname->second[i]);//net_list中存入下一个net;
				net_list_temp.push_back(iter_pinname->second[i]);
				pin_temp.setNetIndex(++net_index);
			}
			//处理pad_exit；
			if(iter_sub->second[i]=="TOP" ||iter_sub->second[i]=="PASTEMASK_TOP"||iter_sub->second[i]=="SOLDERMASK_TOP")
			{
				padexist[1]=1;
			}
			else if(iter_sub->second[i]=="BOTTOM" ||iter_sub->second[i]=="SOLDERMASK_BOTTOM"||iter_sub->second[i]=="PASTEMASK_BOTTOM")
			{
				padexist[layer_sum]=1;
			}
		}
	}

	//----------------------------translate "class Via"------------------------------------------------------
	
	outfilename=ascii.getcmd_path()+"fvia.txt";
	temp_map=ascii.Importa(outfilename);//把fvia信息存到map里；

	map<string,vector<string>>::iterator iter_viax,iter_viay,iter_roa,iter_padstack,iter_net;
	iter_viax=temp_map.find("VIA_X");
	iter_viay=temp_map.find("VIA_Y");
	iter_layer=temp_map.find("SUBCLASS");
	iter_padstack=temp_map.find("PAD_STACK_NAME");
	iter_net=temp_map.find("NET_NAME");
	lines=iter_viax->second.size();

	Via via_temp;
	int via_id=1;
	via_temp.setID(via_id);
	via_temp.setLocation(atof(iter_viax->second[0].c_str()),atof(iter_viay->second[0].c_str()));

	//----------------------------translate "class Component"------------------------------------------------------
	outfilename=ascii.getcmd_path()+"fcomp.txt";
	temp_map=ascii.Importa(outfilename);//把fcomp信息存到map里；

	map<string,vector<string>>::iterator iter_symname,iter_part;//x,y，refdes定义过；
	iter_refdes=temp_map.find("REFDES");
	iter_symname=temp_map.find("SYM_NAME");
	iter_x=temp_map.find("SYM_X");
	iter_y=temp_map.find("SYM_Y");
	iter_roa=temp_map.find("SYM_ROTATE");
	iter_part=temp_map.find("COMP_PART_NUMBER");
	lines=iter_refdes->second.size();

	Component comp_temp;
	for(i=0;i<lines;i++)
	{
		comp_temp.setRefDes(iter_refdes->second[i]);
		comp_temp.setSymbolName(iter_symname->second[i]);
		comp_temp.setLocation(atof(iter_x->second[i].c_str()),atof(iter_y->second[i].c_str()));
		comp_temp.setRotation(atof(iter_roa->second[i].c_str()));
		comp_temp.setPartNumber(iter_part->second[i]);
		components_.push_back(comp_temp);//将每一个component对象push进components_；即写入Layout class；
	}

	//----------------------------translate "class Symbol"------------------------------------------------------
	
	outfilename=ascii.getcmd_path()+"fpin.txt";
	temp_map=ascii.Importa(outfilename);//把fpin信息存到map里；

	map<string,vector<string>>::iterator iter_pinx,iter_piny,iter_pintype;//symname，pinname之前定义过；
	iter_symname=temp_map.find("SYM_NAME");
	iter_pinname=temp_map.find("PIN_NAME");
	iter_pinx=temp_map.find("PIN_X");
	iter_piny=temp_map.find("PIN_Y");
	iter_pintype=temp_map.find("GRAPHIC_DATA_NAME");
	lines=iter_symname->second.size();

	Symbol sym_temp;
	sym_temp.setName(iter_symname->second[0]);
	vector<string> pin_name;
	pin_name.push_back(iter_pinname->second[0]);
	sym_temp.setPinCenter(atof(iter_pinx->second[0].c_str()),atof(iter_piny->second[0].c_str()),1);
	vector<enum TypeShape> pin_type;
	pin_type.push_back(str2enum(iter_pintype->second[0]));
	for(i=1;i<lines;i++)
	{
		if(iter_symname->second[i]==sym_temp.getName())
		{
			if(pin_name.back()!=iter_pinname->second[i])
			{
				pin_name.push_back(iter_pinname->second[i]);
			}
			if(!DouEual(sym_temp.getPinCenter().back().getX(),atof(iter_pinx->second[i].c_str()))||!DouEual(sym_temp.getPinCenter().back().getY(),atof(iter_piny->second[i].c_str())))
				sym_temp.setPinCenter(atof(iter_pinx->second[i].c_str()),atof(iter_piny->second[i].c_str()),0);
			if(pin_type.back()!=str2enum(iter_pintype->second[i]))
				pin_type.push_back(str2enum(iter_pintype->second[i]));//可能有重复，修改；
		}
		else
		{
			sym_temp.setPinName(pin_name);
			pin_name.clear();
			sym_temp.setTypeShape(pin_type);
			pin_type.clear();
			symbols_.push_back(sym_temp);//将symbol对象写入Layout；
			sym_temp.setName(iter_symname->second[i]);
			pin_name.push_back(iter_pinname->second[i]);
			sym_temp.setPinCenter(atof(iter_pinx->second[i].c_str()),atof(iter_piny->second[i].c_str()),1);
			pin_type.push_back(str2enum(iter_pintype->second[i]));
		}
	}

	//----------------------------translate "class Part"------------------------------------------------------

	//part class也从fpin中获取，因此无须重新生成map，可继续使用以上symbol class的map；
	//同理，iterator中的iter_pinname,iter_pinx,iter_piny；及lines也继续沿用；
	map<string,vector<string>>::iterator iter_partnum,iter_parttype;
	iter_partnum=temp_map.find("COMP_PART_NUMBER");
	iter_parttype=temp_map.find("COMP_DEVICE_TYPE");

	Part part;
	part.setPartNumber(iter_partnum->second[0]);
	part.setPartType(iter_parttype->second[0]);
	pin_name.clear();
	pin_name.push_back(iter_pinname->second[0]);
	part.setPinCenter(atof(iter_pinx->second[0].c_str()),atof(iter_piny->second[0].c_str()),1);
	for(i=1;i<lines;i++)
	{
		if(iter_partnum->second[i]==part.getPartNumber())
		{
			if(pin_name.back()!=iter_pinname->second[i])
			{
				pin_name.push_back(iter_pinname->second[i]);
			}
			if(!DouEual(part.getPinCenter().back().getX(),atof(iter_pinx->second[i].c_str()))||!DouEual(part.getPinCenter().back().getY(),atof(iter_piny->second[i].c_str())))
				part.setPinCenter(atof(iter_pinx->second[i].c_str()),atof(iter_piny->second[i].c_str()),0);
		}
		else
		{
			part.setPinName(pin_name);
			pin_name.clear();
			parts_.push_back(part);//将symbol对象写入Layout；
			part.setPartNumber(iter_partnum->second[i]);
	        part.setPartType(iter_parttype->second[i]);
			pin_name.push_back(iter_pinname->second[i]);
			part.setPinCenter(atof(iter_pinx->second[i].c_str()),atof(iter_piny->second[i].c_str()),1);
		}
	}
}

int main()
{
	Layout layout1("D://Cadence SPB//tools//pcb//bin//extracta.exe","E://a//Galileo_G87173_204.brd");
	return 1;
}