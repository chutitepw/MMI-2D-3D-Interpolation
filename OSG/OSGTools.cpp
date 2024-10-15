#include "OSGTools.h"
#include "../IO/BBCFileOBJ.hpp"

#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osgUtil/Optimizer>

///////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> OSGTools::createNodeFileObj(const std::string& file_obj, const double& offset_x, const double& offset_y, const double& offset_z, const double& r, const  double& g, const double& b, const  double& a){
	BBCFileOBJ t_;
	if (!t_.Read(file_obj))
		return NULL;
	osg::ref_ptr<osg::Node> node = createNodeFileObj(t_, offset_x, offset_y, offset_z, r, g, b, a);
	node->setName(file_obj);
	return node;
}
///////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> OSGTools::createNodeFileObj(const BBCFileOBJ& t_, const double& offset_x, const double& offset_y, const double& offset_z, const double& r, const  double& g, const double& b, const  double& a){
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
		vertex->push_back(osg::Vec3(t_.GetPtAt(i).m_X - offset_x, t_.GetPtAt(i).m_Y - offset_y, t_.GetPtAt(i).m_Z - -offset_z));
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
	//geometry->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);
	/*
	osg::Vec4Array* colorsB = new osg::Vec4Array;
	colorsB->push_back(osg::Vec4(r, g, b,  a));
	geometry->setColorArray(colorsB);
	geometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
	*/
	geode->addDrawable(geometry);
	/*
	osg::Geode* geodeL = new osg::Geode;
	geometryL->setVertexArray(vertex);
	geometryL->addPrimitiveSet(lines);
	geodeL->addDrawable(geometryL);
	pGroup->addChild(geodeL);
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.0f, 0.0f, 0.0f, 0.1f)); 
	geometryL->setColorArray(colors);
	geometryL->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
	*/
	osg::ref_ptr<osg::Material> pMaterial = new osg::Material;
	pMaterial->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
	pMaterial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.3, 0, 0.3, 1.0));
	pMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(r, g, b, a));
	//pMaterial->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(r, g, b, a));

	pMaterial->setShininess(osg::Material::FRONT_AND_BACK, 100);
	pMaterial->setAlpha(osg::Material::FRONT_AND_BACK, a);
	geode->getOrCreateStateSet()->setAttribute(pMaterial, osg::StateAttribute::ON);
	geode->getStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	geode->getStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	
	osgUtil::Optimizer optimizer;
	optimizer.optimize(pGroup);

	return pGroup;
}
