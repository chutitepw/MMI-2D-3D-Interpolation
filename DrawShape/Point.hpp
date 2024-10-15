#ifndef SHAPEPOINT
#define SHAPEPOINT

#include "Shape.hpp"
#include <wx/gdicmn.h>

class ShapePoint : public Shape
{
public:
	ShapePoint();
	~ShapePoint();
	void Draw(wxDC*, ScrolledImageComponent*);
	
	void SetData(const wxRealPoint& pt) { m_origin = pt; };
	wxRealPoint GetDatas() { return m_origin; }
	virtual void WriteXML(wxXmlNode * node);
	virtual bool LoadXML(wxXmlNode * node);

	virtual double Distance(const wxRealPoint& pt);
private:
	wxRealPoint m_origin;

};
#endif