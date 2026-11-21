#pragma once
#include <Rendering/Essentials/Vertex.h>
#include <sol/sol.hpp>
#include "Core/ECS/Registry.h"

namespace Scion::Core::ECS
{
/*
 * UV coordinates tell opengl which part of the image that should be used
 * for each triangle when adding textures to a sprite or mesh.
 */
struct UVs
{
	float u{ 0.f };
	float v{ 0.f };
	float uvWidth{ 0.f };
	float uvHeight{ 0.f };
};

struct SpriteComponent
{
	/* The string name of the texture. Used to get the texture from the asset manager. */
	std::string sTextureName{};
	/* The width of the sprite in pixels. */
	float width{ 16.f };
	/* The height of the sprite in pixels. */
	float height{ 16.f };
	/* The UV values to be used for the sprite texture. Usually generated upon creation. */
	UVs uvs{ .u = 0.f, .v = 0.f, .uvWidth = 0.f, .uvHeight = 0.f };
	/* The color to add to the sprite. Defaults to white. */
	Scion::Rendering::Color color{ .r = 255, .g = 255, .b = 255, .a = 255 };
	/* The start position of the x coordinate for sprite sheets. Used to aid in generating the uv values. */
	int startX{ 0 };
	/* The start position of the y coordinate for sprite sheets. Used to aid in generating the uv values. */
	int startY{ 0 };
	/* The layer or z-index of the sprite to be drawn at. */
	int layer{ 0 };
	/* Should the sprite be drawn or hidden? */
	bool bHidden{ false };
	/* Is the tile isometric? */
	bool bIsoMetric{ false };
	/* Iso cell is needed to sort when rendering. */
	int isoCellX{ 0 };
	/* Iso cell is needed to sort when rendering. */
	int isoCellY{ 0 };
	/* The actual uv width, without texel compensation. Needed for Animation system. */
	float uvWidthUnpadded{ 0.f };
	/* The actual uv height, without texel compensation. Needed for Animation system. */
	float uvHeightUnpadded{ 0.f };
	/* The size of a texel for the x axis based on the width of the texture. */
	float texelOffsetX{ 0.f };
	/* The size of a texel for the y axis based on the width of the texture. */
	float texelOffsetY{ 0.f };

	bool bFlipX{ false };
	bool bFlipY{ false };
	// void generate_uvs( int textureWidth, int textureHeight );
	[[nodiscard]] std::string to_string() const;

	static void CreateSpriteLuaBind( sol::state& lua );
};
} // namespace Scion::Core::ECS
