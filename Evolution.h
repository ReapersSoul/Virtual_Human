#pragma once
#include <vector>
#include <algorithm>


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

	template<typename T, typename EvalFunction, typename MutationFunction>
	class Pool {
		std::vector<std::pair<T,double>> pool;
		EvalFunction EF;
		MutationFunction MF;
		int size;
		bool PrintDebug = false;
	public:	

		Pool(int size,EvalFunction e,MutationFunction m) {
			this->size = size;
			pool.resize(size);
			EF = e;
			MF = m;
			//EvaluationFunct = [](T obj) {
			//	return obj;
			//};
			//MutationFunct = [](T obj,int i) {
			//	std::vector<T> ret;
			//	ret.push_back(obj);
			//	for (int j = 0; j < i; j++)
			//	{
			//		ret.push_back(obj += MapRange<double>(rand(), 0, RAND_MAX, -.01, 1));
			//	}
			//	return ret;
			//};
		}

		void SetPrintDebug(bool b) {
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
				if (PrintDebug) {
					for (int j = 0; j < objects.size(); j++)
					{
						printf("Object %d of Generation %d was evaluated at %3.2f!\n", j, i, EF(objects[j]));
					}
					printf("Best object of generation %d was %3.2f!\n", i, EF(BestObject));
				}
			}
			return BestObject;
		}

	};

}