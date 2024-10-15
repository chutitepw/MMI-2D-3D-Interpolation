#ifndef DSHAPEREFERENCE
#define DSHAPEREFERENCE

#include "3DShape.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include <wx/gdicmn.h>
#include "../DPoint.h"
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>

class DShapeReference : public DShape
{
public:
	DShapeReference();
	~DShapeReference();
	void Draw(OSGCanvas*);
	void SetData(const double& X, const double& Y, const double& Z ) { x = X; y = Y;
	z = Z;};
	void SetRefpoint(const int refcount) { Refcount = refcount; };
	virtual void WriteXML(wxXmlNode * node);
	virtual bool LoadXML(wxXmlNode * node);

	virtual double Distance(const double x, const double y, const double z);

	virtual DPoint RSelectShape();
	virtual void SetRefT(wxRealPoint pt, DPoint dpt);

private:
	double x, y, z;
	long Refcount;
	std::pair <wxRealPoint, DPoint> DRefLink;
	osg::PositionAttitudeTransform* sphereXForm = new osg::PositionAttitudeTransform();
	

	osg::Geode* unitSphereGeode = new osg::Geode();
};
#endif