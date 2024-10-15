#include "Reference.hpp"


ShapeReference::ShapeReference() {

}
ShapeReference::~ShapeReference() {

}
void ShapeReference::Draw(wxDC* dc, ScrolledImageComponent* scroll) {
	dc->DrawPoint(scroll->CoordinateImage2Panel(m_origin));
	wxRealPoint m_origin2 = scroll->CoordinateImage2Panel(m_origin);
	scroll->StoreRef(m_origin2);
	std::stringstream x;
	x << std::fixed << std::setprecision(0) << m_origin2.x;
	std::stringstream y;
	y << std::fixed << std::setprecision(0) << m_origin2.y;
	std::string sx = x.str();
	std::string sy = y.str();
	std::string rc = std::to_string(Refcount);
	dc->DrawText("RP" + rc + ": " + sx +" "+ sy , m_origin2.x+5, m_origin2.y+2);
	
}

void ShapeReference::WriteXML(wxXmlNode * node) {
	wxXmlNode * XmlPoint = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("reference"));
	std::string rc = std::to_string(Refcount);
	wxXmlNode * XmlPtN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt"));

	wxString ref;
	ref << Refcount;
	wxXmlNode * XmlRefNumN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Ref"));
	wxXmlNode * XmlRefNum = new wxXmlNode(wxXML_TEXT_NODE, wxT("Ref"), ref);
	XmlRefNumN->AddChild(XmlRefNum);

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

	wxXmlNode * XmlLink = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("link"));
	wxXmlNode * XmlLpt = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt"));
	wxXmlNode * XmlLdpt = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dpt"));

	wxString xlink;
	xlink << RefLink.first.x;

	wxXmlNode * XmlxLinkN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("xlink"));
	wxXmlNode * XmlxLink = new wxXmlNode(wxXML_TEXT_NODE, wxT("xlink"), xlink);
	XmlxLinkN->AddChild(XmlxLink);

	wxString ylink;
	ylink << RefLink.first.y;

	wxXmlNode * XmlyLinkN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("ylink"));
	wxXmlNode * XmlyLink = new wxXmlNode(wxXML_TEXT_NODE, wxT("ylink"), ylink);
	XmlyLinkN->AddChild(XmlyLink);

	wxString dxlink;
	dxlink << RefLink.second.x;

	wxXmlNode * XmldxLinkN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dxlink"));
	wxXmlNode * XmldxLink = new wxXmlNode(wxXML_TEXT_NODE, wxT("dxlink"), dxlink);
	XmldxLinkN->AddChild(XmldxLink);

	wxString dylink;
	dylink << RefLink.second.y;

	wxXmlNode * XmldyLinkN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dylink"));
	wxXmlNode * XmldyLink = new wxXmlNode(wxXML_TEXT_NODE, wxT("dylink"), dylink);
	XmldyLinkN->AddChild(XmldyLink);

	wxString dzlink;
	dzlink << RefLink.second.z;

	wxXmlNode * XmldzLinkN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dzlink"));
	wxXmlNode * XmldzLink = new wxXmlNode(wxXML_TEXT_NODE, wxT("dzlink"), dzlink);
	XmldzLinkN->AddChild(XmldzLink);

	XmlPtN->AddChild(XmlRefNumN);
	XmlPtN->AddChild(XmlxOriginN);
	XmlPtN->AddChild(XmlyOriginN); 

	XmlLpt->AddChild(XmlxLinkN);
	XmlLpt->AddChild(XmlyLinkN);
	XmlLdpt->AddChild(XmldxLinkN);
	XmlLdpt->AddChild(XmldyLinkN);
	XmlLdpt->AddChild(XmldzLinkN);
	XmlLink->AddChild(XmlLpt);
	XmlLink->AddChild(XmlLdpt);

	XmlPoint->AddChild(XmlPtN);
	XmlPoint->AddChild(XmlLink);

	node->AddChild(XmlPoint);
}

bool  ShapeReference::LoadXML(wxXmlNode * node) {
	wxXmlNode *child = node->GetChildren();
	bool pass_x, pass_y;
	pass_x = pass_y = false;
	while (child) {
		if (child->GetName() == "pt") {
			wxXmlNode *child2 = child->GetChildren();
			while (child2) {
				if (child2->GetName() == "Ref") {
					wxString s = child2->GetNodeContent();
					if (s.ToLong(&Refcount) == false)
						return false;
				}
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
double ShapeReference::Distance(const wxRealPoint& pt) {
	wxRealPoint diff = pt - m_origin;
	return sqrt(diff.x*diff.x + diff.y*diff.y);
}
void ShapeReference::SetRefT(wxRealPoint pt, DPoint dpt) {
	RefLink = std::make_pair(pt, dpt);
}