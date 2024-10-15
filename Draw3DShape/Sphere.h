#ifndef DSHAPESPHERE
#define DSHAPESPHERE

#include "3DShape.h"
#include <wx/gdicmn.h>


class DShapeSphere : public DShape
{
public:
	DShapeSphere();
	~DShapeSphere();
	void Draw(OSGCanvas*);
	void SetDatas(const double& X, const double& Y, const double& Z) {

		osg::Vec3 pt(X, Y, Z);
		sp.push_back(pt);
		if (sp.size() > 1) {
			/*Cal radius*/
			double diffx = sp[1].x() - sp[0].x();
			double diffy = sp[1].y() - sp[0].y();
			double diffz = sp[1].z() - sp[0].z();
			m_radius = sqrt(diffx*diffx + diffy * diffy + diffz * diffz);
		}
	};
	virtual void WriteXML(wxXmlNode * node);
	virtual bool LoadXML(wxXmlNode * node);

	virtual double Distance(const double x, const double y, const double z);
	virtual std::vector <osg::Vec3> GetMultiDatas() { return sp; }

private:
	double x, y, z;
	double m_radius;
	osg::PositionAttitudeTransform* sphereXForm = new osg::PositionAttitudeTransform();
	osg::Vec3 pt;
	std::vector <osg::Vec3>sp;

	osg::Geode* unitSphereGeode = new osg::Geode();
};
#endif