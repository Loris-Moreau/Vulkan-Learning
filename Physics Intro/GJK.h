#pragma once
// Gilbert-Johnson-Keerthi (GJK)

#include "code/Contact.h"

#include "code/Math/Vector.h"
#include <vector>

struct Tri;
struct Edge;

class Body;

struct Point
{
    // The point on the minkowski sum
    Vec3 xyz;

    // The point on bodyA
    Vec3 ptA;

    // The point on bodyB
    Vec3 ptB;

    Point() : xyz(0.0f), ptA(0.0f), ptB(0.0f) {}

    const Point& operator = (const Point& rhs)
    {
        xyz = rhs.xyz;
        ptA = rhs.ptA;
        ptB = rhs.ptB;
        return *this;
    }

    bool operator == (const Point& rhs) const
    {
        return ((ptA == rhs.ptA) && (ptB == rhs.ptB) && (xyz == rhs.xyz));
    }
};

Point Support(const Body* bodyA, const Body* bodyB, Vec3 dir, const float bias);

class GJK
{
public:
    static int CompareSigns(float a, float b);

    static Vec2 SignedVolume1D(const Vec3& s1, const Vec3& s2);

    static Vec3 SignedVolume2D(const Vec3& s1, const Vec3& s2, const Vec3& s3);

    static Vec4 SignedVolume3D(const Vec3& s1, const Vec3& s2, const Vec3& s3, const Vec3& s4);

    void TestSignedVolumeProjection();

    // Projects the origin onto the simplex to acquire the new search direction,
    // also checks if the origin is "inside" the simplex.
    static bool SimplexSignedVolumes(Point* pts, const int num, Vec3& newDir, Vec4& lambdasOut);

    // Checks whether the new point already exists in the simplex
    static bool HasPoint(const Point simplexPoints[4], const Point& newPt);

    // Sorts the valid support points to the beginning of the array
    static void SortValids(Point simplexPoints[4], Vec4& lambdas);

    static int NumValids(const Vec4& lambdas);

    bool GJK_DoesIntersect(const Body* bodyA, const Body* bodyB);

    //
    // This borrows our signed volume code to perform the barycentric coordinates.
    static Vec3 BarycentricCoordinates(Vec3 s1, Vec3 s2, Vec3 s3, const Vec3& pt);

    static Vec3 NormalDirection(const Tri& tri, const std::vector< Point >& points);

    static float SignedDistanceToTriangle(const Tri& tri, const Vec3& pt, const std::vector<Point>& points);

    static int ClosestTriangle(const std::vector<Tri>& triangles, const std::vector<Point>& points);

    static bool HasPoint(const Vec3& w, const std::vector<Tri> triangles, const std::vector<Point>& points);

    static int RemoveTrianglesFacingPoint(const Vec3& pt, std::vector<Tri>& triangles, const std::vector<Point>& points);

    static void FindDanglingEdges(std::vector<Edge>& danglingEdges, const std::vector<Tri>& triangles);

    static float EPA_Expand(const Body* bodyA, const Body* bodyB, const float bias, const Point simplexPoints[4], Vec3& ptOnA, Vec3& ptOnB);


    static bool GJK_DoesIntersect(const Body* bodyA, const Body* bodyB, const float bias, Vec3& ptOnA, Vec3& ptOnB);
    
    static void GJK_ClosestPoints(const Body* bodyA, const Body* bodyB, Vec3& ptOnA, Vec3& ptOnB);
};
