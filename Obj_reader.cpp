#include "Painter.h"
#include "Obj_reader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>


void Obj_reader::clear() {
	v.clear(); vt.clear(); vn.clear();
	faces.clear();
}

int Obj_reader::load_from_file(const char *str) {
	clear(); // ? 

	std::ifstream input;
	input.open(str);

	if (!input) {
		std::cout << "loading obj failed! " << std::endl;
		return -1;
	}
	else {
		std::string str;
		float x, y, z, w;
		while (input >> str) {
			
			if (str.substr(0, 1) == "#") {
				getline(input, str);
			}
			else if (str.substr(0, 1) == "v") {
				input >> x >> y >> z;
				v.push_back(wmz::Point3(x, y, z));
			} 
			else if (str.substr(0, 2) == "vn") {
				input >> x >> y >> z;
				vn.push_back(wmz::Point3(x, y, z));
			}
			else if (str.substr(0, 1) == "f") {
				int now = faces.size();
				faces.push_back(std::vector<int >());
				std::stringstream ss; ss.clear();
				getline(input, str);
				ss << str;
				int t;
				while (ss >> t) {
					faces[now].push_back(t);
				}
			}
			else {
			}
		}
	}

	input.close();
	return 0;
}

int Obj_reader::load_from_file(const char *filename, Painter &painter) {
	clear(); // ? 

	std::ifstream input;
	input.open(filename);

	if (!input) {
		std::cout << "loading obj failed! " << std::endl;
		return -1;
	}
	else {
		std::string str;
		
		std::ofstream os;
		float x, y, z, w;
		int nowobj;

		os.open("test.txt");
		os << "test begin " << std::endl;
		
		while (input >> str) {
			
			if (str.substr(0, 1) == "#") {
				getline(input, str);
			}
			else if (str.substr(0, 1) == "g") {
				os << "=========================================" << std::endl;
				getline(input, str);
				
				//nowobj = painter.get_objnumber();
				std::stringstream ss;	//��֪����ʲô�����ˣ�������������f�ǿ���ͬһ��ss�ᱬը
				ss.clear(); ss << str; ss >> str; 
				//painter.add_obj(Object(str.c_str()));
				
				continue;
			}
			else if (str.substr(0, 1) == "v" && str.size() == 1) {
				input >> x >> y >> z;
				v.push_back(wmz::Point3(x, y, z));
			}
			else if (str.substr(0, 2) == "vt") {
				input >> x >> y >> z;
				vt.push_back(wmz::Point3(x, y, z));
			}
			else if (str.substr(0, 2) == "vn") {
				input >> x >> y >> z;
				vn.push_back(wmz::Point3(x, y, z));
			}
			else if (str.substr(0, 1) == "f") {
				std::stringstream ss;

				getline(input, str);

				int  ft = faces.size(), tt = texturepos.size(), nt = normalvec.size();

				faces.push_back(std::vector<int >());
				texturepos.push_back(std::vector<int >());
				normalvec.push_back(std::vector<int >());
				
				if (sscanf_s(str.c_str(), "%f %f", &x, &y) == 2) {
					std::cout << "format 1" << std::endl;
					int x;
					ss.clear(); ss << str;
						
					while (ss >> x) {
						faces[ft].push_back(x);
					}
				}
				else if (sscanf_s(str.c_str(), "%f/%f %f/%f", &x, &y, &x, &y) == 4) {
					//std::cout << "format 2" << std::endl;
					for (int i = 0; i < str.size(); ++i) if (str[i] == '/') str[i] = ' ';

					int x, y;
					ss.clear(); ss << str;
				
					while (ss >> x >>  y) {
						faces[ft].push_back(x);
						texturepos[tt].push_back(y);
					}
				}
				else if (sscanf_s(str.c_str(), "%f/%f/%f %f/%f/%f", &x, &x, &x, &x, &x, &x) == 6) {
					//std::cout << "format 3" << std::endl;
					for (int i = 0; i < str.size(); ++i) if (str[i] == '/') str[i] = ' ';

					int x, y, z;
					ss.clear(); ss << str;

					while (ss >> x >> y >> z) {
						faces[ft].push_back(x);
						texturepos[tt].push_back(y);
						normalvec[nt].push_back(z);
					}
				}
				else if (sscanf_s(str.c_str(), "%f//%f %f//%f", &x, &x, &x, &x) == 4) {
					std::cout << "format 4" << std::endl;
				}
				else {
					std::cout << " unknow format" << std::endl;
				}
			}
			else {
				getline(input, str);
			}
		}
		os << "test end" << std::endl;
		os.close();
	}

	std::cout << "reading finish" << std::endl;

	int nowobj = painter.get_objnumber();
	
	painter.add_obj(Object(filename));

	unsigned int tmpLim = std::max(v.size(), std::max(vt.size(), vn.size()));

	for (int i = 0; i < tmpLim; ++i) {
		//std::cout << i << ' ' << v.size() << ' ' << vt.size() << ' ' << vn.size() << std::endl;
		painter[nowobj].add_all_points(
			i < v.size() ?  v[i]  : wmz::Point3(),
			i < vt.size() ? vt[i] : wmz::Point3(),
			i < vn.size() ? vn[i] : wmz::Point3()
		);
	}

	for (int i = 0; i < faces.size(); ++i) {
		
		wmz::Face tmpface;
		//if (texturepos.size() && !normalvec.size()) {
			//std::cout << faces[i].size() << std::endl;
			for (int j = 0; j < faces[i].size(); ++j) {
				//std::cout << faces[i][j] - 1 << ' ' << texturepos[i][j] - 1 << std::endl;
				tmpface.add(faces[i][j] - 1, \
						    (i < texturepos.size() && j < texturepos[i].size()) ? (texturepos[i][j] - 1) : -1, \
							(i < normalvec.size() && j < normalvec[i].size()) ? (normalvec[i][j] - 1) : -1      
				);		//�±��޸�����Ϊobj���±������1��ʼ
			}
			painter[nowobj].add_face(tmpface);
		
		//}
	}


	if (!normalvec.size()) {
		painter[nowobj].calc_norm_vector();
	}

	std::cout << "finish" << std::endl;

	input.close();
	return 0;
}