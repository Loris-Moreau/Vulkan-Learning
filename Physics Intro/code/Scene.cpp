//
//  Scene.cpp
//
#include "Scene.h"

#include <iostream>
#include <random>
#include <string>

#include "Broadphase.h"
#include "Intersections.h"
#include "../Shape.h"


/*
========================================================================================================

Scene

========================================================================================================
*/

/*
====================================================
Scene::~Scene
====================================================
*/
Scene::~Scene()
{
	for (auto& bodie : bodies)
	{
		delete bodie.shape;
	}
	bodies.clear();
}

/*
====================================================
Scene::Reset
====================================================
*/
void Scene::Reset()
{
	for (auto& bodie : bodies)
	{
		delete bodie.shape;
	}
	bodies.clear();

	Initialize();
}

/*
====================================================
Scene::Initialize
====================================================
*/
void Scene::Initialize()
{
	// Random Generator
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	//std::uniform_int_distribution<> distr(-7, 7); // define the range int
	std::uniform_real_distribution<> test(-7.5f,7.5f); // define the range float
	
	
	Body body;
	// Cochonet
	float radius = 0.25f;
	float x = 0.0f;
	float y = 0.0f;
	body.position = Vec3(x, y, 10);
	body.orientation = Quat(0, 0, 0, 1);
	body.shape = new ShapeSphere(radius);
	body.inverseMass = 1.0f;
	body.elasticity = 0.3f;
	body.friction = 0.4f;
	
	// Random Direction for the Cochonet
	float impulseDirectionX = floorf((float)test(gen) * 10) / 10;
	float impulseDirectionY = floorf((float)test(gen) * 10) / 10;
	
	body.linearVelocity = Vec3(impulseDirectionX, impulseDirectionY, 0);
	bodies.push_back(body);
	// end of Cochonet

	// Debug
	//std::cout << impulseDirectionX << " " << impulseDirectionY <<'\n';
	
	// Balls
	radius *= 3; // 4 Times the radius of the cochonet
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 1; ++j)
		{
			x = (float)(i - 1) * radius * 1.5f;
			y = (float)(j - 1) * radius * 1.5f;
			body.position = Vec3(x, y, 100);
			body.orientation = Quat(0, 0, 0, 1);
			body.shape = new ShapeSphere(radius);
			body.inverseMass = 0.85f;
			body.elasticity = 0.2f;
			body.friction = 0.65f;
			body.linearVelocity = Vec3(5, 5, 0);;
			bodies.push_back(body);
		}
	}
	// end of Balls
	
	// Floor
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			radius = 80.0f;
			x = (float)(i - 1) * radius * 0.25f;
			y = (float)(j - 1) * radius * 0.25f;
			body.position = Vec3(x, y, -radius);
			body.orientation = Quat(0, 0, 0, 1);
			body.shape = new ShapeSphere(radius);
			body.inverseMass = 0.0f;
			body.elasticity = 0.99f;
			body.friction = 0.5f;
			body.linearVelocity.Zero();
			bodies.push_back(body);
		}
	}
	// end of Floor
	
	/* // Walls
	float amountWalls = 4;
	for (int i = 0; i < amountWalls; ++i)
	{
		for (int j = 0; j < amountWalls/2; ++j)
		{
			radius = 80.0f;
			x = (float)(i - 1) * radius * 0.25f + 175;
			y = (float)(j - 1) * radius * 0.25f;
			body.position = Vec3(x, y, 10);
			body.orientation = Quat(0, 0, 0, 1);
			body.shape = new ShapeSphere(radius);
			bodies.push_back(body);
		}
	}
	// end of Walls
	*/
}

/*
====================================================
Scene::Update
====================================================
*/
void Scene::Update(const float dt_sec)
{
	// Gravity
	for (int i = 0; i < bodies.size(); ++i)
	{
		Body& body = bodies[i];
		float mass = 1.0f / body.inverseMass;
		// Gravity needs to be an impulse I
		// I == dp, so F == dp/dt <=> dp = F * dt
		// <=> I = F * dt <=> I = m * g * dt
		Vec3 impulseGravity = Vec3(0, 0, -10) * mass * dt_sec;
		body.ApplyImpulseLinear(impulseGravity);
	}
	
	// Broadphase
	std::vector<CollisionPair> collisionPairs;
	BroadPhase(bodies.data(), bodies.size(), collisionPairs, dt_sec);
	// Collision checks (Narrow phase)
	int numContacts = 0;
	const int maxContacts = bodies.size() * bodies.size();
	Contact* contacts = (Contact*)alloca(sizeof(Contact) * maxContacts);
	for (int i = 0; i < collisionPairs.size(); ++i)
	{
		const CollisionPair& pair = collisionPairs[i];
		Body& bodyA = bodies[pair.a];
		Body& bodyB = bodies[pair.b];
		if (bodyA.inverseMass == 0.0f && bodyB.inverseMass == 0.0f) continue;
		
		Contact contact;
		if (Intersections::Intersect(bodyA, bodyB, dt_sec, contact))
		{
			contacts[numContacts] = contact;
			++numContacts;
		}
	}
	// Sort times of impact
	if (numContacts > 1)
	{
		qsort(contacts, numContacts, sizeof(Contact), Contact::CompareContact);
	}
	
	// Contact resolve in order
	float accumulatedTime = 0.0f;
	for (int i = 0; i < numContacts; ++i)
	{
		Contact& contact = contacts[i];
		const float dt = contact.timeOfImpact - accumulatedTime;
		
		Body* bodyA = contact.a;
		Body* bodyB = contact.b;
		
		// Skip body par with infinite mass
		if (bodyA->inverseMass == 0.0f && bodyB->inverseMass == 0.0f) continue;
		
		// Position update
		for (auto& bodie : bodies)
		{
			bodie.Update(dt);
		}
		Contact::ResolveContact(contact);
		accumulatedTime += dt;
	}
	
	// Other physics behaviours, outside collisions.
	// Update the positions for the rest of this frame's time.
	const float timeRemaining = dt_sec - accumulatedTime;
	if (timeRemaining > 0.0f)
	{
		for (auto& bodie : bodies)
		{
			bodie.Update(timeRemaining);
		}
	}
}
