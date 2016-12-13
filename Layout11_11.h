#pragma once
#include <string>
#include <vector>
#include <fstream>
//#include "../3di_parser/include/tdi.h"



namespace psiv{

enum TypeDevice{};//Component
enum TypePadStack {SMD_PS=1, PTH_PS, Blind_PS, Buried_PS};
enum TypePCB{PCB=1, Package};
enum TypeTechnologyDrill{Mechanical=1, Laser};//Padstack
enum TypeShape{Na=0, Circular=1, Polygon, Oblong_x, Oblong_y, SHAPE};//修改值；刘畅修改；
enum TypeControlledImpedance{SingleEnded=1, Differential};//Stackup
enum TypeConductorLayer{Route=1, Plane};//Stackup
enum TypeConductorFoil{VLP=1, RTF};//Stackup
enum TypeDielectric{Soldermask=1, Core, Prepreg};//Stackup
enum TypeVia{PTH_V=1, Blind_V, Buried_V, Drilled_V};

class Point2D
{
friend Point2D operator-(const Point2D& a, const Point2D& b)
{
	return Point2D(a.getX()-b.getX(),a.getY()-b.getY());
}
public:
	explicit Point2D(){}
	explicit Point2D(double x, double y):x_(x),y_(y){}
	~Point2D(){}
	bool setXY(double x, double y)
	{
		x_ = x; y_ = y;
		return true;
	}
	bool setX(double x) { x_ = x; return true; }
	bool setY(double y) { y_ = y; return true; }
	const double getX() const { return x_; }
	const double getY() const { return y_; }
private:
	double x_;
	double y_;
};

class TLine
{
public:
	explicit TLine(){}
	~TLine(){}

private:
	// a section of transmission line consisting of multiple straight sections
	//string ID;
	int id_;
	int net_index_;  // index of the Net
	int layer_;  // copper layer number
	std::vector<Point2D> path_;  // vector containing the start, stop and all intermediate turning points of the TLine
	std::vector<double> width_;  // values correspond to each straight section of the Path vector; 
	// size of Width vector is smaller than size of Path vector by 1
	// index 0 is generally always resereved in vectors, unless noted otherwise


};

class DblMNM {
public:
	DblMNM(){}
	DblMNM(double a, double b, double c):min_(a),nom_(b),max_(c){}
private:
	double min_;
	double nom_;
	double max_;

};

class IntMNM {
public:
	IntMNM(){}
	IntMNM(int a, int b, int c):min_(a),nom_(b),max_(c){}
private:
	int min_;
	int nom_;
	int max_;
};

class Color {
public: 
  Color() : r_(0), g_(0), b_(0), transp_(-1) {}
  Color(int r, int g, int b, int t = -1) : r_(r), g_(g), b_(b), transp_(t) {}
  // get the red value
  int red() const { return r_; }
  // get the green value
  int green() const { return g_; }
  // get the blue value
  int blue() const { return b_; }
  // get the transparency
  int transparency() const { return transp_; }

private:
  int r_, g_, b_;  // red, green, blue
  int transp_;     // transparency
};

class Padstack
{
public:
	Padstack() {}
	//Padstack(TDI::Geometry*, int layers);
	//Padstack(std::map<std::string,std::vector<std::string>>& temp_map，int i);
	~Padstack() {}
	std::string getName() const { return name_; }
	TypePadStack getType() const { return type_; }
	DblMNM getDrillDiameter() const { return drill_diameter_; }
	TypeTechnologyDrill getDrillTechnology() const { return drill_technology_; }
	DblMNM getPlatingThickness() const { return plating_thickness_; }
	//int getLayerStart() const { return LayerStart; }
	//int getLayerStop() const { return LayerStop; }
	const std::vector<TypeShape>& getPadTypes() const { return pad_types_; }
	const std::vector<Point2D>& getPadShapes(int index) const { return pad_shapes_[index]; }
	const std::vector<std::vector<Point2D> >& getPadShapes() const { return pad_shapes_; }

	const double getRotation(int layer) const;
	//void addPadLayer(TDI::Geometry* );
	void setName(std::string name) { name_ = name; };//加了setpinname函数；
	void setTypePadStack(enum TypePadStack type) {type_=type;};
	void setDrillDiameter(double dia,double pos,double neg);//加了setdrilldia的函数,函数实现在layout_brd;
	void setPadOffset(double x, double y,int flag); 
	void setDriallOffset(double x, double y){DriallOffset.setXY(x,y);}; 
	void setLayerStart(int x){layer_start_=x;};
	void setLayerStop(int x){layer_stop_=x;};
	void setTypeShape(enum TypeShape shape,int layer);
	void setPadShape(std::vector<Point2D> point,int layer) {if(layer==1){ pad_shapes_.clear();pad_shapes_.push_back(point);}else pad_shapes_.push_back(point);};
private:
	std::string name_; // name of the padstack
	enum TypePadStack type_;
	// type of the padstack: { 1:"SMD", 2:"PTH", 3:"Blind", 4:"Buried"}
	// "SMD": surface mount either on Top or Bottom layer only
	// "PTH": plated through hole from Top to Bottom layers
	// "Blind": 指drill没有打通全部还是没有palate全部（即不是全部层都有pad）？drill hole does not go through all layers, but includes either Top or Bottom layer
	// "Buried": drill hole goes through neither top nor bottom layers
	//double DiameterDrillMin; // min, nominal, max of drill diameter at vector indices 1,2,3 (does not apply to SMD)
	//double DiameterDrillNom;
	//double DiameterDrillMax;
	DblMNM drill_diameter_;

	// three-numbers of (min, nominal, max) plating thickness of the barrel
	//double ThicknessPlatingMin;
	//double ThicknessPlatingNom;
	//double ThicknessPlatingMax;
	DblMNM plating_thickness_;
	
	enum TypeTechnologyDrill drill_technology_;
	// 1:"mechanical" or 2:"laser", case insensitive, partial match is ok; informational only

	//start/stop:pad layer
	int layer_start_;  // copper layer number, Top is always layer 1
	int layer_stop_;    // copper layer number

	std::vector<enum TypeShape> pad_types_; 
	// 0:"circular", 1:"polygon" or -1:"na"
	// index 0 is reserved
	// index 1 represents Top layer;
	// the last index represents Bottom layer;
	// "circular" represents circular pad; "polygon" represents arbitrary polygon pad; "na" represents no pad
	std::vector<Point2D> PadOffset; // relative to the drill center, which is always the reference (0,0) of the padstack
	Point2D DriallOffset;

	std::vector<std::vector<Point2D>> pad_shapes_;//case
	// the inner vector contains points for the pad on a given layer; the last point is the same as the first point
	// the indices of the outer vector correspond to layers from Top to Bottom
	// if pad does not exist on a given layer, all Point2D elements of the inner vector should be non-exist or ignored if exist
	// if PadShape is circular, then PadData(1) is center; (2) is on the circle.
	// if PadShape is polygon, then PadData is vertices（若为多边形，data为长宽）
	

};

class Pin
{
public:
	Pin() {}
	//Pin(TDI::Geometry* ,Padstack&, int, int);
	~Pin() {}
	int getColor() const { return color_; };
	std::string getRefDes()const { return refdes_; };
	std::string getPinName()const { return pin_name_; };//加了getpinname函数；
	int getNetIndex() const { return net_index_; };
	//std::string getNumber() const { return pin_number_; } // may use int instead of string
	int getPadstackIndex() const { return padstack_index_; };
	double getRotation() const { return rotation_; }
	Point2D getLocation() const { return location_; }
	void setID(int i) { id_ = i; }
	void setNetIndex(int i) { net_index_ = i; }
	void setRefDes(std::string res) { refdes_ = res; };
	void setPinName(std::string name) { pin_name_ = name; };//加了setpinname函数；
	//void setNum(int i) { pin_number_ = i; }
	void setRefNum(std::string );
	void setPinStack(int i) { padstack_index_ = i; }
	void setPadExist(int i);
	void setLocation(double x, double y) { location_.setXY(x,y); }
	void setRotation(double angle) { rotation_ = angle; }
	void setPadExist(std::vector<int> padexist) {pad_exist_=padexist;}
private:
	int id_;
	int color_;
	std::string refdes_;  // of the component the pin belongs to; every pin must belong to a component (refdes)
	std::string pin_name_;
	////int pin_number_;  // every pin has a "number" indicating its location in the component footprint
	//std::string name_;    // the name of the signal
	// Symbol contains signal name_ info, however, it is possible that each component may have different signal name for the same pin in same Symbol ??

	// "Type" information is in Padstack, should not be duplicated here
	//std::string Type; // SMD, PTH, Blind
	int net_index_; // to which this Pin is connected to
	//Padstack Pinstack;  
	int padstack_index_; //
	std::vector<int> pad_exist_; // 1: exist; 0:non-exist
	// index 0 is reserved
	// index 1 is for layer 1; index N for layer N, so on so forth
	double rotation_;  // degree rotation clockwise of the pin padstack
	// issue: pin mounted from bottom layer ???
	Point2D location_; // x,y coordinates local to the component reference system
};

class Via
{
public:
	Via() {}
	//Via(TDI::Geometry* ,Padstack&, int, int);
	~Via() {}
	int getID() { return id_; }
	int getColor() { return color_; }
	void setID(int i) { id_ = i; }
	void setNetIndex(int t) { net_index_ = t; }
	void setViaStack(int i) { padstack_index_ =i; }
	void setPadExist(int i);
	void setLocation(double x, double y) { location_.setXY(x,y); }
	void setRotation(double angle) { rotation_ = angle; }

private:
	int id_; // a system assigned int value for this via
	//string Type; // PTH, Blind, Buried, Drilled//
	//string Net; // net name to which this via belongs to//判断哪几行属于同一个via：按照drill hole xy判断在同一层，再验证padstack name是否同样，然后再对layer
	//enum TypeVia type_; // PTH, Blind, Buried, Drilled;问题；//???是不是删掉了？？从pad里面找，看每层有无pad，看
	int net_index_; // net name index to which this via belongs to；//与net_list关联；
	int color_;//暂时空缺；
	Point2D location_; 
	int padstack_index_;
	std::vector<int> pad_exist_; 
	double rotation_;////若是circle，且都无padoff，则为0；其他情况暂时空缺，无法找到pad的rotation信息，log一下，做default处理
	int back_drill_layer_start_;  // 暂时空缺starting from this layer, the via is drilled out (removed)
	int back_drill_layer_stop_;   // 暂时空缺the backdrill (i.e. removal of via) is stopped at this layer
	// the start layer number is always smaller than the stop layer number
	DblMNM back_drill_diameter_;//暂时空缺；
};

class Shape
{
public:
	Shape() {}
	//Shape(TDI::Geometry* );
	~Shape() {}
	void setID(int t) { id_ = t; }
	void setNet(int t) { net_index_ = t; }
	void setLayer(int t) { layer_ = t; }
	std::vector<Point2D>& Vertices(){ return vertices_; }
	std::vector<enum TypeShape>& LVT() { return void_types_; }
	std::vector<std::vector<Point2D>>& LVV() { return void_vertices_; }
	int getID(){return id_;}
	int getLayer(){return layer_;}
	void setVertices_(double x,double y,int flag);
	void setVoidVertices(std::vector<Point2D> point,int flag) {if(flag==1){ void_vertices_.clear();void_vertices_.push_back(point);}else void_vertices_.push_back(point);};
private:
	int id_;  
	int net_index_;  // representing the net this Shape belongs to
	int layer_; 
	int color_index_;
	std::vector<Point2D> vertices_;  // 
	std::vector<int> shape_flags_;//2016-11-28，刘畅加；
	//std::vector<string> VoidIDs;  // this is deprecated
	//std::vector<string> VoidTypes;  // "circular", "polygon"
	std::vector<enum TypeShape> void_types_;  // 0:"circular", 1:"polygon"
	std::vector<std::vector<Point2D>> void_vertices_;
	// if circular, first point (vector index 1) is center, second point is (r,0) where r is radius
	// if polygon,  start from index 1; last point is the same as the first point
	std::vector<std::vector<int>> void_flags_;
};
//void分两类，一类是circular，其他形状第二类，圆心
class Component
{
public:
	Component() {}
	//Component(TDI::Component*, std::vector<Pin>& );
	~Component() {}
	void setRefDes(std::string ref) { refdes_ = ref; }
	void setSymbolName(std::string name) {symbol_name_=name;};
	void setLocation(double x, double y) { location_.setXY(x,y); }
	void setRotation(double r) { rotation_ = r; }
	void setPartNumber(std::string part_num) {part_number_=part_num;};
private:
	std::string refdes_;  // reference designator, such as R101, C54, U5, ...
	//string DeviceType;  // a string uniquely representing the component type such as R2k_1%, C2N1, ..
	std::string symbol_name_;
	Point2D location_;  // the symbol origin is placed at this location
	int layer_; // top or bottom placement；
	double rotation_; // degree symbol is rotated CW after placement on top or bottom, always looking from above Top layer
	// this convention is to be verified with Cadence (optional)
	std::vector<int> net_indices_; // list of nets connected to the pins, in the same sequence as the PinNames in Symbol

	std::string part_number_;
};

class Symbol {
public:
	Symbol() {}
	//Symbol(TDI::Component* ,std::vector<Pin>&);
	~Symbol() {}
	const std::vector<Point2D>& getPinCenter() {return pin_centers_;}
	const std::string getName() {return name_;};
	void setName(std::string name) {name_=name;};
	void setPinName(std::vector<std::string> pin_name) {pin_names_=pin_name;};
	void setPinCenter(double x,double y,int flag) ;
	void setTypeShape(std::vector<enum TypeShape> pin_type) {pin_types_=pin_type;};
private:
	std::string name_;
	////std::string part_number_;
	////std::string device_type_;
	// all vectors have index 0 reserved; all data start from vector index 1
	////std::vector<std::string> pin_numbers_;
	std::vector<std::string> pin_names_;//name有重复（与pin_center的index对应，不同pin_center的name可能相同）
	std::vector<Point2D> pin_centers_;  // relative to the origin of the local coordinates of the symbol
	////std::vector<int> pin_stacks_;
	////std::vector<double> pin_rotations_;

	std::vector<enum TypeShape> pin_types_; // pin shape type
	std::vector<std::vector<Point2D>> pin_shapes_;//
	
};

class Part{
public:
	const std::string getPartNumber() {return part_number_;};
	const std::vector<Point2D>& getPinCenter() {return pin_centers_;};
	void setPartNumber(std::string part_num) {part_number_=part_num;};
	void setPartType(std::string part_type) {part_number_=part_type;};
	void setPinName(std::vector<std::string> pin_name) {pin_name_=pin_name;};
	void setPinCenter(double x,double y,int flag) ;
private:
	std::string part_number_;//brd有
	std::string parent_ppt_;
	std::string parent_ppt_part_;
	std::string parent_part_type_;//brd有
	std::string part_name_;
	std::vector<std::string> pin_name_; // index indicates the pin number;问题（与symbol一样，也加上s变复数？）
	std::vector<Point2D> pin_centers_;
};

class ControlledImpedance
{
public:
	ControlledImpedance() {}
	~ControlledImpedance() {}

private:
	std::string name_;
	enum TypeControlledImpedance type_;//--std::string type; // either "single-ended" or "differential", partial match is ok
	//double ValueNom; // impedance value in ohm
	//double ValueMin;  // minimum value of impedance by manufacturing tolerance
	//double ValueMax;
	DblMNM impedance_;
	std::vector<int> copper_layer_number_;
	std::vector<int> reference_layer_above;
	std::vector<int> reference_layer_below;
	std::vector<double> line_width_;
	std::vector<double> pitch_;

};

class Stackup
{
public:
	Stackup(std::string filename);
	Stackup() {}
	~Stackup() {}
	void setCLC(int n) { copper_layer_count_ = n; }
	void setLLT( const std::vector<double>& t) { layer_thickness_ = t; }

private:
	std::string name_;
	int copper_layer_count_; // copper layers only
	double thickness_over_laminate_;
	double thickness_over_copper_;
	double thickness_over_solder_mask_;

	std::vector<int> global_layer_number_;
	std::vector<std::string> global_layer_designator_;
	std::vector<int> conductor_layer_number_;
	std::vector<std::string> conductor_layer_name_;
	// std::vector<double> ConductorLayerThickness;
	std::vector<double> layer_thickness_;
	std::vector<std::string> copper_foil_weight_;
	std::vector<std::string> conductor_layer_type_; // "route", "plane"
	std::vector<std::string> conductor_foil_type_; // "VLP", "RTF", etc.
	//std::vector<string> ConductorLayerDomain;
	//std::vector<vector<string>> ConductorLayerUsage;
	std::vector<double> dk_;
	std::vector<double> df_;
	std::vector<std::string> dielectric_type_; // "soldermask", "core", "prepreg"
	std::vector<std::string> dielectric_make_;
	std::vector<std::string> dielectric_part_number_;
	std::vector<int> ply_;
	std::vector<std::string> glass_weave_;
	std::vector<ControlledImpedance> controlled_impedances_;
};

class Layout
{
public:
	Layout(){};
	//Layout(TDI::TDi& oTdi);
	Layout(const std::string& ext_path,const std::string& brdfilename);
	~Layout(){}
	const std::string getName()const { return name_; }
	const TypePCB getType()const { return type_; }
	const Stackup& getStackup(int index)const { return stackups_[index]; }
	const Component& getComponent(int index)const { return components_[index]; }
	const Symbol& getSymbol(int index)const { return symbols_[index]; }
	const Padstack& getPadstack(int index)const { return padstacks_[index]; }
	const std::vector<std::string>& getNetlist(int index)const { return net_list_[index]; }
	const Shape& getShape(int index)const { return shapes_[index]; }
	const Via& getVia(int index)const { return vias_[index]; }
	const Pin& getPin(int index)const { return pins_[index]; }
	std::vector<Pin>& Pins() { return pins_; }
	std::vector<Via>& Vias() { return vias_; }
	std::vector<Padstack>& Padstacks() { return padstacks_; }

	//void addPadstack(Padstack& t) { Padstacks.push_back(t); }
	//void addPadstacks(std::string filename);
	void setNetList(std::vector<std::vector<std::string>> net) {net_list_=net;};

private:
	std::string name_;
	enum TypePCB type_; // PCB or Package
	std::vector<Stackup> stackups_; // index 0 is default, all others are alternative stackups
	std::vector<Padstack> padstacks_;
	std::vector<Shape> shapes_;  // index 0 is Outline of the board
	std::vector<Via> vias_;
	std::vector<Pin> pins_;
	//std::vector<Trace> Traces;
	// Trace is removed due to redundancy with TLine
	std::vector<TLine> tlines_;
	//Outline is put under Shapes(0)
	//std::vector<vector<Point2D>> Outline; // board outline, index 0 is outer outline, index 1 and beyond are inner holes/cutouts
	std::vector<Component> components_;
	std::vector<Symbol> symbols_;
	std::vector<Part> parts_;
	std::vector<std::vector<std::string>> net_list_;
	// each inner vector<string> contains the following info
	//     (1) the implicit net index, which is the index of the outer vector (0 is reserved)
	//     (2) net name at inner vector index 0
	//     (3) a list of refdes.pin starting from index 1 to which this net is connected to
	
	std::vector<Color> color_table_;
};

}
