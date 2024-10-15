#include "Sphere.h"


DShapeSphere::DShapeSphere() {

}
DShapeSphere::~DShapeSphere() {

}
void DShapeSphere::Draw(OSGCanvas* osg) {

	


	osg::Sphere* unitSphere = new osg::Sphere(osg::Vec3(sp[0].x(), sp[0].y(), sp[0].z()), m_radius);
	osg::ShapeDrawable* unitSphereDrawable = new osg::ShapeDrawable(unitSphere);
	unitSphereDrawable->setColor(osg::Vec4(1.0, 1.0, 1.0, 0.5));

	sphereXForm->setPosition(osg::Vec3(0, 0, 0));

	osg->m_pGroupRefPts->addChild(sphereXForm);

	sphereXForm->addChild(unitSphereGeode);
	unitSphereGeode->addDrawable(unitSphereDrawable);

}

void DShapeSphere::WriteXML(wxXmlNode * node) {
	wxXmlNode * XmlCircle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dsphere"));
	

	wxString radius;
	radius << m_radius;

	wxXmlNode * XmlRadiusN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("radius"));
	wxXmlNode * XmlRadius = new wxXmlNode(wxXML_TEXT_NODE, wxT("radius"), radius);

	XmlRadiusN->AddChild(XmlRadius);
	XmlCircle->AddChild(XmlRadiusN);

	wxXmlNode * XmlPtN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt"));

	wxString centerx;
	centerx << sp[0].x();
	wxXmlNode * XmlxOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("xorigin"));
	wxXmlNode * XmlxOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("xorigin"), centerx);
	XmlxOriginN->AddChild(XmlxOrigin);

	wxString centery;
	centery << sp[0].y();

	wxXmlNode * XmlyOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("yorigin"));
	wxXmlNode * XmlyOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("yorigin"), centery);
	XmlyOriginN->AddChild(XmlyOrigin);

	wxString centerz;
	centerz << sp[0].z();

	wxXmlNode * XmlzOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("zorigin"));
	wxXmlNode * XmlzOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("zorigin"), centerz);
	XmlzOriginN->AddChild(XmlzOrigin);


	XmlPtN->AddChild(XmlxOriginN);
	XmlPtN->AddChild(XmlyOriginN);
	XmlPtN->AddChild(XmlzOriginN);

	XmlCircle->AddChild(XmlPtN);
	node->AddChild(XmlCircle);
}

bool  DShapeSphere::LoadXML(wxXmlNode * node) {
	wxXmlNode *child = node->GetChildren();
	bool pass_x, pass_y, pass_z, pass_r;
	pass_x = pass_y = pass_z = pass_r = false;

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
		if (child->GetName() == "radius") {
			wxString s = child->GetNodeContent();
			if (s.ToCDouble(&m_radius) == false)
				return false;
			pass_r = true;
		}
		if (pass_x && pass_y && pass_z && pass_r) {
			osg::Vec3 pt(x, y, z);
			sp.push_back(pt);
		}
		child = child->GetNext();
	}
	return pass_x && pass_y && pass_z && pass_r;
}
double DShapeSphere::Distance(const double sx, const double sy, const double sz) {
	double diffx = sx - sp[0].x();
	double diffy = sy - sp[0].y();
	double diffz = sz - sp[0].z();
	return fabs(sqrt(diffx*diffx + diffy * diffy + diffz * diffz) - m_radius);
}
