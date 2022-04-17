#pragma once
#include <vector>

namespace Skeleton {
	class Bone {
	protected:
		double startx, starty, startz, endx, endy, endz;
	public:
		void SetAll(double _startx, double _starty, double _startz, double _endx, double _endy, double _endz) {
			startx = _startx;
			starty = _starty;
			startz = _startz;
			endx = _endx;
			endy = _endy;
			endz = _endz;
		}

		std::vector<double> GetData() {
			std::vector<double> ret;
			ret.push_back(startx);
			ret.push_back(starty);
			ret.push_back(startz);
			ret.push_back(endx);
			ret.push_back(endy);
			ret.push_back(endz);
			return ret;
		}
	};

	class Skeleton {
	protected:
		std::vector<Bone> bones;
	public:

		Bone GetBone(int i) {
			return bones[i];
		}

		int Size() {
			return bones.size();
		}

		std::vector<double> GetData() {
			std::vector<double> ret;
			for (int i = 0; i < bones.size(); i++)
			{
				std::vector<double> tmp = bones[i].GetData();
				for (int j = 0; j < tmp.size(); j++)
				{
					ret.push_back(tmp[j]);
				}
			}
			return ret;
		}
	};
}