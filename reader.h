//reader
#ifndef _READER_INCLUDED_
#define _READER_INCLUDED_


void read_config(){
	std::ifstream configFile("config.txt");
    if (!configFile) {
        std::cerr << "Error: Unable to open config.txt" << std::endl;
        return;
    }
    
    camera.pos = {0,0,0};
	camera.beta = 0; // grows towards left
	camera.alpha = 0; // grows towards up

    std::string line;
    int lineCount = 0;
    int colorr,colorg,colorb;
    while (getline(configFile, line)) {
        if (lineCount % 2 == 1) {
        	std::stringstream ss(line);
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
                    ss >> light.pos.x >> light.pos.y >> light.pos.z;
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
    printf("Config read");   
}

std::string filepath1 = "files/cube.vtk";
std::string filepath2 = "files/sphere_octo1.vtk";
std::string filepath3 = "files/sphere_octo3.vtk";

void readVtkFile(const std::string& filepath, Allpoints& allpoints, Scene& scene) {
	allpoints.clear();
	scene.clear();
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filepath << std::endl;
        return;
    }
    
    std::string line;
    bool processingPoints = false;
    bool processingFaces = false;
    
    std::vector<Point*> pointRefs;
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string keyword;
        ss >> keyword;

        // Skip comments and empty lines
        if (keyword.empty() || keyword[0] == '#') continue;

        if (keyword == "POINTS") {
            processingPoints = true;
            processingFaces = false;
            int numPoints;
            ss >> numPoints; 
            std::string type;
            ss >> type; // Type  float

            // Read points
            for (int i = 0; i < numPoints; i++) {
                if (std::getline(file, line)) {
                    std::stringstream pointStream(line);
                    Point p;
                    if (!(pointStream >> p.pos.x >> p.pos.y >> p.pos.z)) {
                    	std::cerr << "Error reading point coordinates on line " << (i + 1) << std::endl;
                    	return;
                	}
                    p.color = main_color; // object color
                    allpoints.push_back(p);
                    pointRefs.push_back(&allpoints.back()); // Store reference to point, might not be needed
                }
            }
        } else if (keyword == "POLYGONS") {
            processingPoints = false;
            processingFaces = true;
            printf("Starting to read polygons\n");
            int numPolygons;
            int numVertices;
            ss >> numPolygons >> numVertices; // Number of polygons and total vertices (ignored)

            // Read faces
            for (int i = 0; i < numPolygons; i++) {
                if (getline(file, line)) {
                    std::stringstream faceStream(line);
                    int vertexCount; // Always 3 for a triangle face
                    faceStream >> vertexCount;

                    if (vertexCount == 3) { // check if file is correct
                        Face face;
                        int indexA, indexB, indexC;
                        faceStream >> indexA >> indexB >> indexC;
						//if(debug)printf("%d %d %d", indexA, indexB, indexC);
						
                        // Set pointers to points from allpoints
						if (indexA >= 0 && indexA < pointRefs.size() &&
    						indexB >= 0 && indexB < pointRefs.size() &&
    						indexC >= 0 && indexC < pointRefs.size()) {

    						face.A = pointRefs[indexA];
    						face.B = pointRefs[indexB];
    						face.C = pointRefs[indexC];

    						// Add face to scene
    						scene.push_back(face);

    						// Get the address of the newly added face in the scene vector
    						Face* addedFace = &scene.back();
							//if(debug)printf("Face added\n");
							
    						// Update point facePtrs vectors, now points have pointers to the added face
    						//if (face.A) addedFace->A->facePtrs.push_back(addedFace);
    						//if (face.B) addedFace->B->facePtrs.push_back(addedFace);
    						//if (face.C) addedFace->C->facePtrs.push_back(addedFace);
    						
    						//if(debug)printf("Points updated\n");
						} else {
    						std::cerr << "Index out of bounds for pointRefs vector." << std::endl;
						}
                        //if(debug)printf("Points got their pointers\n");
                    } else {
                    	printf("Line did not declare 3 vertices\n");
					}
                }
            }
        }
    }

    file.close();
    printf("Vtk is read \n");
}



#endif // reader_INCLUDED