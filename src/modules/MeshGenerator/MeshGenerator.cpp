#include "MeshGenerator.hpp"

#include <raymath.h>

void MeshGenerator::setupMesh(
    Mesh& mesh,
    const TerrainData& data,
    uint32_t totalQuadsNumber
) {
    mesh.vertexCount = totalQuadsNumber * VERTICES_PER_QUAD;
    mesh.vertices = (float*) MemAlloc(
        mesh.vertexCount * COORDS_PER_VERTEX * sizeof(float)
    );

    mesh.texcoords = (float*) MemAlloc(
        mesh.vertexCount * COORDS_PER_TEXTURE * sizeof(float)
    );

    mesh.colors = (uint8_t*) MemAlloc(
        mesh.vertexCount * CHANNELS_PER_COLOR * sizeof(uint8_t)
    );
}

void MeshGenerator::addTriangles(
    Mesh& mesh,
    const TerrainData& data,
    const Vector3& worldSize,
    uint32_t startQuad,
    uint32_t endQuad
) {
    // A-----B
    // |   / |
    // | /   |
    // C-----D
    //
    // CounterClockwise
    // A, C, B
    // B, C, D

    const Vector3 scale {
        worldSize.x / (data.resolutionX - 1),
        worldSize.y,
        worldSize.z / (data.resolutionZ - 1)
    };

    const Vector3 offset {
        -(float(data.resolutionX) - 1) / 2.0f,
        0.0f,
        -(float(data.resolutionZ) - 1) / 2.0f
    };

    const uint32_t quadsPerRow = data.resolutionX - 1;

    uint32_t vertexIndex =
        startQuad * VERTICES_PER_QUAD * COORDS_PER_VERTEX;

    for (uint32_t quad = startQuad; quad < endQuad; quad++) {
        const uint32_t x = quad % quadsPerRow;
        const uint32_t z = quad / quadsPerRow;

        // A
        mesh.vertices[vertexIndex + 0] = (float(x) + offset.x) * scale.x;
        mesh.vertices[vertexIndex + 1] =
            (float(data.heightAt(x, z)) + offset.y) * scale.y;
        mesh.vertices[vertexIndex + 2] = (float(z) + offset.z) * scale.z;

        vertexIndex += 3;

        // C
        mesh.vertices[vertexIndex + 0] = (float(x) + offset.x) * scale.x;
        mesh.vertices[vertexIndex + 1] =
            (float(data.heightAt(x, z + 1)) + offset.y) * scale.y;
        mesh.vertices[vertexIndex + 2] =
            (float(z + 1) + offset.z) * scale.z;

        vertexIndex += 3;

        // B
        mesh.vertices[vertexIndex + 0] =
            (float(x + 1) + offset.x) * scale.x;
        mesh.vertices[vertexIndex + 1] =
            (float(data.heightAt(x + 1, z)) + offset.y) * scale.y;
        mesh.vertices[vertexIndex + 2] = (float(z) + offset.z) * scale.z;

        vertexIndex += 3;

        // B
        mesh.vertices[vertexIndex + 0] =
            (float(x + 1) + offset.x) * scale.x;
        mesh.vertices[vertexIndex + 1] =
            (float(data.heightAt(x + 1, z)) + offset.y) * scale.y;
        mesh.vertices[vertexIndex + 2] = (float(z) + offset.z) * scale.z;

        vertexIndex += 3;

        // C
        mesh.vertices[vertexIndex + 0] = (float(x) + offset.x) * scale.x;
        mesh.vertices[vertexIndex + 1] =
            (float(data.heightAt(x, z + 1)) + offset.y) * scale.y;
        mesh.vertices[vertexIndex + 2] =
            (float(z + 1) + offset.z) * scale.z;

        vertexIndex += 3;

        // D
        mesh.vertices[vertexIndex + 0] =
            (float(x + 1) + offset.x) * scale.x;
        mesh.vertices[vertexIndex + 1] =
            (float(data.heightAt(x + 1, z + 1)) + offset.y) * scale.y;
        mesh.vertices[vertexIndex + 2] =
            (float(z + 1) + offset.z) * scale.z;

        vertexIndex += 3;
    }
}

void MeshGenerator::addTextureCoords(
    Mesh& mesh,
    const TerrainData& data,
    uint32_t startQuad,
    uint32_t endQuad
) {
    const uint32_t quadsPerRow = data.resolutionX - 1;

    uint32_t coordIndex =
        startQuad * VERTICES_PER_QUAD * COORDS_PER_TEXTURE;

    for (uint32_t quad = startQuad; quad < endQuad; quad++) {
        const uint32_t x = quad % quadsPerRow;
        const uint32_t z = quad / quadsPerRow;

        // A
        mesh.texcoords[coordIndex + 0] =
            float(x) / float(data.resolutionX - 1);
        mesh.texcoords[coordIndex + 1] =
            float(z) / float(data.resolutionZ - 1);

        coordIndex += 2;

        // C
        mesh.texcoords[coordIndex + 0] =
            float(x) / float(data.resolutionX - 1);
        mesh.texcoords[coordIndex + 1] =
            float(z + 1) / float(data.resolutionZ - 1);

        coordIndex += 2;

        // B
        mesh.texcoords[coordIndex + 0] =
            float(x + 1) / float(data.resolutionX - 1);
        mesh.texcoords[coordIndex + 1] =
            float(z) / float(data.resolutionZ - 1);

        coordIndex += 2;

        // B
        mesh.texcoords[coordIndex + 0] =
            float(x + 1) / float(data.resolutionX - 1);
        mesh.texcoords[coordIndex + 1] =
            float(z) / float(data.resolutionZ - 1);

        coordIndex += 2;

        // C
        mesh.texcoords[coordIndex + 0] =
            float(x) / float(data.resolutionX - 1);
        mesh.texcoords[coordIndex + 1] =
            float(z + 1) / float(data.resolutionZ - 1);

        coordIndex += 2;

        // D
        mesh.texcoords[coordIndex + 0] =
            float(x + 1) / float(data.resolutionX - 1);
        mesh.texcoords[coordIndex + 1] =
            float(z + 1) / float(data.resolutionZ - 1);

        coordIndex += 2;
    }
}

void MeshGenerator::addLighting(
    Mesh& mesh,
    const TerrainData& data,
    uint32_t startQuad,
    uint32_t endQuad
) {
    const uint32_t quadsPerRow = data.resolutionX - 1;

    uint32_t colorIndex =
        startQuad * VERTICES_PER_QUAD * CHANNELS_PER_COLOR;

    for (uint32_t quad = startQuad; quad < endQuad; quad++) {
        const uint32_t x = quad % quadsPerRow;
        const uint32_t z = quad / quadsPerRow;

        // A
        uint8_t colorIntensity = 255;
        mesh.colors[colorIndex + 0] = colorIntensity;
        mesh.colors[colorIndex + 1] = colorIntensity;
        mesh.colors[colorIndex + 2] = colorIntensity;
        mesh.colors[colorIndex + 3] = 255;

        colorIndex += 4;

        // C
        colorIntensity = 255;
        mesh.colors[colorIndex + 0] = colorIntensity;
        mesh.colors[colorIndex + 1] = colorIntensity;
        mesh.colors[colorIndex + 2] = colorIntensity;
        mesh.colors[colorIndex + 3] = 255;

        colorIndex += 4;

        // B
        colorIntensity = 255;
        mesh.colors[colorIndex + 0] = colorIntensity;
        mesh.colors[colorIndex + 1] = colorIntensity;
        mesh.colors[colorIndex + 2] = colorIntensity;
        mesh.colors[colorIndex + 3] = 255;

        colorIndex += 4;

        // B
        colorIntensity = 255;
        mesh.colors[colorIndex + 0] = colorIntensity;
        mesh.colors[colorIndex + 1] = colorIntensity;
        mesh.colors[colorIndex + 2] = colorIntensity;
        mesh.colors[colorIndex + 3] = 255;

        colorIndex += 4;

        // C
        colorIntensity = 255;
        mesh.colors[colorIndex + 0] = colorIntensity;
        mesh.colors[colorIndex + 1] = colorIntensity;
        mesh.colors[colorIndex + 2] = colorIntensity;
        mesh.colors[colorIndex + 3] = 255;

        colorIndex += 4;

        // D
        colorIntensity = 255;
        mesh.colors[colorIndex + 0] = colorIntensity;
        mesh.colors[colorIndex + 1] = colorIntensity;
        mesh.colors[colorIndex + 2] = colorIntensity;
        mesh.colors[colorIndex + 3] = 255;

        colorIndex += 4;
    }
}

Mesh MeshGenerator::generateMesh(
    const TerrainData& terrainData,
    const Vector3& worldSize
) {
    Mesh mesh {};

    const uint32_t totalQuadsNumber =
        (terrainData.resolutionX - 1) * (terrainData.resolutionZ - 1);

    setupMesh(mesh, terrainData, totalQuadsNumber);

    addTriangles(mesh, terrainData, worldSize, 0, totalQuadsNumber);
    addTextureCoords(mesh, terrainData, 0, totalQuadsNumber);
    addLighting(mesh, terrainData, 0, totalQuadsNumber);

    return mesh;
}

void MeshGenerator::updateMesh(
    Mesh& mesh,
    const TerrainData& terrainData,
    const Vector3& worldSize
) {
    // updateVertices(mesh, terrainData);
    // updateLighting(mesh, terrainData);
}