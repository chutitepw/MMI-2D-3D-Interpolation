#include "ScrolledImageComponent.h"

#include "./DrawShape/Rectangle.hpp"
#include "./DrawShape/Circle.hpp"
#include "./DrawShape/Point.hpp"
#include "./DrawShape/Line.hpp"
#include "./DrawShape/Polygon.hpp"
#include "./DrawShape/Reference.hpp"

#include "./Delaunay2D/delaunay.h"
/*#include "./Delaunay2D/delaunay.h"
#include "./Delaunay2D/triangle.h"
#include "./Delaunay2D/numeric.h"
#include "./Delaunay2D/edge.h"
#include "./Delaunay2D/vector2.h"*/

BEGIN_EVENT_TABLE(ScrolledImageComponent, wxScrolledWindow)
	EVT_MOTION(ScrolledImageComponent::OnMouseMoved)
	EVT_LEFT_DOWN(ScrolledImageComponent::OnMouseDown)
	EVT_PAINT(ScrolledImageComponent::OnPaintEvent)
	EVT_LEFT_DCLICK(ScrolledImageComponent::OnMouseLeftDbleClick)
END_EVENT_TABLE()
std::vector<wxRealPoint> RefPoint;
std::vector<Vector2<float> > points;
Delaunay<float> triangulation;
std::vector<Triangle<float> > triangles;
std::vector<Edge<float> > edges;
///////////////////////////////////////////////////////////////////////////////
ScrolledImageComponent::ScrolledImageComponent() {
	
};
///////////////////////////////////////////////////////////////////////////////
ScrolledImageComponent::ScrolledImageComponent(wxWindow* parent, wxWindowID id) :
	wxScrolledWindow(parent, id),
	m_orientation(Pos_0),
	m_shape_drawing (NULL)
{
	/**
	 * Main 2D Panel

		dfdfdf
	 * @todo Creation
	 * @param pppp badd
	*/
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	SetDoubleBuffered(true);

}
///////////////////////////////////////////////////////////////////////////////
ScrolledImageComponent::~ScrolledImageComponent()
{
	for (unsigned int i = 0; i < m_shapes.size(); ++i)
		delete m_shapes[i];
	m_shapes.clear();
}
///////////////////////////////////////////////////////////////////////////////
const wxRealPoint ScrolledImageComponent::CoordinatePanel2Image(const wxPoint& pt) {
	//gestion du zoom...
	wxRealPoint outB(pt.x/z_x, pt.y / z_y );
	wxRealPoint out;
	switch (m_orientation) {
	case Pos_0:
		out.x = outB.x;
		out.y = outB.y;
		break;
	case Pos_90:
		out.y = m_w - outB.x;
		out.x  = outB.y;
		break;
	case Pos_180:
		out.y = m_h - outB.y;
		out.x = m_w - outB.x;
		break;
	case Pos_270:
		out.y = outB.x;
		out.x = m_h - outB.y;
		break;
	}
	return out;
}
///////////////////////////////////////////////////////////////////////////////
const wxPoint ScrolledImageComponent::CoordinateImage2Panel(const wxRealPoint& pt) {
	wxPoint out(pt);
	switch (m_orientation) {
	case Pos_0:
		break;
	case Pos_90:
		out.y = pt.x;
		out.x = m_w - pt.y;
		break;
	case Pos_180:
		out.y = m_h - pt.y;
		out.x = m_w - pt.x;
		break;
	case Pos_270:
		out.y = m_h - pt.x;
		out.x = pt.y;
		break;
	}
	return out;
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::Rotate90(bool right) {
	wxPoint offset;
	wxImage temp = m_bitmap.ConvertToImage();
	//temp.SetMask(true);
	//temp.SetMaskColour(1, 2, 3);

	wxImage temp2 = temp.Rotate90(right);
	m_w = temp2.GetWidth();
	m_h = temp2.GetHeight();
	SetScrollbars(1, 1, (m_w*z_x), (m_h*z_y), 0, 0);
	m_bitmap = wxBitmap(temp2);

	Refresh();
}
void ScrolledImageComponent::RotateRight()
{
	switch (m_orientation) {
		case Pos_0:
			m_orientation = Pos_90;
			break;
		case Pos_90:
			m_orientation = Pos_180;
			break;
		case Pos_180:
			m_orientation = Pos_270;
			break;
		case Pos_270:
			m_orientation = Pos_0;
			break;
	}
	Rotate90(true);
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::RotateLeft()
{
	switch (m_orientation) {
		case Pos_0:
			m_orientation = Pos_270;
			break;
		case Pos_90:
			m_orientation = Pos_0;
			break;
		case Pos_180:
			m_orientation = Pos_90;
			break;
		case Pos_270:
			m_orientation = Pos_180;
			break;
	}
	Rotate90(false);
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::ZoomI()
{
	m_zoom += 1;
	z_x = pow(2, m_zoom);
	z_y = pow(2, m_zoom);
	SetScrollbars(1, 1, (m_w*z_x), (m_h*z_y), 0, 0);
	Refresh();
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::ZoomO()
{
	m_zoom -= 1;
	z_x = pow(2, m_zoom);
	z_y = pow(2, m_zoom);
	SetScrollbars(1, 1, (m_w*z_x), (m_h*z_y), 0, 0);
	Refresh();
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::ESC()
{
	//poly & line stop
	if (m_shape_drawing != NULL) {
		/*if (m_shape_drawing->NbPtsMax() == false) {
			//to remove last point...
			m_datas.pop_back();
			//put datas
			m_shape_drawing->SetDatas(m_datas);
			// add shape
			m_shapes.push_back(m_shape_drawing);
			m_shape_drawing = NULL;
			m_datas.clear();
			Refresh();

		}*/
		//cancel polyline
		if (m_shape_drawing->NbPtsMax() == false) {
			//to remove last point...
			for (unsigned int i = 0; i < m_datas.size(); ++i)
				m_datas.pop_back();
			m_datas.clear();
			
			m_shape_drawing = NULL;
			Refresh();

		}
		//cancel normal shape
		else if (m_shape_drawing->NbPtsToInitialize() == m_datas.size() && m_shape_drawing->NbPtsMax() == true) {
			m_shape_drawing->SetDatas(m_datas);
			//m_shapes.push_back(m_shape_drawing);
			m_datas.clear();
			m_shape_drawing = NULL;
			Refresh();
		}
		else {

		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::Enter()
{
	if (m_shape_drawing != NULL) {
		if (m_shape_drawing->NbPtsToInitialize() == m_datas.size() && m_shape_drawing->NbPtsMax() == true) {
			m_shape_drawing->SetDatas(m_datas);
			m_shapes.push_back(m_shape_drawing);
			m_datas.clear();
			m_shape_drawing = NULL;
			Refresh();
			return;
		}
		if (m_shape_drawing->NbPtsMax() == false) {
			//to remove last point...
			m_datas.pop_back();
			//put datas
			m_shape_drawing->SetDatas(m_datas);
			// add shape
			m_shapes.push_back(m_shape_drawing);
			m_shape_drawing = NULL;
			m_datas.clear();
			Refresh();

		}
	}

	
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::refText(wxRealPoint pt, DPoint dpt)
{
	if (m_shape_select != NULL)
		m_shape_select->SetRefT(pt, dpt);
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::Remove()
{
	if (m_shape_select != NULL) {
		m_shape_select = NULL;
		m_shapes.erase(m_shapes.begin()+m_select);
	}
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::OnMouseDown(wxMouseEvent& event)
{
	if (m_shape_drawing != NULL) {
		if (m_shape_drawing->NbPtsToInitialize() == m_datas.size() && m_shape_drawing->NbPtsMax()==true) {
			m_shape_drawing->SetDatas(m_datas);
			m_shapes.push_back(m_shape_drawing);
			m_datas.clear();
			m_shape_drawing = NULL;
			Refresh();
			return;
		}
	}
	//Beginning a shape...
	switch (m_tools) {
		case t_Circle:
			if(m_shape_drawing==NULL)
				m_shape_drawing = new ShapeCircle();
			break;
		case t_Rectangle:
			if (m_shape_drawing == NULL)
				m_shape_drawing = new ShapeRectangle();
			break;
		case t_Dot: {
			ShapePoint * sp=new ShapePoint();
			sp->SetData(m_mouse_pos_image);
			m_shapes.push_back(sp);
			Refresh();
			

			break; }
		case t_Line: {
			if (m_shape_drawing == NULL)
				m_shape_drawing = new ShapeLine();
			break;
		}
		case t_Polygon: {
			if (m_shape_drawing == NULL)
				m_shape_drawing = new ShapePolygon();
			break;
		}//t_Polygon
		case t_Select: {
			if (!m_shapes.empty()) {
				double d_min = m_shapes[0]->Distance(m_mouse_pos_image);
				m_select = 0;
				for (unsigned int i = 1; i < m_shapes.size(); ++i) {
					double d = m_shapes[i]->Distance(m_mouse_pos_image);
					if (d <= d_min) {
						d_min = d;
						m_select = i;
						
					}
					else {

					}

				}
				if (m_select == m_selected) {
					m_shape_select = NULL;
					m_selected = 999;
				}
				else {
					m_selected = m_select;
					m_shape_select = m_shapes[m_select];
				}
				
			}
			break;
		}
		case t_Reference: {
			refcount++;
			ShapeReference * sp = new ShapeReference();
			sp->SetData(m_mouse_pos_image);
			sp->SetRefpoint(refcount);
			m_shapes.push_back(sp);
			Refresh();

			

			break; }
		case t_Default:
			break;
	}//case 

	if (m_shape_drawing != NULL) {
		m_datas.push_back(m_mouse_pos_image);
		m_datas.push_back(m_mouse_pos_image);
	}
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::OnMouseMoved(wxMouseEvent& event)
{
	m_mouse_pos_panel = event.GetPosition();
	m_mouse_pos_panel_absolute = CalcUnscrolledPosition(m_mouse_pos_panel);
	m_mouse_pos_image = CoordinatePanel2Image(m_mouse_pos_panel_absolute);
	
	//	if (event.LeftIsDown()) {
	if (m_shape_drawing != NULL){
		if (m_datas.size() > 0)
			m_datas.pop_back();
		m_datas.push_back(m_mouse_pos_image);
		Refresh();
	}
}
///////////////////////////////////////////////////////////////////////////////
bool ScrolledImageComponent::LoadImage(wxString fileName)
{
	/*Reset screen*/
	m_orientation = Pos_0;
	z_x = 1;
	z_y = 1;
	m_image.LoadFile(fileName);
	if (m_image.IsOk() == false) {
		m_w = m_h = 0;
		return false;
	}
	m_w = m_image.GetWidth();
	m_h = m_image.GetHeight();
	SetScrollbars(1, 1, m_w, m_h, 0, 0);
	m_bitmap = wxBitmap(m_image);
	return true;
}
///////////////////////////////////////////////////////////////////////////////
/**********Unuse**********/
void ScrolledImageComponent::SaveImage()
{
	/*std::ofstream f("C:/MMI/Project/test.xml");
	f << "<MMIFile>" << std::endl;
	for (unsigned int i = 0; i < m_shapes.size(); ++i) {
		m_shapes[i]->WriteXML(f);
	}
	f << "</MMIFile>" << std::endl;
	f.close();*/
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::OpenShape(wxXmlNode* child)
{
	m_shapes.clear();
	
	while (child) {
		Shape* shape=NULL;
		if (child->GetName() == "circle") {
			shape = new ShapeCircle();
		}
		else if (child->GetName() == "rectangle") {
			shape = new ShapeRectangle();
		}
		else if (child->GetName() == "point") {
			shape = new ShapePoint();
		}
		else if (child->GetName() == "line") {
			shape = new ShapeLine();
		}
		else if (child->GetName() == "polygon") {
			shape = new ShapePolygon();
		}
		else if (child->GetName() == "reference") {
			shape = new ShapeReference();
			/*Get ref number*/
			wxXmlNode *child2 = child->GetChildren();
			while (child2) {
				if (child2->GetName() == "pt") {
					wxXmlNode *child3 = child2->GetChildren();
					while (child3) {
						if (child3->GetName() == "Ref") {
							wxString s = child3->GetNodeContent();
							s.ToLong(&refcount);
						}
						child3 = child3->GetNext();
					}
				}
				child2 = child2->GetNext();
			}
		}
		if (shape != NULL) {
			bool res = shape->LoadXML(child);
			if (res == true)
				m_shapes.push_back(shape);
			else
				delete shape;
		}
		child = child->GetNext();
	}//while

}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::SaveShape(wxXmlNode* node)
{
	/*wxCharBuffer buffer = fileName.ToUTF8();

	wxXmlDocument doc;
	wxXmlNode * XmlShape = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Shape"));
	doc.SetRoot(XmlShape);
	wxXmlNode * Xml2D = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("2D"));
	XmlShape->AddChild(Xml2D);
	for (unsigned int i = 0; i < m_shapes.size(); ++i) {
		//m_shapes[i]->WriteXML(XmlShape);
		m_shapes[i]->WriteXML(Xml2D);
	}

	wxXmlNode * Xml3D = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("3D"));
	XmlShape->AddChild(Xml3D);
	for (unsigned int i = 0; i < m_shapes.size(); ++i) {
		m_dshapes[i]->WriteXML(Xml2D);
	}
	doc.Save(buffer);//, wxXML_NO_INDENTATION);*/
	wxXmlNode * Xml2D = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("DD"));
	node->AddChild(Xml2D);
	for (unsigned int i = 0; i < m_shapes.size(); ++i) {
		//m_shapes[i]->WriteXML(XmlShape);
		m_shapes[i]->WriteXML(Xml2D);
	}
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::OnMouseLeftDbleClick(wxMouseEvent& event){
	if (m_shape_drawing != NULL) {
		if (m_shape_drawing->NbPtsMax()==false) {
				/*m_shapes.push_back(m_shape_drawing);
				m_shape_drawing = NULL;
				m_datas.clear();
				Refresh();*/
			//to remove last point...
			m_datas.pop_back();
			//put datas
			m_shape_drawing->SetDatas(m_datas);
			// add shape
			m_shapes.push_back(m_shape_drawing);
			m_shape_drawing = NULL;
			m_datas.clear();
			Refresh();

		}
	}
}
///////////////////////////////////////////////////////////////////////////////
wxRealPoint ScrolledImageComponent::RSelect() {
	if (m_shape_select != NULL) {
		if (typeid(*m_shape_select) == typeid(ShapeReference)) {
			wxRealPoint refp = m_shape_select->RSelectShape();
			return refp;
		}
		else {
			wxRealPoint refp;
			refp.x = NULL;
			refp.y = NULL;
			return refp;
		}
	}		
}
///////////////////////////////////////////////////////////////////////////////
void ScrolledImageComponent::OnPaintEvent(wxPaintEvent & evt)
{
	wxAutoBufferedPaintDC dc(this);

	PrepareDC(dc);

	dc.SetBackground(wxBrush(wxColour(200, 200, 200)));
	dc.Clear();
	/*zoom unzoom*/
	dc.SetUserScale(z_x, z_y);

	dc.DrawBitmap(m_bitmap, 0, 0, true);

	m_mouse_pos_panel_absolute = CoordinatePanel2Image(m_mouse_pos_panel_absolute);
	//	z_xx_ = m_mouse_pos_panel_absolute.x;
	//	z_yy_ = m_mouse_pos_panel_absolute.y;


		/*------------------coordinates start--------------------------*/
		/*std::string xx = std::to_string(mouse_x);
		std::string yy = std::to_string(mouse_y);
		//dc.DrawText("Screen coordinates: "+xx, 50, 200);
		//dc.DrawText(yy, 200, 200);
		dc.DrawText(xx, xx_+20, yy_);
		dc.DrawText(yy, xx_+50, yy_);*/

		/*std::string xx_str = std::to_string(z_xx_);
		std::string yy_str = std::to_string(z_yy_);
		//dc.DrawText("Translated zoom coordinates: "+xx_str, 50, 250);
		//dc.DrawText(yy_str, 200, 250);
		dc.DrawText(xx_str, xx_ + 20, yy_+20);
		dc.DrawText(yy_str, xx_ + 50, yy_+20);

		CoordinatePanel2Image(xx_, yy_, xxx_, yyy_);
		std::string xxx_str = std::to_string(xxx_);
		std::string yyy_str = std::to_string(yyy_);
		//dc.DrawText("Rotation coordinates: "+xxx_str, 50, 300);
		//dc.DrawText(yyy_str, 200, 300);
		dc.DrawText(xxx_str, xx_ + 20, yy_ + 60);
		dc.DrawText(yyy_str, xx_ + 50, yy_ + 60);*/
		/*------------------coordinates end--------------------------*/

		//draw shapes
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.SetPen(*wxBLACK_PEN);
	for (unsigned int i = 0; i < m_shapes.size(); ++i)
		m_shapes[i]->Draw(&dc, this);

	//draw drawing_shape
	dc.SetPen(*wxYELLOW_PEN);
	if (m_shape_drawing) {
		if (m_shape_drawing->NbPtsToInitialize() <= m_datas.size())
			m_shape_drawing->SetDatas(m_datas);
		m_shape_drawing->Draw(&dc, this);
	}

	//draw drawing selected shape
	if (m_tools == t_Select) {
		if (!m_shapes.empty()) {
			dc.SetPen(*wxRED_PEN);
			double d_min = m_shapes[0]->Distance(m_mouse_pos_image);
			int i_min = 0;
			unsigned int pos_min = 0;
			for (unsigned int i = 1; i < m_shapes.size(); ++i) {
				double d = m_shapes[i]->Distance(m_mouse_pos_image);
				if (d < d_min) {
					d_min = d;
					i_min = i;
				}
			}
			m_shape_select_temp = m_shapes[i_min];
			m_shape_select_temp->Draw(&dc, this);
		}
	}
	
	if (m_shape_select!=NULL) {
		dc.SetPen(*wxGREEN_PEN);
		m_shape_select->Draw(&dc, this);
	}
	
	/*Delaunay*/
	if (T_Delaunay) {

		//wxMemoryDC dc(m_bitmap);
		dc.SetPen(*wxGREEN_PEN);
		for (unsigned int i = 0; i < edges.size(); ++i) {
			dc.DrawLine(wxCoord(edges[i].p1.x), wxCoord(edges[i].p1.y), wxCoord(edges[i].p2.x), wxCoord(edges[i].p2.y));
		}
		dc.SetPen(*wxRED_PEN);
		for (unsigned int i = 0; i < points.size(); ++i) {
			dc.DrawCircle(wxCoord(points[i].x), wxCoord(points[i].y), 4);
		}
	}

	//must be refresh to load picture
	Refresh();
}

void ScrolledImageComponent::Link2D(std::vector <std::pair <wxRealPoint, DPoint>> Link2D) {
	RefLink = Link2D;


	/*Interpol*/
	if (!RefLink.empty() && RefLink.size() > 2) {
		


			Delaunay();
			//link
			std::vector<Triangle<float>> TRefPoint;
			std::vector<double> area;
			std::vector<wxRealPoint> triangle;
			double sumarea;
			double Tarea;
			for (int i = 0; i < triangles.size(); i++) {
				TRefPoint.push_back(triangles[i]);
			}
			for (int n = 0; n < m_shapes.size(); n++) {
				if (typeid(*m_shapes[n]) == typeid(ShapePoint)) {
					std::string shapetype = "Point";
					/*Find area of each triangle*/
					for (int i = 0; i < TRefPoint.size(); i++) {
						sumarea = 0;
						wxRealPoint temp_pt;
						temp_pt.x = TRefPoint[i].p1.x;
						temp_pt.y = TRefPoint[i].p1.y;
						triangle.push_back(temp_pt);
						wxRealPoint temp_pt2;
						temp_pt2.x = TRefPoint[i].p2.x;
						temp_pt2.y = TRefPoint[i].p2.y;
						triangle.push_back(temp_pt2);
						wxRealPoint temp_pt3;
						temp_pt3.x = TRefPoint[i].p3.x;
						temp_pt3.y = TRefPoint[i].p3.y;
						triangle.push_back(temp_pt3);
						Tarea = Area(triangle);
						/*Find point in triangle*/
						for (int j = 0; j < triangle.size(); j++) {
							wxRealPoint temp_pt;
							temp_pt = triangle[j];
							triangle[j] = m_shapes[n]->GetDatas();
							double calarea = Area(triangle);
							area.push_back(calarea);
							triangle[j] = temp_pt;
							sumarea = sumarea + calarea;
						}
						/*Check if shape is inside triangle*/
						if (sumarea == Tarea) {
							((MyFrame*)(m_parent->GetParent()))->Interpolate_2D_2_3D(shapetype, area, triangle);
							area.clear();
							triangle.clear();
							//break;
						}
						else {
							area.clear();
							triangle.clear();
						}
					}


				}
				if (typeid(*m_shapes[n]) == typeid(ShapeCircle)) {
					std::string shapetype = "Circle";
					std::vector<wxRealPoint> pts = m_shapes[n]->GetMultiDatas();

					for (int k = 0; k < pts.size(); k++) {
						for (int i = 0; i < TRefPoint.size(); i++) {

							sumarea = 0;
							wxRealPoint temp_pt;
							temp_pt.x = TRefPoint[i].p1.x;
							temp_pt.y = TRefPoint[i].p1.y;
							triangle.push_back(temp_pt);
							wxRealPoint temp_pt2;
							temp_pt2.x = TRefPoint[i].p2.x;
							temp_pt2.y = TRefPoint[i].p2.y;
							triangle.push_back(temp_pt2);
							wxRealPoint temp_pt3;
							temp_pt3.x = TRefPoint[i].p3.x;
							temp_pt3.y = TRefPoint[i].p3.y;
							triangle.push_back(temp_pt3);
							Tarea = Area(triangle);

							for (int j = 0; j < triangle.size(); j++) {

								wxRealPoint temp_pt;
								temp_pt = triangle[j];
								triangle[j] = pts[k];
								double calarea = Area(triangle);
								area.push_back(calarea);
								triangle[j] = temp_pt;
								sumarea = sumarea + calarea;
							}
							/*Check if shape is inside triangle*/
							if (sumarea == Tarea) {
								((MyFrame*)(m_parent->GetParent()))->Interpolate_2D_2_3D(shapetype, area, triangle);
								area.clear();
								triangle.clear();
								//break;
							}
							else {
								area.clear();
								triangle.clear();
							}
						}
					}
					((MyFrame*)(m_parent->GetParent()))->OnEndShape(shapetype);

				}
				if (typeid(*m_shapes[n]) == typeid(ShapeLine)) {
					std::string shapetype = "Line";
					std::vector<wxRealPoint> pts = m_shapes[n]->GetMultiDatas();

					for (int k = 0; k < pts.size(); k++) {
						for (int i = 0; i < TRefPoint.size(); i++) {

							sumarea = 0;
							wxRealPoint temp_pt;
							temp_pt.x = TRefPoint[i].p1.x;
							temp_pt.y = TRefPoint[i].p1.y;
							triangle.push_back(temp_pt);
							wxRealPoint temp_pt2;
							temp_pt2.x = TRefPoint[i].p2.x;
							temp_pt2.y = TRefPoint[i].p2.y;
							triangle.push_back(temp_pt2);
							wxRealPoint temp_pt3;
							temp_pt3.x = TRefPoint[i].p3.x;
							temp_pt3.y = TRefPoint[i].p3.y;
							triangle.push_back(temp_pt3);
							Tarea = Area(triangle);

							for (int j = 0; j < triangle.size(); j++) {

								wxRealPoint temp_pt;
								temp_pt = triangle[j];
								triangle[j] = pts[k];
								double calarea = Area(triangle);
								area.push_back(calarea);
								triangle[j] = temp_pt;
								sumarea = sumarea + calarea;
							}
							/*Check if shape is inside triangle*/
							if (sumarea == Tarea) {
								((MyFrame*)(m_parent->GetParent()))->Interpolate_2D_2_3D(shapetype, area, triangle);
								area.clear();
								triangle.clear();
								//break;
							}
							else {
								area.clear();
								triangle.clear();
							}
						}
					}
					/*End Line*/
					((MyFrame*)(m_parent->GetParent()))->OnEndShape(shapetype);
				}
				if (typeid(*m_shapes[n]) == typeid(ShapePolygon)) {
					std::string shapetype = "Polygon";
					std::vector<wxRealPoint> pts = m_shapes[n]->GetMultiDatas();

					for (int k = 0; k < pts.size(); k++) {
						for (int i = 0; i < TRefPoint.size(); i++) {

							sumarea = 0;
							wxRealPoint temp_pt;
							temp_pt.x = TRefPoint[i].p1.x;
							temp_pt.y = TRefPoint[i].p1.y;
							triangle.push_back(temp_pt);
							wxRealPoint temp_pt2;
							temp_pt2.x = TRefPoint[i].p2.x;
							temp_pt2.y = TRefPoint[i].p2.y;
							triangle.push_back(temp_pt2);
							wxRealPoint temp_pt3;
							temp_pt3.x = TRefPoint[i].p3.x;
							temp_pt3.y = TRefPoint[i].p3.y;
							triangle.push_back(temp_pt3);
							Tarea = Area(triangle);

							for (int j = 0; j < triangle.size(); j++) {

								wxRealPoint temp_pt;
								temp_pt = triangle[j];
								triangle[j] = pts[k];
								double calarea = Area(triangle);
								area.push_back(calarea);
								triangle[j] = temp_pt;
								sumarea = sumarea + calarea;
							}
							/*Check if shape is inside triangle*/
							if (sumarea == Tarea) {
								((MyFrame*)(m_parent->GetParent()))->Interpolate_2D_2_3D(shapetype, area, triangle);
								area.clear();
								triangle.clear();
								//break;
							}
							else {
								area.clear();
								triangle.clear();
							}
						}
					}
					/*End Line*/
					((MyFrame*)(m_parent->GetParent()))->OnEndShape(shapetype);
				}
				if (typeid(*m_shapes[n]) == typeid(ShapeRectangle)) {
					std::string shapetype = "Rectangle";
					std::vector<wxRealPoint> pts = m_shapes[n]->GetMultiDatas();

					for (int k = 0; k < pts.size(); k++) {
						for (int i = 0; i < TRefPoint.size(); i++) {

							sumarea = 0;
							wxRealPoint temp_pt;
							temp_pt.x = TRefPoint[i].p1.x;
							temp_pt.y = TRefPoint[i].p1.y;
							triangle.push_back(temp_pt);
							wxRealPoint temp_pt2;
							temp_pt2.x = TRefPoint[i].p2.x;
							temp_pt2.y = TRefPoint[i].p2.y;
							triangle.push_back(temp_pt2);
							wxRealPoint temp_pt3;
							temp_pt3.x = TRefPoint[i].p3.x;
							temp_pt3.y = TRefPoint[i].p3.y;
							triangle.push_back(temp_pt3);
							Tarea = Area(triangle);

							for (int j = 0; j < triangle.size(); j++) {

								wxRealPoint temp_pt;
								temp_pt = triangle[j];
								triangle[j] = pts[k];
								double calarea = Area(triangle);
								area.push_back(calarea);
								triangle[j] = temp_pt;
								sumarea = sumarea + calarea;
							}
							/*Check if shape is inside triangle*/
							if (sumarea == Tarea) {
								((MyFrame*)(m_parent->GetParent()))->Interpolate_2D_2_3D(shapetype, area, triangle);
								area.clear();
								triangle.clear();
								//break;
							}
							else {
								area.clear();
								triangle.clear();
							}
						}
					}
					/*End Line*/
					((MyFrame*)(m_parent->GetParent()))->OnEndShape(shapetype);
				}
				
			}
			TRefPoint.clear();

		
	}
}
void ScrolledImageComponent::SetLink(std::vector <std::pair <wxRealPoint, DPoint>> Link2D) {
	RefLink = Link2D;
}
double ScrolledImageComponent::Area(std::vector<wxRealPoint> &refpt) {
	double area = fabs((refpt[0].x*(refpt[1].y - refpt[2].y) + refpt[1].x*(refpt[2].y - refpt[0].y) + refpt[2].x*(refpt[0].y - refpt[1].y)) / 2);
	return area;
}
void ScrolledImageComponent::GetInter(wxRealPoint &interpt, std::string shapetype) {

	m_datas.push_back(interpt);
	if (m_shape_drawing != NULL) {
			m_shape_drawing->SetDatas(m_datas);
			m_shapes.push_back(m_shape_drawing);
			
			//m_shape_drawing = NULL;
			Refresh();
			return;
	}
	if (shapetype == "Point") {
		ShapePoint * sp = new ShapePoint();
		sp->SetData(interpt);
		m_shapes.push_back(sp);
		Refresh();
	}
		
	if (shapetype == "Circle") {
		if (m_shape_drawing == NULL)
			m_shape_drawing = new ShapeCircle();
	}
		
	if (shapetype == "Line") {
		if (m_shape_drawing == NULL)
			m_shape_drawing = new ShapeLine();
	}
		
	if (shapetype == "Polygon") {
		if (m_shape_drawing == NULL)
			m_shape_drawing = new ShapePolygon();
	}
	
}
void ScrolledImageComponent::ToggleDelaunay() {
	if (T_Delaunay)
		T_Delaunay = false;
	else {
		
	/*	points.clear();
		triangles.clear();
		edges.clear();
		RefPoint.clear();
		for (int i = 0; i < RefLink.size(); ++i) {
			RefPoint.push_back(RefLink[i].first);
			points.push_back(Vector2<float>(RefPoint[i].x, RefPoint[i].y));
		}
		

		triangles = triangulation.triangulate(points);

		edges = triangulation.getEdges();*/
		Delaunay();
		T_Delaunay = true;
	}
		
}
void ScrolledImageComponent::Delaunay() {
	points.clear();
	triangles.clear();
	edges.clear();
	RefPoint.clear();
	for (int i = 0; i < RefLink.size(); ++i) {
		RefPoint.push_back(RefLink[i].first);
		points.push_back(Vector2<float>(RefPoint[i].x, RefPoint[i].y));
	}
	triangles = triangulation.triangulate(points);

	edges = triangulation.getEdges();
		
	

}
void ScrolledImageComponent::EndShape()
{
		m_shape_drawing = NULL;
		m_datas.clear();
}
///////////////////////////////////////////////////////////////////////////////

