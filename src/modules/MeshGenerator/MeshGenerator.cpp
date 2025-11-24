#include "MeshGenerator.hpp"

#include "MeshWrapper/MeshWrapper.hpp"

#include <raymath.h>

Mesh MeshGenerator::generateMesh(const TerrainData& terrainData) {
    const Vector2 resolution {
        .x = terrainData.resolutionX, .y = terrainData.resolutionZ
    };

    const Vector3 centerOffset {
        .x = -float(resolution.x - 1) / 2.0f,
        .y = 0.0f,
        .z = +float(resolution.y - 1) / 2.0f
    };

    float heightScale = 60.f;

    MeshWrapper meshWrapper {resolution};
    for (uint32_t z = 0; z < terrainData.resolutionZ - 1; z++) {
        for (uint32_t x = 0; x < terrainData.resolutionX - 1; x++) {
            // 4 Vertices - A,B,C,D
            // A-----B
            // |   / |
            // | /   |
            // C-----D

            Vector3 pointA {
                .x = float(x),
                .y = terrainData.heightAt(x, z) * heightScale,
                .z = float(z)
            };
            pointA = Vector3Add(pointA, centerOffset);

            Vector3 pointB {
                .x = float(x) + 1,
                .y = terrainData.heightAt(x + 1, z) * heightScale,
                .z = float(z)
            };
            pointB = Vector3Add(pointB, centerOffset);

            Vector3 pointC {
                .x = float(x),
                .y = terrainData.heightAt(x, z + 1) * heightScale,
                .z = float(z) + 1
            };
            pointC = Vector3Add(pointC, centerOffset);

            Vector3 pointD {
                .x = float(x) + 1,
                .y = terrainData.heightAt(x + 1, z + 1) * heightScale,
                .z = float(z) + 1
            };
            pointD = Vector3Add(pointD, centerOffset);

            meshWrapper.addTraingle(pointA, pointC, pointB, resolution);
            meshWrapper.addTraingle(pointB, pointC, pointD, resolution);
        }
    }

    return meshWrapper.getMesh();
}
