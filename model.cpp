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

#include <Model.h>

Model::Model(string filename, glm::mat4 MatrixModel) 
	: MainMatrix(MatrixModel)
{
	auto it = m_vec_g.find(filename);
	if (it != m_vec_g.end()) 
	{
		m_vec = &(it->second);
		string str = filename.substr(0, filename.find("."));
		loadTextures(str, m_vec->size(), "texture.ini", &tex);
		return;
	}
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
	auto m_vec_p = &(m_vec_g.insert(make_pair(filename, vector<vector<glm::vec4>>())).first->second);
	auto m_norm_p = &(m_norm_g.insert(make_pair(filename, vector<vector<glm::vec4>>())).first->second);
	auto m_tex_p = &(m_tex_g.insert(make_pair(filename, vector<vector<glm::vec2>>())).first->second);
	auto m_indices_p = &(m_indices_g.insert(make_pair(filename, vector<vector<unsigned int>>())).first->second);
	for (int k = 0; k < scene->mNumMeshes; k++) 
	{
		m_vec_p->push_back(vector < glm::vec4>());
		m_norm_p->push_back(vector < glm::vec4>());
		m_tex_p->push_back(vector < glm::vec2>());
		m_indices_p->push_back(vector<unsigned int>());
		auto mesh = scene->mMeshes[k];
		for (int i = 0; i < mesh->mNumVertices; i++) 
		{
			aiVector3D vertex = mesh->mVertices[i];
			(*m_vec_p)[k].push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

			vertex = mesh->mNormals[i];
			(*m_norm_p)[k].push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 0));

			vertex = mesh->mTextureCoords[0][i];
			(*m_tex_p)[k].push_back(glm::vec2(vertex.x, vertex.y));
		}
		for (int l = 0; l < mesh->mNumFaces; l++) 
		{
			aiFace& face = mesh->mFaces[l];
			for (int j = 0; j < face.mNumIndices; j++) 
			{
				(*m_indices_p)[k].push_back(face.mIndices[j]);
			}
		}
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		for (int i = 0; i < 19; i++) 
		{
			material->GetTextureCount(aiTextureType(i));
		}
		aiString mat_str;
		material->GetTexture(aiTextureType_SPECULAR, 0, &mat_str);
	}
	string str = filename.substr(0, filename.find("."));
	loadTextures(str, scene->mNumMeshes, "texture.ini", &tex);
	m_vec = m_vec_p;
	m_norm = m_norm_p;
	m_tex = m_tex_p;
	m_indices = m_indices_p;
}
void Model::draw(glm::f32* V, glm::f32* P) {
	for (int i = 0; i < m_vec->size(); i++) 
	{
		sp->use();

		glUniformMatrix4fv(sp->u("V"), 1, false, V);
		glUniformMatrix4fv(sp->u("P"), 1, false, P);
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(MainMatrix));

		glEnableVertexAttribArray(sp->a("texCoord"));
		glVertexAttribPointer(sp->a("texCoord"), 2, GL_FLOAT, false, 0, (*m_tex)[i].data());

		glEnableVertexAttribArray(sp->a("normal"));
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, (*m_norm)[i].data());

		glEnableVertexAttribArray(sp->a("vertex"));
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, (*m_vec)[i].data());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex[i]);
		glUniform1i(sp->u("tex"), 0);

		glDrawElements(GL_TRIANGLES, (*m_indices)[i].size(), GL_UNSIGNED_INT, (*m_indices)[i].data());

		glDisableVertexAttribArray(sp->a("normal"));
		glDisableVertexAttribArray(sp->a("vertex"));
		glDisableVertexAttribArray(sp->a("texCoord"));
	}
}
GLuint Model::readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	vector<unsigned char> image; 
	unsigned width, height; 
	unsigned error = lodepng::decode(image, width, height, filename);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}

void Model::loadTextures(string fname, int maxI, string ini_f, vector<GLuint>* vec) 
{
	vector<pair<int, string>> f_names;
	string _fname{ ".\\textures\\"};
	_fname += fname;
	_fname += "\\";
	_fname += ini_f;
	ifstream in(_fname);
	if (!in.is_open()) 
	{
		for (int i = 0; i < maxI; i++)vec->push_back(-1);
		return;
	}
	string str1, str2, line;
	int index;
	while (!in.eof()) 
	{
		getline(in, line);
		int pos = line.find("=");
		if (pos == string::npos)continue;
		str1 = line.substr(0, pos);
		str2 = line.substr(pos + 1, string::npos);
		while (str2[0] == ' ')str2 = str2.substr(1, string::npos);
		if (str2 == "\n" || str2.size() == 0)continue;
		index = atoi(str1.c_str());
		f_names.push_back(pair<int, string>{index, str2});
	}
	int j{ 0 };
	for (int i = 0; i < maxI; i++) 
	{
		if (j != f_names.size() && f_names[j].first == i)
		{
			string tex_file = "textures/" + fname + "/" + f_names[j].second;
			j++;
			auto it1 = tex_global.find(tex_file);
			if (it1 == tex_global.end()) 
			{
				auto it = tex_global.insert(make_pair(tex_file.c_str(), readTexture((tex_file).c_str())));
				vec->push_back(it.first->second);
			}
			else vec->push_back(it1->second);
		}
		else vec->push_back(-1);
	}

}