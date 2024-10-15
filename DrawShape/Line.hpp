#ifndef SHAPELINE
#define SHAPELINE

#include "Shape.hpp"
#include <wx/gdicmn.h>

class ShapeLine : public Shape
{
public:
	ShapeLine();
	~ShapeLine();
	void Draw(wxDC*, ScrolledImageComponent*);

	void SetDatas(const std::vector<wxRealPoint>& pt) { 
		m_data.clear();
		m_data = pt;
	};
	std::vector<wxRealPoint> GetMultiDatas() { return m_data; }
	virtual void WriteXML(wxXmlNode * node);
	virtual bool LoadXML(wxXmlNode * node);

	virtual int  NbPtsToInitialize()const { return 2; };
	virtual bool NbPtsMax()const { return false; }
	virtual double Distance(const wxRealPoint& pt);

private:
	std::vector<wxRealPoint> m_data;
};
#endif