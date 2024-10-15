#include "3DReference.h"


DShapeReference::DShapeReference() {

}
DShapeReference::~DShapeReference() {

}
void DShapeReference::Draw(OSGCanvas* osg) {

	//dc->DrawPoint(scroll->CoordinateImage2Panel(m_origin));

	osg::Sphere* unitSphere = new osg::Sphere(osg::Vec3(x, y, z), 0.1);
	osg::ShapeDrawable* unitSphereDrawable = new osg::ShapeDrawable(unitSphere);
	unitSphereDrawable->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));

	sphereXForm->setPosition(osg::Vec3(0, 0, 0));

	osg->m_pGroupRefPts->addChild(sphereXForm);

	sphereXForm->addChild(unitSphereGeode);
	unitSphereGeode->addDrawable(unitSphereDrawable);

	if (DRefLink.first.x != NULL && DRefLink.first.y != NULL) {
		osgText::Text* textOne = new osgText::Text();
		textOne->setCharacterSize(0.5);
		textOne->setFont("C:/WINDOWS/Fonts/impact.ttf");

		std::stringstream ss;
		ss << DRefLink.first.x << "," << DRefLink.first.y;
		std::string var = ss.str();
		textOne->setText(var);
		textOne->setAxisAlignment(osgText::Text::SCREEN);
		textOne->setPosition(osg::Vec3(DRefLink.second.x + 1, DRefLink.second.y, DRefLink.second.z));
		textOne->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));

		unitSphereGeode->addDrawable(textOne);
	}
	/*Reference Text*/
	/*osgText::Text* textOne = new osgText::Text();
	textOne->setCharacterSize(0.5);
	textOne->setFont("C:/WINDOWS/Fonts/impact.ttf");
	
	std::stringstream ss;
	ss << "x : " << x << " y : " << y << " z : " << z;
	std::string var = ss.str();
	textOne->setText(var);
	textOne->setAxisAlignment(osgText::Text::SCREEN);
	textOne->setPosition(osg::Vec3(x, y, z));
	textOne->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));

	unitSphereGeode->addDrawable(textOne);*/

}

void DShapeReference::WriteXML(wxXmlNode * node) {
	wxXmlNode * XmlPoint = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dreference"));
	std::string rc = std::to_string(Refcount);
	wxXmlNode * XmlPtN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt"));

	wxString ref;
	ref << Refcount;
	wxXmlNode * XmlRefNumN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Ref"));
	wxXmlNode * XmlRefNum = new wxXmlNode(wxXML_TEXT_NODE, wxT("Ref"), ref);
	XmlRefNumN->AddChild(XmlRefNum);

	wxString centerx;
	centerx << x;
	wxXmlNode * XmlxOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("xorigin"));
	wxXmlNode * XmlxOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("xorigin"), centerx);
	XmlxOriginN->AddChild(XmlxOrigin);

	wxString centery;
	centery << y;

	wxXmlNode * XmlyOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("yorigin"));
	wxXmlNode * XmlyOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("yorigin"), centery);
	XmlyOriginN->AddChild(XmlyOrigin);

	wxString centerz;
	centerz << z;

	wxXmlNode * XmlzOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("zorigin"));
	wxXmlNode * XmlzOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("zorigin"), centerz);
	XmlzOriginN->AddChild(XmlzOrigin);

	wxXmlNode * XmlLink = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("link"));
	wxXmlNode * XmlLpt = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt"));
	wxXmlNode * XmlLdpt = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dpt"));

	wxString xlink;
	xlink << DRefLink.first.x;

	wxXmlNode * XmlxLinkN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("xlink"));
	wxXmlNode * XmlxLink = new wxXmlNode(wxXML_TEXT_NODE, wxT("xlink"), xlink);
	XmlxLinkN->AddChild(XmlxLink);

	wxString ylink;
	ylink << DRefLink.first.y;

	wxXmlNode * XmlyLinkN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("ylink"));
	wxXmlNode * XmlyLink = new wxXmlNode(wxXML_TEXT_NODE, wxT("ylink"), ylink);
	XmlyLinkN->AddChild(XmlyLink);

	wxString dxlink;
	dxlink << DRefLink.second.x;

	wxXmlNode * XmldxLinkN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dxlink"));
	wxXmlNode * XmldxLink = new wxXmlNode(wxXML_TEXT_NODE, wxT("dxlink"), dxlink);
	XmldxLinkN->AddChild(XmldxLink);

	wxString dylink;
	dylink << DRefLink.second.y;

	wxXmlNode * XmldyLinkN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dylink"));
	wxXmlNode * XmldyLink = new wxXmlNode(wxXML_TEXT_NODE, wxT("dylink"), dylink);
	XmldyLinkN->AddChild(XmldyLink);

	wxString dzlink;
	dzlink << DRefLink.second.z;

	wxXmlNode * XmldzLinkN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dzlink"));
	wxXmlNode * XmldzLink = new wxXmlNode(wxXML_TEXT_NODE, wxT("dzlink"), dzlink);
	XmldzLinkN->AddChild(XmldzLink);


	XmlPtN->AddChild(XmlRefNumN);
	XmlPtN->AddChild(XmlxOriginN);
	XmlPtN->AddChild(XmlyOriginN);
	XmlPtN->AddChild(XmlzOriginN);

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

bool  DShapeReference::LoadXML(wxXmlNode * node) {
	wxXmlNode *child = node->GetChildren();
	bool pass_x, pass_y, pass_z, pass_lx, pass_ly, pass_ldx, pass_ldy, pass_ldz;
	pass_x = pass_y = pass_z = pass_lx = pass_ly = pass_ldx = pass_ldy = pass_ldz = false;
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
					if (s.ToCDouble(&x) == false)
						return false;
					pass_x = true;
				}
				if (child2->GetName() == "yorigin") {
					wxString s = child2->GetNodeContent();
					if (s.ToCDouble(&y) == false)
						return false;
					pass_y = true;
				}
				if (child2->GetName() == "zorigin") {
					wxString s = child2->GetNodeContent();
					if (s.ToCDouble(&z) == false)
						return false;
					pass_z = true;
				}
				child2 = child2->GetNext();
			}
		}
		if (child->GetName() == "link") {
			wxXmlNode *child2 = child->GetChildren();
			while (child2) {
				if (child2->GetName() == "pt") {
					wxXmlNode *child3 = child2->GetChildren();
					while (child3) {
						if (child3->GetName() == "xlink") {
							wxString s = child3->GetNodeContent();
							if (s.ToCDouble(&DRefLink.first.x) == false)
								return false;
							pass_lx = true;
						}
						if (child3->GetName() == "ylink") {
							wxString s = child3->GetNodeContent();
							if (s.ToCDouble(&DRefLink.first.y) == false)
								return false;
							pass_ly = true;
						}
						child3 = child3->GetNext();
					}
				}
				if (child2->GetName() == "dpt") {
					wxXmlNode *child3 = child2->GetChildren();
					while (child3) {
						if (child3->GetName() == "dxlink") {
							wxString s = child3->GetNodeContent();
							if (s.ToCDouble(&DRefLink.second.x) == false)
								return false;
							pass_ldx = true;
						}
						if (child3->GetName() == "dylink") {
							wxString s = child3->GetNodeContent();
							if (s.ToCDouble(&DRefLink.second.y) == false)
								return false;
							pass_ldy = true;
						}
						if (child3->GetName() == "dzlink") {
							wxString s = child3->GetNodeContent();
							if (s.ToCDouble(&DRefLink.second.z) == false)
								return false;
							pass_ldz = true;
						}
						child3 = child3->GetNext();
					}
				}
				child2 = child2->GetNext();
			}
		}
		child = child->GetNext();
	}
	return pass_x && pass_y && pass_z && pass_lx && pass_ly && pass_ldx && pass_ldy && pass_ldz;
}
double DShapeReference::Distance(const double sx, const double sy, const double sz) {
	double diffx = sx - x;
	double diffy = sy - y;
	double diffz = sz - z;
	return sqrt(diffx*diffx + diffy*diffy + diffz * diffz);
}
DPoint DShapeReference::RSelectShape() {
	DPoint xyz;
	xyz.x = x;
	xyz.y = y;
	xyz.z = z;
	return xyz;
}
void DShapeReference::SetRefT(wxRealPoint pt, DPoint dpt) {
	/*Reference Text*/
	osgText::Text* textOne = new osgText::Text();
	textOne->setCharacterSize(0.5);
	textOne->setFont("C:/WINDOWS/Fonts/impact.ttf");

	std::stringstream ss;
	ss << pt.x << "," << pt.y;
	std::string var = ss.str();
	textOne->setText(var);
	textOne->setAxisAlignment(osgText::Text::SCREEN);
	textOne->setPosition(osg::Vec3(dpt.x + 1, dpt.y, dpt.z));
	textOne->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	
	unitSphereGeode->addDrawable(textOne);

	DRefLink = std::make_pair(pt, dpt);
}