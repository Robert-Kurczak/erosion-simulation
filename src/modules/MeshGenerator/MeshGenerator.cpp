#include "MeshGenerator.hpp"

#include "MeshWrapper/MeshWrapper.hpp"

#include <cmath>
#include <raymath.h>

Vector3 MeshGenerator::getPoint(
    const Vector2& index,
    const Vector3& scale,
    const Vector3& offset,
    const TerrainData& terrainData
) {
    const Vector3 point {
        .x = index.x * scale.x,
        .y = float(terrainData.heightAt(index) * scale.y),
        .z = index.y * scale.z
    };

    return Vector3Add(point, offset);
}

Mesh MeshGenerator::generateMesh(
    const TerrainData& terrainData,
    const Vector3& worldSize
) {
    const Vector2 resolution {
        .x = float(terrainData.resolutionX),
        .y = float(terrainData.resolutionZ)
    };

    const Vector3 scale {
        .x = worldSize.x / resolution.x,
        .y = worldSize.y,
        .z = worldSize.z / resolution.y
    };

    const Vector3 centerOffset {
        .x = -(resolution.x - 1) * scale.x / 2.0f,
        .y = 0.0f,
        .z = -(resolution.y - 1) * scale.z / 2.0f
    };

    MeshWrapper meshWrapper {resolution};

    for (uint32_t z = 0; z < terrainData.resolutionZ - 1; z++) {
        for (uint32_t x = 0; x < terrainData.resolutionX - 1; x++) {
            // 6 Vertices per quad
            // A, C, B
            // B, C, D
            //
            // A-----B
            // |   / |
            // | /   |
            // C-----D

            const Vector2 pointAIndex {.x = float(x), .y = float(z)};
            const Vector3 pointA =
                getPoint(pointAIndex, scale, centerOffset, terrainData);

            const Vector2 pointBIndex {.x = float(x) + 1, .y = float(z)};
            const Vector3 pointB =
                getPoint(pointBIndex, scale, centerOffset, terrainData);

            const Vector2 pointCIndex {.x = float(x), .y = float(z) + 1};
            const Vector3 pointC =
                getPoint(pointCIndex, scale, centerOffset, terrainData);

            const Vector2 pointDIndex {
                .x = float(x) + 1, .y = float(z) + 1
            };
            const Vector3 pointD =
                getPoint(pointDIndex, scale, centerOffset, terrainData);

            meshWrapper.addTriangle(pointA, pointC, pointB);
            meshWrapper.addTextureTriangle(
                pointAIndex, pointBIndex, pointCIndex, resolution
            );

            meshWrapper.addTriangle(pointB, pointC, pointD);
            meshWrapper.addTextureTriangle(
                pointBIndex, pointCIndex, pointDIndex, resolution
            );
        }
    }

    return meshWrapper.getMesh();
}

void MeshGenerator::updateMesh(
    Mesh& mesh,
    const TerrainData& terrainData
) {}