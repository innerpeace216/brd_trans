#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "brdTrans.h"
#include "Layout11_11.h"

using namespace brdTrans;
using namespace std;
using namespace psiv;

enum TypeShape str2enum(string str)
{
	if(str=="CIRCLE")
	    return Circular;
	else if(str=="")
		return Na;
	else if(str=="OBLONG_X")
		return Oblong_x;
	else if(str=="OBLONG_Y")
		return Oblong_y;
	else if(str=="SHAPE")
		return Shape;
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
		pad_types_.push_back((enum TypeShape)-2);//���ն��壬indexΪ0���ǲ���Ϊ��ЧTypeShape,index��1��ʼ��
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
void Shape::setVertices_(double x,double y,int flag)
{
	Point2D temp;
	temp.setXY(x,y);
	if(flag==1)
	{
		vertices_.clear();
		vertices_.push_back(temp);
	}
	else
		vertices_.push_back(temp);
}
Layout::Layout(const std::string& ext_path,const std::string& brdfilename)
{
	//----------------------------ת��ǰ׼��----------------------------------------------------------------------
	brd2asc ascii(ext_path,brdfilename);//����12��excel�ļ�����ASCII�ļ�����
	//ÿ��out.txt�ļ��������ȴ��map��ʽ����ת��ʱ���ң�����temp_mapÿ����һ��out�ļ�ʱѭ�����ã�
	map<string,vector<string>> temp_map,temp_map_alt;
	string outfilename;
	
	//----------------------------ͨ��flayer��ȡlayer��������-----------------------------------------------------
	outfilename=ascii.getcmd_path()+"flayer.txt";
	temp_map=ascii.Importa(outfilename);//��flayer��Ϣ�浽��ʱ����map�
	map<string,vector<string>>::iterator iter;
	iter=temp_map.begin();
	int layer_sum;
	layer_sum=(iter->second.size()-1)/2;
	//----------------------------ͨ��fnet��ȡnet_list����--------------------------------------------------------
	map<int,string> net_list;//netlist�洢������netname�����Ӧ��index��
	outfilename=ascii.getcmd_path()+"fnet.txt";
	temp_map=ascii.Importa(outfilename);//��flayer��Ϣ�浽��ʱ����map�
	map<string,vector<string>>::iterator iter;
	iter=temp_map.begin();
	int lines,i;
	lines=iter->second.size();
	for(i=1;i<=lines;i++)
	{
		net_list[i]=iter->second[i-1];
	}
	//----------------------------translate "class Padstack"------------------------------------------------------

	outfilename=ascii.getcmd_path()+"paddef.txt";
	temp_map=ascii.Importa(outfilename);//��paddef��Ϣ�浽��ʱ����map�
	outfilename=ascii.getcmd_path()+"fcpad.txt";
	temp_map_alt=ascii.Importa(outfilename);//��fcpad��Ϣ�浽��һ���油��ʱ����map�

	map<string,vector<string>>::iterator iter_padname,iter_num,iter_typepad,iter_layer,iter_padwidth,iter_padhght,iter_drillpos,iter_drillneg,iter_xoff,iter_yoff;
	iter_padname=temp_map.find("PAD_NAME");//����name_�����ص�������
	iter_num=temp_map.find("REC_NUMBER");
	iter_layer=temp_map.find("LAYER");
	iter_padwidth=temp_map.find("PADWIDTH");
	iter_padhght=temp_map.find("PADHGHT");
	iter_typepad=temp_map.find("PADSHAPE1");
	iter_drillpos=temp_map.find("APADHGHT");
	iter_drillneg=temp_map.find("APADXOFF");
	iter_xoff==temp_map.find("PADXOFF");
	iter_yoff==temp_map.find("PADYOFF");
	int layer_temp;
	lines=iter_padname->second.size();//paddef�м�¼��padԪ�صĸ�������������������

	Padstack padstack_temp;//��ʱ���ÿһ��padstack����
	/*padstack_temp.setName(iter_padname->second[0]);//д���һ��pad�����name��
	if(iter_padwidth->second[0]!="0")
	{
		padstack_temp.setLayerStart(1);
	}*/
	vector<Point2D> padshape_temp;//��ʱ���ÿ���padshape;
	Point2D point_temp;//��ʱ���ÿ��padshape�ĵ����㣻
	map<int,string> padstack_list;//����padstack��index�б�
	pair<int,string> list_temp;//�ݴ��ͽ��б��ÿһ��padstack��
	int pad_index=0;//list�е�index��

	for(i=0;i<lines;i++)
	{
		if(iter_num->second[i]=="1")
		{
			layer_temp=1;
			if(i!=0)
				padstacks_.push_back(padstack_temp);//����һ��padstack����push��padstacks_��
			
			padstack_temp.setName(iter_padname->second[i]);//д����һ��padstack�����name��
			padstack_list[++pad_index]=padstack_temp.getName();//����һ��padstackά�����б�
			padstack_temp.setTypeShape(str2enum(iter_typepad->second[i]),layer_temp);
			padstack_temp.setPadOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()),layer_temp);
			//����padshape
			

			if(iter_typepad->second[i]!="")
	        {
		        padstack_temp.setLayerStart(1);
				//������pad��padshape��
				if(iter_typepad->second[i]=="CIRCLE")
				{
					point_temp.setXY(0,0);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(0,atof(iter_padwidth->second[i].c_str()));
					padshape_temp.push_back(point_temp);
				}
				if(iter_typepad->second[i]=="SHAPE")
				{
					point_temp.setXY(atof(iter_padwidth->second[i].c_str()),atof(iter_padhght->second[i].c_str()));
				    padshape_temp.push_back(point_temp);
				}
				else
				{
					point_temp.setXY(atof(iter_padwidth->second[i].c_str())/2,atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(atof(iter_padwidth->second[i].c_str())/2,-atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(-atof(iter_padwidth->second[i].c_str())/2,-atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(-atof(iter_padwidth->second[i].c_str())/2,atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
				}
				padstack_temp.setPadShape(padshape_temp,layer_temp=1);
				padshape_temp.clear();
	        }
			else
			{
				padstack_temp.setLayerStart(0);//����top����pad��layer_start_=0��
				//������pad��padshape��
				point_temp.setXY(0,0);
				padshape_temp.push_back(point_temp);

				padstack_temp.setPadShape(padshape_temp,layer_temp=1);
				padshape_temp.clear();
			}
		}
		else if(iter_num->second[i]=="3")
		{
			layer_temp=2;
			padstack_temp.setTypeShape(str2enum(iter_typepad->second[i]),layer_temp);
			padstack_temp.setPadOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()),layer_temp);
			//����padshape��
			if(iter_typepad->second[i]=="")
			{
				padstack_temp.setLayerStop(1);//˵��ֻ��top����pad��layerstop=1��
				//������pad��padshape��
				point_temp.setXY(0,0);
				padshape_temp.push_back(point_temp);

				padstack_temp.setPadShape(padshape_temp,layer_temp=1);
				padshape_temp.clear();
			}
			else
			{
				if(iter_typepad->second[i]=="CIRCLE")
				{
					point_temp.setXY(0,0);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(0,atof(iter_padwidth->second[i].c_str()));
					padshape_temp.push_back(point_temp);
				}
				if(iter_typepad->second[i]=="SHAPE")
				{
					point_temp.setXY(atof(iter_padwidth->second[i].c_str()),atof(iter_padhght->second[i].c_str()));
				    padshape_temp.push_back(point_temp);
				}
				else
				{
					point_temp.setXY(atof(iter_padwidth->second[i].c_str())/2,atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(atof(iter_padwidth->second[i].c_str())/2,-atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(-atof(iter_padwidth->second[i].c_str())/2,-atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(-atof(iter_padwidth->second[i].c_str())/2,atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
				}
				padstack_temp.setPadShape(padshape_temp,layer_temp=1);
				padshape_temp.clear();
			}
		}
		else if(atoi(iter_num->second[i].c_str())>3&&atoi(iter_num->second[i].c_str())<=layer_sum)
		{
			//˵������layer1-layerN���Լ�GND��֮�䣻
			layer_temp=atoi(iter_num->second[i].c_str())-1;
			padstack_temp.setTypeShape(str2enum(iter_typepad->second[i]),layer_temp);
			padstack_temp.setPadOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()),layer_temp);
			//����padshape��
				if(iter_typepad->second[i]=="CIRCLE")
				{
					point_temp.setXY(0,0);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(0,atof(iter_padwidth->second[i].c_str()));
					padshape_temp.push_back(point_temp);
				}
				else if(iter_typepad->second[i]=="SHAPE")
				{
					point_temp.setXY(atof(iter_padwidth->second[i].c_str()),atof(iter_padhght->second[i].c_str()));
				    padshape_temp.push_back(point_temp);
				}
				else if(iter_typepad->second[i]=="")
			    {
					//������pad��padshape��
					point_temp.setXY(0,0);
					padshape_temp.push_back(point_temp);
			    }
				else
				{
					point_temp.setXY(atof(iter_padwidth->second[i].c_str())/2,atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(atof(iter_padwidth->second[i].c_str())/2,-atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(-atof(iter_padwidth->second[i].c_str())/2,-atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(-atof(iter_padwidth->second[i].c_str())/2,atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
				}
				padstack_temp.setPadShape(padshape_temp,layer_temp=1);
				padshape_temp.clear();
	        
			
		}
		else if(atoi(iter_num->second[i].c_str())==layer_sum+1)
		{
			//˵����bottom�㣻
			layer_temp=layer_sum;
			padstack_temp.setTypeShape(str2enum(iter_typepad->second[i]),layer_temp);
				padstack_temp.setPadOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()),layer_temp);
			if(iter_typepad->second[i]!="")
			{
				padstack_temp.setLayerStop(layer_temp);
				//������pad��padshape��
				if(iter_typepad->second[i]=="CIRCLE")
				{
					point_temp.setXY(0,0);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(0,atof(iter_padwidth->second[i].c_str()));
					padshape_temp.push_back(point_temp);
				}
				if(iter_typepad->second[i]=="SHAPE")
				{
					point_temp.setXY(atof(iter_padwidth->second[i].c_str()),atof(iter_padhght->second[i].c_str()));
				    padshape_temp.push_back(point_temp);
				}
				else
				{
					point_temp.setXY(atof(iter_padwidth->second[i].c_str())/2,atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(atof(iter_padwidth->second[i].c_str())/2,-atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(-atof(iter_padwidth->second[i].c_str())/2,-atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
					point_temp.setXY(-atof(iter_padwidth->second[i].c_str())/2,atof(iter_padhght->second[i].c_str())/2);
					padshape_temp.push_back(point_temp);
				}
				padstack_temp.setPadShape(padshape_temp,layer_temp=1);
				padshape_temp.clear();
			}
			else
			{
				//������pad��padshape��
				point_temp.setXY(0,0);
				padshape_temp.push_back(point_temp);

				padstack_temp.setPadShape(padshape_temp,layer_temp=1);
				padshape_temp.clear();
			}
		}
		else if(atoi(iter_num->second[i].c_str())==layer_sum+8)//
		{
			//˵����drill�㣬����drill�����ݣ�
			padstack_temp.setDrillDiameter(atof(iter_typepad->second[i].c_str()),atof(iter_drillpos->second[i].c_str()),atof(iter_drillneg->second[i].c_str()));
			padstack_temp.setDriallOffset(atof(iter_xoff->second[i].c_str()),atof(iter_yoff->second[i].c_str()));
		}
	}
	
	//----------------------------translate "class Pin"------------------------------------------------------ 
	
	outfilename=ascii.getcmd_path()+"fpin.txt";
	temp_map=ascii.Importa(outfilename);//��fpin��Ϣ�浽map�

	map<string,vector<string>>::iterator iter_pinname,iter_netname,iter_refdes,iter_rota,iter_x,iter_y,iter_sub;
	iter_pinname=temp_map.find("PIN_NAME");//����pin_name_�����ص�������
	iter_refdes=temp_map.find("REFDES");
	iter_netname=temp_map.find("NET_NAME");
	iter_rota=temp_map.find("PIN_ROTATION");
	iter_x=temp_map.find("PIN_X");
	iter_y=temp_map.find("PIN_Y");
	iter_sub=temp_map.find("SUBCLASS");
	lines=iter_pinname->second.size();//fpin�м�¼��pinԪ�صĸ�������������������

	Pin pin_temp;//��ʱ���ÿһ��pin����
	//�Ƚ���һ������д�����
	pin_temp.setPinName(iter_pinname->second[0]);
	pin_temp.setRefDes(iter_refdes->second[0]);
	int pin_id=1;
	pin_temp.setID(pin_id);//pin��id_��1��ʼ��¼��
	//int net_index=1;
	//pin_temp.setNetIndex(net_index);//net_list��index��1��ʼ��
	//net_list_temp.insert(iter_netname->second[0]);//net_list�д����һ��net;
	//net_list_temp.insert(iter_pinname->second[0]);//net_list�д����һ��net��ĵ�һ��pin��
	//vector<vector<string>> net_list_;
	//vector<string> net_temp;
	//net_list_.push_back(net_temp);
	map<int,string>::iterator it;
	for(it=net_list.begin();it!=net_list.end();++it)
	{
		if(it->second==iter_netname->second[0])
			pin_temp.setNetIndex(it->first);
	}
	//pad_exist_����ȷ��һ��set�����ľ�����룻
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
		
		if(DouEual(pin_x,pin_temp.getLocation().getX())&&DouEual(pin_y,pin_temp.getLocation().getY()))//дһ��doubleequal�����Ƚ�����double��
		{
			//����pad_exit��
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
			pins_.push_back(pin_temp);//������һ��pin����ǰ����pin����push��vector<Pin> pins_�У�д��Layout class��
			pin_temp.setLocation(pin_x,pin_y);
			pin_temp.setRotation(atof(iter_rota->second[i].c_str()));
			pin_temp.setPinName(iter_pinname->second[i]);
			pin_temp.setID(++pin_id);
			pin_temp.setRefDes(iter_refdes->second[i]);

			//����net_index;
			/*if(net_list_temp[i-1]==iter_netname->second[i])
			{
				pin_temp.setNetIndex(net_index);
				net_list_temp.push_back(iter_pinname->second[i]);
			}
			else
			{
				net_list_temp.clear();
				net_list_temp.push_back(iter_netname->second[i]);//net_list�д�����һ��net;
				net_list_temp.push_back(iter_pinname->second[i]);
				pin_temp.setNetIndex(++net_index);
			}*/
		   for(it=net_list.begin();it!=net_list.end();++it)
			{
				if(it->second==iter_netname->second[i])
					pin_temp.setNetIndex(it->first);
			}
			//����pad_exit��
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
	temp_map=ascii.Importa(outfilename);//��fvia��Ϣ�浽map�

	map<string,vector<string>>::iterator iter_drillx,iter_drilly,iter_viax,iter_viay,iter_roa,iter_padstack;//iter_netname��֮ǰ��pin�ﶨ���;
	iter_drillx=temp_map.find("DRILL_HOLE_X");
	iter_drilly=temp_map.find("DRILL_HOLE_Y");
	iter_viax=temp_map.find("VIA_X");
	iter_viay=temp_map.find("VIA_Y");
	iter_layer=temp_map.find("SUBCLASS");
	iter_padstack=temp_map.find("PAD_STACK_NAME");
	iter_netname=temp_map.find("NET_NAME");
	lines=iter_viax->second.size();

	Via via_temp;
	int via_id=1;
	string drillx_temp,drilly_temp;
	drillx_temp=iter_drillx->second[0];
	drilly_temp=iter_drilly->second[0];
	via_temp.setID(via_id);
	via_temp.setLocation(atof(iter_viax->second[0].c_str()),atof(iter_viay->second[0].c_str()));
	for(it=net_list.begin();it!=net_list.end();++it)
	{
		if(it->second==iter_netname->second[0])
			via_temp.setNetIndex(it->first);
	}
	for(it=padstack_list.begin();it!=padstack_list.end();++it)
	{
		if(it->second==iter_padstack->second[0])
			via_temp.setViaStack(it->first);
	}
	for(i=1;i<lines;i++)
	{
		if((drillx_temp==iter_drillx->second[i])&&(drilly_temp==iter_drilly->second[i]))
		{
			//˵�����ɻ�����ͬһ��via��
			//����pad_exit������
		}
		else
		{
			//˵��������һ��via�ˣ�
			vias_.push_back(via_temp);//����һ��viaѹ��vias_��
			drillx_temp=iter_drillx->second[i];
			drilly_temp=iter_drilly->second[i];
			via_temp.setID(++via_id);
			via_temp.setLocation(atof(iter_viax->second[i].c_str()),atof(iter_viay->second[i].c_str()));
			//Ѱ�Ҹ�via��Ӧ��netlist��index��
			for(it=net_list.begin();it!=net_list.end();++it)
			{
				if(it->second==iter_netname->second[i])
					via_temp.setNetIndex(it->first);
			}
			//Ѱ�Ҹ�via��Ӧ��patstack_list��index��
			for(it=padstack_list.begin();it!=padstack_list.end();++it)
			{
				if(it->second==iter_padstack->second[i])
					via_temp.setViaStack(it->first);
			}
		}
	}

	//----------------------------translate "class Component"------------------------------------------------------
	outfilename=ascii.getcmd_path()+"fcomp.txt";
	temp_map=ascii.Importa(outfilename);//��fcomp��Ϣ�浽map�

	map<string,vector<string>>::iterator iter_symname,iter_part;//x,y��refdes�������
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
		components_.push_back(comp_temp);//��ÿһ��component����push��components_����д��Layout class��
	}

	//----------------------------translate "class Symbol"------------------------------------------------------
	
	outfilename=ascii.getcmd_path()+"fpin.txt";
	temp_map=ascii.Importa(outfilename);//��fpin��Ϣ�浽map�

	map<string,vector<string>>::iterator iter_pinx,iter_piny,iter_pintype;//symname��pinname֮ǰ�������
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
				pin_type.push_back(str2enum(iter_pintype->second[i]));//�������ظ����޸ģ�
		}
		else
		{
			sym_temp.setPinName(pin_name);
			pin_name.clear();
			sym_temp.setTypeShape(pin_type);
			pin_type.clear();
			symbols_.push_back(sym_temp);//��symbol����д��Layout��
			sym_temp.setName(iter_symname->second[i]);
			pin_name.push_back(iter_pinname->second[i]);
			sym_temp.setPinCenter(atof(iter_pinx->second[i].c_str()),atof(iter_piny->second[i].c_str()),1);
			pin_type.push_back(str2enum(iter_pintype->second[i]));
		}
	}

	//----------------------------translate "class Part"------------------------------------------------------

	//part classҲ��fpin�л�ȡ�����������������map���ɼ���ʹ������symbol class��map��
	//ͬ��iterator�е�iter_pinname,iter_pinx,iter_piny����linesҲ�������ã�
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
			parts_.push_back(part);//��symbol����д��Layout��
			part.setPartNumber(iter_partnum->second[i]);
	        part.setPartType(iter_parttype->second[i]);
			pin_name.push_back(iter_pinname->second[i]);
			part.setPinCenter(atof(iter_pinx->second[i].c_str()),atof(iter_piny->second[i].c_str()),1);
		}
	}

	//----------------------------translate "class Shape"------------------------------------------------------
	outfilename=ascii.getcmd_path()+"fetch.txt";
	temp_map=ascii.Importa(outfilename);//��fetch��Ϣ�浽map�

	map<string,vector<string>>::iterator iter_etchtype,iter_etchname,iter_pointx,iter_pointy,iter_centerx,iter_centery,iter_ra,iter_record;//iter_layer,iter_netname֮ǰ�������
	iter_etchtype=temp_map.find("GRAPHIC_DATA_10");
	iter_record=temp_map.find("RECORD_TAG");
	iter_pointx=temp_map.find("GRAPHIC_DATA_1");
	iter_pointy=temp_map.find("GRAPHIC_DATA_2");
	iter_etchname=temp_map.find("GRAPHIC_DATA_NAME");
	iter_centerx=temp_map.find("GRAPHIC_DATA_5");
	iter_centery=temp_map.find("GRAPHIC_DATA_6");
	iter_ra=temp_map.find("GRAPHIC_DATA_7");
	iter_layer=temp_map.find("SUBCLASS");//iter_layerǰ�涨�����
	lines=iter_record->second.size();

	Shape shape_temp;
	int lastag1=0,lastag2=0,lastag3=0,nowtag1=0,nowtag2=0,nowtag3=0;
	vector<Point2D> Void_temp;
	Point2D Voidpoint_temp;
	for(i=0;i<lines;i++)
	{
		if(iter_etchtype->second[i]=="SHAPE")
		{
			
		
			//����shapevertices��
			sscanf(iter_record->second[i].c_str(),"%d %d %d",nowtag1,nowtag2,nowtag3);
			if(nowtag1==lastag1)
			{
				//˵��ͬһ��shape��������㡣������
				shape_temp.setVertices_(atof(iter_pointx->second[i].c_str()),atof(iter_pointy->second[i].c_str()),0);
			}
			else
			{
				//˵����ͬshape������һ���µ�shape��������
				
				shapes_.push_back(shape_temp);
				//��clear��clear������
				Void_temp.clear();
				//setlayer��
				if(iter_layer->second[i]=="TOP")
					shape_temp.setLayer(1);
				else if(iter_layer->second[i]=="PWR")
					shape_temp.setLayer(2);
				else if(iter_layer->second[i]=="BOTTOM")
					shape_temp.setLayer(layer_sum);
				else if(iter_layer->second[i]=="GND")
					shape_temp.setLayer(layer_sum-1);
				else
					shape_temp.setLayer(atoi(iter_layer->second[i].substr(4,1).c_str()));
				//����һ�����ݡ�����
				shape_temp.setVertices_(atof(iter_pointx->second[i].c_str()),atof(iter_pointy->second[i].c_str()),1);
			}
		}
		else if(iter_etchtype->second[i]=="VOID")
		{
			sscanf(iter_record->second[i].c_str(),"%d %d %d",nowtag1,nowtag2,nowtag3);
			
			if(nowtag3==2)
			{
				//shape�ĵڶ���void��
				
				if(nowtag3!=lastag3)
				{
					//�ڶ���void�ĵ�һ���㣻
					shape_temp.setVoidVertices(Void_temp,1);//��ǰһ��shape��void_Vertices_���㣬������һ��void����shape��void_Vertices_��
					Void_temp.clear();//��ǰһ��void�����е����㣻
					Voidpoint_temp.setXY(atof(iter_pointx->second[i].c_str()),atof(iter_pointy->second[i].c_str()));
					Void_temp.push_back(Voidpoint_temp);//����ڶ���void�ĵ�һ���㣻
				}
				else
				{
					//�ڶ���void�������㣻
					Voidpoint_temp.setXY(atof(iter_pointx->second[i].c_str()),atof(iter_pointy->second[i].c_str()));
					Void_temp.push_back(Voidpoint_temp);
				}
			}
			else 
			{
				//���ڶ���void������void��
				if(nowtag3!=lastag3)
				{
					//void�ĵ�һ���㣻
					Void_temp.clear();//��ǰһ��void�����е����㣻
					Voidpoint_temp.setXY(atof(iter_pointx->second[i].c_str()),atof(iter_pointy->second[i].c_str()));
					Void_temp.push_back(Voidpoint_temp);//����void�ĵ�һ���㣻
				
				}
				else
				{
					//void�������㣻
					Voidpoint_temp.setXY(atof(iter_pointx->second[i].c_str()),atof(iter_pointy->second[i].c_str()));
					Void_temp.push_back(Voidpoint_temp);
				}
			}
		}
		lastag1=nowtag1;lastag2=nowtag2;lastag3=nowtag3;
	}
}

int main()
{
	Layout layout1("D://Cadence SPB//tools//pcb//bin//extracta.exe","E://a//Galileo_G87173_204.brd");
	return 1;
}