#ifndef DSHAPE
#define DSHAPE

#include <wx/dc.h>
#include <fstream>
#include <wx/xml/xml.h>

#include "../OSG/OSGCanvas.h"
#include "../Constants.h"

#include "../DPoint.h"
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
class DShape
{
public:
	DShape() {};
	virtual ~DShape() {};
	virtual void Draw(OSGCanvas*) = 0;

	virtual void WriteXML(wxXmlNode * node) {};
	virtual bool LoadXML(wxXmlNode * node) { return false; };

	virtual void SetDatas(const double& X, const double& Y, const double& Z) {};
	virtual int  NbPtsToInitialize()const { return 0; };

	virtual bool NbPtsMax()const { return true; }
	virtual double Distance(const double x, const double y, const double z) { return DISTANCE_MAX; }

	virtual DPoint RSelectShape() { DPoint xyz; return xyz; }
	virtual DPoint GetDatas() { DPoint xyz; return xyz; }
	virtual std::vector <osg::Vec3> GetMultiDatas() { std::vector <osg::Vec3> dorigin; return dorigin; }
	virtual void SetRefT(wxRealPoint pt, DPoint dpt) {};
private:

};

#endif