#include "3DPoint.h"


DShapePoint::DShapePoint() {

}
DShapePoint::~DShapePoint() {

}
void DShapePoint::Draw(OSGCanvas* osg) {

	//dc->DrawPoint(scroll->CoordinateImage2Panel(m_origin));

	osg::Sphere* unitSphere = new osg::Sphere(osg::Vec3(x, y, z), 0.2);
	osg::ShapeDrawable* unitSphereDrawable = new osg::ShapeDrawable(unitSphere);
	unitSphereDrawable->setColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));

	sphereXForm->setPosition(osg::Vec3(0, 0, 0));

	osg->m_pGroupRefPts->addChild(sphereXForm);

	sphereXForm->addChild(unitSphereGeode);
	unitSphereGeode->addDrawable(unitSphereDrawable);

}

void DShapePoint::WriteXML(wxXmlNode * node) {
	wxXmlNode * XmlPoint = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dpoint"));
	wxXmlNode * XmlPtN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt"));

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


	XmlPtN->AddChild(XmlxOriginN);
	XmlPtN->AddChild(XmlyOriginN);
	XmlPtN->AddChild(XmlzOriginN);

	XmlPoint->AddChild(XmlPtN);
	node->AddChild(XmlPoint);
}

bool  DShapePoint::LoadXML(wxXmlNode * node) {
	wxXmlNode *child = node->GetChildren();
	bool pass_x, pass_y, pass_z;
	pass_x = pass_y = pass_z = false;
	
	while (child) {
		if (child->GetName() == "pt") {
			wxXmlNode *child2 = child->GetChildren();
			while (child2) {
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
		child = child->GetNext();
	}
	return pass_x && pass_y && pass_z;
}
double DShapePoint::Distance(const double sx, const double sy, const double sz) {
	double diffx = sx - x;
	double diffy = sy - y;
	double diffz = sz - z;
	return sqrt(diffx*diffx + diffy * diffy + diffz * diffz);
}
