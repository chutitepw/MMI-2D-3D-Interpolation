#include "3DPolygon.h"


DShapePolygon::DShapePolygon() {

}
DShapePolygon::~DShapePolygon() {

}
void DShapePolygon::Draw(OSGCanvas* osg) {

	if (sp.size() > 1) {
		osg::Geometry* PolyGeo = new osg::Geometry();

		// this time we'll prealloacte the vertex array to the size 
		// and then use an iterator to fill in the values, a bit perverse
		// but does demonstrate that we have just a standard std::vector underneath.
		
		osg::Vec3Array* vertices = new osg::Vec3Array;
		for (int i = 0; i < sp.size(); i++)
			vertices->push_back(sp[i]);
		// pass the created vertex array to the points geometry object.
		PolyGeo->setVertexArray(vertices);

		// set the colors as before, plus using the aobve
		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		PolyGeo->setColorArray(colors);
		PolyGeo->setColorBinding(osg::Geometry::BIND_OVERALL);


		// set the normal in the same way color.
		osg::Vec3Array* normals = new osg::Vec3Array;
		normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
		PolyGeo->setNormalArray(normals);
		PolyGeo->setNormalBinding(osg::Geometry::BIND_OVERALL);


		// This time we simply use primitive, and hardwire the number of coords to use 
		// since we know up front,
		PolyGeo->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, sp.size()));


		// add the points geomtry to the geode.
		unitSphereGeode->addDrawable(PolyGeo);
		sphereXForm->addChild(unitSphereGeode);
		if (sp.size() > 2) {
			if (osg->m_pGroupRefPts->getNumChildren() >= 1) {
				osg::Node* node = osg->m_pGroupRefPts->getChild(osg->m_pGroupRefPts->getNumChildren() - 1);
				osg->m_pGroupRefPts->replaceChild(node, sphereXForm);
			}
			else {
				osg->m_pGroupRefPts->addChild(sphereXForm);
			}

		}
		else {
			osg->m_pGroupRefPts->addChild(sphereXForm);
		}
		

	}
}

void DShapePolygon::WriteXML(wxXmlNode * node) {
	wxXmlNode * XmlLine = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dpolygon"));
	for (unsigned int it = 0; it < sp.size(); it++) {
		wxXmlNode * XmlPtN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt"));

		wxString centerx;
		centerx << sp[it].x();
		wxXmlNode * XmlxOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("xorigin"));
		wxXmlNode * XmlxOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("xorigin"), centerx);
		XmlxOriginN->AddChild(XmlxOrigin);

		wxString centery;
		centery << sp[it].y();

		wxXmlNode * XmlyOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("yorigin"));
		wxXmlNode * XmlyOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("yorigin"), centery);
		XmlyOriginN->AddChild(XmlyOrigin);

		wxString centerz;
		centerz << sp[it].z();

		wxXmlNode * XmlzOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("zorigin"));
		wxXmlNode * XmlzOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("zorigin"), centerz);
		XmlzOriginN->AddChild(XmlzOrigin);


		XmlPtN->AddChild(XmlxOriginN);
		XmlPtN->AddChild(XmlyOriginN);
		XmlPtN->AddChild(XmlzOriginN);

		XmlLine->AddChild(XmlPtN);
	}
	node->AddChild(XmlLine);
}

bool  DShapePolygon::LoadXML(wxXmlNode * node) {
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
		if (pass_x && pass_y && pass_z) {
			osg::Vec3 pt(x, y, z);
			sp.push_back(pt);
		}
		child = child->GetNext();
	}
	return pass_x && pass_y && pass_z;
}
double DShapePolygon::Distance(const double sx, const double sy, const double sz) {
	double diffx = sx - sp[0].x();
	double diffy = sy - sp[0].y();
	double diffz = sz - sp[0].z();
	return sqrt(diffx*diffx + diffy * diffy + diffz * diffz);
}
