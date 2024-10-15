#include "BBCFileOBJ.hpp"


#include <fstream>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////
BBCFileOBJ::BBCFileOBJ():
m_coord_text(false),
m_coord_norm(false)
{
	m_barycenter.m_X = 0.;
	m_barycenter.m_Y = 0.;
	m_barycenter.m_Z = 0.;
}
///////////////////////////////////////////////////////////////////////////////
BBCFileOBJ::~BBCFileOBJ(){}

///////////////////////////////////////////////////////////////////////////////
const bool BBCFileOBJ::Read(const std::string& filename){
	std::ifstream file(filename);
	if (!file.good())
		return false;
	m_filename = filename;

	std::string line;
	while (!file.eof()){
		std::getline(file, line);
		Parse(line);
	}
	m_barycenter.m_X = m_barycenter.m_Y = m_barycenter.m_Z = 0.0;
	double size = (double)m_pts.size();
	for (unsigned int i = 0; i < m_pts.size(); ++i) {
		m_barycenter.m_X = m_barycenter.m_X + m_pts[i].m_X / size;
		m_barycenter.m_Y = m_barycenter.m_Y + m_pts[i].m_Y / size;
		m_barycenter.m_Z = m_barycenter.m_Z + m_pts[i].m_Z / size;
	}
	return true;

}
///////////////////////////////////////////////////////////////////////////////
void BBCFileOBJ::Parse(const std::string& ligne){
	std::string prefixe = ligne.substr(0, 2);
	if (prefixe == "v "){
		ReadPts(ligne);
		return;
	}
	if (prefixe == "vn"){
		ReadPtsNorm(ligne);
		return;
	}
	if (prefixe == "vt"){
		ReadPtsText(ligne);
		return;
	}
	if (prefixe == "f "){
		ReadFace(ligne);
		return;
	}
}
///////////////////////////////////////////////////////////////////////////////
void BBCFileOBJ::ReadPts(const std::string& ligne){
	//v -13.629332 -16.843836 24.357468
	std::stringstream ss;
	ss << ligne.substr(2,ligne.size()-2);
	ObjPt pt;
	ss >> pt.m_X >> pt.m_Y >> pt.m_Z;
	m_pts.push_back(pt);
}
///////////////////////////////////////////////////////////////////////////////
void BBCFileOBJ::ReadFace(const std::string& ligne){
	//f 551/858/551 530/908/530 620/896/620
	std::stringstream ss;
	ss << ligne.substr(2, ligne.size() - 2);
	ObjFace f;
	std::string a, b, c;
	ss >> a >> b >> c;

	unsigned int a1, b1, c1, a2, b2, c2, a3, b3, c3;
	unsigned int s1 = Decompose(a, a1, b1, c1);
	unsigned int s2 = Decompose(b, a2, b2, c2);
	unsigned int s3 = Decompose(c, a3, b3, c3);
	f.m_a = a1;
	f.m_b = a2;
	f.m_c = a3;
	if (m_coord_text){
		f.m_at = b1;
		f.m_bt = b2;
		f.m_ct = b3;
	}
	if (m_coord_norm){
		f.m_an = c1;
		f.m_bn = c2;
		f.m_cn = c3;
	}
	m_faces.push_back(f);
}
///////////////////////////////////////////////////////////////////////////////
unsigned int BBCFileOBJ::Decompose(const std::string& ligne, unsigned int& a, unsigned int&b, unsigned int&c){
	unsigned int slash = 0;
	std::vector<unsigned int> pos;
	pos.resize(3,0);
	for (unsigned int i = 0; i < ligne.size(); ++i){
		if (ligne[i] == '/'){
			pos[slash] = i;
			slash++;
		}
	}
	if (slash == 0)
		a = atoi(ligne.c_str());
	else {
		if (pos[1] - pos[0]>1)
			m_coord_text = true;
		if (pos[2] - pos[1]>1)
			m_coord_norm = true;
		a = atoi(ligne.substr(0, pos[0]).c_str());
		b = atoi(ligne.substr(pos[0] + 1, pos[1]-pos[0]).c_str());
		c = atoi(ligne.substr(pos[1] + 1, pos[2] - pos[1]).c_str());
	}
	return slash;
}

///////////////////////////////////////////////////////////////////////////////
void BBCFileOBJ::ReadPtsNorm(const std::string& ligne){
	//vn -0.722136 -0.104018 0.683886
	std::stringstream ss;
	ss << ligne.substr(2, ligne.size() - 2);
	ObjPt pt;
	ss >> pt.m_X >> pt.m_Y >> pt.m_Z;
	m_pts_norm.push_back(pt);
}
///////////////////////////////////////////////////////////////////////////////
void BBCFileOBJ::ReadPtsText(const std::string& ligne){
	//vt 0.020594 0.123872
	std::stringstream ss;
	ss << ligne.substr(2, ligne.size() - 2);
	ObjPtTex pt;
	ss >> pt.m_u >> pt.m_v ;
	m_pts_text.push_back(pt);
}
