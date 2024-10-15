#ifndef _OSGFILEOBJ_H_
#define _OSGFILEOBJ_H_

#include "../IO/BBCFileOBJ.hpp"
#include <osg/Node>
#include <osg/Material>

class OSGFileOBJ{
public :
	OSGFileOBJ(const std::string& file, const double&r, const double&g, const double&b, const double&a);
	~OSGFileOBJ();

	bool Init();

	osg::ref_ptr<osg::Node> CreateNode();

	osg::ref_ptr<osg::Node> createNodeFileObj(const std::string& file_obj, const double& offset_x, const double& offset_y, const double& offset_z, const double& r, const  double& g, const double& b, const  double& a);
	osg::ref_ptr<osg::Node> createNodeFileObjBarycenter(const std::string& file_obj, const double& r, const  double& g, const double& b, const  double& a);
private:
	osg::ref_ptr<osg::Node> createNodeFileObj(const BBCFileOBJ& t_, const double& offset_x, const double& offset_y, const double& offset_z, const double& r, const  double& g, const double& b, const  double& a);
	BBCFileOBJ m_file;
	std::string m_filename;

	osg::ref_ptr<osg::Material> m_pMaterial;
	osg::ref_ptr<osg::Vec3Array> m_pvertex;
	osg::ref_ptr<osg::Vec3Array> m_pnormals;
};
#endif