#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tchar.h>
#include <Windows.h>
#include "brdTrans.h"


using namespace brdTrans;
using namespace std;



int brd2asc::Extracta(const string& ext_path,const string& fbrd,const string& cmd_path)
{
	string fext,fcmd1,fcmd2,fout1,fout2,fpath1,fpath2;

	fext = ext_path + "//extracta.exe";
	
    fcmd1 = cmd_path + "cmd1.txt"; 
	fcmd2 = cmd_path + "cmd2.txt";
	cout << fcmd1 <<endl;
	
	//fout1="E://a//fpin.txt E://a//fvia.txt E://a//fetch.txt E://a//fboard.txt E://a//ffgeo.txt E://a//fcpad.txt";
	fout1=cmd_path+"fpin.txt "+cmd_path+"fvia.txt "+cmd_path+"fetch.txt "+cmd_path+"fboard.txt "+cmd_path+"ffgeo.txt "+cmd_path+"fcpad.txt";
	fpath1 =  fext;//此处路径a文件夹需改成cmd_path。
	fpath1 += " ";
	fpath1 += fbrd;
	fpath1 += " ";
	fpath1 += fcmd1;
	fpath1 += " ";
	fpath1 += fout1;

	fout2=cmd_path+"fnet.txt "+cmd_path+"fcomp.txt "+cmd_path+"flayer.txt "+cmd_path+"paddef.txt "+cmd_path+"fcpin.txt "+cmd_path+"fconn.txt";
	fpath2 =  fext;
	fpath2 += " ";
	fpath2 += fbrd;
	fpath2 += " ";
	fpath2 += fcmd2;
	fpath2 += " ";
	fpath2 += fout2;
	
	
	system(fpath1.c_str());
	system("pause");
	return 0;

}

int brd2asc::writeCMD(const string& cmd_path)
{
	string cmd_path1,cmd_path2;
	ofstream fcmd1_File,fcmd2_File;
	cmd_path1=cmd_path+"cmd1.txt";
	cmd_path2=cmd_path+"cmd2.txt";
	fcmd1_File.open(cmd_path1,ios::trunc);
	fcmd2_File.open(cmd_path2,ios::trunc);

	//write the first ten sections into cmd1.txt;

	//full_geometry,class=pin;
	fcmd1_File << "FULL_GEOMETRY" << "\n";
	fcmd1_File << "CLASS=\"PIN\"" << "\n";
	fcmd1_File << "SUBCLASS" << "\n";
	fcmd1_File << "RECORD_TAG" << "\n";
	fcmd1_File << "GRAPHIC_DATA_NAME" << "\n";
	fcmd1_File << "GRAPHIC_DATA_NUMBER" << "\n";
	fcmd1_File << "GRAPHIC_DATA_1" << "\n";
	fcmd1_File << "GRAPHIC_DATA_2" << "\n";
	fcmd1_File << "GRAPHIC_DATA_3" << "\n";
	fcmd1_File << "GRAPHIC_DATA_4" << "\n";
	fcmd1_File << "GRAPHIC_DATA_5" << "\n";
	fcmd1_File << "GRAPHIC_DATA_6" << "\n";
	fcmd1_File << "GRAPHIC_DATA_7" << "\n";
	fcmd1_File << "GRAPHIC_DATA_8" << "\n";
	fcmd1_File << "GRAPHIC_DATA_9" << "\n";
	fcmd1_File << "GRAPHIC_DATA_10" << "\n";
	fcmd1_File << "REFDES" << "\n";
	fcmd1_File << "PIN_NAME" << "\n";
	fcmd1_File << "PIN_NUMBER" << "\n";
	fcmd1_File << "PAD_STACK_NAME" << "\n";
	fcmd1_File << "PAD_SHAPE_NAME" << "\n";
	fcmd1_File << "PAD_TYPE" << "\n";
	fcmd1_File << "PAD_FLASH" << "\n";
	fcmd1_File << "DRILL_HOLE_X" << "\n";
	fcmd1_File << "DRILL_HOLE_Y" << "\n";
	fcmd1_File << "SYM_NAME" << "\n";
	fcmd1_File << "SYM_TYPE" << "\n";
	fcmd1_File << "NET_NAME" << "\n";
	fcmd1_File << "PIN_X" << "\n";
	fcmd1_File << "PIN_Y" << "\n";
	fcmd1_File << "END" << "\n";
	fcmd1_File << " " << "\n";
	//full_geometry,class=via class;
	fcmd1_File << "FULL_GEOMETRY" << "\n";
	fcmd1_File << "CLASS=\"VIA CLASS\"" << "\n";
	fcmd1_File << "SUBCLASS" << "\n";
	fcmd1_File << "RECORD_TAG" << "\n";
	fcmd1_File << "GRAPHIC_DATA_NAME" << "\n";
	fcmd1_File << "GRAPHIC_DATA_NUMBER" << "\n";
	fcmd1_File << "GRAPHIC_DATA_1" << "\n";
	fcmd1_File << "GRAPHIC_DATA_2" << "\n";
	fcmd1_File << "GRAPHIC_DATA_3" << "\n";
	fcmd1_File << "GRAPHIC_DATA_4" << "\n";
	fcmd1_File << "GRAPHIC_DATA_5" << "\n";
	fcmd1_File << "GRAPHIC_DATA_6" << "\n";
	fcmd1_File << "GRAPHIC_DATA_7" << "\n";
	fcmd1_File << "GRAPHIC_DATA_8" << "\n";
	fcmd1_File << "GRAPHIC_DATA_9" << "\n";
	fcmd1_File << "GRAPHIC_DATA_10" << "\n";
	fcmd1_File << "REFDES" << "\n";
	fcmd1_File << "PIN_NUMBER" << "\n";
	fcmd1_File << "PAD_STACK_NAME" << "\n";
	fcmd1_File << "PAD_SHAPE_NAME" << "\n";
	fcmd1_File << "PAD_TYPE" << "\n";
	fcmd1_File << "PAD_FLASH" << "\n";
	fcmd1_File << "DRILL_HOLE_X" << "\n";
	fcmd1_File << "DRILL_HOLE_Y" << "\n";
	fcmd1_File << "NET_NAME" << "\n";
	fcmd1_File << "VIA_X" << "\n";
	fcmd1_File << "VIA_Y" << "\n";
	fcmd1_File << "END" << "\n";
	fcmd1_File << " " << "\n";
	//Full Geometry CLASS = "ETCH"
	fcmd1_File << "FULL_GEOMETRY" << "\n";
	fcmd1_File << "CLASS=\"ETCH\"" << "\n";
	fcmd1_File << "SUBCLASS" << "\n";
	fcmd1_File << "RECORD_TAG" << "\n";
	fcmd1_File << "GRAPHIC_DATA_NAME" << "\n";
	fcmd1_File << "GRAPHIC_DATA_NUMBER" << "\n";
	fcmd1_File << "GRAPHIC_DATA_1" << "\n";
	fcmd1_File << "GRAPHIC_DATA_2" << "\n";
	fcmd1_File << "GRAPHIC_DATA_3" << "\n";
	fcmd1_File << "GRAPHIC_DATA_4" << "\n";
	fcmd1_File << "GRAPHIC_DATA_5" << "\n";
	fcmd1_File << "GRAPHIC_DATA_6" << "\n";
	fcmd1_File << "GRAPHIC_DATA_7" << "\n";
	fcmd1_File << "GRAPHIC_DATA_8" << "\n";
	fcmd1_File << "GRAPHIC_DATA_9" << "\n";
	fcmd1_File << "GRAPHIC_DATA_10" << "\n";
	fcmd1_File << "REFDES" << "\n";
	fcmd1_File << "PIN_NUMBER" << "\n";
	fcmd1_File << "NET_NAME" << "\n";
	fcmd1_File << "END" << "\n";
	fcmd1_File << " " << "\n";
	//Full Geometry CLASS = "BOARD GEOMETRY"
	fcmd1_File << "FULL_GEOMETRY" << "\n";
	fcmd1_File << "CLASS=\"BOARD GEOMETRY\"" << "\n";
	fcmd1_File << "SUBCLASS=\"OUTLINE\"" << "\n";
	fcmd1_File << "RECORD_TAG" << "\n";
	fcmd1_File << "GRAPHIC_DATA_NAME" << "\n";
	fcmd1_File << "GRAPHIC_DATA_NUMBER" << "\n";
	fcmd1_File << "GRAPHIC_DATA_1" << "\n";
	fcmd1_File << "GRAPHIC_DATA_2" << "\n";
	fcmd1_File << "GRAPHIC_DATA_3" << "\n";
	fcmd1_File << "GRAPHIC_DATA_4" << "\n";
	fcmd1_File << "GRAPHIC_DATA_5" << "\n";
	fcmd1_File << "GRAPHIC_DATA_6" << "\n";
	fcmd1_File << "GRAPHIC_DATA_7" << "\n";
	fcmd1_File << "GRAPHIC_DATA_8" << "\n";
	fcmd1_File << "GRAPHIC_DATA_9" << "\n";
	fcmd1_File << "GRAPHIC_DATA_10" << "\n";
	fcmd1_File << "END" << "\n";
	fcmd1_File << " " << "\n";
	//COMPOSITE_PAD
	fcmd1_File << "COMPOSITE_PAD" << "\n";
	fcmd1_File << "CLASS" << "\n";
	fcmd1_File << "PAD_STACK_NAME" << "\n";
	fcmd1_File << "PAD_STACK_INNER_LAYER" << "\n";
	fcmd1_File << "PAD_STACK_TYPE" << "\n";
	fcmd1_File << "GRAPHIC_DATA_NAME" << "\n";
	fcmd1_File << "GRAPHIC_DATA_NUMBER" << "\n";
	fcmd1_File << "GRAPHIC_DATA_1" << "\n";
	fcmd1_File << "GRAPHIC_DATA_2" << "\n";
	fcmd1_File << "GRAPHIC_DATA_3" << "\n";
	fcmd1_File << "GRAPHIC_DATA_4" << "\n";
	fcmd1_File << "START_LAYER_NAME" << "\n";
	fcmd1_File << "END_LAYER_NAME" << "\n";
	fcmd1_File << "REFDES" << "\n";
	fcmd1_File << "PIN_NUMBER" << "\n";
	fcmd1_File << "PIN_NAME" << "\n";
	fcmd1_File << "SYM_MIRROR" << "\n";
	fcmd1_File << "PIN_X" << "\n";
	fcmd1_File << "PIN_Y" << "\n";
	fcmd1_File << "VIA_MIRROR" << "\n";
	fcmd1_File << "VIA_X" << "\n";
	fcmd1_File << "VIA_Y" << "\n";
	fcmd1_File << "NET_NAME" << "\n";
	fcmd1_File << "DRILL_HOLE_NAME" << "\n";
	fcmd1_File << "DRILL_HOLE_X" << "\n";
	fcmd1_File << "DRILL_HOLE_Y" << "\n";
	fcmd1_File << "DRILL_HOLE_PLATING" << "\n";
	fcmd1_File << "END" << "\n";
	fcmd1_File << " " << "\n";
	//NET
	fcmd1_File << "NET" << "\n";
	fcmd1_File << "NET_NAME" << "\n";
	fcmd1_File << "END" << "\n";
	//COMPONENT
	fcmd1_File << "REFDES" << "\n";
	fcmd1_File << "COMP_CLASS" << "\n";
	fcmd1_File << "COMP_PACKAGE" << "\n";
	fcmd1_File << "COMP_DEVICE_TYPE" << "\n";
	fcmd1_File << "COMP_VALUE" << "\n";
	fcmd1_File << "COMP_PART_NUMNER" << "\n";
	fcmd1_File << "SYM_MIRROR" << "\n";
	fcmd1_File << "SYM_NAME" << "\n";
	fcmd1_File << "SYM_ROTATE" << "\n";
	fcmd1_File << "SYM_TYPE" << "\n";
	fcmd1_File << "END" << "\n";
	fcmd1_File << " " << "\n";
	//LAYER
	fcmd1_File << "LAYER_SORT" << "\n";
	fcmd1_File << "LAYER_SUBCLASS" << "\n";
	fcmd1_File << "LAYER_ARTWORK" << "\n";
	fcmd1_File << "LAYER_USE" << "\n";
	fcmd1_File << "LAYER_CONDUCTOR" << "\n";
	fcmd1_File << "LAYER_DIELECTRIC_CONSTANT" << "\n";
	fcmd1_File << "LAYER_ELECTRICAL_CONDUCTIVITY" << "\n";
	fcmd1_File << "LAYER_MATERIAL" << "\n";
	fcmd1_File << "LAYER_SHIELD_LAYER" << "\n";
	fcmd1_File << "LAYER_THERMAL_CONDUCTIVITY" << "\n";
	fcmd1_File << "LAYER_THICKNESS" << "\n";
	fcmd1_File << "LAYER_TYPE" << "\n";
	fcmd1_File << "LAYER_LOSS_TANGENT" << "\n";
	fcmd1_File << "LAYER_FREQUENCY_DEPENDENT_FILE" << "\n";
	fcmd1_File << "END" << "\n";
	fcmd1_File << " " << "\n";
	//PAD_DEF
	fcmd1_File << "PAD_DEF" << "\n";
	fcmd1_File << "END" << "\n";
	fcmd1_File << " " << "\n";
	//COMPONENT_PIN
	fcmd1_File << "COMPONENT_PIN" << "\n";
	fcmd1_File << "REFDES_SORT" << "\n";
	fcmd1_File << "PIN_NUMBER_SORT" << "\n";
	fcmd1_File << "REFDES" << "\n";
	fcmd1_File << "PIN_NUMBER" << "\n";
	fcmd1_File << "PIN_X" << "\n";
	fcmd1_File << "PIN_Y" << "\n";
	fcmd1_File << "PIN_EDITED" << "\n";
	fcmd1_File << "PIN_COMMON_CODE" << "\n";
	fcmd1_File << "PIN_SWAP_CODE" << "\n";
	fcmd1_File << "PIN_TYPE" << "\n";
	fcmd1_File << "PAD_STACK_NAME" << "\n";
	fcmd1_File << "NET_NAME" << "\n";
	fcmd1_File << "PIN_FLOATING_PIN" << "\n";
	fcmd1_File << "PIN_GROUND" << "\n";
	fcmd1_File << "PIN_NC" << "\n";
	fcmd1_File << "PIN_NO_PIN_ESCAPE" << "\n";
	fcmd1_File << "PIN_NO_SHAPE_CONNECT" << "\n";
	fcmd1_File << "PIN_NO_SWAP_PIN" << "\n";
	fcmd1_File << "PIN_PAD_STACK_NAME" << "\n";
	fcmd1_File << "PIN_PINUSE" << "\n";
	fcmd1_File << "PIN_PIN_ESCAPE" << "\n";
	fcmd1_File << "PIN_POWER" << "\n";
	fcmd1_File << "END" << "\n";
	fcmd1_File << " " << "\n";
	fcmd1_File.close();

	//write the second two sections into cmd2.txt;

	//CONNECTIVITY
	fcmd2_File << "CONNECTIVITY" << "\n";
	fcmd2_File << "NET_NAME_SORT" << "\n";
	fcmd2_File << "NODE_SORT" << "\n";
	fcmd2_File << "NODE_1_NUMBER" << "\n";
	fcmd2_File << "NODE_2_NUMBER" << "\n";
	fcmd2_File << "RECORD_TAG" << "\n";
	fcmd2_File << "CLASS" << "\n";
	fcmd2_File << "SUBCLASS" << "\n";
	fcmd2_File << "NET_NAME" << "\n";
	fcmd2_File << "GRAPHIC_DATA_NAME" << "\n";
	fcmd2_File << "GRAPHIC_DATA_NUMBER" << "\n";
	fcmd2_File << "GRAPHIC_DATA_1" << "\n";
	fcmd2_File << "GRAPHIC_DATA_2" << "\n";
	fcmd2_File << "GRAPHIC_DATA_3" << "\n";
	fcmd2_File << "GRAPHIC_DATA_4" << "\n";
	fcmd2_File << "GRAPHIC_DATA_5" << "\n";
	fcmd2_File << "GRAPHIC_DATA_6" << "\n";
	fcmd2_File << "GRAPHIC_DATA_7" << "\n";
	fcmd2_File << "GRAPHIC_DATA_8" << "\n";
	fcmd2_File << "GRAPHIC_DATA_9" << "\n";
	fcmd2_File << "GRAPHIC_DATA_10" << "\n";
	fcmd2_File << "NODE_CONNECTS" << "\n";
	fcmd2_File << "RAT_CONNECTED" << "\n";
	fcmd2_File << "REFDES" << "\n";
	fcmd2_File << "PIN_NUMBER" << "\n";
	fcmd2_File << "PIN_TYPE" << "\n";
	fcmd2_File << "PIN_X" << "\n";
	fcmd2_File << "PIN_Y" << "\n";
	fcmd2_File << "VIA_X" << "\n";
	fcmd2_File << "VIA_Y" << "\n";
	fcmd2_File << "VIA_MIRROR" << "\n";
	fcmd2_File << "PAD_STACK_NAME" << "\n";
	fcmd2_File << "START_LAYER_NAME" << "\n";
	fcmd2_File << "END_LAYER_NAME" << "\n";
	fcmd2_File << "COMP_DEVICE_TYPE" << "\n";
	fcmd2_File << "COMP_TERMINATOR_PACK" << "\n";
	fcmd2_File << "COMP_VALUE" << "\n";
	fcmd2_File << "END" << "\n";
	fcmd2_File << " " << "\n";
	//FULL_GEOMETRY_BASEVIEW
	fcmd2_File << "FULL_GEOMETRY" << "\n";
	fcmd2_File << "CLASS" << "\n";
	fcmd2_File << "SUBCLASS" << "\n";
	fcmd2_File << "RECORD_TAG" << "\n";
	fcmd2_File << "GRAPHIC_DATA_NAME" << "\n";
	fcmd2_File << "GRAPHIC_DATA_NUMBER" << "\n";
	fcmd2_File << "GRAPHIC_DATA_1" << "\n";
	fcmd2_File << "GRAPHIC_DATA_2" << "\n";
	fcmd2_File << "GRAPHIC_DATA_3" << "\n";
	fcmd2_File << "GRAPHIC_DATA_4" << "\n";
	fcmd2_File << "GRAPHIC_DATA_5" << "\n";
	fcmd2_File << "GRAPHIC_DATA_6" << "\n";
	fcmd2_File << "GRAPHIC_DATA_7" << "\n";
	fcmd2_File << "GRAPHIC_DATA_8" << "\n";
	fcmd2_File << "GRAPHIC_DATA_9" << "\n";
	fcmd2_File << "GRAPHIC_DATA_10" << "\n";
	fcmd2_File << "REFDES" << "\n";
	fcmd2_File << "PIN_NUMBER" << "\n";
	fcmd2_File << "PAD_STACK_NAME" << "\n";
	fcmd2_File << "PAD_SHAPE_NAME" << "\n";
	fcmd2_File << "PAD_TYPE" << "\n";
	fcmd2_File << "PAD_FLASH" << "\n";
	fcmd2_File << "DRILL_HOLE_X" << "\n";
	fcmd2_File << "DRILL_HOLE_Y" << "\n";
	fcmd2_File << "SYM_NAME" << "\n";
	fcmd2_File << "SYM_TYPE" << "\n";
	fcmd2_File << "NET_NAME" << "\n";
	fcmd2_File << "PIN_X" << "\n";
	fcmd2_File << "PIN_Y" << "\n";
	fcmd2_File << "VIA_X" << "\n";
	fcmd2_File << "VIA_Y" << "\n";
	fcmd2_File << "SEG_CAPACITANCE" << "\n";
	fcmd2_File << "SEG_IMPEDANCE" << "\n";
	fcmd2_File << "SEG_INDUCTANCE" << "\n";
	fcmd2_File << "SEG_PROPAGATION_DELAY" << "\n";
	fcmd2_File << "SEG_RESISTANCE" << "\n";
	fcmd2_File << "END" << "\n";
	fcmd2_File << " " << "\n";
	fcmd1_File.close();

	return 1;

}


brd2asc::brd2asc(const std::string& ext_path,const std::string& brdfilename)
{
	//获取运行程序所在路径,将cmd文件和out文件暂存在运行程序路径中。
	TCHAR cmd_path[_MAX_PATH + 1]={0};
    GetModuleFileName(NULL, cmd_path, _MAX_PATH);
    (_tcsrchr(cmd_path, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串

	writeCMD(cmd_path);
	Extracta(ext_path,brdfilename,cmd_path);


}











