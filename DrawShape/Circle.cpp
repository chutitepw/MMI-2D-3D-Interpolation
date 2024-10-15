#include "Circle.hpp"


ShapeCircle::ShapeCircle() {

}
ShapeCircle::~ShapeCircle() {

}
void ShapeCircle::Draw(wxDC* dc, ScrolledImageComponent* scroll) {
	wxPoint pt=scroll->CoordinateImage2Panel(m_origin);
	dc->DrawCircle(pt, m_radius);
}

void ShapeCircle::WriteXML(wxXmlNode * node) {
	wxXmlNode * XmlCircle= new wxXmlNode(wxXML_ELEMENT_NODE, wxT("circle"));

	wxString radius;
	radius << m_radius;
	
	wxXmlNode * XmlRadiusN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("radius"));
	wxXmlNode * XmlRadius = new wxXmlNode(wxXML_TEXT_NODE, wxT("radius"), radius);
	XmlRadiusN->AddChild(XmlRadius);
	XmlCircle->AddChild(XmlRadiusN);

	wxXmlNode * XmlPtN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt"));
	wxString centerx;
	centerx << m_origin.x;

	
	wxXmlNode * XmlCentreXN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("centerX"));
	wxXmlNode * XmlCentreX = new wxXmlNode(wxXML_TEXT_NODE, wxT("center"), centerx);
	XmlCentreXN->AddChild(XmlCentreX);

	wxString centery;
	centery << m_origin.y;
	
	wxXmlNode * XmlCentreYN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("centerY"));
	wxXmlNode * XmlCentreY = new wxXmlNode(wxXML_TEXT_NODE, wxT("center"), centery);
	XmlCentreYN->AddChild(XmlCentreY);

	XmlPtN->AddChild(XmlCentreXN);
	XmlPtN->AddChild(XmlCentreYN);
	XmlCircle->AddChild(XmlPtN);

	node->AddChild(XmlCircle);
}

bool  ShapeCircle::LoadXML(wxXmlNode * node) {
	wxXmlNode *child = node->GetChildren();
	bool pass_x, pass_y, pass_r;
	pass_x= pass_y = pass_r = false;
	while (child) {
		if (child->GetName() == "radius") {
			wxString s = child->GetNodeContent();
			if (s.ToCDouble(&m_radius) == false)
				return false;
			pass_r = true;
		}
		if (child->GetName() == "pt") {
			wxXmlNode *child2 = child->GetChildren();
			while (child2) {
				if (child2->GetName() == "centerX") {
					wxString s = child2->GetNodeContent();
					if (s.ToCDouble(&m_origin.x) == false)
						return false;
					pass_x = true;
				}
				if (child2->GetName() == "centerY") {
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
	return pass_r && pass_x && pass_y;
}
double ShapeCircle::Distance(const wxRealPoint& pt) {
	wxRealPoint diff = pt - m_origin;
	return fabs(sqrt(diff.x*diff.x + diff.y*diff.y) - m_radius);
}