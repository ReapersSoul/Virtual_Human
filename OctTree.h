#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


// Include GLM
#include <glm/glm.hpp>
using namespace glm;

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

struct Point {
    Point(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    double x, y, z;

    // Skips Math.sqrt for faster comparisons
    double sqDistanceFrom(Point * other) {
        double dx = other->x - this->x;
        double dy = other->y - this->y;
        double dz = other->z - this->z;

        return dx * dx + dy * dy + dz * dz;
    }

    // Pythagorus: a^2 = b^2 + c^2
    double distanceFrom(Point * other) {
        return sqrt(this->sqDistanceFrom(other));
    }
};

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
        double minX = x - (w/2);
        double maxX = x + (w/2);
        double minY = y - (h/2);
        double maxY = y + (h/2);
        double minZ = z - (l/2);
        double maxZ = z + (l/2);
        glLineWidth(.5);
        glBegin(GL_LINES);
        glVertex3f(minX, minY, minZ);
        glVertex3f(maxX, minY, minZ);
        glVertex3f(minX, minY, minZ);
        glVertex3f(minX, maxY, minZ);
        glVertex3f(minX, minY, minZ);
        glVertex3f(minX, minY, maxZ);
        glVertex3f(maxX, maxY, minZ);
        glVertex3f(maxX, maxY, maxZ);
        glVertex3f(maxX, minY, minZ);
        glVertex3f(maxX, minY, maxZ);
        glVertex3f(minX, maxY, minZ);
        glVertex3f(minX, maxY, maxZ);
        glVertex3f(maxX, maxY, minZ);
        glVertex3f(maxX, minY, minZ);
        glVertex3f(maxX, maxY, minZ);
        glVertex3f(minX, maxY, minZ);
        glVertex3f(maxX, maxY, maxZ);
        glVertex3f(maxX, minY, maxZ);
        glVertex3f(maxX, maxY, maxZ);
        glVertex3f(minX, maxY, maxZ);
        glVertex3f(minX, minY, maxZ);
        glVertex3f(maxX, minY, maxZ);
        glVertex3f(minX, minY, maxZ);
        glVertex3f(minX, maxY, maxZ);
        glEnd();
    }

    Cube(double x, double  y, double z , double  w, double  h, double l) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
        this->h = h;
        this->l = l;
        this->left = x - w / 2;
        this->right = x + w / 2;
        this->top = y - h / 2;
        this->bottom = y + h / 2;
        this->front = z-l/2;
        this->back = z+l/2;
    }
    Cube() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->w = 1;
        this->h = 1;
        this->l = 1;
        this->left = x - w / 2;
        this->right = x + w / 2;
        this->top = y - h / 2;
        this->bottom = y + h / 2;
        this->front = z-l/2;
        this->back = z+l/2;
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

    bool contains(Point * point) {
        return (
            this->left <= point->x && point->x <= this->right &&
            this->top <= point->y && point->y <= this->bottom  &&
            this->front <= point->y && point->y <= this->back 
            );
    }

    bool intersects(Cube range) {
        return !(
            (this->right < range.left || range.right < this->left) &&
            this->bottom < range.top || range.bottom < this->top &&
            this->back > range.front || range.back > this->front
            );
    }

    Cube subdivide(Quadrant quadrant) {
        switch (quadrant) {
        case NEU:
            return Cube(this->x + this->w / 4, this->y - this->h / 4, this->z + this->l / 4, this->w / 2, this->h / 2, this->l / 2);
        case NWU:
            return Cube(this->x - this->w / 4, this->y - this->h / 4, this->z + this->l / 4, this->w / 2, this->h / 2, this->l / 2);
        case SEU:
            return Cube(this->x + this->w / 4, this->y + this->h / 4, this->z + this->l / 4, this->w / 2, this->h / 2, this->l / 2);
        case SWU:
            return Cube(this->x - this->w / 4, this->y + this->h / 4, this->z + this->l / 4, this->w / 2, this->h / 2, this->l / 2);
        case NED:
            return Cube(this->x + this->w / 4, this->y - this->h / 4, this->z - this->l / 4, this->w / 2, this->h / 2, this->l / 2);
        case NWD:
            return Cube(this->x - this->w / 4, this->y - this->h / 4, this->z - this->l / 4, this->w / 2, this->h / 2, this->l / 2);
        case SED:
            return Cube(this->x + this->w / 4, this->y + this->h / 4, this->z - this->l / 4, this->w / 2, this->h / 2, this->l / 2);
        case SWD:
            return Cube(this->x - this->w / 4, this->y + this->h / 4, this->z - this->l / 4, this->w / 2, this->h / 2, this->l / 2);
        }
    }

    double xDistanceFrom(Point * point) {
        if (this->left <= point->x && point->x <= this->right) {
            return 0;
        }

        return min(
            abs(point->x - this->left),
            abs(point->x - this->right)
        );
    }

    double yDistanceFrom(Point * point) {
        if (this->top <= point->y && point->y <= this->bottom) {
            return 0;
        }

        return min(
            abs(point->y - this->top),
            abs(point->y - this->bottom)
        );
    }
    double zDistanceFrom(Point * point) {
        if (this->front <= point->z && point->z <= this->back) {
            return 0;
        }

        return min(
            abs(point->z - this->front),
            abs(point->z - this->back)
        );
    }

    // Skips Math.sqrt for faster comparisons
    double sqDistanceFrom(Point * point) {
        double dx = this->xDistanceFrom(point);
        double dy = this->yDistanceFrom(point);
        double dz = this->zDistanceFrom(point);

        return dx * dx + dy * dy+ dz * dz;
    }

    // Pythagorus: a^2 = b^2 + c^2
    double distanceFrom(Point * point) {
        return sqrt(this->sqDistanceFrom(point));
    }
};

// circle class for a circle shaped query
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
        // check if the point is in the circle by checking if the euclidean distance of
        // the point and the center of the circle if smaller or equal to the radius of
        // the circle
        double d = pow((point.x - this->x), 2) + pow((point.y - this->y), 2);
        return d <= this->rSquared;
    }

    double intersects(Cube range) {

        double xDist = abs(range.x - this->x);
        double yDist = abs(range.y - this->y);
        double zDist = abs(range.z - this->z);

        // radius of the circle
        double r = this->r;

        double w = range.w / 2;
        double h = range.h / 2;
        double l = range.l / 2;

        double edges = pow((xDist - w), 2) + pow((yDist - h), 2) + pow((zDist - l), 2);

        // no intersection
        if (xDist > (r + w) || yDist > (r + h) || zDist > (r + l))
            return false;

        // intersection within the circle
        if (xDist <= w || yDist <= h || zDist <= l)
            return true;

        // intersection on the edge of the circle
        return edges <= this->rSquared;
    }
};

struct OctTree {
    int MAX_DEPTH = 8;

    Cube boundary;
    int capacity;
    std::vector<Point *> points;
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

        delete northeastup;
        delete northwestup;
        delete southeastup;
        delete southwestup;
        delete northeastdown;
        delete northwestdown;
        delete southeastdown;
        delete southwestdown;
        northeastup=new OctTree();
        northwestup = new OctTree();
        southeastup = new OctTree();
        southwestup = new OctTree();
        northeastdown = new OctTree();
        northwestdown = new OctTree();
        southeastdown = new OctTree();
        southwestdown = new OctTree();
        northeastup=nullptr;
        northwestup=nullptr;
        southeastup=nullptr;
        southwestup=nullptr;
        northeastdown=nullptr;
        northwestdown=nullptr;
        southeastdown=nullptr;
        southwestdown=nullptr;
    }

    std::vector<OctTree *> get_children() {
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
        for (int i = 0; i < points.size();i++) {
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

    bool insert(Point * point) {
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

    std::vector<Point *> query(Cube range, std::vector<Point*> * found=nullptr) {
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

        for (int i = 0; i < points.size();i++) {
            if (range.contains(points[i])) {
                found->push_back(points[i]);
            }
        }

        return *found;
    }

    std::vector<Point*> closest(Point* searchPoint, int maxCount = 1, int maxDistance = 9999999999999999) {
        double sqMaxDistance = pow(maxDistance ,2);
        return kNearest(searchPoint, maxCount, sqMaxDistance, 0, 0).found;
    }

    struct kNerestSet {
        std::vector<Point*> found;
        double furthestSqDistance;
    };

    kNerestSet kNearest(Point * searchPoint, int maxCount,double sqMaxDistance,double furthestSqDistance,int foundSoFar) {
        std::vector<Point*> found;

        if (divided) {
            std::vector<OctTree*> children = get_children();

            std::sort(children.begin(), children.end(),[searchPoint](OctTree* a, OctTree* b){
                a->boundary.sqDistanceFrom(searchPoint) - b->boundary.sqDistanceFrom(searchPoint);
                });

            for (int i = 0; i < children.size(); i++)
            {
                double sqDistance = children[i]->boundary.sqDistanceFrom(searchPoint);
                if (sqDistance > sqMaxDistance) {
                    return;
                }
                else if (foundSoFar < maxCount || sqDistance < furthestSqDistance) {
                    kNerestSet result = children[i]->kNearest(searchPoint, maxCount, sqMaxDistance, furthestSqDistance, foundSoFar);
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
                a->sqDistanceFrom(searchPoint) - b->sqDistanceFrom(searchPoint);
                });
            for (int i = 0; i < points.size(); i++)
            {
                double sqDistance = points[i]->sqDistanceFrom(searchPoint);
                if (sqDistance > sqMaxDistance) {
                    return;
                }
                else if (foundSoFar < maxCount || sqDistance < furthestSqDistance) {
                    found.push_back(points[i]);
                    furthestSqDistance = max(sqDistance, furthestSqDistance);
                    foundSoFar++;
                }
            }
        }

        std::sort(found.begin(), found.end(), [searchPoint](Point* a,Point* b) {
            a->sqDistanceFrom(searchPoint) - b->sqDistanceFrom(searchPoint);
            });

        kNerestSet ret;
        ret.found = found;
        ret.furthestSqDistance=sqrt(furthestSqDistance);
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
                southeastup->get_length()+
                northwestdown->get_length() +
                northeastdown->get_length() +
                southwestdown->get_length() +
                southeastdown->get_length()
                );
        }

        return points.size();
    }

    void Draw() {
        boundary.Draw();

        if (divided) {
            northwestup->Draw();
            northeastup->Draw();
            southwestup->Draw();
            southeastup->Draw();
            northwestdown->Draw();
            northeastdown->Draw();
            southwestdown->Draw();
            southeastdown->Draw();
        }
    }
};
