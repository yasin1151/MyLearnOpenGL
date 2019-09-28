#pragma once
#define STD_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "defines.h"
#include "Shader/CMyShader.hpp"
#include "stb_image.h"
#include <iostream>
using namespace std;


float g_MixNum = 0.0f;

void TextureTestProcessInput(GLFWwindow* pWindow)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(pWindow, true);
	}
	else if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS)
	{
		g_MixNum = min(1.0f, g_MixNum + 0.01f);
	}
	else if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		g_MixNum = max(0, g_MixNum - 0.01f);
	}
}


UINT LoadTexture(const string& szPath)
{
	UINT texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(szPath.c_str(), &width, &height, &nrChannels, 0);
	cout << width << "," << height << "," << nrChannels << endl;
	if (data)
	{
		UINT nImgType = GL_RGB;
		if (-1 != szPath.find(".png"))
		{
			nImgType = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, nImgType, width, height, 0, nImgType, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else
	{
		cout << "Failed to load texture" << endl;
	}

	stbi_image_free(data);

	return texture;
}

void TextureTestTestLoop(GLFWwindow* pWindow)
{
	CMyShader oShader("Res/ShaderCode/test.vs", "Res/ShaderCode/test.fs");



	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // 左上
	};

	unsigned short indices[] = {
		0, 1, 3,
		1, 2, 3
	};


	UINT texture1 = LoadTexture("Res/textures/container.jpg");
	UINT texture2 = LoadTexture("Res/textures/awesomeface.png");

	UINT VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	oShader.UseShader();
	oShader.SetInt("texture1", 0);
	oShader.SetInt("texture2", 1);

	while (!glfwWindowShouldClose(pWindow))
	{

		TextureTestProcessInput(pWindow);
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		oShader.UseShader();
		oShader.SetFloat("mixNum", g_MixNum);
		cout << "Mix Num :" << g_MixNum << endl;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}
}