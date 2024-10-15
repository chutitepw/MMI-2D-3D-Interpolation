#ifndef DSHAPEPOINT
#define DSHAPEPOINT

#include "3DShape.h"
#include <wx/gdicmn.h>


class DShapePoint : public DShape
{
public:
	DShapePoint();
	~DShapePoint();
	void Draw(OSGCanvas*);
	void SetData(const double& X, const double& Y, const double& Z) {
		x = X; y = Y;
		z = Z;
	};
	virtual void WriteXML(wxXmlNode * node);
	virtual bool LoadXML(wxXmlNode * node);

	virtual double Distance(const double x, const double y, const double z);


private:
	double x, y, z;
	osg::PositionAttitudeTransform* sphereXForm = new osg::PositionAttitudeTransform();
	DPoint GetDatas() {
		DPoint xyz; 
		xyz.x = x;
		xyz.y = y;
		xyz.z = z;
		return xyz; }

	osg::Geode* unitSphereGeode = new osg::Geode();
};
#endif