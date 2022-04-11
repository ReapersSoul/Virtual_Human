#pragma once

//network
#include "WINNETLIB.h"

// Include GLEW
#include <GL/glew.h>

//imageloader
#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

//glu
#include <GL/GLU.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

//filesystem
#include <filesystem>

namespace fs = std::filesystem;

//glwindow
#include "GLWINDOW.h"

//genral includes
#include <iostream>
#include <random>
#include <math.h>
#include <thread>

#include "Virtual_Human.h"
#include <chrono>
namespace chrono= std::chrono;

//globals
ImGuiContext* ImGui_Context;
Virtual_Human::Physics phys;
bool wait = false;
//end gloabals


void DataFunct(GLWindow* wind) {
    phys.Step(.01);
}

void SetupFunct(GLWindow* wind) {
    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}



void KeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {

    switch (key)
    {
    case GLFW_KEY_T:
        break;
    case GLFW_KEY_UP:
        break;
    case GLFW_KEY_DOWN:
        break;
    default:
        break;
    }
}

void UpdateFunct(GLWindow* wind) {
    ImGui::SetCurrentContext(ImGui_Context);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glLoadIdentity();
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


int ii = 0;
int Cap = 2;
int sides = 200;
double radius = 1.5;
double speed = 0;
double fps = 60;
double z = 1;

Virtual_Human::Object obj;
Virtual_Human::OctTree tree;
Virtual_Human::Cube bound(0,0,0,1,1,1);
Virtual_Human::Cube search(0,0,0,1,1,1);
int polycount = 10;
double hue = 0;
double hueSet = 0;

void DrawFunct(GLWindow* wind) {
    
    auto start = chrono::high_resolution_clock::now();
    ImGui::SetCurrentContext(ImGui_Context);

    if(ImGui::Begin("RenderOptions")){
        ImGui::InputInt("Sides",&obj.polysides, 1, 5);
        if (ImGui::Button("Reset")) {
            wait = true;
            tree.Clear();
            tree = Virtual_Human::OctTree(bound,Cap);
            if (polycount != obj.PhysicalPoints.size()) {
                obj.PhysicalPoints.resize(polycount);
            }
            //generate volumetric cube
            for (int i = 0; i < obj.PhysicalPoints.size(); i++) {
                Virtual_Human::Point p(MapRange<double>(rand(), 0, RAND_MAX, -.9, .9), MapRange<double>(rand(), 0, RAND_MAX, -.9, .9), MapRange<double>(rand(), 0, RAND_MAX, -.9, .9));
                Virtual_Human::DisplayPoint pp = Virtual_Human::DisplayPoint(p, Virtual_Human::Point(0,1,0));
                pp.Velocity.RandomizeInRange(-.001, .001);
                obj.PhysicalPoints[i] = pp;
                tree.insert(&obj.PhysicalPoints[i]);
            }
            phys.Objects.clear();
            phys.Objects.push_back(&obj);
            wait = false;
        }
        ImGui::InputInt("Count",&polycount, 100, 500);
        ImGui::InputDouble("Radius",&obj.polyradius,.01,.5);
        ImGui::InputDouble("speed",&speed,.01,.5);
        ImGui::InputDouble("camdist",&radius,.01,.5);
        ImGui::InputDouble("camheight",&z,.01,.5);
        ImGui::InputInt("capacity",&Cap,1,5);
        ImGui::InputDouble("hue speed",&hueSet,.01,1);
    }
    ImGui::End();

    glScalef(.25, .25, .25);
    gluPerspective(radians(170.00), wind->getHeight() / wind->getWidth(), .01, 100000000000.00);
    float theta = 2.0f * 3.1415926f * float((ii*speed)) / float(sides);//get the current angle

    float x = radius * cosf(theta);//calculate the x component
    float y = radius * sinf(theta);//calculate the y component
    gluLookAt(x, y, z, 0, 0, -1, 0, 0, 1);
    ii++;
    
    obj.draw();
    glColor3d(0, 1, 0);
    phys.wait = true;
    phys.Draw();
    glColor3d(1, 0, 0);
    search.Draw();
    std::vector<Virtual_Human::Point *> tmp = tree.query(search);
    for (int i = 0; i < tmp.size(); i++)
    {
        ((Virtual_Human::DisplayPoint*)tmp[i])->color = Virtual_Human::HSVtoRGB(hue,100,100);
    }
    hue += hueSet;
    if (hue > 360) {
        hue = 0;
    }
    phys.wait = false;
    glScalef(4, 4, 4);
    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
    int days = duration.count() / 86400000;
    int hours = (duration.count() - (days * 86400000)) / 3600000;
    int minutes = (duration.count() - (hours * 3600000)) / 60000;
    int seconds = (duration.count() - (minutes * 60000)) / 1000;
    int miliseconds = (duration.count() - (seconds * 1000));

    //limit frames
    if (miliseconds < 1000/fps) {
        Sleep((1000 / fps)-duration.count());
    }
    //system("cls");
    //printf("Draw took %d:%d:%d:%d:%d (D:H:M:S:MS) to complete.\n", days, hours, minutes, seconds, miliseconds);
    //printf("fps %3.4f\n", 1/(((double)duration.count() / 1000.00)));
}

int main()
{
    tree = Virtual_Human::OctTree(bound, Cap);
    phys.boundry = search;
    srand(time(0));
    // Initialise all DevIL functionality
    ilutRenderer(ILUT_OPENGL);
    ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);    // Tell DevIL that we're using OpenGL for our rendering

    //init glwindow
    GLWindow wind;
    wind.SetDPS(0);
    wind.SetFPS(0);
    wind.SetKeyHandle(&KeyHandler);
    wind.SetSetupFunct(&SetupFunct);
    wind.SetUpdateFunct(&UpdateFunct);
    wind.SetDrawFunct(&DrawFunct);
    wind.SetDataFunct(&DataFunct);
    wind.Init();


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui_Context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(wind.GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();


    wind.Loop();


    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
