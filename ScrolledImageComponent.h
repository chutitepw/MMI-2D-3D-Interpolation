#ifndef MYSCROLLIMAGECOMPONENT
#define MYSCROLLIMAGECOMPONENT

#include "wx/wx.h"
#include <iostream>
#include "wx/dcgraph.h"


#include <wx/dcbuffer.h>
#include <wx/dcclient.h>
#include <wx/graphics.h>

#include <wx/bitmap.h>
#include <stdlib.h>
#include <wx/dc.h>

#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <wx/xml/xml.h>


#include <iterator>
#include <array>

/*******************/
#include "MyFrame.h"

class Shape;

enum ImageOrientation {
	Pos_0,
	Pos_90,
	Pos_180,
	Pos_270

};
enum DrawingTools {
	t_Default,
	t_Circle,
	t_Rectangle,
	t_Dot,
	t_Line,
	t_Polygon,
	t_Select,
	t_Reference
};


class ScrolledImageComponent : public wxScrolledWindow
{
	
	wxBitmap m_bitmap;
	/**
	* @param m_image 2D Panel
	*/
	wxImage m_image;

	ImageOrientation m_orientation;
	DrawingTools m_tools;

	/*shapes "saved"*/
	std::vector<Shape*> m_shapes;
	Shape * m_shape_drawing;
	Shape * m_shape_select=NULL;
	Shape * m_shape_select_temp = NULL;

	/*select & delete*/
	int m_select = 0;
	int m_selected = 999;

	/*reference*/
	long refcount = 0;
	

	wxPoint m_mouse_pos_panel;
	wxPoint m_mouse_pos_panel_absolute;
	wxRealPoint m_mouse_pos_image;

	std::vector<wxRealPoint> m_datas;
	/*zoom*/
	double z_x = 1.00, z_y = 1.00;
	int m_zoom=0;


	/*save*/
	unsigned char* m_myImage;//unuse

	/*size*/
	int m_w, m_h;

	/*coordinate*/
	/*
	int xx_, yy_;
	int z_xx_, z_yy_;
	int xxx_, yyy_;
	*/
	void Rotate90(bool right);
public:
	/**
	* Main 2D Panel
	*/
	ScrolledImageComponent();
	ScrolledImageComponent(wxWindow* parent, wxWindowID id);
	~ScrolledImageComponent();

	bool LoadImage(wxString fileName);

	void SaveImage();//unuse

	void OnMouseMoved(wxMouseEvent& event);
	void OnMouseDown(wxMouseEvent& event);
	void OnMouseLeftDbleClick(wxMouseEvent& event);
	void OnPaintEvent(wxPaintEvent & evt);

	void OpenShape(wxXmlNode* child);
	void SaveShape(wxXmlNode* node);

	void RotateRight();
	void RotateLeft();

	void Circle(){ m_tools = t_Circle;}
	void Rectangle(){ m_tools = t_Rectangle;}
	void Dot(){ m_tools = t_Dot;}
	void Default(){ m_tools = t_Default;}
	void Line()	{ m_tools = t_Line;}
	void Polygon(){ m_tools = t_Polygon;}
	void Select() { m_tools = t_Select; }
	void Reference() { m_tools = t_Reference; }
	void ZoomI();
	void ZoomO();
	void ESC();
	void Enter();
	void Remove();

	wxRealPoint RSelect();
	const wxRealPoint CoordinatePanel2Image(const wxPoint& pt);
	const wxPoint CoordinateImage2Panel(const wxRealPoint& pt);

	void refText(wxRealPoint pt, DPoint dpt);
	std::vector<wxRealPoint> m_reference;
	void StoreRef(const wxRealPoint& pt) { m_reference.push_back(pt); };
	std::vector<wxRealPoint> GetRef() { if (!m_reference.empty()) { return m_reference; } };

	void Link2D(std::vector <std::pair <wxRealPoint, DPoint>> Link2D);
	void SetLink(std::vector <std::pair <wxRealPoint, DPoint>> Link2D);
	double Area(std::vector<wxRealPoint> &refpt);
	std::vector <std::pair <wxRealPoint, DPoint>> RefLink;
	//wxRealPoint interpol;
	void GetInter(wxRealPoint &interpt, std::string shapetype);
	void Delaunay();
	void ToggleDelaunay();
	bool T_Delaunay=false;
	void EndShape();
	DECLARE_EVENT_TABLE()
	
	/**/
	//OSGCanvas *osg;
};

#endif