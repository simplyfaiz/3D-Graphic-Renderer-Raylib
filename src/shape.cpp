#include "shape.h"
#include "raylib-cpp.hpp"
#include "vector3D.h"
#include <fstream>
#include <iostream>
#include <string>

void Shape::move(const Vec3 &delta)
{
    position.x_ += delta.x_;
    position.y_ += delta.y_;
    position.z_ += delta.z_;
}

void Shape::rotate(const Vec3 &delta)
{
    rotation.x_ += delta.x_;
    rotation.y_ += delta.y_;
    rotation.z_ += delta.z_;
}

void Shape::setColor(raylib::Color color)
{
    for (auto &edge : edges)
    {
        edge.setColor(color);
    }
}

// Saves shape as a text file in the given path, Default mode: override
void Shape::saveShape(std::string filePath)
{
    std::ofstream ofile(filePath);

    if (!ofile.is_open())
    {
        std::cout << "Save failed, Failed to open " << filePath + "\n";
        return;
    }

    ofile << "VERTICES " << vertices.size() << "\n";
    for (const auto &point : vertices)
    {
        ofile << point.x_ << " " << point.y_ << " " << point.z_ << "\n";
    }

    ofile << "EDGES " << edges.size() << "\n";
    for (const auto &edge : edges)
    {
        ofile << edge.first << " " << edge.second << " ";

        ofile << (int)edge.color.GetR() << " " << (int)edge.color.GetG() << " "
              << (int)edge.color.GetB() << " " << (int)edge.color.GetA()
              << "\n";
    }

    ofile << "TRANSFORM\n";
    ofile << position.x_ << " " << position.y_ << " " << position.z_ << "\n";
    ofile << rotation.x_ << " " << rotation.y_ << " " << rotation.z_ << "\n";
}

// Loads itself from a text file at the given path.
void Shape::loadShape(std::string filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cout << "Load failed, Failed to open " << filePath + "\n";
        return;
    }

    vertices.clear();
    edges.clear();

    std::string label;
    int count;
    
    // Vertices
    file >> label >> count; 
    if (label != "VERTICES") // "VERTICES N"
    {
        std::cout << "Invalid format (VERTICES)\n";
        return;
    }

    for (int i = 0; i < count; i++)
    {
        Vec3 v;
        file >> v.x_ >> v.y_ >> v.z_;
        vertices.push_back(v);
    }

    // Edges
    file >> label >> count; // "EDGES N"
    if (label != "EDGES")
    {
        std::cout << "Invalid format (EDGES)\n";
        return;
    }

    for (int i = 0; i < count; i++)
    {
        EdgeIndexPair edge(0, 0);

        int r, g, b, a;

        file >> edge.first >> edge.second >> r >> g >> b >> a;

        edge.setColor(raylib::Color(r, g, b, a));

        edges.push_back(edge);
    }

    // Transformations
    file >> label; // "TRANSFORM"
    if (label != "TRANSFORM")
    {
        std::cout << "Invalid format (TRANSFORM)\n";
        return;
    }

    file >> position.x_ >> position.y_ >> position.z_;
    file >> rotation.x_ >> rotation.y_ >> rotation.z_;
}

// Getter, Setter
void          Shape::EdgeIndexPair::setColor(raylib::Color c) { this-> color = c; }
raylib::Color Shape::EdgeIndexPair::getColor()                { return color; }
