#ifndef SHAPE
#define SHAPE

#include <wx/dc.h>
#include <fstream>
#include <wx/xml/xml.h>

#include "../ScrolledImageComponent.h"
#include "../Constants.h"
class Shape
{
public:
	Shape() {};
	virtual ~Shape() {};
	virtual void Draw(wxDC*, ScrolledImageComponent*) = 0;

	/*virtual void WriteXML(std::ofstream &) {};
	virtual bool LoadXML() { return true; };*/

	virtual void WriteXML(wxXmlNode * node) {};
	virtual bool LoadXML(wxXmlNode * node) { return false; };

	virtual void SetDatas(const std::vector<wxRealPoint> & datas) {};
	
	virtual int  NbPtsToInitialize()const { return 0; };
	
	virtual bool NbPtsMax()const { return true; }
	virtual double Distance(const wxRealPoint& pt) { return DISTANCE_MAX; }
	
	virtual wxRealPoint RSelectShape() { wxRealPoint refp; return refp; }

	virtual wxRealPoint GetDatas() { wxRealPoint origin; return origin; }

	virtual std::vector<wxRealPoint> GetMultiDatas() { std::vector<wxRealPoint> origin; return origin; }

	virtual void SetRefT(wxRealPoint pt, DPoint dpt) {};
	virtual int ReturnRef() { return 0; }

};

#endif