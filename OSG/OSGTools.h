#ifndef _OSGTOOLS_H_
#define _OSGTOOLS_H_

#include <string>
#include <osg/Node>

class BBCFileOBJ;

class OSGTools{
public :
	static osg::ref_ptr<osg::Node> createNodeFileObj(const std::string& file_obj, const double& offset_x, const double& offset_y, const double& offset_z, const double& r = 1.0, const  double& g = 1.0, const double& b = 1.0, const  double& a = 1.0);
	static osg::ref_ptr<osg::Node> createNodeFileObjBin(const std::string& file_obj, const double& offset_x, const double& offset_y, const double& offset_z, const double& r = 1.0, const  double& g = 1.0, const double& b = 1.0, const  double& a = 1.0);
private:
	static osg::ref_ptr<osg::Node> createNodeFileObj(const BBCFileOBJ& file_obj, const double& offset_x, const double& offset_y, const double& offset_z, const double& r, const  double& g, const double& b, const  double& a);
};
#endif