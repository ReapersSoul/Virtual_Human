#pragma once
#include <vector>
#include "Human.h"
#include "Parallel Accelorator.h"
#include "OctTree.h"

namespace Physics {

	template<typename T>
	struct Vec3 {
		T x, y, z;
	};
	struct Point {
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
			Objects = OctTree<CollisionObject*>(boundry, capacity);
			for (int i = 0; i < VObjects.size(); i++)
			{
				OctTree<CollisionObject*>::Obj obj;
				obj.obj = VObjects[i];
				obj.pos.x = VObjects[i]->GetPos().x;
				obj.pos.y = VObjects[i]->GetPos().y;
				obj.pos.z = VObjects[i]->GetPos().z;
				Objects.insert(obj);
			}
			for (int i = 0; i < VObjects.size(); i++)
			{
				//modify search field to be around object;
				Cube ModifiedSearch = searchSize;
				ModifiedSearch.x += VObjects[i]->GetPos().x;
				ModifiedSearch.y += VObjects[i]->GetPos().y;
				ModifiedSearch.z += VObjects[i]->GetPos().z;
				//query Oct-Tree for objects in modified search field
				std::vector<OctTree<CollisionObject*>::Obj> vals = Objects.query(ModifiedSearch);
				//execute the collisions and physics stuff in parallel 
				CollisionObject* tmp = VObjects[i];
				std::vector<Collision> collisions = pa.ExecuteCalls<Collision, OctTree<CollisionObject*>::Obj>(vals, std::function<Collision(OctTree<CollisionObject*>::Obj)>([tmp](OctTree<CollisionObject*>::Obj data) {
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
			OctTree<CollisionObject*>::Obj tobj;
			tobj.obj = obj;
			tobj.pos.x = pos.x;
			tobj.pos.y = pos.y;
			tobj.pos.z = pos.z;
			Objects.insert(tobj);
		}

		void Draw() {

		}
		void DefaultWorld() {

		}

	private:
		double gravity = 0.1;
		int capacity;
		int depth;
		bool wait;
		Cube boundry;
		Cube searchSize;

		typedef int funct(CollisionObject data);
		Parallel_Accelorator::Parallel_Accelorator pa;
		OctTree<CollisionObject *> Objects;
		std::vector<CollisionObject *> VObjects;
		
	};
}