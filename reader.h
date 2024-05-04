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
                	int colorr,colorg,colorb;
                    ss >> colorr >> colorg >> colorb;
                    light.color = RGB(colorr,colorg,colorb);
                    break;
                case 11:
                    ss >> objectColor[0] >> objectColor[1] >> objectColor[2];
                    break;
                case 13:
                    ss >> objectDiffusion[0] >> objectDiffusion[1] >> objectDiffusion[2];
                    break;
                case 15:
                    ss >> objectMirror[0] >> objectMirror[1] >> objectMirror[2];
                    break;
                case 17:
                    ss >> objectAmbient[0] >> objectAmbient[1] >> objectAmbient[2];
                    break;
            }
        }
    }
        
}





#endif // reader_INCLUDED