#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "LightDiretional.h"
#include "Mesh.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
unsigned int LoadImageToGPU(const char* fileName, GLint internalFormat, GLenum format, int textureIndex);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

#pragma region Model Data
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

float vertices[] = {
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};
#pragma endregion

#pragma region Camera Declare
//��ʼ�������
	//Camera myCamera(glm::vec3(0, 0, 3.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1.0f, 0));
Camera myCamera(glm::vec3(0, 0, 3.0f), glm::radians(0.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
#pragma endregion

#pragma region Light Declare
LightDiretional LightDir(
	glm::vec3(1.0f, 1.0f, 2.0f),
	glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0)
);
#pragma endregion

#pragma region Input Declare
float lastX, lastY;
bool firstMouse = true;
#pragma endregion

int main(int argc, char* argv[])
{
	std::string exePath = argv[0];
	//std::cout << exePath.substr(0, exePath.find_last_of('\\')) + "\\model\\nanosuit.obj";
#pragma region Open a window
	//Glfw��ʼ��
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	//glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#pragma endregion

#pragma region Init shader Program
	Shader* testShader = new Shader("vertexSource.vert", "fragmentSource.frag");
	Shader* skyboxShader = new Shader("skybox.vert", "skybox.frag");
#pragma endregion

#pragma region Init Material
	Material* myMaterial = new Material(testShader,
		glm::vec3(1.0f, 0.5f, 0.3f),
		glm::vec3(1.0f, 0.5f, 0.31f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f);
#pragma endregion


	//Mesh cube(vertices);
	//Model model(exePath.substr(0, exePath.find_last_of('\\')) + "\\model\\nanosuit.obj");
#pragma region Init and Load Models to VAO, VBO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//��ʼ�����루ֻ����һ�� (�����������Ƶ���ı�))
	// 1. ��VAO
	glBindVertexArray(VAO);
	// 2. �����ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 4. �趨��������ָ��
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ��ɫ����
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/
	//������������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//��������
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
#pragma endregion

#pragma region Init and Load Textures
	//�ڼ���ͼƬǰ����ͼƬ��ת����
	//stbi_set_flip_vertically_on_load(true);
	//��������
	unsigned int texture1, texture2;
	// ���ز���������
	texture1 = LoadImageToGPU("container2.png", GL_RGBA, GL_RGBA, 0);
	texture2 = LoadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, 1);

	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#pragma endregion

#pragma region Prepare MVP matrices
	//�ռ�任����
	glm::mat4 modelMat;
	//modelMat = glm::rotate(modelMat, glm::radians(-1.0f), glm::vec3(1.0f, 1.0, 0.5f));
	glm::mat4 viewMat;
	//viewMat = glm::translate(viewMat, glm::vec3(0, 0, -3.0f));
	//viewMat = myCamera.GetViewMatrix();
	glm::mat4 projMat;
	projMat = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
#pragma endregion


#pragma region 
	while (!glfwWindowShouldClose(window))
	{
		// ����
		processInput(window);

		// ����
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//�������������
		viewMat = myCamera.GetViewMatrix();

		for (unsigned int i = 0; i < 10; i++)
		{
			//Set Model matrix
			modelMat = glm::translate(glm::mat4(1.0f), cubePositions[i]);

			//Set View and Projection Matrices here if you want
			//

			//Set Material -> Shader Program
			testShader->use();
			//���������λ��
			testShader->setVec3("viewPos", myCamera.Position);
			//Set Material -> Texture
			glBindTexture(GL_TEXTURE_2D, texture1);
			glBindTexture(GL_TEXTURE_2D, texture2);
			//Set Material -> Uniforms
			//��������
			//glUniform1i(glGetUniformLocation(testShader->ID, "ourTexture"), 0);
			//glUniform1i(glGetUniformLocation(testShader->ID, "ourTexture2"), 1);
			//���ÿռ����
			testShader->setMat4("modelMat", modelMat);
			testShader->setMat4("viewMat", viewMat);
			testShader->setMat4("projMat", projMat);

			//����������ɫ
			testShader->setVec3("objectColor", 1.0f, 1.0f, 1.0f);
			//���ù���
			testShader->setVec3("lightDir", LightDir.direction.x, LightDir.direction.y, LightDir.direction.z);
			testShader->setVec3("light.position", 2.0f, 5.0f, 5.0f);
			testShader->setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
			testShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
			testShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);


			myMaterial->shader->setVec3("material.ambient", myMaterial->ambient);
			myMaterial->shader->setVec3("material.diffuse", myMaterial->diffuse);
			myMaterial->shader->setVec3("material.specular", myMaterial->specular);
			myMaterial->shader->setFloat("material.shininess", myMaterial->shininess);

			//Set Model
			glBindVertexArray(VAO);
			//Draw Call
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//cube.Draw(myMaterial->shader);
			//model.Draw(myMaterial->shader);
		}

		// ��鲢�����¼����������壬Ϊ��һ����׼��
		glfwSwapBuffers(window);
		glfwPollEvents();
		myCamera.UpdateCameraPos();
	}
	//���VAO,VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteVertexArrays(1, &skyboxVAO);
	//glDeleteBuffers(1, &skyboxVBO);

	//�˳�����
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	float speed = 0.1f;//�����ٶ�
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//�Ƿ����˷��ؼ�
		glfwSetWindowShouldClose(window, true);
	//ǰ���ƶ�
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		myCamera.speedZ = speed;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		myCamera.speedZ = -speed;
	else
		myCamera.speedZ = 0;
	//�����ƶ�
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		myCamera.speedX = -speed;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		myCamera.speedX = speed;
	else
		myCamera.speedX = 0;
	//�����ƶ�
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		myCamera.speedY = speed;
	else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		myCamera.speedY = -speed;
	else
		myCamera.speedY = 0;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse == true)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	float deltaX, deltaY;
	deltaX = xPos - lastX;
	deltaY = yPos - lastY;

	lastX = xPos;
	lastY = yPos;

	myCamera.ProcessMouseMovement(deltaX, deltaY);

	//std::cout << deltaX << "	" << deltaY << std::endl;
}

unsigned int LoadImageToGPU(const char* fileName, GLint internalFormat, GLenum format, int textureIndex)
{
	unsigned int texBuffer;
	glGenTextures(1, &texBuffer);
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, texBuffer);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texBuffer;
}
