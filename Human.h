#pragma once
#include <string>
#include "Brain.h"
#include "Evolution.h"
#include "Eye.h"
#include "Ear.h"
#include "Skeleton.h"

namespace Human {

    class Human {
        std::vector<std::pair<std::vector<double>, std::string>> * Senses;
        Eye::Eye Eyes[2];
        Ear::Ear Ears[2];
        Skeleton::Skeleton skeleton;

        Brain::Brain<double> brain;


        static double AFunct(double x) {
            if (x < 0) {
                return .01 * x;
            }
            else if (x >= 0) {
                return x;
            }
        }

        std::vector<std::pair<std::vector<double>, std::vector<double>>> memory;

        double bladder=0;
        double Excrement=0;
        double Food=100;
        double Water=100;
        double Tempature=97.5;
        bool Pregnant = false;
        bool SexIsMale = false;
        

    public:
        Human() {};

        Human(int memorySize, int EyeResolution, double Eye_Radius_FOV, double Eye_Focus, int AudioSamplesPerStep, Skeleton::Skeleton skel, std::vector<std::pair<std::vector<double>, std::string>>* senses) {
            Eyes[0].SetResolution(EyeResolution);
            Eyes[0].SetFocus(Eye_Focus);
            Eyes[0].SetFov(Eye_Radius_FOV);
            Eyes[1].SetResolution(EyeResolution);
            Eyes[1].SetFocus(Eye_Focus);
            Eyes[1].SetFov(Eye_Radius_FOV);
            skeleton = skel;

            memory.resize(memorySize);
            brain.SetActFunct(AFunct);
            brain.SetWeightSizes(900 * 3, { 900,300,150,75,3 });
            std::vector<double> bias = { 1,1,1,1,1 };
            brain.SetBias(bias);
            brain.Randomize();
        }

        std::vector<double> getInputs() {
            std::vector<double> ret;
            for (int j = 0; j < memory.size(); j++)
            {
                for (int i = 0; i < memory[j].first.size(); i++)
                {
                    ret.push_back(memory[j].first[i]);
                }
                for (int i = 0; i < memory[j].second.size(); i++)
                {
                    ret.push_back(memory[j].second[i]);
                }
            }
            std::vector<double> tmp = Eyes[0].GetData();
            for (int i = 0; i < tmp.size(); i++)
            {
                ret.push_back(tmp[i]);
            }
            tmp = Eyes[1].GetData();
            for (int i = 0; i < tmp.size(); i++)
            {
                ret.push_back(tmp[i]);
            }
            tmp = skeleton.GetData();
            for (int i = 0; i < tmp.size(); i++)
            {
                ret.push_back(tmp[i]);
            }
            for (int i = 0; i < Senses->size(); i++)
            {
                std::vector<double> tmp = (*Senses)[i].first;
                for (int j = 0; j < tmp.size(); j++)
                {
                    ret.push_back(tmp[j]);
                }
            }
        }

        void SetState(std::vector<double> outs) {
            //Eyes[0].SetRot();
            //Eyes[0].SetFocus();
            //Eyes[1].SetRot();
            //Eyes[1].SetFocus();
            //for (int i = 0; i < skeleton.Size(); i++)
            //{
            //    skeleton.GetBone(i).SetAll();
            //}
        }

        void Mutate(double stepsize) {
            brain.MutateSelf(stepsize);
        }

        Human GetMutate(double stepsize) {
            Human h = *this;
            h.Mutate(stepsize);
            return h;
        }

        void Step() {
            Food -= .001;
            Water -= .001;
            Excrement += .001;
            bladder += .001;
            SetState(brain.Process(getInputs()));
        }
    };
}