#include "Rectangle.hpp"


ShapeRectangle::ShapeRectangle() {

}
ShapeRectangle::~ShapeRectangle() {

}
void ShapeRectangle::Draw(wxDC* dc, ScrolledImageComponent* scroll) {
	wxPoint a = scroll->CoordinateImage2Panel(m_A);
	wxPoint b = scroll->CoordinateImage2Panel(m_B);
	dc->DrawRectangle(a, wxSize(b.x-a.x,b.y-a.y));
}

void ShapeRectangle::WriteXML(wxXmlNode * node) {
	wxXmlNode * XmlRectangle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("rectangle"));
	wxXmlNode * XmlPtN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt"));
	wxXmlNode * XmlPtN2 = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt2"));
	wxString xorigin, yorigin;
	xorigin << m_A.x;
	yorigin << m_A.y;

	wxXmlNode * XmlxOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Ax"));
	wxXmlNode * XmlxOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("Ax"), xorigin);
	XmlxOriginN->AddChild(XmlxOrigin);
	XmlPtN->AddChild(XmlxOriginN);

	wxXmlNode * XmlyOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Ay"));
	wxXmlNode * XmlyOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("Ay"), yorigin);
	XmlyOriginN->AddChild(XmlyOrigin);
	XmlPtN->AddChild(XmlyOriginN);

	XmlRectangle->AddChild(XmlPtN);


	wxString Width, Height;
	Width << m_B.x;
	Height << m_B.y;

	wxXmlNode * XmlWidthN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Bx"));
	wxXmlNode * XmlWidth = new wxXmlNode(wxXML_TEXT_NODE, wxT("Bx"), Width);
	XmlWidthN->AddChild(XmlWidth);

	wxXmlNode * XmlHeighN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("By"));
	wxXmlNode * XmlHeight = new wxXmlNode(wxXML_TEXT_NODE, wxT("By"), Height);
	XmlHeighN->AddChild(XmlHeight);

	XmlPtN2->AddChild(XmlWidthN);
	XmlPtN2->AddChild(XmlHeighN);

	XmlRectangle->AddChild(XmlPtN2);
	node->AddChild(XmlRectangle);
}

bool  ShapeRectangle::LoadXML(wxXmlNode * node) {
	wxXmlNode *child = node->GetChildren();
	bool pass_x, pass_y, pass_w, pass_h;
	pass_x = pass_y = pass_w = pass_h = false;
	while (child) {
		if (child->GetName() == "pt") {
			wxXmlNode *child2 = child->GetChildren();
			while(child2) {
				if (child2->GetName() == "Ax") {
					wxString s = child2->GetNodeContent();
					if (s.ToCDouble(&m_A.x) == false)
						return false;
					pass_x = true;
				}
				if (child2->GetName() == "Ay") {
					wxString s = child2->GetNodeContent();
					if (s.ToCDouble(&m_A.y) == false)
						return false;
					pass_y = true;
				}
				child2 = child2->GetNext();
			}
		}
		if (child->GetName() == "pt2") {
			wxXmlNode *child2 = child->GetChildren();
			while (child2) {
				if (child2->GetName() == "Bx") {
					wxString s = child2->GetNodeContent();
					if (s.ToCDouble(&m_B.x) == false)
						return false;
					pass_w = true;
				}
				if (child2->GetName() == "By") {
					wxString s = child2->GetNodeContent();
					if (s.ToCDouble(&m_B.y) == false)
						return false;
					pass_h = true;
				}
				child2 = child2->GetNext();
			}
		}
		
		
		child = child->GetNext();
	}
	return pass_w && pass_h && pass_x && pass_y;
}
double ShapeRectangle::Distance(const wxRealPoint& pt) {
	double Min_a = std::min(fabs((pt - m_A).x), fabs((pt - m_A).y));
	double Min_b = std::min(fabs((pt - m_B).x), fabs((pt - m_B).y));
	return std::min(Min_a,Min_b);
}