#pragma once

#include "code/Math/Bounds.h"
#include "code/Math/Matrix.h"
#include "code/Math/Quat.h"

class Shape
{
public:
	enum class ShapeType
	{
		SHAPE_SPHERE,
	};
	
	virtual Mat3 InertiaTensor() const = 0;
	
	virtual ShapeType GetType() const = 0;
	virtual Vec3 GetCenterOfMass() const { return centerOfMass; }

	virtual Bounds GetBounds(const Vec3& pos, const Quat& orient) const;
	virtual Bounds GetBounds() const;
	
protected:
	Vec3 centerOfMass;
};

class ShapeSphere : public Shape
{
public:
	ShapeSphere(float radiusP) : radius(radiusP)
	{
		centerOfMass.Zero();
	}
	
	Mat3 InertiaTensor() const override;
	
	ShapeType GetType() const override { return ShapeType::SHAPE_SPHERE; }
	
	//Bounds GetBounds(const Vec3& pos, const Quat& orient) const override;
	//Bounds GetBounds() const override;
	
	Bounds GetBounds(const Vec3& pos, const Quat& orient) const override
	{
		Bounds tmp;
		tmp.mins = Vec3(-radius) + pos;
		tmp.maxs = Vec3(radius) + pos;
	
		return tmp;
	}

	Bounds GetBounds() const override
	{
		Bounds tmp;
		tmp.mins = Vec3(-radius);
		tmp.maxs = Vec3(radius);
	
		return tmp;
	}

	float radius;
};
