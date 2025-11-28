#pragma once

#include <raylib.h>
#include <stdint.h>

class MeshWrapper {
private:
    Mesh mesh_ {};

    uint32_t vertexIndex = 0;
    uint32_t textureCoordIndex = 0;
    uint32_t normalIndex = 0;
    uint32_t colorIndex = 0;

public:
    MeshWrapper(const Vector2& resolution);

    void addVertex(const Vector3& coords);

    void addTextureCoords(
        const Vector2& index,
        const Vector2& resolution
    );

    void addNormal(const Vector3 coords);

    void addColor(const Color& color);

    void addTriangle(
        const Vector3& pointA,
        const Vector3& pointB,
        const Vector3& pointC
    );

    void addTextureTriangle(
        const Vector2& pointAIndex,
        const Vector2& pointBIndex,
        const Vector2& pointCIndex,
        const Vector2& resolution
    );

    Mesh& getMesh();
};