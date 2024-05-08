#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>

#include "Shader.h"
#include "Mesh.h"

using namespace std;

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipo da função de callback do mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Protótipos das funções
int setupGeometry();
int generateCircle(float radius, int nPoints);
int setupSprite();
int loadTexture(string path);
int loadSimpleObj(string filepath, int &nVerts, glm::vec3 color);
vector<float> loadMtl(string filepath, GLuint &texID);
void controlMesh(Mesh mesh);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Variáveis auxiliares
bool rotateX = false, rotateY = false, rotateZ = false;
bool firstMouse = true;
float angle = 0.0f;
float transX = 0.0f, transY = 0.0f, transZ = 0.0f, scale = 1.0f;
float sensitivity = 0.05f, pitch = 0.0f, yaw = -90.0f;
const float pi = 3.1419f;
double lastX, lastY;
int nVerts;
int selection;
GLuint texID1, texID2, texID3;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 6.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 model = glm::mat4(1);
glm::mat4 view = glm::mat4(1);
glm::mat4 projection = glm::mat4(1);

int main() {
	// Inicialização da GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola 3D -- Estevan!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	// Desabilita o desenho do cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD: carrega todos os ponteiros de funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); 
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	Shader shader = Shader("vertex.vs", "fragment.fs");

	// Ativando o shader
	glUseProgram(shader.ID);

	// Matriz de view - posição e orientação da câmera
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("view", value_ptr(view));

	// Matriz de projeção perspectiva - definindo o volume de visualização (frustum)
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	shader.setMat4("projection", glm::value_ptr(projection));

	// Carregando um objeto e armazenando o identificador na memória
	GLuint VAO1 = loadSimpleObj("../../3D_Models/Suzanne/SuzanneTriTextured.obj", nVerts, glm::vec3(0.0f, 0.0f, 0.0f));
	GLuint VAO2 = loadSimpleObj("../../3D_Models/Planetas/planeta.obj", nVerts, glm::vec3(0.0f, 0.0f, 0.0f));
	GLuint VAO3 = loadSimpleObj("../../3D_Models/Novos/cienciaDaComputacao.obj", nVerts, glm::vec3(0.0f, 0.0f, 0.0f));
	
	// Lendo o arquivo mtl (lendo os coeficientes e a textura)
	vector<float> coefficients1 = loadMtl("../../3D_Models/Suzanne/SuzanneTriTextured.mtl", texID1);
	vector<float> coefficients2 = loadMtl("../../3D_Models/Planetas/planeta.mtl", texID2);
	vector<float> coefficients3 = loadMtl("../../3D_Models/Novos/cienciaDaComputacao.mtl", texID3);

	// Criando e inicializando os meshs
	Mesh suzanne, planeta, logo;
	suzanne.initialize(VAO1, nVerts, texID2, coefficients1, &shader, glm::vec3(-1.5, 0.0, 0.0));
	planeta.initialize(VAO2, nVerts, texID3, coefficients2, &shader, glm::vec3(1.5, 0.0, 0.0));
	logo.initialize(VAO3, nVerts, texID1, coefficients3, &shader, glm::vec3(0.0, 3.0, -5.0), glm::vec3(1.0, 1.0, 1.0));

	// Definindo a fonte de luz pontual
	shader.setVec3("lightPos", -50.0, 10.0, 20.0);
	shader.setVec3("lightColor", 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		// Checa se houve eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		// Atualizando a posição e orientação da câmera
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader.setMat4("view", glm::value_ptr(view));

		// Atualizando o shader com a posição da câmera
		shader.setVec3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
		
		// Chamada de desenho - drawcall
		suzanne.update();
		suzanne.draw();

		planeta.update();
		planeta.draw();

		logo.update();
		logo.draw();
		
		// Controle dos objetos, porém a parcela especular "rotaciona" junto com o
		// objeto, o que não deveria acontecer já que a fonte de luz é estática...
		//angle = (float)glfwGetTime();
		//if (selection == 1) {
		//	controlMesh(suzanne);
		//	suzanne.draw();

		//	planeta.angle = (float)glfwGetTime() * 10;
		//	planeta.axis = glm::vec3(0.0f, 1.0f, 0.0f);
		//	planeta.update();
		//	planeta.draw();

		//	logo.update();
		//	logo.draw();
		//}
		//else if (selection == 2) {
		//	suzanne.update();
		//	suzanne.draw();

		//	controlMesh(planeta);
		//	planeta.draw();

		//	logo.update();
		//	logo.draw();
		//}
		//else if (selection == 3) {
		//	suzanne.update();
		//	suzanne.draw();

		//	planeta.angle = (float)glfwGetTime() * 10;
		//	planeta.axis = glm::vec3(0.0f, 1.0f, 0.0f);
		//	planeta.update();
		//	planeta.draw();

		//	controlMesh(logo);
		//	logo.draw();
		//}
		//else {
		//	suzanne.update();
		//	suzanne.draw();

		//	planeta.angle = (float)glfwGetTime() * 10;
		//	planeta.axis = glm::vec3(0.0f, 1.0f, 0.0f);
		//	planeta.update();
		//	planeta.draw();

		//	logo.update();
		//	logo.draw();
		//}

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteVertexArrays(1, &VAO3);

	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		selection = 0;
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		selection = 1;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		selection = 2;
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		selection = 3;
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		rotateX = true;
		rotateY = true;
		rotateZ = true;
	}

	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		rotateX = true;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}

	float cameraSpeed = 0.10f;

	if (key == GLFW_KEY_W) {
		cameraPos += cameraFront * cameraSpeed;
	}
	
	if (key == GLFW_KEY_S) {
		cameraPos -= cameraFront * cameraSpeed;
	}

	if (key == GLFW_KEY_A) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (key == GLFW_KEY_D) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (key == GLFW_KEY_SPACE) {
		cameraPos += cameraUp * cameraSpeed;
	}

	if (key == GLFW_KEY_LEFT_CONTROL) {
		cameraPos -= cameraUp * cameraSpeed;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	//cout << xpos << " " << ypos << endl;
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double offsetx = xpos - lastX;
	double offsety = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	offsetx *= sensitivity;
	offsety *= sensitivity;

	pitch += offsety;
	yaw += offsetx;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um cubo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry() {
	// Aqui setamos as coordenadas x, y e z do cubo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {
		//Base da pirâmide: 2 triângulos
		//x     y     z    r    g    b
		-0.5, -0.5, -0.5, 1.0, 1.0, 0.0,
		-0.5, -0.5,  0.5, 0.0, 1.0, 1.0,
		 0.5, -0.5, -0.5, 1.0, 0.0, 1.0,

		-0.5, -0.5,  0.5, 1.0, 1.0, 0.0,
		 0.5, -0.5,  0.5, 0.0, 1.0, 1.0,
		 0.5, -0.5, -0.5, 1.0, 0.0, 1.0,
		
		//
		-0.5, -0.5, -0.5, 1.0, 1.0, 0.0,
		 0.0,  0.5,  0.0, 1.0, 1.0, 0.0,
		 0.5, -0.5, -0.5, 1.0, 1.0, 0.0,

		-0.5, -0.5, -0.5, 1.0, 0.0, 1.0,
		 0.0,  0.5,  0.0, 1.0, 0.0, 1.0,
		-0.5, -0.5,  0.5, 1.0, 0.0, 1.0,

	    -0.5, -0.5,  0.5, 1.0, 1.0, 0.0,
		 0.0,  0.5,  0.0, 1.0, 1.0, 0.0,
		 0.5, -0.5,  0.5, 1.0, 1.0, 0.0,

		 0.5, -0.5,  0.5, 0.0, 1.0, 1.0,
		 0.0,  0.5,  0.0, 0.0, 1.0, 1.0,
		 0.5, -0.5, -0.5, 0.0, 1.0, 1.0,

		// Chão
		//x     y     z    r    g    b
		-5.0, -0.5, -5.0, 0.5, 0.5, 0.5,
		-5.0, -0.5,  5.0, 0.5, 0.5, 0.5,
		 5.0, -0.5, -5.0, 0.5, 0.5, 0.5,

		-5.0, -0.5,  5.0, 0.5, 0.5, 0.5,
		 5.0, -0.5,  5.0, 0.5, 0.5, 0.5,
		 5.0, -0.5, -5.0, 0.5, 0.5, 0.5
	};

	GLuint VBO, VAO;

	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	// Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	// Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

int generateCircle(float radius, int nPoints) {
	int totalSize = (nPoints + 2) * 3;
	GLfloat* vertices = new GLfloat[totalSize];

	// Primeiro colocamos o ponto central do círculo (x, y, z)
	vertices[0] = 0.0; // x
	vertices[1] = 0.0; // y
	vertices[2] = 0.0; // z

	float angle = 0.0;
	float slice = 2 * pi / (GLfloat)nPoints;
	
	for (int i = 3; i < totalSize; i += 3) {
		float x = radius * cos(angle) * 100;
		float y = radius * sin(angle) * 100;
		float z = 0.0;

		vertices[i] = x;
		vertices[i + 1] = y;
		vertices[i + 2] = z;

		angle += slice;
	}

	GLuint VBO, VAO;

	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	// Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

int setupSprite() {
	GLuint VAO;
	GLuint VBO, EBO;

	float vertices[] = {
		 // posicoes        // cores          // coordenadas de textura
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // superior direito
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // inferior direito
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // inferior esquerdo
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // superior esquerdo
	};

	unsigned int indices[] = {
		0, 1, 3, // primeiro triangulo
		1, 2, 3  // segundo triangulo
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Primeiro atributo - Layout 0 - posição - 3 valores - x, y, z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Segundo atributo - Layout 1 - cor - 3 valores - r, g, b
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Terceiro atributo - Layout 2 - coordenadas de textura - 2 valores - s, t (ou u, v)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // desvincula

	return VAO;
}

int loadTexture(string path) {
	GLuint texID;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Ajusta os parâmetros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data) {
		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

int loadSimpleObj(string filepath, int &nVerts, glm::vec3 color) {
	vector <glm::vec3> vertices;
	vector <GLuint> indices;
	vector <glm::vec2> texCoords;
	vector <glm::vec3> normals;
	vector <GLfloat> vbuffer;

	ifstream inputFile;
	inputFile.open(filepath.c_str());

	if (inputFile.is_open()) {
		char line[100];
		string sline;

		while (!inputFile.eof()) {
			inputFile.getline(line, 100);
			sline = line;
			string word;
			istringstream ssline(line);
			ssline >> word;
			//cout << word << " ";

			if (word == "v") {
				glm::vec3 v;
				ssline >> v.x >> v.y >> v.z;
				vertices.push_back(v);
			}
			if (word == "vt") {
				glm::vec2 vt;
				ssline >> vt.s >> vt.t;
				texCoords.push_back(vt);
			}
			if (word == "vn") {
				glm::vec3 vn;
				ssline >> vn.x >> vn.y >> vn.z;
				normals.push_back(vn);
			}
			if (word == "f") {
				string tokens[3];
				ssline >> tokens[0] >> tokens[1] >> tokens[2];

				for (int i = 0; i < 3; i++) {
					// Recuperando os indices de v
					int pos = tokens[i].find("/");
					string token = tokens[i].substr(0, pos);
					int index = atoi(token.c_str()) - 1;
					indices.push_back(index);

					vbuffer.push_back(vertices[index].x);
					vbuffer.push_back(vertices[index].y);
					vbuffer.push_back(vertices[index].z);
					vbuffer.push_back(color.r);
					vbuffer.push_back(color.g);
					vbuffer.push_back(color.b);

					// Recuperando os indices de vts
					tokens[i] = tokens[i].substr(pos + 1);
					pos = tokens[i].find("/");
					token = tokens[i].substr(0, pos);
					index = atoi(token.c_str()) - 1;
					vbuffer.push_back(texCoords[index].s);
					vbuffer.push_back(texCoords[index].t);
					
					// Recuperando os indices de vns
					tokens[i] = tokens[i].substr(pos + 1);
					index = atoi(tokens[i].c_str()) - 1;
					vbuffer.push_back(normals[index].x);
					vbuffer.push_back(normals[index].y);
					vbuffer.push_back(normals[index].z);
				}
			}
		}
	} else {
		cout << "Problema ao encontrar o arquivo " << filepath << endl;
	}

	inputFile.close();
	GLuint VBO, VAO;
	nVerts = vbuffer.size() / 11; // 3 pos + 3 cor + 3 normal + 2 texcoord

	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, vbuffer.size() * sizeof(GLfloat), vbuffer.data(), GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	// Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	// Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Atributo coordenada de textura (s, t)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Atributo normal do vértice (x, y, z)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);
	return VAO;
}

vector<float> loadMtl(string filepath, GLuint &texID) {
	vector <GLfloat> coefficients;

	ifstream inputFile;
	inputFile.open(filepath.c_str());
	
	if (inputFile.is_open()) {
		char line[100];
		string sline;

		while (!inputFile.eof()) {
			inputFile.getline(line, 100);
			sline = line;
			string word;
			istringstream ssline(line);
			ssline >> word;
			
			if (word == "Ns") {
				float value;
				ssline >> value;
				coefficients.push_back(value);
			}

			if (word == "Ka" || word == "Ks" || word == "Kd") {
				float value1;
				//float value2, value3;
				ssline >> value1;
				//ssline >> value2 >> value3;
				coefficients.push_back(value1);
				//coefficients.push_back(value2);
				//coefficients.push_back(value3);
			}

			if (word == "map_Kd") {
				string path;
				ssline >> path;
				texID = loadTexture(path);
			}
		}
	}
	else {
		cout << "Não foi possível ler o arquivo mtl..." << endl;
	}
	
	return coefficients;
}

void controlMesh(Mesh mesh) {
	if (rotateX && rotateY && rotateZ) {
		mesh.angle = angle * 25;
		mesh.axis = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	else if (rotateX) {
		mesh.angle = angle * 25;
		mesh.axis = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (rotateY) {
		mesh.angle = angle * 25;
		mesh.axis = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else if (rotateZ) {
		mesh.angle = angle * 25;
		mesh.axis = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	mesh.update();
}