#pragma once

class IScene {
public:
    virtual void setup() = 0;
    virtual void draw() = 0;
};