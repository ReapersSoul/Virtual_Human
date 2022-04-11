#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <string>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


// Include GLM
#include <glm/glm.hpp>
using namespace glm;

//imgui
#include "IMGUI/imgui_impl_opengl3.h"
#include "IMGUI/imgui_stdlib.h"
#include "IMGUI/imgui_impl_glfw.h"


class GLWindow
{
public:


	bool Init() {
		if (!glfwInit())
			return false;

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(width, height, Name.c_str(), NULL, NULL);
		if (!window)
		{
			glfwTerminate();
		}
		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		if (FocusHandler != nullptr) {
			glfwSetWindowFocusCallback(window, FocusHandler);
		}
		if (KeyHandler != nullptr) {
			glfwSetKeyCallback(window, KeyHandler);
		}
		if (MousePosHandler != nullptr) {
			glfwSetCursorPosCallback(window, MousePosHandler);
		}
		if (MouseButtonHandler != nullptr) {
			glfwSetMouseButtonCallback(window, MouseButtonHandler);
		}


		// Initialize GLEW
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			getchar();
			glfwTerminate();
			return false;
		}

		if (DataFunct != nullptr) {
			DataThread = new std::thread(&GLWindow::RunData, this);
		}
		return true;
	}

	void Loop() {
		if (SetupFunct != nullptr) {
			SetupFunct(this);
		}

		do {
			glfwGetWindowSize(window, &width, &height);
			if (UpdateFunct != nullptr) {
				UpdateFunct(this);
			}

			if (DrawFunct != nullptr) {
				DrawFunct(this);
			}
			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
			if (fps > 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds((1000 / fps)));
			}
			if (glfwWindowShouldClose(window)) {
				ShouldRun = false;
			}
		} while (ShouldRun);
	}

	GLWindow() {}

	~GLWindow() {
		if (DataThread != nullptr) {
			DataThread->join();
		}
		glfwTerminate();
	}

	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	void SetShouldRun(bool sr) {
		ShouldRun = sr;
	}
	bool GetShouldRun() {
		return ShouldRun;
	}

	void SetDrawFunct(void (*drawFunct)(GLWindow* wind)) {
		DrawFunct = drawFunct;
	}
	void SetDataFunct(void (*dataFunct)(GLWindow* wind)) {
		DataFunct = dataFunct;
	}
	void SetUpdateFunct(void (*updateFunct)(GLWindow* wind)) {
		UpdateFunct = updateFunct;
	}
	void SetSetupFunct(void (*setupFunct)(GLWindow* wind)) {
		SetupFunct = setupFunct;
	}
	void SetFocusHandle(void (*focusHandler)(GLFWwindow* window, int focused)) {
		FocusHandler = focusHandler;
	}
	void SetKeyHandle(void (*keyHandler)(GLFWwindow* window, int key, int scancode, int action, int mods)) {
		KeyHandler = keyHandler;
	}
	void SetMousePosHandle(void (*mousePosHandler)(GLFWwindow* window, double xpos, double ypos))
	{
		MousePosHandler = mousePosHandler;
	}
	void SetMouseButtonHandle(void (*mouseButtonHandler)(GLFWwindow* window, int button, int action, int mods)) {
		MouseButtonHandler = mouseButtonHandler;
	}

	void SetFPS(int _FPS) {
		fps = _FPS;
	}
	int GetFPS() {
		return fps;
	}
	void SetDPS(int _DPS) {
		dps = _DPS;
	}
	int GetDPS() {
		return dps;
	}

	GLFWwindow* GetWindow() {
		return window;
	}



protected:

	GLFWwindow* window;
	std::string Name = "Basic Window";
	bool ShouldRun = true;
	bool IsFocused = false;
	int width = 1024;
	int height = 768;
	int fps = 60;
	int dps = 60;

	void (*FocusHandler)(GLFWwindow* window, int focused);
	void (*KeyHandler)(GLFWwindow* window, int key, int scancode, int action, int mods);
	void (*MousePosHandler)(GLFWwindow* window, double xpos, double ypos);
	void (*MouseButtonHandler)(GLFWwindow* window, int button, int action, int mods);

	std::thread* DataThread;

	void RunData() {
		do {
			if (DataFunct != nullptr) {
				DataFunct(this);
			}
			if (dps > 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds((1000 / dps)));
			}
		} while (ShouldRun);
	}

	void (*DrawFunct)(GLWindow* wind);
	void (*UpdateFunct)(GLWindow* wind);
	void (*SetupFunct)(GLWindow* wind);
	void (*DataFunct)(GLWindow* wind);

};