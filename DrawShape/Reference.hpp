#ifndef SHAPEREFERENCE
#define SHAPEREFERENCE

#include "Shape.hpp"
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include <wx/gdicmn.h>

class ShapeReference : public Shape
{
public:
	ShapeReference();
	~ShapeReference();
	void Draw(wxDC*, ScrolledImageComponent*);
	void SetData(const wxRealPoint& pt) { m_origin = pt; };
	void SetRefpoint(const int refcount) { Refcount = refcount; };
	virtual void WriteXML(wxXmlNode * node);
	virtual bool LoadXML(wxXmlNode * node);

	virtual double Distance(const wxRealPoint& pt);

	virtual wxRealPoint RSelectShape() {return m_origin; }
	virtual void SetRefT(wxRealPoint pt, DPoint dpt);
	virtual int ReturnRef() { return Refcount; }
private:
	wxRealPoint m_origin;
	long Refcount;
	std::pair <wxRealPoint, DPoint> RefLink;
};
#endif