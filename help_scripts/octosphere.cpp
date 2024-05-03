#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

struct Point {
    float x;
    float y;
    float z;

    // Constructor
    Point(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f)
        : x(x_), y(y_), z(z_) {}

    // Normalize the point to the surface of the sphere
    void normalize() {
        float length = std::sqrt(x * x + y * y + z * z);
        if (length > 0.0f) {
            x /= length;
            y /= length;
            z /= length;
        }
    }
};

struct Triangle {
    int v1;
    int v2;
    int v3;

    // Constructor
    Triangle(int v1_ = 0, int v2_ = 0, int v3_ = 0)
        : v1(v1_), v2(v2_), v3(v3_) {}
};

// Function to generate a VTK file representing a sphere based on an octahedron
void generateVTKSphere(const std::string& filename, int n) {
    std::vector<Point> points;
    std::vector<Triangle> triangles;

    // Define the initial vertices of the octahedron
    points.emplace_back(1.0f, 0.0f, 0.0f);
    points.emplace_back(-1.0f, 0.0f, 0.0f);
    points.emplace_back(0.0f, 1.0f, 0.0f);
    points.emplace_back(0.0f, -1.0f, 0.0f);
    points.emplace_back(0.0f, 0.0f, 1.0f);
    points.emplace_back(0.0f, 0.0f, -1.0f);

    // Define the initial triangles of the octahedron
    triangles.emplace_back(0, 2, 4);
    triangles.emplace_back(0, 4, 3);
    triangles.emplace_back(0, 3, 5);
    triangles.emplace_back(0, 5, 2);
    triangles.emplace_back(1, 4, 2);
    triangles.emplace_back(1, 3, 4);
    triangles.emplace_back(1, 5, 3);
    triangles.emplace_back(1, 2, 5);

    // Subdivide each triangle n times
    for (int i = 0; i < n; ++i) {
        std::vector<Triangle> newTriangles;
        for (const auto& triangle : triangles) {
            // Get the vertices of the triangle
            Point p1 = points[triangle.v1];
            Point p2 = points[triangle.v2];
            Point p3 = points[triangle.v3];

            // Calculate the midpoints of the edges
            Point m1((p1.x + p2.x) / 2, (p1.y + p2.y) / 2, (p1.z + p2.z) / 2);
            Point m2((p2.x + p3.x) / 2, (p2.y + p3.y) / 2, (p2.z + p3.z) / 2);
            Point m3((p3.x + p1.x) / 2, (p3.y + p1.y) / 2, (p3.z + p1.z) / 2);

            // Normalize the midpoints to lie on the sphere
            m1.normalize();
            m2.normalize();
            m3.normalize();

            // Add the midpoints to the points list
            int midIndex1 = points.size();
            points.push_back(m1);
            int midIndex2 = points.size();
            points.push_back(m2);
            int midIndex3 = points.size();
            points.push_back(m3);

            // Create new triangles from the midpoints and original vertices
            newTriangles.emplace_back(triangle.v1, midIndex1, midIndex3);
            newTriangles.emplace_back(triangle.v2, midIndex2, midIndex1);
            newTriangles.emplace_back(triangle.v3, midIndex3, midIndex2);
            newTriangles.emplace_back(midIndex1, midIndex2, midIndex3);
        }

        // Update the triangles list
        triangles = newTriangles;
    }

    // Write the VTK file header
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }
    outFile << "# vtk DataFile Version 3.0\n";
    outFile << "Sphere example\n";
    outFile << "ASCII\n";
    outFile << "DATASET POLYDATA\n";

    // Write the points (vertices) section
    outFile << "POINTS " << points.size() << " float\n";
    for (const auto& point : points) {
        outFile << point.x << " " << point.y << " " << point.z << "\n";
    }

    // Write the polygons (triangles) section
    outFile << "POLYGONS " << triangles.size() << " " << triangles.size() * 4 << "\n"; // Each triangle has 3 vertices
    for (const auto& triangle : triangles) {
        outFile << "3 " << triangle.v1 << " " << triangle.v2 << " " << triangle.v3 << "\n";
    }

    // Close the output file
    outFile.close();

    std::cout << "VTK file of a sphere generated: " << filename << std::endl;
}

int main() {
    int n = 3; // Number of times to subdivide each face
    float radius = 3.0; // Radius of the sphere

    // Generate the VTK file of a sphere
    generateVTKSphere("sphere_octo3.vtk", n);

    std::cout << "VTK file of a sphere generated: sphere.vtk" << std::endl;
    return 0;
}