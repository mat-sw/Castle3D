///*
//Niniejszy program jest wolnym oprogramowaniem; możesz go
//rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
//Licencji Publicznej GNU, wydanej przez Fundację Wolnego
//Oprogramowania - według wersji 2 tej Licencji lub(według twojego
//wyboru) którejś z późniejszych wersji.
//
//Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
//użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
//gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
//ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
//Powszechnej Licencji Publicznej GNU.
//
//Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
//Powszechnej Licencji Publicznej GNU(GNU General Public License);
//jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
//Place, Fifth Floor, Boston, MA  02110 - 1301  USA
//*/

#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <shaderprogram.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <utility>
#include <algorithm>
#include "lodepng.h"

#include <fstream>
#include <map>

using namespace std;

class Model {
private:
	vector<vector<glm::vec4>>* m_vec;
	vector<vector<glm::vec4>>* m_norm;
	vector<vector<glm::vec2>>* m_tex;
	vector<vector<unsigned int>>* m_indices;

	vector<GLuint> tex;
	vector<GLuint> specular;
	vector<GLuint> norm;
	vector<GLuint> height;

public:
	map<string, GLuint> tex_global;
	map<string, vector<vector<glm::vec4>>> m_vec_g;
	map<string, vector<vector<glm::vec4>>> m_norm_g;
	map<string, vector<vector<glm::vec2>>> m_tex_g;
	map<string, vector<vector<unsigned int>>> m_indices_g;
	
	glm::mat4 MainMatrix;

	Model(string filename, glm::mat4 MatrixModel);
	void draw(glm::f32* V, glm::f32* P);
	GLuint readTexture(const char* filename);
	void loadTextures(string filename, int maxIndex, string ini_file, vector<GLuint>* vec);

};