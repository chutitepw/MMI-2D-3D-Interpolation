#ifndef SHAPERECTANGLE
#define SHAPERECTANGLE

#include "Shape.hpp"
#include <wx/gdicmn.h>

class ShapeRectangle : public Shape
{
public:
	ShapeRectangle();
	~ShapeRectangle();
	void Draw(wxDC*, ScrolledImageComponent*) ;

	void SetA(const wxRealPoint& pt) { m_A = pt; };
	void SetB(const wxRealPoint&pt) { m_B =pt ; };

	virtual void WriteXML(wxXmlNode * node);
	virtual bool LoadXML(wxXmlNode * node);
	std::vector<wxRealPoint> GetMultiDatas() {
		wxRealPoint m_C(m_A.x, m_B.y);
		wxRealPoint m_D(m_B.x, m_A.y);
		/*For link*/
		m_data.push_back(m_A);
		m_data.push_back(m_C);
		m_data.push_back(m_B);
		m_data.push_back(m_D); 
		return m_data; }
	virtual void SetDatas(const std::vector<wxRealPoint> & datas) {
		m_A = datas[0];
		m_B = datas[1];
		
	};
	virtual int  NbPtsToInitialize()const { return 2; };
	virtual double Distance(const wxRealPoint& pt);
private:
	wxRealPoint m_A;
	wxRealPoint m_B;
	std::vector<wxRealPoint> m_data;
};
#endif