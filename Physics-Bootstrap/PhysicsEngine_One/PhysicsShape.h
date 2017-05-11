#pragma once

class PhysicsShape
{
public:
	enum class ShapeType
	{
		Plane,
		AABB,
		Sphere
	};

	PhysicsShape() = delete;
	PhysicsShape(ShapeType type) : m_type(type) {};
	~PhysicsShape() {}

	ShapeType GetType() const { return m_type; }

private:
	// Each Shape will have a type variable
	ShapeType m_type;
};