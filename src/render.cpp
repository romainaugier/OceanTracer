#include "render.h"

void GenerateTiles(Tiles& tiles, 
                   const Settings& settings) noexcept
{   
    constexpr uint8_t tilesize = 16;

    const uint16_t tileCountX = maths::ceil(settings.xres / tilesize);
    const uint16_t tileCountY = maths::ceil(settings.yres / tilesize);

    const uint8_t lastTileSizeX = maths::ceil(maths::fmod(settings.xres, tilesize)) == 0 ? 32 : maths::ceil(maths::fmod(settings.xres, tilesize));
    const uint8_t lastTileSizeY = maths::ceil(maths::fmod(settings.yres, tilesize)) == 0 ? 32 : maths::ceil(maths::fmod(settings.yres, tilesize));

    tiles.count = tileCountX * tileCountY;

    tiles.tiles.reserve(tiles.count);

    uint16_t idx = 0;

    for (int y = 0; y < settings.yres; y += tilesize)
    {
        for (int x = 0; x < settings.xres; x += tilesize)
        {
            Tile tmpTile;
            tmpTile.id = idx;
            tmpTile.x_start = x; tmpTile.x_end = x + tilesize;
            tmpTile.y_start = y; tmpTile.y_end = y + tilesize;

            if (x + tilesize > settings.xres)
            {
                tmpTile.x_end = x + lastTileSizeX;
                tmpTile.size_x = lastTileSizeX;
            }
            if (y + tilesize > settings.yres)
            {
                tmpTile.y_end = y + lastTileSizeY;
                tmpTile.size_y = lastTileSizeY;
            }

            tmpTile.pixels = new color[tmpTile.size_x * tmpTile.size_y];
            tmpTile.randoms = new float[tmpTile.size_x * tmpTile.size_y * 2];

            tiles.tiles.push_back(tmpTile);

            idx++;
        }
    }
}

void ReleaseTiles(Tiles& tiles) noexcept
{
    for (auto& tile : tiles.tiles)
    {
        delete[] tile.pixels;
        tile.pixels = nullptr;

        delete[] tile.randoms;
        tile.randoms = nullptr;
    }
}

void SetTilePixel(Tile& tile, const vec3& color, uint32_t x, uint32_t y) noexcept
{
    tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].R = color.x;
    tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].G = color.y;
    tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].B = color.z;
}

void Render(color* __restrict buffer,
            const Ocean& ocean,
            const Sky& sky,
            const uint32_t* blueNoise,
            const uint64_t& seed,
            const uint64_t& sample,
            const Tiles& tiles, 
            const Camera& cam, 
            const Settings& settings) noexcept
{
    static tbb::affinity_partitioner partitioner;

    constexpr float gamma = 1.0f / 2.2f;

    tbb::parallel_for(tbb::blocked_range<size_t>(0, tiles.count), [&](const tbb::blocked_range<size_t>& r)
        {
            for (size_t t = r.begin(), t_end = r.end(); t < t_end; t++)
            {
                RenderTile(ocean, sky, blueNoise, seed, sample, tiles.tiles[t], cam, settings);

                for (int y = 0; y < tiles.tiles[t].size_y; y++)
                {
                    for (int x = 0; x < tiles.tiles[t].size_x; x++)
                    {
                        buffer[tiles.tiles[t].x_start + x + (tiles.tiles[t].y_start + y) * settings.xres].R = maths::pow(tiles.tiles[t].pixels[x + y * tiles.tiles[t].size_x].R, gamma);
                        buffer[tiles.tiles[t].x_start + x + (tiles.tiles[t].y_start + y) * settings.xres].G = maths::pow(tiles.tiles[t].pixels[x + y * tiles.tiles[t].size_x].G, gamma);
                        buffer[tiles.tiles[t].x_start + x + (tiles.tiles[t].y_start + y) * settings.xres].B = maths::pow(tiles.tiles[t].pixels[x + y * tiles.tiles[t].size_x].B, gamma);
                    }
                }
            }

        }, partitioner);
}

void RenderTile(const Ocean& ocean,
                const Sky& sky,
                const uint32_t* blueNoise,
                const uint64_t& seed,
                const uint64_t& sample,
                const Tile& tile,
                const Camera& cam,
                const Settings& settings) noexcept
{
    // Method to see if we intersect something in the tile by tracing a ray at each corner. If we don't, just 
    // sample the background and do another tile
    // Not very robust, but it will get somewhere
    uint32_t pos2d[8] = { tile.x_start, tile.y_start, tile.x_end - 1, tile.y_start,
                          tile.x_start, tile.y_end - 1, tile.x_end, tile.y_end - 1 };

    bool hasHitSomething = false;

    for(uint8_t i = 0; i < 4; i++)
    {
        RayHit tmpRayHit;

        SetPrimaryRay(tmpRayHit, cam, pos2d[i * 2], pos2d[i * 2 + 1], settings.xres, settings.yres, blueNoise, sample);

        if(Intersect(ocean, tmpRayHit))
        {
            hasHitSomething = true;
            break;
        }
    }

    if(hasHitSomething)
    {
        for (int y = tile.y_start; y < tile.y_end; y++)
        {
            for (int x = tile.x_start; x < tile.x_end; x++)
            {
                vec3 output = vec3(maths::constants::zero);

                RayHit tmpRayHit;

                SetPrimaryRay(tmpRayHit, cam, x, y, settings.xres, settings.yres, blueNoise, sample);
                
                if(Intersect(ocean, tmpRayHit))
                {
                    if(Raymarch(ocean, tmpRayHit, settings.time))
                    {
                        const vec3 hitNormal = WaveNormal(ocean, vec2(tmpRayHit.hit.pos.x, tmpRayHit.hit.pos.z), settings.time);
                        const vec3 r = reflect(tmpRayHit.ray.direction, hitNormal);
                        // output = lerp(vec3(0.0f, 1.0f, 0.0f), hitNormal, 1.0f / (tmpRayHit.ray.t * 0.01f + 1.0f));
                        output = SampleSky(r, sky);
                    }
                    else
                    {
                        output = SampleSky(tmpRayHit.ray.direction, sky);
                    }
                }
                else
                {
                    output = SampleSky(tmpRayHit.ray.direction, sky);
                }

                const vec3 outputCorrected = vec3(std::isnan(output.x) ? 0.5f : output.x, 
                                                  std::isnan(output.y) ? 0.5f : output.y, 
                                                  std::isnan(output.z) ? 0.5f : output.z);

                // const float pixelR = tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].R;
                // const float pixelG = tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].G;
                // const float pixelB = tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].B;

                // tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].R = maths::lerp(pixelR, outputCorrected.x, 1.0f / static_cast<float>(sample));
                // tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].G = maths::lerp(pixelG, outputCorrected.y, 1.0f / static_cast<float>(sample));
                // tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].B = maths::lerp(pixelB, outputCorrected.z, 1.0f / static_cast<float>(sample));
                
                tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].R = outputCorrected.x;
                tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].G = outputCorrected.y;
                tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].B = outputCorrected.z;
            }
        }
    }
    else
    {
        for (int y = tile.y_start; y < tile.y_end; y++)
        {
            for (int x = tile.x_start; x < tile.x_end; x++)
            {
                RayHit tmpRayHit;

                SetPrimaryRay(tmpRayHit, cam, x, y, settings.xres, settings.yres, blueNoise, sample);
                
                const vec3 output = SampleSky(tmpRayHit.ray.direction, sky);

                const float pixelR = tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].R;
                const float pixelG = tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].G;
                const float pixelB = tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].B;

                tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].R = maths::lerp(pixelR, output.x, 1.0f / static_cast<float>(sample));
                tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].G = maths::lerp(pixelG, output.y, 1.0f / static_cast<float>(sample));
                tile.pixels[(x - tile.x_start) + (y - tile.y_start) * tile.size_y].B = maths::lerp(pixelB, output.z, 1.0f / static_cast<float>(sample));
            }
        }
    }
}

vec3 Pathtrace(const Ocean& ocean,
               const Sky& sky,
               const uint32_t* blueNoise,
               const uint32_t x,
               const uint32_t y,
			   const uint32_t sample,
               const uint32_t seed, 
               RayHit& rayhit) noexcept
{

}
