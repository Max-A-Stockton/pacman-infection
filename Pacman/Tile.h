#pragma once

#include "S2D\S2D.h"

using namespace S2D;

/// <summary>
/// Controls collision detection
/// </summary>
enum class TileCollision
{
	/// <summary>
	/// Pacman can pass through
	/// </summary>
	Passable = 0,
	/// <summary>
	/// Pacman cannot pass through
	/// </summary>
	Impassable,
};

struct Tile
{
public:
	Texture2D* Texture;
	TileCollision Collision;

	static const int Width;
	static const int Height;

	static const Vector2* Size;

	Tile(Texture2D* texture, TileCollision collision);
	~Tile(void);

};