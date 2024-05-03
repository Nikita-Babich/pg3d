#include <iostream>
#include <fstream>
#include <cmath>

struct Point {
    float x;
    float y;
    float z;
};

// Function to generate a VTK file of a sphere
void generateVTKSphere(const std::string& filename, int latitudeSegments, int longitudeSegments, float radius) {
    std::ofstream outFile(filename);

    if (!outFile) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }

    // Write the VTK file header
    outFile << "# vtk DataFile Version 3.0\n";
    outFile << "Sphere example\n";
    outFile << "ASCII\n";
    outFile << "DATASET POLYDATA\n";

    // Calculate the total number of points (vertices)
    int totalPoints = (latitudeSegments + 1) * (longitudeSegments + 1);

    // Write the points (vertices) section
    outFile << "POINTS " << totalPoints << " float\n";
    for (int lat = 0; lat <= latitudeSegments; ++lat) {
        float theta = (lat * M_PI) / latitudeSegments; // Angle in radians
        for (int lon = 0; lon <= longitudeSegments; ++lon) {
            float phi = (lon * 2 * M_PI) / longitudeSegments; // Angle in radians

            // Calculate the x, y, and z coordinates of the point on the sphere
            float x = radius * sin(theta) * cos(phi);
            float y = radius * sin(theta) * sin(phi);
            float z = radius * cos(theta);

            // Write the point to the file
            outFile << x << " " << y << " " << z << "\n";
        }
    }

    // Calculate the total number of triangles (polygons)
    int totalTriangles = latitudeSegments * longitudeSegments * 2;

    // Write the polygons (triangles) section
    outFile << "POLYGONS " << totalTriangles << " " << totalTriangles * 4 << "\n"; // Each triangle has 3 vertices

    for (int lat = 0; lat < latitudeSegments; ++lat) {
        for (int lon = 0; lon < longitudeSegments; ++lon) {
            // Calculate the indices of the vertices that make up the 2 triangles for each segment

            // Indices of the vertices
            int v1 = lat * (longitudeSegments + 1) + lon;
            int v2 = v1 + 1;
            int v3 = (lat + 1) * (longitudeSegments + 1) + lon;
            int v4 = v3 + 1;

            // Triangle 1
            outFile << "3 " << v1 << " " << v2 << " " << v3 << "\n";
            // Triangle 2
            outFile << "3 " << v2 << " " << v4 << " " << v3 << "\n";
        }
    }

    // Close the output file
    outFile.close();
}

int main() {
    // Parameters for the sphere
    int latitudeSegments = 20; // Number of latitude segments
    int longitudeSegments = 20; // Number of longitude segments
    float radius = 3.0; // Radius of the sphere

    // Generate the VTK file of a sphere
    generateVTKSphere("sphere.vtk", latitudeSegments, longitudeSegments, radius);

    std::cout << "VTK file of a sphere generated: sphere.vtk" << std::endl;
    return 0;
}