#pragma once
#include <iostream>
#include <vector>
#include <random>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


// Include GLM
#include <glm/glm.hpp>
using namespace glm;

template<typename T>
T MapRange(T val, T inMin, T inMax, T outMin, T outMax) {
    double slope = 1.0 * (outMax - outMin) / (inMax - inMin);
    return outMin + slope * (val - inMin);
}

namespace Virtual_Human {


    struct Point {

        // Skips Math.sqrt for faster comparisons
        double sqDistanceFrom(Point* other) {
            double dx = other->x - this->x;
            double dy = other->y - this->y;
            double dz = other->z - this->z;

            return dx * dx + dy * dy + dz * dz;
        }

        // Pythagorus: a^2 = b^2 + c^2
        double distanceFrom(Point* other) {
            return sqrt(this->sqDistanceFrom(other));
        }
        double x, y, z = 0;

        Point(double x, double y, double z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        Point() {
            x, y, z = 0;
        }

        void Normalize() {
            double total = x + y + z;
            x = x / total;
            y = y / total;
            z = z / total;
        }
        void Randomize() {
            x = MapRange<double>(rand(), 0, RAND_MAX, -1, 1);
            y = MapRange<double>(rand(), 0, RAND_MAX, -1, 1);
            z = MapRange<double>(rand(), 0, RAND_MAX, -1, 1);
        }
        void RandomizeInRange(double min, double max) {
            x = MapRange<double>(rand(), 0, RAND_MAX, min, max);
            y = MapRange<double>(rand(), 0, RAND_MAX, min, max);
            z = MapRange<double>(rand(), 0, RAND_MAX, min, max);
        }
        void Invert() {
            x = -x;
            y = -y;
            z = -z;
        }

        Point operator *(Point p) {
            Point ret;
            ret.x = x * p.x;
            ret.y = y * p.y;
            ret.z = z * p.z;
            return ret;
        }
        Point operator +(Point p) {
            Point ret;
            ret.x = x + p.x;
            ret.y = y + p.y;
            ret.z = z + p.z;
            return ret;
        }
        Point operator *(double p) {
            Point ret;
            ret.x = x * p;
            ret.y = y * p;
            ret.z = z * p;
            return ret;
        }
        Point operator +(double p) {
            Point ret;
            ret.x = x + p;
            ret.y = y + p;
            ret.z = z + p;
            return ret;
        }
        Point operator /(double p) {
            Point ret;
            ret.x = x / p;
            ret.y = y / p;
            ret.z = z / p;
            return ret;
        }
        Point operator =(Point p) {
            Point ret;
            ret.x = p.x;
            ret.y = p.y;
            ret.z = p.z;
            x = p.x;
            y = p.y;
            z = p.z;
            return ret;
        }
    };

    struct DisplayPoint :public Point {
        Point color;
        Point Velocity;
        DisplayPoint(Point point, Point color) {
            this->color = color;
            Velocity = Point(0, 0, 0);
            x = point.x;
            y = point.y;
            z = point.z;
        }
        DisplayPoint() {
            color.x = 1;
            color.y = 1;
            color.z = 1;
            x = 0;
            y = 0;
            z = 0;
        }

        DisplayPoint operator =(Point p) {
            DisplayPoint ret;
            ret.x = p.x;
            ret.y = p.y;
            ret.z = p.z;
            x = p.x;
            y = p.y;
            z = p.z;
            return ret;
        }
        DisplayPoint operator =(DisplayPoint p) {
            DisplayPoint ret;
            ret.x = p.x;
            ret.y = p.y;
            ret.z = p.z;
            x = p.x;
            y = p.y;
            z = p.z;
            ret.color.x = p.color.x;
            ret.color.y = p.color.y;
            ret.color.z = p.color.z;
            color.x = p.color.x;
            color.y = p.color.y;
            color.z = p.color.z;
            ret.Velocity.x = p.Velocity.x;
            ret.Velocity.y = p.Velocity.y;
            ret.Velocity.z = p.Velocity.z;
            Velocity.x = p.Velocity.x;
            Velocity.y = p.Velocity.y;
            Velocity.z = p.Velocity.z;
            return ret;
        }

        DisplayPoint operator +(DisplayPoint p) {
            DisplayPoint ret;
            ret.x += p.x;
            ret.y += p.y;
            ret.z += p.z;
            x += p.x;
            y += p.y;
            z += p.z;
            ret.color.x += p.color.x;
            ret.color.y += p.color.y;
            ret.color.z += p.color.z;
            color.x += p.color.x;
            color.y += p.color.y;
            color.z += p.color.z;
            ret.Velocity.x += p.Velocity.x;
            ret.Velocity.y += p.Velocity.y;
            ret.Velocity.z += p.Velocity.z;
            Velocity.x += p.Velocity.x;
            Velocity.y += p.Velocity.y;
            Velocity.z += p.Velocity.z;
            return ret;
        }
        DisplayPoint operator *(double p) {
            DisplayPoint ret;
            ret.x *= p;
            ret.y *= p;
            ret.z *= p;
            x *= p;
            y *= p;
            z *= p;
            return ret;
        }
        DisplayPoint operator +(double p) {
            DisplayPoint ret;
            ret.x += p;
            ret.y += p;
            ret.z += p;
            x += p;
            y += p;
            z += p;
            return ret;
        }
        DisplayPoint operator +(Point p) {
            DisplayPoint ret;
            ret.x += p.x;
            ret.y += p.y;
            ret.z += p.z;
            x += p.x;
            y += p.y;
            z += p.z;
            return ret;
        }
        DisplayPoint operator +=(Point p) {
            DisplayPoint ret;
            ret.x += p.x;
            ret.y += p.y;
            ret.z += p.z;
            x += p.x;
            y += p.y;
            z += p.z;
            return ret;
        }
    };

    struct Bone {
        Point start;
        Point end;
    };

    void DrawPolygon(int sides, double radius, DisplayPoint pos) {
        glTranslated(pos.x, pos.y, pos.z);
        glColor3d(pos.color.x, pos.color.y, pos.color.z);
        GLUquadricObj* pObj = gluNewQuadric();
        gluQuadricDrawStyle(pObj, GLU_FILL);
        gluSphere(pObj, radius, 5, 5);
        gluDeleteQuadric(pObj);
        glTranslated(-pos.x, -pos.y, -pos.z);
    }

    Point HSVtoRGB(double H, double S, double V) {
        if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
            return Point();
        }
        double s = S / 100;
        double v = V / 100;
        double C = s * v;
        double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
        double m = v - C;
        double r, g, b;
        if (H >= 0 && H < 60) {
            r = C, g = X, b = 0;
        }
        else if (H >= 60 && H < 120) {
            r = X, g = C, b = 0;
        }
        else if (H >= 120 && H < 180) {
            r = 0, g = C, b = X;
        }
        else if (H >= 180 && H < 240) {
            r = 0, g = X, b = C;
        }
        else if (H >= 240 && H < 300) {
            r = X, g = 0, b = C;
        }
        else {
            r = C, g = 0, b = X;
        }
        double R = MapRange<float>((r + m) * 255, 0, 255, 0, 1);
        double G = MapRange<float>((g + m) * 255, 0, 255, 0, 1);
        double B = MapRange<float>((b + m) * 255, 0, 255, 0, 1);
        return Point(R, G, B);
    }

    struct SensorPoint : public DisplayPoint {
        double val = 0;

        void SetVal(double _val) {
            val = _val;
            color = HSVtoRGB(val, 1, 1);
        }
    };

    struct Ear {
        double catchRadius;
    };

    struct Eye {
        Point pos;
        Point dir;
        double fov;
        double focus;
    };

    struct Volume {
        std::vector<Point*> points;
    };

    struct Object {
        std::vector<DisplayPoint> PhysicalPoints;
        double Elasticity = 0;
        int polysides = 3;
        double polyradius = .05;
        std::vector<double> ElasticityWeights;
        void draw() {
            //draw volumetric cube
            for (int i = 0; i < PhysicalPoints.size(); i++) {
                DrawPolygon(polysides, polyradius, PhysicalPoints[i]);
            }
        }
    };

    struct Virtual_Human {
        Eye  eyes[2];
        std::vector<DisplayPoint> PhysicalPoints;
        std::vector<SensorPoint*>  Sour_TastePoints;
        std::vector<SensorPoint*>  Sweet_TastePoints;
        std::vector<SensorPoint*>  Salty_TastePoints;
        std::vector<SensorPoint*>  Spicy_TastePoints;
        std::vector<SensorPoint*>  Compression_Strech_Points;
        std::vector<SensorPoint*> Pain_Pleasure_Points;
        std::vector<SensorPoint*> Itch_Discomfort_Points;
        std::vector<DisplayPoint*> PinnedPoints;
        std::vector<Bone> Base_Bones;
        std::vector < std::vector<std::vector<Bone>>> Animation;
        std::vector<std::vector<std::vector<std::vector<double>>>> weights;
        int SelectedAnimation = 0;
        int frame = 0;
        int ParticleResolution = 3;
        double Elasticity = 0;
        int polysides = 3;
        double polyradius = .05;
        std::vector<double> ElasticityWeights;
        std::vector<double> BoneWeights;
        std::vector<double> SenseWeights;
        void Draw() {
            switch (SelectedAnimation)
            {
            case 0:
                //base bones
                DrawPointsOnBones(PhysicalPoints, Base_Bones, weights[0][0]);
                break;
            default:
                DrawPointsOnBones(PhysicalPoints, Animation[SelectedAnimation - 1][frame], weights[SelectedAnimation - 1][frame]);
                break;
            }
        }
        void DrawPointsOnBones(std::vector<DisplayPoint> dp, std::vector<Bone> bones, std::vector<std::vector<double>> weights) {
            for (int i = 0; i < dp.size(); i++)
            {
                DisplayPoint point = dp[i];
                for (int j = 0; j < bones.size(); j++)
                {
                    point = point + (bones[j].start * weights[i][j]);
                }
                DrawPolygon(ParticleResolution, .50, point);
            }

        }
    };

    struct Collision {
        //add extra collision data
        bool Collided;
        //Point CollisionDirection;
        //Point CollisionPosition;
        Point* CollisionObj1;
        Point* CollisionObj2;
        double DistanceToObject;
    };

    Collision isColiding(Point* p1, Point* p2, double radius) {
        Collision ret;
        ret.DistanceToObject = glm::sqrt(glm::pow(p1->x - p2->x, 2) + glm::pow(p1->y - p2->y, 2) + glm::pow(p1->z - p2->z, 2));
        ret.Collided = ret.DistanceToObject < (radius * 2);
        //ret.CollisionDirection.x = p1->x - p2->x;
        //ret.CollisionDirection.y = p1->y - p2->y;
        //ret.CollisionDirection.z = p1->z - p2->z;
        //ret.CollisionPosition.x = (p1->x + p2->x) / 2;
        //ret.CollisionPosition.x = (p1->y + p2->y) / 2;
        //ret.CollisionPosition.x = (p1->z + p2->z) / 2;
        //ret.CollisionDirection.Normalize();
        ret.CollisionObj1 = p1;
        ret.CollisionObj2 = p2;
        //return collision object for extra collision data
        return ret;
    }

    // QuadTree
    double min(double d1, double d2) {
        if (d1 < d2) {
            return d1;
        }
        else {
            return d2;
        }
    }

    double max(double d1, double d2) {
        if (d1 > d2) {
            return d1;
        }
        else {
            return d2;
        }
    }

    enum Quadrant
    {
        NEU,
        NWU,
        SEU,
        SWU,
        NED,
        NWD,
        SED,
        SWD
    };

    struct Cube {

        void Draw() {
            double minX = x - (w);
            double maxX = x + (w);
            double minY = y - (h);
            double maxY = y + (h);
            double minZ = z - (l);
            double maxZ = z + (l);
            glLineWidth(.5);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBegin(GL_QUADS);
            glVertex3f(maxX,maxY,minZ);
            glVertex3f(minX,maxY,minZ);
            glVertex3f(minX,maxY,maxZ);
            glVertex3f(maxX,maxY,maxZ);

            // Bottom face (y = -1.0f)
            glVertex3f(maxX,minY,maxZ);
            glVertex3f(minX,minY,maxZ);
            glVertex3f(minX,minY,minZ);
            glVertex3f(maxX,minY,minZ);

            // Front face  (z = 1.0f)
            glVertex3f(maxX,maxY,maxZ);
            glVertex3f(minX,maxY,maxZ);
            glVertex3f(minX,minY,maxZ);
            glVertex3f(maxX,minY,maxZ);

            // Back face (z = -1.0f)
            glVertex3f(maxX,minY,minZ);
            glVertex3f(minX,minY,minZ);
            glVertex3f(minX,maxY,minZ);
            glVertex3f(maxX,maxY,minZ);

            // Left face (x = -1.0f)
            glVertex3f(minX,maxY,maxZ);
            glVertex3f(minX,maxY,minZ);
            glVertex3f(minX,minY,minZ);
            glVertex3f(minX,minY,maxZ);

            // Right face (x = 1.0f)
            glVertex3f(maxX,maxY,minZ);
            glVertex3f(maxX,maxY,maxZ);
            glVertex3f(maxX,minY,maxZ);
            glVertex3f(maxX,minY,minZ);
            glEnd();  // End of drawing color-cube
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        Cube(double x, double  y, double z, double  w, double  h, double l) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
            this->h = h;
            this->l = l;
            this->left = x - w;
            this->right = x + w;
            this->top = y - h;
            this->bottom = y + h;
            this->front = z - l;
            this->back = z + l;
        }
        Cube() {
            this->x = 0;
            this->y = 0;
            this->z = 0;
            this->w = 1;
            this->h = 1;
            this->l = 1;
            this->left = x - w;
            this->right = x + w;
            this->top = y - h;
            this->bottom = y + h;
            this->front = z - l;
            this->back = z + l;
        }

        double x;
        double  y;
        double z;
        double  w;
        double  h;
        double l;
        double  left;
        double  right;
        double top;
        double bottom;
        double front;
        double back;

        bool contains(Point* point) {
            return (
                this->left <= point->x && point->x <= this->right &&
                this->top <= point->y && point->y <= this->bottom &&
                this->front <= point->y && point->y <= this->back
                );
        }

        bool intersects(Cube range) {
            return !(
                this->right < range.left || range.right < this->left ||
                this->bottom < range.top || range.bottom < this->top ||
                this->back < range.front || range.back < this->front
                );
        }

        Cube subdivide(Quadrant quadrant) {
            switch (quadrant) {
            case NEU:
                return Cube(this->x + this->w / 2, this->y + this->h / 2, this->z + this->l / 2, this->w/2, this->h/2, this->l/2);
            case NWU:
                return Cube(this->x - this->w / 2, this->y + this->h / 2, this->z + this->l / 2, this->w/2, this->h/2, this->l/2);
            case SEU:
                return Cube(this->x + this->w / 2, this->y - this->h / 2, this->z + this->l / 2, this->w/2, this->h/2, this->l/2);
            case SWU:
                return Cube(this->x - this->w / 2, this->y - this->h / 2, this->z + this->l / 2, this->w/2, this->h/2, this->l/2);
            case NED:
                return Cube(this->x + this->w / 2, this->y + this->h / 2, this->z - this->l / 2, this->w/2, this->h/2, this->l/2);
            case NWD:
                return Cube(this->x - this->w / 2, this->y + this->h / 2, this->z - this->l / 2, this->w/2, this->h/2, this->l/2);
            case SED:
                return Cube(this->x + this->w / 2, this->y - this->h / 2, this->z - this->l / 2, this->w/2, this->h/2, this->l/2);
            case SWD:
                return Cube(this->x - this->w / 2, this->y - this->h / 2, this->z - this->l / 2, this->w/2, this->h/2, this->l/2);
            }
        }

        double xDistanceFrom(Point* point) {
            if (this->left <= point->x && point->x <= this->right) {
                return 0;
            }

            return min(
                abs(point->x - this->left),
                abs(point->x - this->right)
            );
        }

        double yDistanceFrom(Point* point) {
            if (this->top <= point->y && point->y <= this->bottom) {
                return 0;
            }

            return min(
                abs(point->y - this->top),
                abs(point->y - this->bottom)
            );
        }
        double zDistanceFrom(Point* point) {
            if (this->front <= point->z && point->z <= this->back) {
                return 0;
            }

            return min(
                abs(point->z - this->front),
                abs(point->z - this->back)
            );
        }

        double sqDistanceFrom(Point* point) {
            double dx = this->xDistanceFrom(point);
            double dy = this->yDistanceFrom(point);
            double dz = this->zDistanceFrom(point);

            return dx * dx + dy * dy + dz * dz;
        }

        double distanceFrom(Point* point) {
            return sqrt(this->sqDistanceFrom(point));
        }
    };

    struct Sphere {
        Sphere(double x, double y, double z, double r) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->r = r;
            this->rSquared = this->r * this->r;
        }
        double x, y, z, r, rSquared;

        bool contains(Point point) {
            double d = pow((point.x - this->x), 2) + pow((point.y - this->y), 2);
            return d <= this->rSquared;
        }

        double intersects(Cube range) {

            double xDist = abs(range.x - this->x);
            double yDist = abs(range.y - this->y);
            double zDist = abs(range.z - this->z);

            double r = this->r;

            double w = range.w / 2;
            double h = range.h / 2;
            double l = range.l / 2;

            double edges = pow((xDist - w), 2) + pow((yDist - h), 2) + pow((zDist - l), 2);

            if (xDist > (r + w) || yDist > (r + h) || zDist > (r + l))
                return false;

            if (xDist <= w || yDist <= h || zDist <= l)
                return true;

            return edges <= this->rSquared;
        }
    };

    struct OctTree {
        int MAX_DEPTH = 8;

        Cube boundary;
        int capacity;
        std::vector<Point*> points;
        bool divided;
        int depth;

        OctTree(Cube boundary, int capacity = 8, int _depth = 0) {
            this->boundary = boundary;
            this->capacity = capacity;
            this->divided = false;
            this->depth = _depth;
        }
        OctTree() {
            this->boundary = Cube();
            this->capacity = 0;
            this->divided = false;
            this->depth = 0;
        }

        OctTree* northeastup;
        OctTree* northwestup;
        OctTree* southeastup;
        OctTree* southwestup;
        OctTree* northeastdown;
        OctTree* northwestdown;
        OctTree* southeastdown;
        OctTree* southwestdown;

        void Clear() {
            if (divided) {
                northeastup->Clear();
                northwestup->Clear();
                southeastup->Clear();
                southwestup->Clear();
                northeastdown->Clear();
                northwestdown->Clear();
                southeastdown->Clear();
                southwestdown->Clear();
                northeastup = new OctTree();
                northwestup = new OctTree();
                southeastup = new OctTree();
                southwestup = new OctTree();
                northeastdown = new OctTree();
                northwestdown = new OctTree();
                southeastdown = new OctTree();
                southwestdown = new OctTree();
                delete northeastup;
                delete northwestup;
                delete southeastup;
                delete southwestup;
                delete northeastdown;
                delete northwestdown;
                delete southeastdown;
                delete southwestdown;
                northeastup = NULL;
                northwestup = NULL;
                southeastup = NULL;
                southwestup = NULL;
                northeastdown = NULL;
                northwestdown = NULL;
                southeastdown = NULL;
                southwestdown = NULL;
            }
        }

        std::vector<OctTree*> get_children() {
            if (this->divided) {
                return{
                    this->northeastup,
                        this->northwestup,
                        this->southeastup,
                        this->southwestup,
                    this->northeastdown,
                        this->northwestdown,
                        this->southeastdown,
                        this->southwestdown
                };
            }
            else {
                return std::vector<OctTree*>();
            }
        }

        void subdivide() {
            this->northeastup = new OctTree(this->boundary.subdivide(NEU), this->capacity, this->depth + 1);
            this->northwestup = new OctTree(this->boundary.subdivide(NWU), this->capacity, this->depth + 1);
            this->southeastup = new OctTree(this->boundary.subdivide(SEU), this->capacity, this->depth + 1);
            this->southwestup = new OctTree(this->boundary.subdivide(SWU), this->capacity, this->depth + 1);
            this->northeastdown = new OctTree(this->boundary.subdivide(NED), this->capacity, this->depth + 1);
            this->northwestdown = new OctTree(this->boundary.subdivide(NWD), this->capacity, this->depth + 1);
            this->southeastdown = new OctTree(this->boundary.subdivide(SED), this->capacity, this->depth + 1);
            this->southwestdown = new OctTree(this->boundary.subdivide(SWD), this->capacity, this->depth + 1);

            this->divided = true;

            // Move points to children.
            // This improves performance by placing points
            // in the smallest available rectangle.
            for (int i = 0; i < points.size(); i++) {
                bool inserted = northeastup->insert(points[i]) ||
                    northwestup->insert(points[i]) ||
                    southeastup->insert(points[i]) ||
                    southwestup->insert(points[i]) ||
                    northeastdown->insert(points[i]) ||
                    northwestdown->insert(points[i]) ||
                    southeastdown->insert(points[i]) ||
                    southwestdown->insert(points[i]);
            }

            this->points = std::vector<Point*>();
        }

        bool insert(Point* point) {
            if (!boundary.contains(point)) {
                return false;
            }

            if (!divided) {
                if (points.size() < capacity ||
                    depth == MAX_DEPTH) {
                    points.push_back(point);
                    return true;
                }

                subdivide();
            }

            return (
                northeastup->insert(point) ||
                northwestup->insert(point) ||
                southeastup->insert(point) ||
                southwestup->insert(point) ||
                northeastdown->insert(point) ||
                northwestdown->insert(point) ||
                southeastdown->insert(point) ||
                southwestdown->insert(point)
                );
        }

        std::vector<Point*> query(Cube range, std::vector<Point*>* found = nullptr) {
            if (!found) {
                found = new std::vector<Point*>;
            }

            if (!range.intersects(boundary)) {
                return *found;
            }

            if (divided) {
                northwestup->query(range, found);
                northeastup->query(range, found);
                southwestup->query(range, found);
                southeastup->query(range, found);
                northwestdown->query(range, found);
                northeastdown->query(range, found);
                southwestdown->query(range, found);
                southeastdown->query(range, found);
                return *found;
            }

            for (int i = 0; i < points.size(); i++) {
                if (range.contains(points[i])) {
                    found->push_back(points[i]);
                }
            }

            return *found;
        }

        void Dump(std::vector<DisplayPoint> * data) {
            for (int i = 0; i < data->size(); i++)
            {
                insert(&(*data)[i]);
            }
        }

        std::vector<Point*> closest(Point* searchPoint, int maxCount = 1, int maxDistance = 9999999999999999) {
            double sqMaxDistance = pow(maxDistance, 2);
            return kNearest(searchPoint, maxCount, sqMaxDistance, 0, 0).found;
        }

        struct kNearestSet {
            std::vector<Point*> found;
            double furthestSqDistance;
            kNearestSet() {
                found.resize(0);
                furthestSqDistance = 0;
            }
        };

        kNearestSet kNearest(Point* searchPoint, int maxCount, double sqMaxDistance, double furthestSqDistance, int foundSoFar) {
            std::vector<Point*> found;

            if (divided) {
                std::vector<OctTree*> children = get_children();

                std::sort(children.begin(), children.end(), [searchPoint](OctTree* a, OctTree* b) {
                    return a->boundary.sqDistanceFrom(searchPoint) - b->boundary.sqDistanceFrom(searchPoint);
                    });

                for (int i = 0; i < children.size(); i++)
                {
                    double sqDistance = children[i]->boundary.sqDistanceFrom(searchPoint);
                    if (sqDistance > sqMaxDistance) {
                        return kNearestSet();
                    }
                    else if (foundSoFar < maxCount || sqDistance < furthestSqDistance) {
                        kNearestSet result = children[i]->kNearest(searchPoint, maxCount, sqMaxDistance, furthestSqDistance, foundSoFar);
                        std::vector<Point*> childPoints = result.found;
                        for (int i = 0; i < childPoints.size(); i++)
                        {
                            found.push_back(childPoints[i]);
                        }
                        foundSoFar += childPoints.size();
                        furthestSqDistance = result.furthestSqDistance;
                    }
                }
            }
            else {
                std::sort(points.begin(), points.end(), [searchPoint](Point* a, Point* b) {
                    return a->sqDistanceFrom(searchPoint) - b->sqDistanceFrom(searchPoint);
                    });
                for (int i = 0; i < points.size(); i++)
                {
                    double sqDistance = points[i]->sqDistanceFrom(searchPoint);
                    if (sqDistance > sqMaxDistance) {
                        return kNearestSet();
                    }
                    else if (foundSoFar < maxCount || sqDistance < furthestSqDistance) {
                        found.push_back(points[i]);
                        furthestSqDistance = max(sqDistance, furthestSqDistance);
                        foundSoFar++;
                    }
                }
            }

            std::sort(found.begin(), found.end(), [searchPoint](Point* a, Point* b) {
                return a->sqDistanceFrom(searchPoint) - b->sqDistanceFrom(searchPoint);
                });

            kNearestSet ret;
            ret.found = found;
            ret.furthestSqDistance = sqrt(furthestSqDistance);
            return ret;
        }

        //void merge(OctTree* other, int capacity) {
        //    double left = min(boundary.left, other.boundary.left);
        //    double right = max(boundary.right, other.boundary.right);
        //    double top = min(boundary.top, other.boundary.top);
        //    double bottom = max(boundary.bottom, other.boundary.bottom);

        //    double height = bottom - top;
        //    double width = right - left;

        //    double midX = left + width / 2;
        //    double midY = top + height / 2;

        //    Cube boundary = new Cube(midX, midY, width, height);
        //    OctTree result = new OctTree(boundary, capacity);

        //    forEach(point = > result.insert(point));
        //    other.forEach(point = > result.insert(point));

        //    return result;
        //}

        int get_length() {
            if (divided) {
                return (
                    northwestup->get_length() +
                    northeastup->get_length() +
                    southwestup->get_length() +
                    southeastup->get_length() +
                    northwestdown->get_length() +
                    northeastdown->get_length() +
                    southwestdown->get_length() +
                    southeastdown->get_length()
                    );
            }

            return points.size();
        }

        void Draw() {
            try {
                if (divided) {
                    if (northwestup != nullptr) {
                        northwestup->Draw();
                    }
                    if (northeastup != nullptr) {
                        northeastup->Draw();
                    }
                    if (southwestup != nullptr) {
                        southwestup->Draw();
                    }
                    if (southeastup != nullptr) {
                        southeastup->Draw();
                    }
                    if (northwestdown != nullptr) {
                        northwestdown->Draw();
                    }
                    if (northeastdown != nullptr) {
                        northeastdown->Draw();
                    }
                    if (southwestdown != nullptr) {
                        southwestdown->Draw();
                    }
                    if (southeastdown != nullptr) {
                        southeastdown->Draw();
                    }
                }
                else {
                    boundary.Draw();
                }
            }
            catch (std::exception e) {
                printf(e.what());
            }
        }
    };


    struct Physics {
        double gravity = 0.1;
        double pointSize = 0.5;
        int capacity;
        int depth;
        bool wait;
        Cube boundry;
        Cube searchSize;
        OctTree tree;
        std::vector<Object*> Objects;
        std::vector<Object*> Points;


        void Draw() {
             tree.Draw();
        }

        void Step(double speed) {
            if (!wait) {
                for (int i = 0; i < Objects.size(); i++) {
                    tree.Clear();
                    tree = OctTree(boundry, capacity, depth);
                    tree.Dump(&Objects[i]->PhysicalPoints);
                    std::vector<Collision> collisions;
                    std::vector<Point*> data = tree.query(searchSize);
                    for (int j = 0; j < data.size(); j++)
                    {
                        for (int k = 0; k < data.size(); k++)
                        {
                            if (j == k) {
                                continue;
                            }
                            Collision coll = isColiding(data[i], data[i], pointSize);
                            if (coll.Collided) {
                                collisions.push_back(coll);
                            }
                        }
                        (*data[j]) = (*data[j]) + (((DisplayPoint*)data[j])->Velocity * speed);
                    }

                    for (int k = 0; k < collisions.size(); k++)
                    {
                        Point velkeep = ((DisplayPoint*)collisions[k].CollisionObj1)->Velocity * .9;
                        Point veltransfer = ((DisplayPoint*)collisions[k].CollisionObj1)->Velocity * .1;
                        Point velkeep2 = ((DisplayPoint*)collisions[k].CollisionObj2)->Velocity * .9;
                        Point veltransfer2 = ((DisplayPoint*)collisions[k].CollisionObj2)->Velocity * .1;
                        ((DisplayPoint*)collisions[k].CollisionObj1)->Velocity = velkeep + veltransfer2;
                        ((DisplayPoint*)collisions[k].CollisionObj2)->Velocity = velkeep2 + veltransfer;
                    }
                }
            }
            Sleep(200);
        }
    };
}