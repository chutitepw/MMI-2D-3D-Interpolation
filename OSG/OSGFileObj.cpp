#include "OSGFileObj.h"

#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osgUtil/Optimizer>



///////////////////////////////////////////////////////////////////////////////
OSGFileOBJ::OSGFileOBJ(const std::string& file, const double&r, const double&g, const double&b, const double&a) :
	m_filename(file)
{
	m_pMaterial = new osg::Material;
	m_pMaterial->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
	m_pMaterial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.3, 0, 0.3, 1.0));
	m_pMaterial->setShininess(osg::Material::FRONT_AND_BACK, 100);
	m_pMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(r, g, b, a));
	m_pMaterial->setAlpha(osg::Material::FRONT_AND_BACK, a);
}
///////////////////////////////////////////////////////////////////////////////
OSGFileOBJ::~OSGFileOBJ(){

}
///////////////////////////////////////////////////////////////////////////////
bool OSGFileOBJ::Init(){
	if (!m_file.Read(m_filename))
		return false;
	unsigned int nbpts = m_file.NbPts();
	m_pvertex = new osg::Vec3Array;
	m_pnormals = new osg::Vec3Array;

	m_pvertex->reserve(nbpts);
	m_pnormals->reserve(nbpts);
	for (unsigned int i = 0; i < nbpts; ++i){
		m_pvertex->push_back(osg::Vec3(m_file.GetPtAt(i).m_X, m_file.GetPtAt(i).m_Y, m_file.GetPtAt(i).m_Z));
		m_pnormals->push_back(osg::Vec3(0, 0, 0));
	}

	osg::ref_ptr<osg::DrawElementsUInt> m_ptriangle = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	osg::ref_ptr<osg::DrawElementsUInt> m_plines = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);

	for (unsigned int i = 0; i < m_file.NbFaces(); ++i){
		ObjFace face = m_file.GetFaceAt(i);
		m_ptriangle->push_back(face.m_a - 1);
		m_ptriangle->push_back(face.m_b - 1);
		m_ptriangle->push_back(face.m_c - 1);
		osg::Vec3 a(m_file.GetPtAt(face.m_a - 1).m_X, m_file.GetPtAt(face.m_a - 1).m_Y, m_file.GetPtAt(face.m_a - 1).m_Z);
		osg::Vec3 b(m_file.GetPtAt(face.m_b - 1).m_X, m_file.GetPtAt(face.m_b - 1).m_Y, m_file.GetPtAt(face.m_b - 1).m_Z);
		osg::Vec3 c(m_file.GetPtAt(face.m_c - 1).m_X, m_file.GetPtAt(face.m_c - 1).m_Y, m_file.GetPtAt(face.m_c - 1).m_Z);
		osg::Vec3 n = (b - a) ^ (c - a);
		n.normalize();
		m_pnormals->at(face.m_a - 1) = m_pnormals->at(face.m_a - 1) + n;
		m_pnormals->at(face.m_b - 1) = m_pnormals->at(face.m_b - 1) + n;
		m_pnormals->at(face.m_c - 1) = m_pnormals->at(face.m_c - 1) + n;

		m_plines->push_back(face.m_a - 1);
		m_plines->push_back(face.m_b - 1);
		m_plines->push_back(face.m_b - 1);
		m_plines->push_back(face.m_c - 1);
		m_plines->push_back(face.m_c - 1);
		m_plines->push_back(face.m_a - 1);
	}
	for (unsigned int i = 0; i < nbpts; ++i){
		m_pnormals->at(i).normalize();
	}


	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	geometry->setVertexArray(m_pvertex);
	geometry->addPrimitiveSet(m_ptriangle);
	geometry->setNormalArray(m_pnormals, osg::Array::BIND_PER_VERTEX);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode; 
	geode->getOrCreateStateSet()->setAttribute(m_pMaterial, osg::StateAttribute::ON);
	geode->getStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	geode->getStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	geode->addDrawable(geometry);


	osgUtil::Optimizer optimizer;
	optimizer.optimize(geode);


	return true;
}
osg::ref_ptr<osg::Node> OSGFileOBJ::CreateNode(){
	return osg::ref_ptr<osg::Node>();
}


///////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> OSGFileOBJ::createNodeFileObj(const std::string& file_obj, const double& offset_x, const double& offset_y, const double& offset_z, const double& r, const  double& g, const double& b, const  double& a){
	BBCFileOBJ t_;
	if (!t_.Read(file_obj))
		return NULL;

	/*get barzcenter*/
	ObjPt bary=t_.GetBarycenter();
	bary.m_X;

	osg::ref_ptr<osg::Node> node = createNodeFileObj(t_, offset_x, offset_y, offset_z,r, g, b, a);
	node->setName(file_obj);
	return node;
}
///////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> OSGFileOBJ::createNodeFileObj(const BBCFileOBJ& t_, const double& offset_x, const double& offset_y, const double& offset_z, const double& r, const  double& g, const double& b, const  double& a){
	osg::ref_ptr<osg::Group> pGroup = new osg::Group;

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	pGroup->addChild(geode);

	osg::ref_ptr<osg::StateSet> state = geode->getOrCreateStateSet();
	state->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	geode->setStateSet(state);

	osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	std::vector<unsigned int> nb_normal;
	for (unsigned int i = 0; i < t_.NbPts(); ++i){
		vertex->push_back(osg::Vec3(t_.GetPtAt(i).m_X - offset_x, t_.GetPtAt(i).m_Y - offset_y, t_.GetPtAt(i).m_Z- offset_z));
		normals->push_back(osg::Vec3(0,0,0));
		nb_normal.push_back(0);
	}

	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	osg::ref_ptr<osg::Geometry> geometryL = new osg::Geometry();
	 
	osg::ref_ptr<osg::DrawElementsUInt> triangle = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	osg::ref_ptr<osg::DrawElementsUInt> lines = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	for (unsigned int i = 0; i < t_.NbFaces(); ++i){
		ObjFace face = t_.GetFaceAt(i);
		triangle->push_back(face.m_a - 1);
		triangle->push_back(face.m_b - 1);
		triangle->push_back(face.m_c - 1);
		osg::Vec3 a(t_.GetPtAt(face.m_a - 1).m_X, t_.GetPtAt(face.m_a - 1).m_Y, t_.GetPtAt(face.m_a - 1).m_Z);
		osg::Vec3 b(t_.GetPtAt(face.m_b - 1).m_X, t_.GetPtAt(face.m_b - 1).m_Y, t_.GetPtAt(face.m_b - 1).m_Z);
		osg::Vec3 c(t_.GetPtAt(face.m_c - 1).m_X, t_.GetPtAt(face.m_c - 1).m_Y, t_.GetPtAt(face.m_c - 1).m_Z);
		osg::Vec3 n = (b - a) ^ (c - a);
		n.normalize();
		normals->at(face.m_a - 1) = normals->at(face.m_a - 1) + n;
		normals->at(face.m_b - 1) = normals->at(face.m_b - 1) + n;
		normals->at(face.m_c - 1) = normals->at(face.m_c - 1) + n;
		nb_normal[face.m_a - 1] = nb_normal[face.m_a - 1] + 1;
		nb_normal[face.m_b - 1] = nb_normal[face.m_b - 1] + 1;
		nb_normal[face.m_c - 1] = nb_normal[face.m_c - 1] + 1;

		lines->push_back(face.m_a - 1);
		lines->push_back(face.m_b - 1);
		lines->push_back(face.m_b - 1);
		lines->push_back(face.m_c - 1);
		lines->push_back(face.m_c - 1);
		lines->push_back(face.m_a - 1);
	}
	for (unsigned int i = 0; i < t_.NbPts(); ++i){
		normals->at(i).normalize();
	}
	geometry->setVertexArray(vertex);
	geometry->addPrimitiveSet(triangle);


	geometry->setNormalArray(normals, osg::Array::BIND_PER_VERTEX);
	geode->addDrawable(geometry);
	
	osg::ref_ptr<osg::Material> pMaterial = new osg::Material;
	pMaterial->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
	pMaterial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.3, 0, 0.3, 1.0));
	pMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(r, g, b, a));
	
	pMaterial->setShininess(osg::Material::FRONT_AND_BACK, 100);
	pMaterial->setAlpha(osg::Material::FRONT_AND_BACK, a);
	geode->getOrCreateStateSet()->setAttribute(pMaterial, osg::StateAttribute::ON);
	geode->getStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	geode->getStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	
	osgUtil::Optimizer optimizer;
	optimizer.optimize(pGroup);

	return pGroup;
}
