#pragma once
#include <vector>

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
		T (*ActFunct)(T x);

		T dot(Layer one, Layer two) {
			T ret = NULL;
			for (int i = 0; i < one.size(); i++)
			{
				ret += one[i] * two[i];
			}
			return ret;
		}

		Layer NextLayer(Layer first, WeightLayer weights) {
			Layer ret;
			for (int i = 0; i < weights.size(); i++)
			{
				ret.push_back(ActFunct(dot(first,weights[i]))+Bias[i]);
			}
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
			ret.MutateSelf(stepsize);
			return ret;
		}

		void SetActFunct(T(*actfunct)(T x)) {
			ActFunct = actfunct;
			return;
		}

		void SetWeightSizes(int inputs, std::vector<int> sizes) {
			for (int i = 0; i < sizes.size(); i++)
			{
				WeightLayer tmp;
				if(i==0){
					tmp.resize(inputs);
				}
				else {
					tmp.resize(sizes[i]);
				}
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
				data = NextLayer(data, weights[layerindex]);
			}
			return data;
		}
	};

}