#pragma once
#include <vector>
#include <functional>
#include <algorithm>
#include "ColoredText.h"

namespace Evolution {

	double SummedError(std::vector<double> Returned, std::vector<double> Expected) {
		double ret = 0;
		for (int i = 0; i < Returned.size(); i++)
		{
			ret += abs(Expected[i] - Returned[i]);
		}
		return ret;
	}

	template<typename T>
	T MapRange(T val, T inMin, T inMax, T outMin, T outMax) {
		double slope = 1.0 * (outMax - outMin) / (inMax - inMin);
		return outMin + slope * (val - inMin);
	}

	template<typename T>
	class Pool {
		std::vector<std::pair<T,double>> pool;
		std::function<double(T)> EF;
		std::function<std::vector<T>(T,int)> MF;
		int size;
		
	public:	
		static enum DebugStyles
		{
			ClearAfterGeneration,
			Normal,
			None
		};
	private:
		DebugStyles PrintDebug = Normal;
	public:

		Pool(int size, std::function<double(T)> e, std::function<std::vector<T>(T, int)> m) {
			this->size = size;
			pool.resize(size);
			EF = e;
			MF = m;
		}

		void SetPrintDebug(DebugStyles b) {
			PrintDebug = b;
		}

		T Evolve(int generations,T obj) {
			T BestObject=obj;
			for (int i = 0; i <generations; i++)
			{
				std::vector<T> objects = MF(BestObject, size);
				std::sort(objects.begin(), objects.end(), [this](T obj1, T obj2) {
					return EF(obj1) < EF(obj2);
					});
				BestObject = objects[0];
				if (PrintDebug==Normal|| PrintDebug == ClearAfterGeneration) {
					for (int j = 0; j < objects.size(); j++)
					{
						printfc("%4CObject %d of %2CGeneration %d was evaluated at %3C%3.2f!\n",true, j, i, EF(objects[j]));
					}
					printfc("%10CBest object of %2Cgeneration %d was %3C%3.2f!\n",true, i, EF(BestObject));
				}
				if (PrintDebug == ClearAfterGeneration) {
					HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
					COORD coordScreen;
					coordScreen.X = 0;
					coordScreen.Y = 0;
					SetConsoleCursorPosition(hConsole, coordScreen);
				}
			}
			return BestObject;
		}

	};

}