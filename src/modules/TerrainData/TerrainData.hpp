#pragma once

#include "WaterDroplet/WaterDroplet.hpp"

#include <assert.h>
#include <raylib.h>
#include <stdint.h>
#include <vector>

class TerrainData {
private:
    const uint32_t resolutionX_;
    const uint32_t resolutionZ_;
    const Vector3 worldSize_;
    const Vector3 worldPosition_;
    const BoundingBox boundingBox_;

    std::vector<double> heightMap_;
    std::vector<Color> colorMap_;
    std::vector<WaterDroplet> rainMap_;

public:
    TerrainData(
        const uint32_t resolutionX,
        const uint32_t resolutionZ,
        const Vector3& worldSize,
        const Vector3& worldPosition,
        uint32_t rainDropsAmount
    );

    uint32_t getResolutionX() const;
    uint32_t getResolutionZ() const;

    const Vector3& getWorldSize() const;
    const Vector3& getWorldPosition() const;
    const BoundingBox& getBoundingBox() const;

    const std::vector<double>& getHeightMap() const;
    std::vector<double>& getHeightMap();

    const std::vector<Color>& getColorMap() const;
    std::vector<Color>& getColorMap();

    const std::vector<WaterDroplet>& getRainMap() const;
    std::vector<WaterDroplet>& getRainMap();

    bool isInsideBoundingBox(const Vector2& worldPosition) const;

    Vector2 worldPositionToIndices(const Vector2& worldPosition) const;
    Vector2 indicesToWorldPosition(uint32_t x, uint32_t z) const;

    double heightAt(uint32_t index) const;
    double heightAt(uint32_t x, uint32_t z) const;

    Color colorAt(uint32_t index) const;
    Color colorAt(uint32_t x, uint32_t z) const;
    Color& mutableColorAtWorld(const Vector2& worldPosition);
};