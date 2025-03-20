#pragma once

#include "code/Math/Vector.h"

// Gilbert-Johnson-Keerthi (GJK)

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
    int CompareSigns(float a, float b);

    Vec2 SignedVolume1D(const Vec3& s1, const Vec3& s2);

    Vec3 SignedVolume2D(const Vec3& s1, const Vec3& s2, const Vec3& s3);

    Vec4 SignedVolume3D(const Vec3& s1, const Vec3& s2, const Vec3& s3, const Vec3& s4);

    void TestSignedVolumeProjection();

    // Projects the origin onto the simplex to acquire the new search direction,
    // also checks if the origin is "inside" the simplex.
    bool SimplexSignedVolumes(Point* pts, const int num, Vec3& newDir, Vec4& lambdasOut);

    // Checks whether the new point already exists in the simplex
    bool HasPoint(const Point simplexPoints[4], const Point& newPt);

    // Sorts the valid support points to the beginning of the array
    void SortValids(Point simplexPoints[4], Vec4& lambdas);

    static int NumValids(const Vec4& lambdas);

    bool GJK_DoesIntersect(const Body* bodyA, const Body* bodyB);
};
