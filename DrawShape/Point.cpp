#include "Point.hpp"


ShapePoint::ShapePoint() {

}
ShapePoint::~ShapePoint() {

}
void ShapePoint::Draw(wxDC* dc, ScrolledImageComponent* scroll) {
	dc->DrawPoint(scroll->CoordinateImage2Panel(m_origin));
}

void ShapePoint::WriteXML(wxXmlNode * node) {
	wxXmlNode * XmlPoint = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("point"));
	wxXmlNode * XmlPtN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt"));
	wxString centerx;
	centerx << m_origin.x;

	wxXmlNode * XmlxOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("xorigin"));
	wxXmlNode * XmlxOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("xorigin"), centerx);
	XmlxOriginN->AddChild(XmlxOrigin);

	wxString centery;
	centery << m_origin.y;

	wxXmlNode * XmlyOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("yorigin"));
	wxXmlNode * XmlyOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("yorigin"), centery);
	XmlyOriginN->AddChild(XmlyOrigin);

	XmlPtN->AddChild(XmlxOriginN);
	XmlPtN->AddChild(XmlyOriginN);
	XmlPoint->AddChild(XmlPtN);
	node->AddChild(XmlPoint);
}

bool  ShapePoint::LoadXML(wxXmlNode * node) {
	wxXmlNode *child = node->GetChildren();
	bool pass_x, pass_y;
	pass_x = pass_y = false;
	while (child) {
		if (child->GetName() == "pt") {
			wxXmlNode *child2 = child->GetChildren();
			while (child2) {
				if (child2->GetName() == "xorigin") {
					wxString s = child2->GetNodeContent();
					if (s.ToCDouble(&m_origin.x) == false)
						return false;
					pass_x = true;
				}
				if (child2->GetName() == "yorigin") {
					wxString s = child2->GetNodeContent();
					if (s.ToCDouble(&m_origin.y) == false)
						return false;
					pass_y = true;
				}
				child2 = child2->GetNext();
			}
		}
		
		child = child->GetNext();
	}
	return pass_x && pass_y;
}
double ShapePoint::Distance(const wxRealPoint& pt) { 
	wxRealPoint diff = pt-m_origin;
	return sqrt(diff.x*diff.x+ diff.y*diff.y );
}
