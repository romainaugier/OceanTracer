#pragma once

#include "mat44.h"
#include "sampling.h"
#include "camera.h"
#include "settings.h"
#include "GL/glew.h"
#include "sky.h"
#include "ocean.h"
#include "tbb/tbb.h"

#include <vector>
#include <iostream>

typedef struct { GLfloat R, G, B; } color;

#undef min, max

struct alignas(32) Tile
{
	color* pixels = nullptr;
	float* randoms;

	uint16_t x_start, x_end;
	uint16_t y_start, y_end;
	uint16_t id;

	uint8_t size_x = 16; // 16x16 size
	uint8_t size_y = 16;
};

struct Tiles
{
	std::vector<Tile> tiles;
	
	uint16_t count;
};

void GenerateTiles(Tiles& tiles, 
				   const Settings& settings) noexcept;

void ReleaseTiles(Tiles& tiles) noexcept;

void SetTilePixel(Tile& tile, const vec3& color, uint32_t x, uint32_t y) noexcept;

void Render(color* __restrict buffer,
		    const Ocean& ocean,
			const Sky& sky,
			const uint32_t* blueNoise,
			const uint64_t& seed, 
			const uint64_t& sample,
			const Tiles& tiles, 
			const Camera& cam, 
			const Settings& settings) noexcept;

void RenderTile(const Ocean& ocean,
				const Sky& sky,
				const uint32_t* blueNoise,
				const uint64_t& seed,
				const uint64_t& sample,
				const Tile& tile,
				const Camera& cam,
				const Settings& settings) noexcept;

vec3 Pathtrace(const Ocean& ocean,
			   const Sky& sky,
			   const uint32_t* blueNoise,
			   const uint32_t x,
               const uint32_t y,
			   const uint32_t sample,
			   const uint32_t seed,
			   RayHit& rayhit) noexcept;