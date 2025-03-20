#pragma once

#include "Math/Vector.h"

class MBounds
{
public:
	MBounds() { Clear(); }
	MBounds( const MBounds& rhs ) : mins( rhs.mins ), maxs( rhs.maxs ) {}
	const MBounds& operator =( const MBounds& rhs );
	~MBounds() {}
	
	void Clear() { mins = Vec3( 1e6 ); maxs = Vec3( -1e6 ); }
	
	bool DoesIntersect( const MBounds& rhs ) const;
	
	void Expand( const Vec3 * pts, const int num );
	void Expand( const Vec3& rhs );
	void Expand( const MBounds& rhs );
	
	float WidthX() const { return maxs.x - mins.x; }
	float WidthY() const { return maxs.y - mins.y; }
	float WidthZ() const { return maxs.z - mins.z; }
	
public:
	Vec3 mins;
	Vec3 maxs;
};
