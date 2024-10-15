#ifndef DSHAPEPOLYGON
#define DSHAPEPOLYGON

#include "3DShape.h"
#include <wx/gdicmn.h>


class DShapePolygon : public DShape
{
public:
	DShapePolygon();
	~DShapePolygon();
	void Draw(OSGCanvas*);
	void SetDatas(const double& X, const double& Y, const double& Z) {

		osg::Vec3 pt(X, Y, Z);
		sp.push_back(pt);
	};
	virtual void WriteXML(wxXmlNode * node);
	virtual bool LoadXML(wxXmlNode * node);

	virtual double Distance(const double x, const double y, const double z);
	virtual std::vector <osg::Vec3> GetMultiDatas() { return sp; }

private:
	double x, y, z;
	osg::PositionAttitudeTransform* sphereXForm = new osg::PositionAttitudeTransform();
	osg::Vec3 pt;
	std::vector <osg::Vec3>sp;

	osg::Geode* unitSphereGeode = new osg::Geode();
};
#endif