/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 12/05/2023
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupGeometry();
void moveCube(glm::mat4 model, float angle, GLuint shaderID, float offsetXY);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1000, HEIGHT = 1000;

// Variáveis auxiliares
bool rotateX = false, rotateY = false, rotateZ = false;
float angle = 0.0f, transX = 0.0f, transY = 0.0f, transZ = 0.0f, scale = 0.5f, offsetXY = 0.0f;
glm::mat4 model = glm::mat4(1); // matriz identidade;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 450\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"uniform mat4 model;\n"
"out vec4 finalColor;\n"
"void main()\n"
"{\n"
//...pode ter mais linhas de código aqui!
"gl_Position = model * vec4(position, 1.0);\n"
"finalColor = vec4(color, 1.0);\n"
"}\0";

// Código fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 450\n"
"in vec4 finalColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = finalColor;\n"
"}\n\0";

// Função MAIN
int main() {
	// Inicialização da GLFW
	glfwInit();

	// Muita atenção aqui: alguns ambientes não aceitam essas configurações
	// Você deve adaptar para a versão do OpenGL suportada por sua placa
	// Sugestão: comente essas linhas de código para descobrir a versão e
	// depois atualize (por exemplo: 4.5 com 4 e 5)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola 3D -- Estevan!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros de funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Gerando dois buffers simples, com a geometria de um cubo
	GLuint VAOa = setupGeometry();
	GLuint VAOb = setupGeometry();

	glUseProgram(shaderID);

	// Não há necessidade de se passar a matriz model via uniform fora do loop...
	//GLint modelLoc = glGetUniformLocation(shaderID, "model");
	//model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window)) {
		// Checa se houve eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		angle = (GLfloat)glfwGetTime();
		
		offsetXY = 0.0f;
		moveCube(model, angle, shaderID, offsetXY);
		glBindVertexArray(VAOa);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDrawArrays(GL_POINTS, 0, 36);
		glBindVertexArray(0);
	
		offsetXY = 0.75f;
		moveCube(model, angle, shaderID, offsetXY);
		glBindVertexArray(VAOb);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDrawArrays(GL_POINTS, 0, 36);
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAOa);
	glDeleteVertexArrays(1, &VAOb);
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

	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		transY += 0.1f;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		transX -= 0.1f;
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		transY -= 0.1f;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		transX += 0.1f;
	}

	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		transZ += 0.2f;
	}

	if (key == GLFW_KEY_J && action == GLFW_PRESS) {
		transZ -= 0.2f;
	}

	if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS) {
		scale -= 0.1f;
	}

	if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS) {
		scale += 0.1f;
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		rotateX = true;
		rotateY = true;
		rotateZ = true;
	}
}

// Esta função está bastante hardcoded - objetivo é compilar e "buildar" um programa de
// shader simples e único neste exemplo de código
// O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
// fragmentShader source no iniçio deste arquivo
// A função retorna o identificador do programa de shader
int setupShader() {
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
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
		// Face frontal: vermelho
		-0.5, -0.5, -0.5, 1.0, 0.0, 0.0, // Vértice 1
		-0.5,  0.5, -0.5, 1.0, 0.0, 0.0, // Vértice 2
		 0.5,  0.5, -0.5, 1.0, 0.0, 0.0, // Vértice 3
		 0.5,  0.5, -0.5, 1.0, 0.0, 0.0, // Vértice 3
		 0.5, -0.5, -0.5, 1.0, 0.0, 0.0, // Vértice 4
		-0.5, -0.5, -0.5, 1.0, 0.0, 0.0, // Vértice 1

		// Face traseira: verde
		-0.5, -0.5,  0.5, 0.0, 1.0, 0.0, // Vértice 5
		 0.5, -0.5,  0.5, 0.0, 1.0, 0.0, // Vértice 6
		 0.5,  0.5,  0.5, 0.0, 1.0, 0.0, // Vértice 7
		 0.5,  0.5,  0.5, 0.0, 1.0, 0.0, // Vértice 7
		-0.5,  0.5,  0.5, 0.0, 1.0, 0.0, // Vértice 8
		-0.5, -0.5,  0.5, 0.0, 1.0, 0.0, // Vértice 5

		// Face superior: azul
		-0.5,  0.5, -0.5, 0.0, 0.0, 1.0, // Vértice 2
		-0.5,  0.5,  0.5, 0.0, 0.0, 1.0, // Vértice 8
		 0.5,  0.5,  0.5, 0.0, 0.0, 1.0, // Vértice 7
		 0.5,  0.5,  0.5, 0.0, 0.0, 1.0, // Vértice 7
		 0.5,  0.5, -0.5, 0.0, 0.0, 1.0, // Vértice 3
		-0.5,  0.5, -0.5, 0.0, 0.0, 1.0, // Vértice 2

		// Face inferior: amarelo
		-0.5, -0.5, -0.5, 1.0, 1.0, 0.0, // Vértice 1
		 0.5, -0.5, -0.5, 1.0, 1.0, 0.0, // Vértice 4
		 0.5, -0.5,  0.5, 1.0, 1.0, 0.0, // Vértice 6
		 0.5, -0.5,  0.5, 1.0, 1.0, 0.0, // Vértice 6
		-0.5, -0.5,  0.5, 1.0, 1.0, 0.0, // Vértice 5
		-0.5, -0.5, -0.5, 1.0, 1.0, 0.0, // Vértice 1

		// Face direita: ciano
		 0.5, -0.5, -0.5, 0.0, 1.0, 1.0, // Vértice 4
		 0.5,  0.5, -0.5, 0.0, 1.0, 1.0, // Vértice 3
		 0.5,  0.5,  0.5, 0.0, 1.0, 1.0, // Vértice 7
		 0.5,  0.5,  0.5, 0.0, 1.0, 1.0, // Vértice 7
		 0.5, -0.5,  0.5, 0.0, 1.0, 1.0, // Vértice 6
		 0.5, -0.5, -0.5, 0.0, 1.0, 1.0, // Vértice 4

		 // Face esquerda: magenta
		 -0.5, -0.5, -0.5, 1.0, 0.0, 1.0, // Vértice 1
		 -0.5, -0.5,  0.5, 1.0, 0.0, 1.0, // Vértice 5
		 -0.5,  0.5,  0.5, 1.0, 0.0, 1.0, // Vértice 8
		 -0.5,  0.5,  0.5, 1.0, 0.0, 1.0, // Vértice 8
		 -0.5,  0.5, -0.5, 1.0, 0.0, 1.0, // Vértice 2
		 -0.5, -0.5, -0.5, 1.0, 0.0, 1.0, // Vértice 1
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

void moveCube(glm::mat4 model, float angle, GLuint shaderID, float offsetXY) {
	model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(transX - offsetXY, transY - offsetXY, transZ));

	if (rotateX && rotateY && rotateZ) {
		model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 1.0f));
	}
	else if (rotateX) {
		model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (rotateY) {
		model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (rotateZ) {
		model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	model = glm::scale(model, glm::vec3(scale, scale, scale));

	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));
}