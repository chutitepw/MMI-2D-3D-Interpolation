#include "Line.hpp"


ShapeLine::ShapeLine() {

}
ShapeLine::~ShapeLine() {

}
void ShapeLine::Draw(wxDC* dc, ScrolledImageComponent* scroll) {
	if (m_data.size() < 2)
		return;

	//convert 2 points (bottom left and up rigtht) image 2 panel coordinate
	std::vector<wxPoint> m_pos;
	m_pos.reserve(m_data.size());
	for (unsigned int i = 0; i < m_data.size(); i++) {
		m_pos.push_back(scroll->CoordinateImage2Panel(m_data[i]));
	}
	dc->DrawLines(m_pos.size(), &m_pos[0]);
}

void ShapeLine::WriteXML(wxXmlNode * node) {
	wxXmlNode * XmlPoint = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("line"));

	/*wxString size;
	size << m_origin.size();

	wxXmlNode * XmlSizeN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("size"));
	wxXmlNode * XmlSize = new wxXmlNode(wxXML_TEXT_NODE, wxT("size"), size);
	XmlSizeN->AddChild(XmlSize);
	XmlPoint->AddChild(XmlSizeN);*/

	for (unsigned int it = 0; it < m_data.size(); it++) {
	
		wxXmlNode * XmlPtN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt"));
		wxString centerx;
		centerx << m_data[it].x;

		wxXmlNode * XmlxOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("xorigin"));
		wxXmlNode * XmlCentreX = new wxXmlNode(wxXML_TEXT_NODE, wxT("xorigin"), centerx);
		XmlxOriginN->AddChild(XmlCentreX);

		wxString centery;
		centery << m_data[it].y;

		wxXmlNode * XmlyOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("yorigin"));
		wxXmlNode * XmlCentreY = new wxXmlNode(wxXML_TEXT_NODE, wxT("yorigin"), centery);
		XmlyOriginN->AddChild(XmlCentreY);


		XmlPtN->AddChild(XmlxOriginN);
		XmlPtN->AddChild(XmlyOriginN);
		XmlPoint->AddChild(XmlPtN);
	}
	node->AddChild(XmlPoint);
}

bool  ShapeLine::LoadXML(wxXmlNode * node) {
	wxXmlNode *child = node->GetChildren();
	bool pass_x, pass_y;
	pass_x = pass_y = false;
	bool point = false;
	double xorigin, yorigin;
	while (child) {
		
		if (child->GetName() == "pt") {
			wxXmlNode *child2 = child->GetChildren();
			while (child2) {
				if (child2->GetName() == "xorigin") {
					wxString s = child2->GetNodeContent();
					if (s.ToCDouble(&xorigin) == false)
						return false;
					pass_x = true;
				}
				if (child2->GetName() == "yorigin") {
					wxString s = child2->GetNodeContent();
					if (s.ToCDouble(&yorigin) == false)
						return false;
					pass_y = true;
					point = true;
				}
				child2 = child2->GetNext();
			}
		}
		
		if (point == true && pass_x == true && pass_y == true) {
			m_data.push_back(wxPoint(xorigin, yorigin));
			point = false;
		}
		
		child = child->GetNext();
	}
	
	return pass_x && pass_y;
}
double ShapeLine::Distance(const wxRealPoint& pt) {
	/*wxRealPoint diff = m_data[1]-m_data[0];
	return fabs((diff.y*pt.x) - (diff.x*pt.y) + (m_data[1].x *m_data[0].y) - (m_data[1].y *m_data[0].x)) / sqrt((diff.x*diff.x) + (diff.y*diff.y));*/
	std::vector<double> L_min;
	for (int i = 0; i < m_data.size() - 1; i++) {
		wxRealPoint diff = m_data[i + 1] - m_data[i];
		L_min.push_back(fabs((diff.y*pt.x) - (diff.x*pt.y) + (m_data[i + 1].x *m_data[i].y) - (m_data[i + 1].y *m_data[i].x)) / sqrt((diff.x*diff.x) + (diff.y*diff.y)));
	}
	
	return *std::min_element(L_min.begin(), L_min.end());
}


