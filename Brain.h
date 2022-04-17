#pragma once
#include <functional>
#include <vector>
#include <fstream>
#include "json.hpp"

namespace Brain {

	template<typename T>
	T MapRange(T val, T inMin, T inMax, T outMin, T outMax) {
		double slope = 1.0 * (outMax - outMin) / (inMax - inMin);
		return outMin + slope * (val - inMin);
	}

	template<typename T>
	class Brain {
		typedef std::vector<T> Layer;
		typedef std::vector<Layer> WeightLayer;
		std::vector<WeightLayer> weights;
		Layer Bias;
		std::function<double(double)> ActFunct;

		T dot(Layer one, Layer two) {
			T ret = NULL;
			for (int i = 0; i < one.size(); i++)
			{
				ret += one[i] * two[i];
			}
			return ret;
		}

		Layer NextLayer(Layer first, WeightLayer weights,int layer) {
			Layer ret;
			//if (threaded) {
			//	std::vector()


			//}
			//else {
				for (int i = 0; i < weights.size(); i++)
				{
					ret.push_back(ActFunct(dot(first, weights[i])) + Bias[layer]);
				}
			//}
			return ret;
		}
	public:
		void Randomize() {
			for (int i = 0; i < weights.size(); i++)
			{
				for (int j = 0; j < weights[i].size(); j++)
				{
					for (int k = 0; k < weights[i][j].size(); k++)
					{
						weights[i][j][k] = MapRange<T>(rand(), 0, RAND_MAX, -1.0, 1.0);
					}
				}
			}
			for (int i = 0; i < Bias.size(); i++)
			{
				Bias[i] = MapRange<T>(rand(), 0, RAND_MAX, -1.0, 1.0);
			}
		}

		Brain() {
			Randomize();
		}

		void MutateSelf(double stepsize) {
			for (int i = 0; i < weights.size(); i++)
			{
				for (int j = 0; j < weights[i].size(); j++)
				{
					for (int k = 0; k < weights[i][j].size(); k++)
					{
						weights[i][j][k] += MapRange<T>(rand(), 0, RAND_MAX, -stepsize, stepsize);
					}
				}
			}
			for (int i = 0; i < Bias.size(); i++)
			{
				Bias[i] += MapRange<T>(rand(), 0, RAND_MAX, -stepsize, stepsize);
			}
		}

		Brain Mutate(double stepsize) {
			Brain ret;
			ret.SetActFunct(ActFunct);
			ret.SetWeights(this->GetWeights());
			ret.SetBias(this->GetBias());
			ret.MutateSelf(stepsize);
			return ret;
		}

		void SetActFunct(std::function<double(double)> actfunct) {
			ActFunct = actfunct;
			return;
		}

		void SetWeightSizes(int inputs, std::vector<int> sizes) {
			for (int i = 0; i < sizes.size(); i++)
			{
				WeightLayer tmp;
				tmp.resize(sizes[i]);
				for (int j = 0; j < tmp.size(); j++)
				{
					if (i == 0) {
						tmp[j].resize(inputs);
					}
					else {
						tmp[j].resize(sizes[i - 1]);
					}
				}
				weights.push_back(tmp);
			}
		}
		void SetWeights(std::vector<WeightLayer> weights) {
			this->weights = weights;
		}
		std::vector<WeightLayer> GetWeights() {
			return weights;
		}

		void SetBias(Layer bias) {
			Bias = bias;
		}
		Layer GetBias() {
			return Bias;
		}

		Layer Process(Layer inputs) {
			Layer data = inputs;
			for (int layerindex = 0; layerindex < weights.size(); layerindex++)
			{
				data = NextLayer(data, weights[layerindex],layerindex);
			}
			return data;
		}

		Brain operator=(Brain b) {
			Bias = b.Bias;
			weights = b.weights;
			return *this;
		}
		bool operator==(Brain b) {
			bool ret = true;
			for (int i = 0; i < weights.size(); i++)
			{
				for (int j = 0; j < weights[i].size(); j++)
				{
					for (int k = 0; k < weights[i][j].size(); k++)
					{
						if (this->GetWeights()[i][j][k] != b.GetWeights()[i][j][k]) {
							ret = false;
						}
					}
				}
				if (this->GetBias()[i] != b.GetBias()[i]) {
					ret = false;
				}
			}
			return ret;
		}

		void DrawNetwork() {

		}

		void SaveNetwork(std::string filename,std::string ActFunctDesc) {
			std::ofstream out(filename);
			nlohmann::json bj;
			bj["actfunct"] = ActFunctDesc;
			bj["Weights"]["size"] = weights.size();
			for (int i = 0; i < weights.size(); i++)
			{
				bj["Weights"][std::to_string(i)]["size"] = weights[i].size();
				for (int j = 0; j < weights[i].size(); j++)
				{
					bj["Weights"][std::to_string(i)][std::to_string(j)]["size"] = weights[i][j].size();
					for (int k = 0; k < weights[i][j].size(); k++)
					{
						bj["Weights"][std::to_string(i)][std::to_string(j)][std::to_string(k)] = weights[i][j][k];
					}
				}
				bj["Bias"][std::to_string(i)] = Bias[i];
			}
			out.write(nlohmann::to_string(bj).c_str(), nlohmann::to_string(bj).size());
			out.close();
		}

		std::string LoadNetwork(std::string filename) {
			std::ifstream in(filename);
			std::string input((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
			in.close();
			nlohmann::json bj;
			try
			{
				bj = nlohmann::json::parse(input);
			}
			catch (nlohmann::json::parse_error& ex)
			{
				std::cerr << "parse error at byte " << ex.byte << std::endl;
			}
			weights.resize(bj["Weights"]["size"].get<int>());
			Bias.resize(bj["Weights"]["size"].get<int>());
			for (int i = 0; i < bj["Weights"]["size"].get<int>(); i++)
			{
				weights[i].resize(bj["Weights"][std::to_string(i)]["size"].get<int>());
				for (int j = 0; j < bj["Weights"][std::to_string(i)]["size"].get<int>(); j++)
				{
					weights[i][j].resize(bj["Weights"][std::to_string(i)][std::to_string(j)]["size"].get<int>());
					for (int k = 0; k < bj["Weights"][std::to_string(i)][std::to_string(j)]["size"].get<int>(); k++)
					{
						weights[i][j][k]=bj["Weights"][std::to_string(i)][std::to_string(j)][std::to_string(k)].get<double>();
					}
				}
				Bias[i] = bj["Bias"][std::to_string(i)].get<double>();
			}
			return bj["actfunct"].get<std::string>();
		}
	};

}