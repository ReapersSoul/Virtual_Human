#pragma once
#include <vector>

namespace Eye {
	class Eye {
		int resolution;
		double Radius_FOV;
		double focus;
		double rotx;
		double roty;
		double rotz;
	public:
		std::vector<double> RayCast() {

		}
		void SetResolution(int f) {
			resolution = f;
		}
		void SetFocus(double f) {
			focus = f;
		}
		void SetFov(double f) {
			Radius_FOV = f;
		}
		void SetRot(double x, double y, double z) {
			rotx = x;
			roty = y;
			rotz = z;
		}
		double GetRotX() {
			return rotx;
		}
		double GetRotY() {
			return roty;
		}
		double GetRotZ() {
			return rotz;
		}

		std::vector<double> GetData() {
			std::vector<double> ret;
			ret.push_back(rotx);
			ret.push_back(roty);
			ret.push_back(rotz);
			std::vector<double> img = RayCast();
			for (int i = 0; i < img.size(); i++)
			{
				ret.push_back(img[i]);
			}
		}
	};
}