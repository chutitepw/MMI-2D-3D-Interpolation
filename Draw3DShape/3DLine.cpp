#include "3DLine.h"


DShapeLine::DShapeLine() {

}
DShapeLine::~DShapeLine() {

}
void DShapeLine::Draw(OSGCanvas* osg) {

	if (sp.size() > 1) {
		osg::Geometry* linesGeom = new osg::Geometry();

		osg::Vec3Array* vertices = new osg::Vec3Array;
		osg::Vec3Array::iterator vitr = vertices->begin();
		for (int i = 0; i < sp.size(); i++)
			vertices->push_back(sp[i]);

		osg::LineWidth* linewidth = new osg::LineWidth();
		linewidth->setWidth(10);

		osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
		color->push_back(osg::Vec4(1.0, 1.0, 0.0, 1.0));

		linesGeom->setVertexArray(vertices);
		linesGeom->setColorArray(color.get());
		linesGeom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

		osg::Vec3Array* normals = new osg::Vec3Array;
		normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
		linesGeom->setNormalArray(normals);
		linesGeom->setNormalBinding(osg::Geometry::BIND_OVERALL);

		linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, sp.size()));

		unitSphereGeode->getOrCreateStateSet()->setAttributeAndModes(linewidth, osg::StateAttribute::OVERRIDE);


		// add the points geomtry to the geode.
		unitSphereGeode->addDrawable(linesGeom);

		//redraw and replace old node
		if (sp.size() > 2) {
			if(osg->m_pGroupRefPts->getNumChildren() >= 1){
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
		

		sphereXForm->addChild(unitSphereGeode);
	}
}

void DShapeLine::WriteXML(wxXmlNode * node) {
	wxXmlNode * XmlLine = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dline"));
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

bool  DShapeLine::LoadXML(wxXmlNode * node) {
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
double DShapeLine::Distance(const double sx, const double sy, const double sz) {
	double diffx = sx - sp[0].x();
	double diffy = sy - sp[0].y();
	double diffz = sz - sp[0].z();
	return sqrt(diffx*diffx + diffy * diffy + diffz * diffz);
}
