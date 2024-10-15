#ifndef SHAPECIRCLE
#define SHAPECIRCLE

#include "Shape.hpp"
#include <wx/gdicmn.h>

class ShapeCircle : public Shape
{
public:
	ShapeCircle();
	~ShapeCircle();
	void Draw(wxDC*, ScrolledImageComponent*);

	void SetOrigin(const wxRealPoint& pt) { m_origin = pt; };
	void SetRadius(const float &r) { m_radius = r; };

	virtual void WriteXML(wxXmlNode * node) ;
	virtual bool LoadXML(wxXmlNode * node) ;
	std::vector<wxRealPoint> GetMultiDatas() { return pts; }
	virtual void SetDatas(const std::vector<wxRealPoint> & datas) {
		pts = datas;
		m_origin=datas[0];
		m_radius = sqrt((m_origin.x - datas[1].x)*(m_origin.x - datas[1].x) + (m_origin.y - datas[1].y)*(m_origin.y - datas[1].y));
	};
	virtual int  NbPtsToInitialize()const { return 2; };
	virtual double Distance(const wxRealPoint& pt);
private:
	wxRealPoint m_origin;
	double  m_radius;
	std::vector<wxRealPoint> pts;
};
#endif