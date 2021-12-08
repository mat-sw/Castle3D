#define GLM_FORCE_RADIANS

#include "model.h"

vector<Model*> models;
vector<glm::mat4> objects;
vector<string> filenames;

float speed_y{ 0 }; //[radiany/s]
float speed_x{ 0 }; //[radiany/s]
float w_speed{ 0 };
float sky_speed{ 0.05 };

glm::vec3 pos{ glm::vec3(0, 200, -180) };

// Zrodla swiatla
vector<glm::vec4> LightPositions =
{
	glm::vec4(-1000.0f, 500.0f, -500.0f, 1.0f),
	glm::vec4(1000.0f, 500.0f, -500.0f, 1.0f),
	glm::vec4(1000.0f, 500.0f, 2000.0f, 1.0f),
	glm::vec4(-1000.0f, 500.0f, 2000.0f, 1.0f),
	glm::vec4(0.0f, 600.0f, 500.0f, 1.0f)
};

#ifndef CONSTANTS_H
#define CONSTANTS_H

const float PI{ 3.141592653f };
const int height{ 1080 };
const int width{ 1920 };
const float As = width / height;

#endif

// Obsluga bledow
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
void LoadObjects();
// Obsluga klawiatury
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) speed_x = 1.5;
		if (key == GLFW_KEY_D) speed_x = -1.5;
		if (key == GLFW_KEY_W) speed_y = -1.5;
		if (key == GLFW_KEY_S) speed_y = 1.5;
		if (key == GLFW_KEY_UP) w_speed = 200;
		if (key == GLFW_KEY_DOWN) w_speed = -200;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A) speed_x = 0;
		if (key == GLFW_KEY_D) speed_x = 0;
		if (key == GLFW_KEY_W) speed_y = 0;
		if (key == GLFW_KEY_S) speed_y = 0;
		if (key == GLFW_KEY_UP) w_speed = 0;
		if (key == GLFW_KEY_DOWN) w_speed = 0;
	}
}

void initOpenGLProgram(GLFWwindow* window)
{
	initShaders();
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 M = glm::mat4(1.0f);

	glm::mat4 BRIDGE = M;
	BRIDGE = glm::rotate(BRIDGE, PI, glm::vec3(0, 1, 0));
	BRIDGE = glm::scale(BRIDGE, glm::vec3(0.1, 0.1, 0.1));
	objects.push_back(BRIDGE);
	filenames.push_back("Drawbridge3.fbx");

	glm::mat4 WALL1 = BRIDGE;
	WALL1 = glm::translate(WALL1, glm::vec3(2100, 0, -650));
	objects.push_back(WALL1);
	filenames.push_back("Wall07.fbx");

	// start
	glm::mat4 B1 = WALL1; // stajnia 
	B1 = glm::translate(B1, glm::vec3(50.0f, 0.0f, -400.0f));
	B1 = glm::rotate(B1, PI, glm::vec3(0.0f, 1.0f, 0.0f));
	B1 = glm::scale(B1, glm::vec3(0.95f, 0.95f, 0.95f));
	objects.push_back(B1);
	filenames.push_back("Building2.fbx");

	glm::mat4 K = B1; // Konie
	K = glm::translate(K, glm::vec3(-30.0f, 0.0f, -20.0f));
	for (int i = 0; i < 3; ++i)
	{
		objects.push_back(K);
		filenames.push_back("Horse.fbx");
		K = glm::translate(K, glm::vec3(-250.f, 0.0f, 0.0f));
	}

	glm::mat4 B2 = B1; // stajnia 2
	B2 = glm::translate(B2, glm::vec3(-1550.0f, 0.0f, 1300.0f));
	B2 = glm::rotate(B2, PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	objects.push_back(B2);
	filenames.push_back("Building2.fbx");

	glm::mat4 K2 = B2; // Konie 2
	K2 = glm::translate(K2, glm::vec3(-30.0f, 0.0f, -20.0f));
	for (int i = 0; i < 3; ++i)
	{
		objects.push_back(K2);
		filenames.push_back("Horse.fbx");
		K2 = glm::translate(K2, glm::vec3(-250.f, 0.0f, 0.0f));
	}

	glm::mat4 S1 = B1; // Twierdza
	S1 = glm::translate(S1, glm::vec3(-1500.0f, 0.0f, 3700.0f));
	S1 = glm::scale(S1, glm::vec3(1.5f, 1.5f, 1.5f));
	S1 = glm::rotate(S1, -PI * 3 / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	objects.push_back(S1);
	filenames.push_back("Stronghold.fbx");

	// mury od twierdzy
	glm::mat4 WALLS01 = S1;
	WALLS01 = glm::translate(WALLS01, glm::vec3(-1300.0f, 0.0f, 500.0f));
	WALLS01 = glm::scale(WALLS01, glm::vec3(0.45f, 1.0f, 1.0f));
	objects.push_back(WALLS01);
	filenames.push_back("Wall03.fbx");

	glm::mat4 WALLS02 = S1;
	WALLS02 = glm::translate(WALLS02, glm::vec3(900.0f, 0.0f, 1150.0f));
	WALLS02 = glm::rotate(WALLS02, -PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	WALLS02 = glm::scale(WALLS02, glm::vec3(0.55f, 0.8f, 1.0f));
	objects.push_back(WALLS02);
	filenames.push_back("Wall03.fbx");

	glm::mat4 WALLS03 = WALLS02;
	WALLS03 = glm::translate(WALLS03, glm::vec3(1100.0f, 0.0f, -750.0f));
	WALLS03 = glm::rotate(WALLS03, PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	WALLS03 = glm::scale(WALLS03, glm::vec3(0.8f, 1.0f, 1.0f));
	objects.push_back(WALLS03);
	filenames.push_back("Wall03.fbx");

	glm::mat4 K1 = S1;
	K1 = glm::translate(K1, glm::vec3(-1100.0f, 0.0f, 2200.0f));
	K1 = glm::rotate(K1, PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	K1 = glm::scale(K1, glm::vec3(0.8f, 0.6f, 0.8f));
	objects.push_back(K1);
	filenames.push_back("Building5.fbx");

	glm::mat4 BED = K1; // Konie 2
	BED = glm::translate(BED, glm::vec3(0.0f, 0.0f, 0.0f));
	//BED = glm::scale(BED, glm::vec3(0.0f, 200.0f, 200.0f));
	for (int i = 0; i < 3; ++i)
	{
		objects.push_back(BED);
		filenames.push_back("Bed.fbx");
		BED = glm::translate(BED, glm::vec3(-10.f, 0.0f, 0.0f));
	}

	glm::mat4 CH1 = S1;
	CH1 = glm::translate(CH1, glm::vec3(-200.0f, 0.0f, 4700.0f));
	CH1 = glm::scale(CH1, glm::vec3(0.9f, 0.9f, 0.9f));
	CH1 = glm::rotate(CH1, PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	objects.push_back(CH1);
	filenames.push_back("Church.fbx");

	glm::mat4 TS1 = WALLS03;
	TS1 = glm::translate(TS1, glm::vec3(-1200.0f, 0.0f, 0.0f));
	objects.push_back(TS1);
	filenames.push_back("Tower2.fbx");

	// koniec murow od twierdzy
	// koniec
	glm::mat4 TOWER1 = WALL1;
	TOWER1 = glm::translate(TOWER1, glm::vec3(1150.0f, 0.0f, 0.0f));
	TOWER1 = glm::rotate(TOWER1, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	TOWER1 = glm::translate(TOWER1, glm::vec3(150.0f, 0.0f, -50.0f));
	objects.push_back(TOWER1);
	filenames.push_back("Tower7.fbx");

	glm::mat4 WALL2 = TOWER1;
	WALL2 = glm::translate(WALL2, glm::vec3(200.0f, 0.0f, 0.0f));
	WALL2 = glm::rotate(WALL2, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	WALL2 = glm::translate(WALL2, glm::vec3(1100.0f, 0.0f, 0.0f));
	objects.push_back(WALL2);
	filenames.push_back("Wall07.fbx");

	glm::mat4 TOWER2 = WALL2;
	TOWER2 = glm::translate(TOWER2, glm::vec3(1150.0f, 0.0f, 0.0f));
	TOWER2 = glm::rotate(TOWER2, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	TOWER2 = glm::translate(TOWER2, glm::vec3(150.0f, 0.0f, -50.0f));
	objects.push_back(TOWER2);
	filenames.push_back("Tower7.fbx");

	glm::mat4 WALL3 = TOWER2;
	WALL3 = glm::translate(WALL3, glm::vec3(200.0f, 0.0f, 0.0f));
	WALL3 = glm::rotate(WALL3, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	WALL3 = glm::translate(WALL3, glm::vec3(1100.0f, 0.0f, 0.0f));
	objects.push_back(WALL3);
	filenames.push_back("Wall07.fbx");

	glm::mat4 TOWER3 = WALL3;
	TOWER3 = glm::translate(TOWER3, glm::vec3(1150.0f, 0.0f, 0.0f));
	TOWER3 = glm::rotate(TOWER3, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	TOWER3 = glm::translate(TOWER3, glm::vec3(150.0f, 0.0f, -50.0f));
	objects.push_back(TOWER3);
	filenames.push_back("Tower7.fbx");

	glm::mat4 WALL4 = TOWER3;
	WALL4 = glm::translate(WALL4, glm::vec3(200.0f, 0.0f, 0.0f));
	WALL4 = glm::rotate(WALL4, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	WALL4 = glm::translate(WALL4, glm::vec3(1100.0f, 0.0f, 0.0f));
	objects.push_back(WALL4);
	filenames.push_back("Wall07.fbx");

	glm::mat4 TOWER4 = WALL4;
	TOWER4 = glm::translate(TOWER4, glm::vec3(1150.0f, 0.0f, 0.0f));
	TOWER4 = glm::rotate(TOWER4, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	TOWER4 = glm::translate(TOWER4, glm::vec3(150.0f, 0.0f, -50.0f));
	objects.push_back(TOWER4);
	filenames.push_back("Tower7.fbx");

	glm::mat4 WALL5 = TOWER4;
	WALL5 = glm::translate(WALL5, glm::vec3(200.0f, 0.0f, 0.0f));
	WALL5 = glm::rotate(WALL5, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	WALL5 = glm::translate(WALL5, glm::vec3(1100.0f, 0.0f, 0.0f));
	objects.push_back(WALL5);
	filenames.push_back("Wall07.fbx");

	glm::mat4 TOWER5 = WALL5;
	TOWER5 = glm::translate(TOWER5, glm::vec3(1150.0f, 0.0f, 0.0f));
	objects.push_back(TOWER5);
	filenames.push_back("Tower7.fbx");

	glm::mat4 WALL6 = TOWER5;
	WALL6 = glm::translate(WALL6, glm::vec3(1000, 0, 0));
	objects.push_back(WALL6);
	filenames.push_back("Wall07.fbx");

	glm::mat4 TOWER6 = WALL6;
	TOWER6 = glm::translate(TOWER6, glm::vec3(1000.0f, 0.0f, 0.0f));
	objects.push_back(TOWER6);
	filenames.push_back("Tower7.fbx");

	glm::mat4 WALL7 = TOWER6;
	WALL7 = glm::translate(WALL7, glm::vec3(1000, 0, 0));
	objects.push_back(WALL7);
	filenames.push_back("Wall07.fbx");

	glm::mat4 TOWER7 = WALL7;
	TOWER7 = glm::translate(TOWER7, glm::vec3(1150.0f, 0.0f, 0.0f));
	TOWER7 = glm::rotate(TOWER7, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	TOWER7 = glm::translate(TOWER7, glm::vec3(150.0f, 0.0f, -50.0f));
	objects.push_back(TOWER7);
	filenames.push_back("Tower7.fbx");

	glm::mat4 WALL8 = TOWER7;
	WALL8 = glm::translate(WALL8, glm::vec3(200.0f, 0.0f, 0.0f));
	WALL8 = glm::rotate(WALL8, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	WALL8 = glm::translate(WALL8, glm::vec3(1100.0f, 0.0f, 0.0f));
	objects.push_back(WALL8);
	filenames.push_back("Wall07.fbx");

	glm::mat4 TOWER8 = WALL8;
	TOWER8 = glm::translate(TOWER8, glm::vec3(1150.0f, 0.0f, 0.0f));
	TOWER8 = glm::rotate(TOWER8, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	TOWER8 = glm::translate(TOWER8, glm::vec3(150.0f, 0.0f, -50.0f));
	objects.push_back(TOWER8);
	filenames.push_back("Tower7.fbx");

	glm::mat4 WALL9 = TOWER8;
	WALL9 = glm::translate(WALL9, glm::vec3(200.0f, 0.0f, 0.0f));
	WALL9 = glm::rotate(WALL9, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	WALL9 = glm::translate(WALL9, glm::vec3(1100.0f, 0.0f, 0.0f));
	objects.push_back(WALL9);
	filenames.push_back("Wall07.fbx");

	glm::mat4 TOWER9 = WALL9;
	TOWER9 = glm::translate(TOWER9, glm::vec3(1150.0f, 0.0f, 0.0f));
	TOWER9 = glm::rotate(TOWER9, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	TOWER9 = glm::translate(TOWER9, glm::vec3(150.0f, 0.0f, -50.0f));
	objects.push_back(TOWER9);
	filenames.push_back("Tower7.fbx");

	glm::mat4 WALL10 = TOWER9;
	WALL10 = glm::translate(WALL10, glm::vec3(200.0f, 0.0f, 0.0f));
	WALL10 = glm::rotate(WALL10, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	WALL10 = glm::translate(WALL10, glm::vec3(1100.0f, 0.0f, 0.0f));
	objects.push_back(WALL10);
	filenames.push_back("Wall07.fbx");

	glm::mat4 TOWER10 = WALL10;
	TOWER10 = glm::translate(TOWER10, glm::vec3(1150.0f, 0.0f, 0.0f));
	TOWER10 = glm::rotate(TOWER10, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	TOWER10 = glm::translate(TOWER10, glm::vec3(150.0f, 0.0f, -50.0f));
	objects.push_back(TOWER10);
	filenames.push_back("Tower7.fbx");

	glm::mat4 WALL11 = TOWER10;
	WALL11 = glm::translate(WALL11, glm::vec3(200.0f, 0.0f, 0.0f));
	WALL11 = glm::rotate(WALL11, PI / 8, glm::vec3(0.0f, 1.0f, 0.0f));
	WALL11 = glm::translate(WALL11, glm::vec3(1100.0f, 0.0f, 0.0f));
	objects.push_back(WALL11);
	filenames.push_back("Wall07.fbx");
		
	glm::mat4 GROUND = M;
	GROUND = glm::translate(GROUND, glm::vec3(0.0f, -50.0f, 500.0f));
	GROUND = glm::scale(GROUND, glm::vec3(100, 0.1, 100));
	objects.push_back(GROUND);
	filenames.push_back("Cube.fbx");

	glm::mat4 SKY = M;
	SKY = glm::translate(SKY, glm::vec3(0.0f, 50.0f, 500.0f));
	SKY = glm::scale(SKY, glm::vec3(1710, 1710, 1710));
	objects.push_back(SKY);
	filenames.push_back("Sphere.fbx");

	LoadObjects();
}

void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
}

void LoadObjects() {
	for (int i = 0; i < objects.size(); i++) 
	{
		models.push_back(new Model(filenames[i], objects[i]));
	}
}

// obliczanie macierzy obrotu przy poruszaniu
glm::vec3 ChangeDirection(float angle_x, float angle_y) {
	glm::vec4 direction = glm::vec4(0, 0, 1, 0);
	glm::mat4 M = glm::rotate(glm::mat4(1.0f), angle_y, glm::vec3(0, 1, 0));
	M = glm::rotate(M, angle_x, glm::vec3(1, 0, 0));
	direction = M * direction;
	return glm::vec3(direction);
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y, float angle_sky) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(pos, pos + ChangeDirection(angle_x, angle_y), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(60.0f), As, 0.5f, 5000.0f); //Wylicz macierz rzutowania

	sp->use();

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	glUniform4fv(sp->u("lp1"), 1, glm::value_ptr(LightPositions[0]));
	glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(LightPositions[1]));
	glUniform4fv(sp->u("lp3"), 1, glm::value_ptr(LightPositions[2]));
	glUniform4fv(sp->u("lp4"), 1, glm::value_ptr(LightPositions[3]));
	glUniform4fv(sp->u("lp5"), 1, glm::value_ptr(LightPositions[4]));

	// Obrot nieba
	
	glm::mat4 SKY = objects[objects.size() - 1];
	objects.pop_back();
	models.pop_back();
	SKY = glm::rotate(SKY, angle_sky, glm::vec3(0.3f, 0.8f, 0.5f));
	objects.push_back(SKY);
	models.push_back(new Model(filenames[filenames.size() - 1], objects[objects.size() - 1]));
	
	// Rysowanie modeli
	for (int i = 0; i < models.size(); i++) {
		models.at(i)->draw(glm::value_ptr(V), glm::value_ptr(P));
	}	

	glfwSwapBuffers(window);
}

int main(void) {
	GLFWwindow* window;

	if (!glfwInit()) {
		fprintf(stderr, "Blad GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(width, height, "Medieval Castle", NULL, NULL);

	if (!window) {
		fprintf(stderr, "Blad tworzenia okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Blad GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window);

	float angle_y{ 0 };
	float angle_x{ 0 };
	float angle_sky{ 0 };
	glfwSetTime(0); //Wyzeruj licznik czasu
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle_y += speed_y * glfwGetTime();
		angle_x += speed_x * glfwGetTime();
		angle_sky = sky_speed * glfwGetTime();
		// ograniczanie dla latania góra / dół
		if ((pos.y > 10) && (pos.y < 250))
		{
			pos += (float)(w_speed * glfwGetTime()) * ChangeDirection(angle_y, angle_x);
		}
		else
		{
			pos += glm::vec3(1, 0, 1) * (float)(w_speed * glfwGetTime()) * ChangeDirection(angle_y, angle_x);
			if (pos.y < 10)
			{
				pos.y = 11;
			}
			else
			{
				pos.y = 249;
			}
		}
		glfwSetTime(0);
		drawScene(window, angle_y, angle_x, angle_sky);
		glfwPollEvents();
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(0);
}