//reader
#ifndef _READER_INCLUDED_
#define _READER_INCLUDED_


void read_config(){
	ifstream configFile("config.txt");
    if (!configFile) {
        cerr << "Error: Unable to open config.txt" << endl;
        return;
    }
    
    std::string line;
    int lineCount = 0;
    int colorr,colorg,colorb;
    while (getline(configFile, line)) {
        if (lineCount % 2 == 1) {
        	stringstream ss(line);
            switch (lineCount) {
                case 1:
                    ss >> Pmode;
                    break;
                case 3:
                    ss >> Dmode;
                    break;
                case 5:
                    ss >> camera.pos.x >> camera.pos.y >> camera.pos.z;
                    break;
                case 7:
                    ss >> light.x >> light.y >> light.z;
                    break;
                case 9:
                	
                    ss >> colorr >> colorg >> colorb;
                    light.color = RGB(colorr,colorg,colorb);
                    break;
                case 11:
                    
                    ss >> colorr >> colorg >> colorb;
                    main_color = RGB(colorr,colorg,colorb);
                    break;
                case 13:
                    ss >> colorr >> colorg >> colorb;
                    diffusion = RGB(colorr,colorg,colorb);
                    break;
                case 15:
                    ss >> colorr >> colorg >> colorb;
                    mirror = RGB(colorr,colorg,colorb);
                    break;
                case 17:
                    ss >> colorr >> colorg >> colorb;
                    ambience = RGB(colorr,colorg,colorb);
                    break;
            }
        }
    }
        
}

std::string filepath = "files/cube.vtk";
void readVtkFile(const string& filepath, Allpoints& allpoints, Scene& scene) {
	allpoints.clear();
	scene.clear();
    ifstream file(filepath);
    if (!file) {
        cerr << "Error: Unable to open file " << filepath << endl;
        return;
    }
    
    string line;
    bool processingPoints = false;
    bool processingFaces = false;
    
    std::vector<Point*> pointRefs;
    
    while (getline(file, line)) {
        stringstream ss(line);
        std::string keyword;
        ss >> keyword;

        // Skip comments and empty lines
        if (keyword.empty() || keyword[0] == '#') continue;

        if (keyword == "POINTS") {
            processingPoints = true;
            processingFaces = false;
            int numPoints;
            ss >> numPoints; 
            string type;
            ss >> type; // Type  float

            // Read points
            for (int i = 0; i < numPoints; i++) {
                if (getline(file, line)) {
                    stringstream pointStream(line);
                    Point p;
                    pointStream >> p.x >> p.y >> p.z;
                    p.color = main_color; // object color
                    allpoints.push_back(p);
                    pointRefs.push_back(&allpoints.back()); // Store reference to point, might not be needed
                }
            }
        } else if (keyword == "POLYGONS") {
            processingPoints = false;
            processingFaces = true;
            int numPolygons;
            int numVertices;
            ss >> numPolygons >> numVertices; // Number of polygons and total vertices (ignored)

            // Read faces
            for (int i = 0; i < numPolygons; i++) {
                if (getline(file, line)) {
                    stringstream faceStream(line);
                    int vertexCount; // Always 3 for a triangle face
                    faceStream >> vertexCount;

                    if (vertexCount == 3) { // check if file is correct
                        Face face;
                        int indexA, indexB, indexC;
                        faceStream >> indexA >> indexB >> indexC;

                        // Set pointers to points from allpoints
                        face.A = pointRefs[indexA];
                        face.B = pointRefs[indexB];
                        face.C = pointRefs[indexC];

                        // Add face to scene
                        scene.push_back(face);

                        // Update point facePtrs vectors, now points have pointers to face
                        face.A->facePtrs.push_back(&scene.back());
                        face.B->facePtrs.push_back(&scene.back());
                        face.C->facePtrs.push_back(&scene.back());
                    }
                }
            }
        }
    }

    file.close();
    
}



#endif // reader_INCLUDED