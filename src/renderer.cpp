#include "vector3D.h"
#include "renderer.h"
#include "globals.h"
#include "raylib-cpp.hpp"
#include "camera.h"

// #TODO turn this into Compute renderer, not Destructive

// clang-format off
/*
- Inverts Y axis around the origin
- Centers origin on 0,0
*/
void Renderer::transformPoint(Vec3 &p)
{
    p.y_ = -p.y_;
    p.x_ += (GetScreenWidth()  - POINT_SIZE) / 2;
    p.y_ += (GetScreenHeight() - POINT_SIZE) / 2;
}

/* 
- Project the 3D Point onto the 2D screen
*/
void Renderer::projectPoint(Vec3 &p)
{
    p.z_ = p.z_ == 0 ? 0.01 : p.z_;

    float x = p.x_ / p.z_ * focal;
    float y = p.y_ / p.z_ * focal;

    p.x_ = x; 
    p.y_ = y; 
    // p.z = 0;
}

/*
- Projects 3D point to 2D
- Transforms to cartesian plane
*/
void Renderer::preparePoint(Vec3 &p)
{
    projectPoint(p);              // Convert from 3D to 2D
    transformPoint(p);            // Translate to cartesiasn plane
}

// Render the 2D point on screen in given Color
void Renderer::drawPoint(Vec3 &p, Color color)
{
    // Turn point into renderable rectangles with size
    raylib::Rectangle rect = {p.x_, p.y_, POINT_SIZE, POINT_SIZE};  
    rect.Draw(color); 
}

// - Rotate, Scale, Translate shape to its accurate position
void Renderer::transformShape(Shape &shape)
{
    // Rotate #TODO
    // Transform ={scale,translate,rotate}, project, render

    // Move
    for (auto &point : shape.vertices)
    {
        point.x_ += shape.position.x_;
        point.y_ += shape.position.y_;
        point.z_ += shape.position.z_;
        
    }
}

void Renderer::projectShape(Shape &shape)
{
    for (auto &point : shape.vertices)
    {
        preparePoint(point);
    }
}

/*
- Draws the points or their connections/edges on screen
*/
void Renderer::drawShape(Shape &shape)
{
    for(auto &edge: shape.edges) 
    {
        Vec3 A = shape.vertices[edge.first]; 
        Vec3 B = shape.vertices[edge.second];

        float offset = POINT_SIZE/2;
        DrawLine(A.x_+offset, A.y_+offset, B.x_+offset, B.y_+offset, edge.getColor());  // Render Edge
        // drawPoint(A,RED);
        // drawPoint(B,RED);
    }
}

void Renderer::transformWorld(World &world)
{
    for (auto &shape : world.shapes)
    {
        // shape.move({camDx,camDy,camDy});
        shape.position.x_ -= world.camera.position.x_;
        shape.position.y_ -= world.camera.position.y_;
        shape.position.z_ -= world.camera.position.z_;
        transformShape(shape);
    }
}

void Renderer::projectWorld(World & world)
{
    for (auto &shape : world.shapes)
    {
        projectShape(shape);
    }
}

// Draw given world to screen
void Renderer::drawWorld(World &world) 
{
    BeginDrawing();
    ClearBackground(BLACK);

    for(auto &shape: world.shapes) 
    {
        drawShape(shape);
    }

    EndDrawing();
}

/*
- Transforms
- Projects
- Prepares world for rendering to 2D screen
*/
void Renderer::prepareWorld(World &world)
{
    transformWorld(world);
    projectWorld(world);    
}

void Renderer::pushWorldIntoView(World &world, int amount)
{
    for (auto &shape : world.shapes)
    {
        for (auto &point : shape.vertices)
        {
            point.z_+=amount;
        }
    }
}

// Render world via render pipeline, Transform->Project->Draw
void Renderer::render(World &world)
{
    World        worldCopy = world;  // Copy the original to not cause floating drift in original world
    prepareWorld(worldCopy);         // Transform, Project, and Input
    drawWorld   (worldCopy);         // Draw to screen
}

// clang-format on
float Renderer::focal = 500.f;