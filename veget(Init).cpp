#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

void Veget::Init()
{
    std::ifstream datas("../Mini-libs/veget/DataTrees.json");

    ParamsTrees p;

    char type[100];
    char shape[100];
    char pathTex[100];

    while(1)
    {
        std::string line;

        if(!getline(datas, line))
        {
            break;
        }

        std::string lineTrim = line;

        lineTrim.erase(0, lineTrim.find("\""));

        if(lineTrim.find("\"Specy\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"Specy\": \"%s\",", type);

            if(strrchr(type, '"') != nullptr)
            {
                char *quote = strrchr(type, '"');
                *quote = '\0';
            }
        }

        else
        if(lineTrim.find("\"heightMin\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"heightMin\": %d,", &p.heightMin);
        }

        else
        if(lineTrim.find("\"heightMax\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"heightMax\": %d,", &p.heightMax);
        }

        else
        if(lineTrim.find("\"trunkRadius\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"trunkRadius\": %f,", &p.trunkRadius);
        }

        else
        if(lineTrim.find("\"ratioHeight\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"ratioHeight\": %f,", &p.ratioHeight);
        }

        else
        if(lineTrim.find("\"coefTex\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"coefTex\": %f,", &p.coefTex);
        }

        else
        if(lineTrim.find("\"deltaAngle\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"deltaAngle\": %d,", &p.deltaAngle);
        }

        else
        if(lineTrim.find("\"beginBranchs\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"beginBranchs\": %f,", &p.beginBranchs);
        }

        else
        if(lineTrim.find("\"angleBranchs\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"angleBranchs\": %f,", &p.angleBranchs);
        }

        else
        if(lineTrim.find("\"ratioRadiusBranchs\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"ratioRadiusBranchs\": %f,", &p.ratioRadiusBranchs);
        }

        else
        if(lineTrim.find("\"nbBranchsMin\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"nbBranchsMin\": %d,", &p.nbBranchsMin);
        }

        else
        if(lineTrim.find("\"ratioRadiusBranchs\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"ratioRadiusBranchs\": %f,", &p.ratioRadiusBranchs);
        }

        else
        if(lineTrim.find("\"nbBranchsMin\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"nbBranchsMin\": %d,", &p.nbBranchsMin);
        }

        else
        if(lineTrim.find("\"nbBranchsMax\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"nbBranchsMax\": %d,", &p.nbBranchsMax);
        }

        else
        if(lineTrim.find("\"lengthBranchsMax\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"lengthBranchsMax\": %f,", &p.lengthBranchsMax);
        }

        else
        if(lineTrim.find("\"lengthBranchsMin\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "		\"lengthBranchsMin\": %f,", &p.lengthBranchsMin);
        }

        else
        if(lineTrim.find("\"branchsShape\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"branchsShape\": \"%s\",", shape);

            if(strrchr(shape, '"') != nullptr)
            {
                char *quote = strrchr(shape, '"');
                *quote = '\0';
            }
        }

        else
        if(lineTrim.find("\"tex\"") != std::string::npos)
        {
            sscanf(lineTrim.c_str(), "\"tex\": \"%s\",", pathTex);

            if(strrchr(pathTex, '"') != nullptr)
            {
                char *quote = strrchr(pathTex, '"');
                *quote = '\0';
            }
        }

        if(std::string(type) == "TREE_PINE")
        {
            p.type = TREE_PINE;
        }
        else
        if(std::string(type) == "TREE_OAK")
        {
            p.type = TREE_OAK;
        }
        else
        if(std::string(type) == "TREE_PLANE")
        {
            p.type = TREE_PLANE;
        }
        else
        if(std::string(type) == "TREE_CYPRESS")
        {
            p.type = TREE_CYPRESS;
        }
        else
        if(std::string(type) == "TREE_FIR")
        {
            p.type = TREE_FIR;
        }
        else
        if(std::string(type) == "TREE_OLIV")
        {
            p.type = TREE_OLIV;
        }
        else
        if(std::string(type) == "TREE_FIG")
        {
            p.type = TREE_FIG;
        }
        else
        if(std::string(type) == "TREE_POPLAR")
        {
            p.type = TREE_POPLAR;
        }

        if(std::string(shape) == "VEGET_SHAPE_RANDOM")
        {
            p.branchsShape = VEGET_SHAPE_RANDOM;
        }
        else
        if(std::string(shape) == "VEGET_SHAPE_CONE")
        {
            p.branchsShape = VEGET_SHAPE_CONE;
        }

        if(line.find("}") != std::string::npos)
        {
            //std::cout << p.heightMin << " " << p.heightMin << " " << p.trunkRadius << " " << p.ratioHeight << " " << p.coefTex << " " << p.deltaAngle << " " << p.beginBranchs << " " << p.angleBranchs << " " << p.ratioRadiusBranchs << " " << p.nbBranchsMin << " " << p.nbBranchsMax << " " << p.lengthBranchsMin << " " << p.lengthBranchsMax << std::endl;

            paramsTrees.push_back(p);

            std::string barkPath = "../Mini-libs/veget/Textures/" + std::string(pathTex) + "/Bark.png";
            std::string barkNormalPath = "../Mini-libs/veget/Textures/" + std::string(pathTex) + "/Bark_normals.png";
            std::string branchPath = "../Mini-libs/veget/Textures/" + std::string(pathTex) + "/Branch.png";

            Veget_Tex vt;

            vt.treeType = p.type;
            vt.barkTex = loadTexture(barkPath.c_str(), 1);
            vt.barkNormalTex = loadTexture(barkNormalPath.c_str(), 1);
            vt.branchTex = loadTexture(branchPath.c_str(), 1);

            textures.push_back(vt);
        }
    }

    datas.close();

    isInitialized = true;
}
