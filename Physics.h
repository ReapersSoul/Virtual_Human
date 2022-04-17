#pragma once
#include <vector>
#include "Human.h"
#include "Parallel Accelorator.h"
#include "OctTree.h"

namespace Physics {

	template<typename T>
	struct Vec3 {
		T x, y, z;

		Vec3<T> operator*(T v) {
			x *= v;
			y *= v;
			z *= v;
		}
		Vec3<T> operator/(T v) {
			x /= v;
			y /= v;
			z /= v;
		}
		Vec3<T> operator+(T v) {
			x += v;
			y += v;
			z += v;
		}
		Vec3<T> operator-(T v) {
			x -= v;
			y -= v;
			z -= v;
		}
		Vec3<T> operator*(Vec3<T> v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
		}
		Vec3<T> operator/(Vec3<T> v) {
			x /= v.x;
			y /= v.y;
			z /= v.z;
		}
		Vec3<T> operator+(Vec3<T> v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}
		Vec3<T> operator-(Vec3<T> v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}
	};
	struct Point {
		Point() {};
		Point(double _x, double _y, double _z,double _r) {
			pos.x = _x;
			pos.y = _y;
			pos.z = _z;
			radius = _r;
		}
		Vec3<double> pos;
		double radius;
	};

	class CollisionObject;

	struct Collision {
		//add extra collision data
		bool Collided;
		//Point CollisionDirection;
		//Point CollisionPosition;
		CollisionObject* CollisionObj1;
		CollisionObject* CollisionObj2;
		double DistanceToObject;
	};

	class CollisionObject {
	public:
		void OnColision(CollisionObject * collision) {

		}

		//override me
		Vec3<double> GetPos() {};

		CollisionObject* operator= (CollisionObject* t) {
			return this;
		}
		bool operator== (CollisionObject* t) {
			return false;
		}
		bool operator== (CollisionObject t) {
			return false;
		}

		Collision CheckCollision(CollisionObject * obj) {
			if (typeid(obj) == typeid(this)) {

			}
		};
	};

	class Raycast :public CollisionObject {
		Vec3<double> origin, Direction;
		double step_Size;
		double Max_Dist;
		double Dist = 0;
		bool Collided = false;
		Collision ReturnCollision;
		Vec3<double> current = origin;
		Collision CastRay(Vec3<double> origin, Vec3<double> Direction,double step_Size,double Max_Dist) {
			 

		}

		void step(std::vector<OctTree::Sphere *> s) {
			if(Dist < Max_Dist && !Collided) {
				Vec3<double> tmpdist = (Direction * step_Size);
				Vec3<double> nextPoint = current + (Direction * step_Size);
				for (int i = 0; i < s.size(); i++)
				{
					s[i]->contains(OctTree::Point(current.x,current.y,current.z));
				}
				Dist += sqrt(pow(tmpdist.x, 2) + pow(tmpdist.y, 2) + pow(tmpdist.z, 2));
				current = nextPoint;
			}
		}
	};

	namespace Geometry {
		class Cube {
		public:

		private:

		};
	}

	class Physics
	{
	public:
		Physics() {};
		~Physics();
		static Collision CheckCollision(CollisionObject* obj1, CollisionObject* obj2) {
			obj1->CheckCollision(obj1);
		}

		void step(double stepsize) {
			Objects.Clear();
			Objects = OctTree::OctTree<CollisionObject*>(boundry, capacity);
			for (int i = 0; i < VObjects.size(); i++)
			{
				OctTree::OctTree<CollisionObject*>::Obj obj;
				obj.obj = VObjects[i];
				obj.pos.x = VObjects[i]->GetPos().x;
				obj.pos.y = VObjects[i]->GetPos().y;
				obj.pos.z = VObjects[i]->GetPos().z;
				Objects.insert(obj);
			}
			for (int i = 0; i < VObjects.size(); i++)
			{
				//modify search field to be around object;
				OctTree::Cube ModifiedSearch = searchSize;
				ModifiedSearch.x += VObjects[i]->GetPos().x;
				ModifiedSearch.y += VObjects[i]->GetPos().y;
				ModifiedSearch.z += VObjects[i]->GetPos().z;
				//query Oct-Tree for objects in modified search field
				std::vector<OctTree::OctTree<CollisionObject*>::Obj> vals = Objects.query(ModifiedSearch);
				//execute the collisions and physics stuff in parallel 
				CollisionObject* tmp = VObjects[i];
				std::vector<Collision> collisions = Parallel_Accelorator::Parallel_Accelorator<Collision, OctTree::OctTree<CollisionObject*>::Obj>(vals, std::function<Collision(OctTree::OctTree<CollisionObject*>::Obj)>([tmp](OctTree::OctTree<CollisionObject*>::Obj data) {
					return CheckCollision(tmp, data.obj);
					}));
				//for each collisions do something about it
				for (int j = 0; j < collisions.size(); j++)
				{
					//inform each object about its collision and the information
					collisions[j].CollisionObj1->OnColision(collisions[j].CollisionObj2);
					collisions[j].CollisionObj2->OnColision(collisions[j].CollisionObj1);
				}
			}
		}
		void insert(CollisionObject* obj,Vec3<double> pos) {
			OctTree::OctTree<CollisionObject*>::Obj tobj;
			tobj.obj = obj;
			tobj.pos.x = pos.x;
			tobj.pos.y = pos.y;
			tobj.pos.z = pos.z;
			Objects.insert(tobj);
		}

		void Draw() {
			Objects.Draw();
		}
		void DefaultWorld() {

		}

	private:
		double gravity = 0.1;
		int capacity;
		int depth;
		bool wait;
		OctTree::Cube boundry;
		OctTree::Cube searchSize;

		typedef int funct(CollisionObject data);
		OctTree::OctTree<CollisionObject *> Objects;
		std::vector<CollisionObject *> VObjects;
		
	};
}