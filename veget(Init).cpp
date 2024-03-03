#include <dirent.h>
#include <conio.h>
#include <fstream>
#include <SDL2/SDL_image.h>
#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

namespace Veget
{
    void VegetGenerator::Init()
    {
        InitParams();
        LoadTextures();
        createModels();
    }

    void VegetGenerator::InitParams()
    {
        std::ifstream file("../Mini-libs/Veget/DataTrees.json");

        Params p;
        std::string type = "";

        while(1)
        {
            std::string line;

            if(!getline(file, line))
            {
                break;
            }

            if(line.find("\"tree\"") != std::string::npos)
            {
                std::string tree = trim(line);

                tree.erase(0, tree.find(":") + 1);

                if(tree.rfind(",") != std::string::npos)
                {
                    tree.erase(tree.rfind(","));
                }

                if(tree == "1")
                {
                    p.tree = true;
                }
                else
                if(tree == "0")
                {
                    p.tree = false;
                }
            }

            else
            if(line.find("\"Specy\"") != std::string::npos)
            {
                type = trim(line);

                type.erase(0, type.find(":") + 2);
                type.erase(type.rfind("\""));
            }

            else
            if(line.find("\"heightMin\"") != std::string::npos)
            {
                std::string hMin = trim(line);

                hMin.erase(0, hMin.find(":") + 1);

                if(hMin.rfind(",") != std::string::npos)
                {
                    hMin.erase(hMin.rfind(","));
                }

                p.heightMin = atoi(hMin.c_str());
            }

            else
            if(line.find("\"heightMax\"") != std::string::npos)
            {
                std::string hMax = trim(line);

                hMax.erase(0, hMax.find(":") + 1);

                if(hMax.rfind(",") != std::string::npos)
                {
                    hMax.erase(hMax.rfind(","));
                }

                p.heightMax = atoi(hMax.c_str());
            }

            else
            if(line.find("\"radiusMin\"") != std::string::npos)
            {
                std::string rMin = trim(line);

                rMin.erase(0, rMin.find(":") + 1);

                if(rMin.rfind(",") != std::string::npos)
                {
                    rMin.erase(rMin.rfind(","));
                }

                p.radiusMin = atoi(rMin.c_str());
            }

            else
            if(line.find("\"radiusMax\"") != std::string::npos)
            {
                std::string rMax = trim(line);

                rMax.erase(0, rMax.find(":") + 1);

                if(rMax.rfind(",") != std::string::npos)
                {
                    rMax.erase(rMax.rfind(","));
                }

                p.radiusMax = atoi(rMax.c_str());
            }

            else
            if(line.find("\"ratioTopBottom\"") != std::string::npos)
            {
                std::string ratio = trim(line);

                ratio.erase(0, ratio.find(":") + 1);

                if(ratio.rfind(",") != std::string::npos)
                {
                    ratio.erase(ratio.rfind(","));
                }

                p.ratioTopBottom = atof(ratio.c_str());
            }

            else
            if(line.find("\"ratioBranchTrunkRadius\"") != std::string::npos)
            {
                std::string ratio = trim(line);

                ratio.erase(0, ratio.find(":") + 1);

                if(ratio.rfind(",") != std::string::npos)
                {
                    ratio.erase(ratio.rfind(","));
                }

                p.ratioBranchTrunkRadius = atof(ratio.c_str());
            }

            else
            if(line.find("\"ratioBranchTrunkLength\"") != std::string::npos)
            {
                std::string ratio = trim(line);

                ratio.erase(0, ratio.find(":") + 1);

                if(ratio.rfind(",") != std::string::npos)
                {
                    ratio.erase(ratio.rfind(","));
                }

                p.ratioBranchTrunkLength = atof(ratio.c_str());
            }

            else
            if(line.find("\"texKey\"") != std::string::npos)
            {
                p.texKey = trim(line);

                p.texKey.erase(0, p.texKey.find(":") + 2);
                p.texKey.erase(p.texKey.rfind("\""));
            }

            else
            if(line.find("\"beginBranch\"") != std::string::npos)
            {
                std::string bBranch = trim(line);

                bBranch.erase(0, bBranch.find(":") + 1);

                if(bBranch.rfind(",") != std::string::npos)
                {
                    bBranch.erase(bBranch.rfind(","));
                }

                p.beginBranch = atof(bBranch.c_str());
            }

            else
            if(line.find("\"angleBranch\"") != std::string::npos)
            {
                std::string aBranch = trim(line);

                aBranch.erase(0, aBranch.find(":") + 1);

                if(aBranch.rfind(",") != std::string::npos)
                {
                    aBranch.erase(aBranch.rfind(","));
                }

                p.angleBranch = atof(aBranch.c_str());
            }

            else
            if(line.find("\"shape\"") != std::string::npos)
            {
                p.shape = trim(line);

                p.shape.erase(0, p.shape.find(":") + 2);
                p.shape.erase(p.shape.rfind("\""));
            }

            else
            if(line.find("}") != std::string::npos)
            {
                params[type] = p;
            }
        }

        file.close();
    }

    void VegetGenerator::LoadTextures()
    {
        DIR *dir = opendir("../Mini-libs/veget/Textures/");

        while(1)
        {
            dirent *element = readdir(dir);

            if(element == nullptr)
            {
                break;
            }

            const std::string name = std::string(element->d_name);
            const std::string path = "../Mini-libs/veget/Textures/" + name;

            if(name.find(".png") != std::string::npos)
            {
                GLuint tex;

                tex = loadTexture(path.c_str(), 1);

                std::string specie = name;
                specie.erase(specie.rfind("."));

                textures[specie] = tex;
            }
        }

        closedir(dir);
    }

    void VegetGenerator::createModels()
    {
        for(const auto &p : params)
        {
            VertexBuffer model;

            if(p.second.tree)
            {
                std::vector<glm::vec3> skeleton;
                float trunkRadius;

                createTrunk(p.first, skeleton, &trunkRadius, &model);
                createBranchs(p.first, skeleton, trunkRadius, &model);
            }

            else
            {
                createGrass(&model);
            }

            model.pathTex = p.second.texKey;
            model.specie = p.first;

            models.push_back(model);
        }
    }

    void VegetGenerator::createGrass(VertexBuffer *model)
    {
        const float width = 2.0f;
        const float height = 2.0f;

        model->coordVert.push_back(-width/2);
        model->coordVert.push_back(0.0f);
        model->coordVert.push_back(0.0f);

        model->coordVert.push_back(-width/2);
        model->coordVert.push_back(0.0f);
        model->coordVert.push_back(height);

        model->coordVert.push_back(width/2);
        model->coordVert.push_back(0.0f);
        model->coordVert.push_back(0.0f);

        //////////////////////////////////////

        model->coordVert.push_back(width/2);
        model->coordVert.push_back(0.0f);
        model->coordVert.push_back(0.0f);

        model->coordVert.push_back(-width/2);
        model->coordVert.push_back(0.0f);
        model->coordVert.push_back(height);

        model->coordVert.push_back(width/2);
        model->coordVert.push_back(0.0f);
        model->coordVert.push_back(height);

        ////////////////////////////////////////////////////////

        model->coordVert.push_back(0.0f);
        model->coordVert.push_back(-width/2);
        model->coordVert.push_back(0.0f);

        model->coordVert.push_back(0.0f);
        model->coordVert.push_back(-width/2);
        model->coordVert.push_back(height);

        model->coordVert.push_back(0.0f);
        model->coordVert.push_back(width/2);
        model->coordVert.push_back(0.0f);

        //////////////////////////////////////

        model->coordVert.push_back(0.0f);
        model->coordVert.push_back(width/2);
        model->coordVert.push_back(0.0f);

        model->coordVert.push_back(0.0f);
        model->coordVert.push_back(-width/2);
        model->coordVert.push_back(height);

        model->coordVert.push_back(0.0f);
        model->coordVert.push_back(width/2);
        model->coordVert.push_back(height);

        ////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////

        for(size_t i = 0 ; i < 2 ; i++)
        {
            model->coordTex.push_back(0.0f);
            model->coordTex.push_back(0.0f);

            model->coordTex.push_back(0.0f);
            model->coordTex.push_back(1.0f);

            model->coordTex.push_back(1.0f);
            model->coordTex.push_back(0.0f);

            //////////////////////////////////////

            model->coordTex.push_back(1.0f);
            model->coordTex.push_back(0.0f);

            model->coordTex.push_back(0.0f);
            model->coordTex.push_back(1.0f);

            model->coordTex.push_back(1.0f);
            model->coordTex.push_back(1.0f);
        }

        for(size_t i = 0 ; i < 12 ; i++)
        {
            model->normals.push_back(0.0f);
            model->normals.push_back(0.0f);
            model->normals.push_back(0.0f);
        }
    }

    std::vector<float> VegetGenerator::getPositionsFromSpecie(const std::string specie)
    {
        std::vector<float> positions;

        for(const Item it : items)
        {
            if(it.type == specie)
            {
                positions.push_back(it.pos.x);
                positions.push_back(it.pos.y);
                positions.push_back(it.pos.z);
            }
        }

        return positions;
    }

    std::vector<float> VegetGenerator::getScalesFromSpecie(const std::string specie)
    {
        std::vector<float> scales;

        for(const Item it : items)
        {
            if(it.type == specie)
            {
                float scale = it.scale;

                if(scale == 0.0f)
                {
                    scale = (rand() % (15 - 5 + 1) + 5) / 10.0f;
                }

                scales.push_back(scale);
            }
        }

        return scales;
    }

    void VegetGenerator::Finalize()
    {
        for(size_t i = 0 ; i < models.size() ; i++)
        {
            glGenBuffers(1, &models[i].VBO);
            glGenVertexArrays(1, &models[i].VAO);

            glBindBuffer(GL_ARRAY_BUFFER, models[i].VBO);
            glBindVertexArray(models[i].VAO);

            models[i].positions = getPositionsFromSpecie(models[i].specie);
            models[i].scales = getScalesFromSpecie(models[i].specie);

            for(size_t j = 0 ; j < models[i].positions.size()/3 ; j++)
            {
                const float angle = rand() % 360;

                models[i].angles.push_back(angle);
            }

            const size_t datasSize = (models[i].coordVert.size() + models[i].coordTex.size() + models[i].normals.size() + models[i].positions.size() + models[i].scales.size() + models[i].angles.size()) * sizeof(float);

            glBufferData(GL_ARRAY_BUFFER, datasSize, nullptr, GL_DYNAMIC_DRAW);

            glBufferSubData(GL_ARRAY_BUFFER, 0, models[i].coordVert.size() * sizeof(float), models[i].coordVert.data());
            glBufferSubData(GL_ARRAY_BUFFER, models[i].coordVert.size() * sizeof(float), models[i].coordTex.size() * sizeof(float), models[i].coordTex.data());
            glBufferSubData(GL_ARRAY_BUFFER, models[i].coordVert.size() * sizeof(float) + models[i].coordTex.size() * sizeof(float), models[i].normals.size() * sizeof(float), models[i].normals.data());
            glBufferSubData(GL_ARRAY_BUFFER, models[i].coordVert.size() * sizeof(float) + models[i].coordTex.size() * sizeof(float) + models[i].normals.size() * sizeof(float), models[i].positions.size() * sizeof(float), models[i].positions.data());
            glBufferSubData(GL_ARRAY_BUFFER, models[i].coordVert.size() * sizeof(float) + models[i].coordTex.size() * sizeof(float) + models[i].normals.size() * sizeof(float) + models[i].positions.size() * sizeof(float), models[i].scales.size() * sizeof(float), models[i].scales.data());
            glBufferSubData(GL_ARRAY_BUFFER, models[i].coordVert.size() * sizeof(float) + models[i].coordTex.size() * sizeof(float) + models[i].normals.size() * sizeof(float) + models[i].positions.size() * sizeof(float) + models[i].scales.size() * sizeof(float), models[i].angles.size() * sizeof(float), models[i].angles.data());

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(models[i].coordVert.size() * sizeof(float)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(models[i].coordVert.size() * sizeof(float) + models[i].coordTex.size() * sizeof(float)));
            glEnableVertexAttribArray(2);

            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(models[i].coordVert.size() * sizeof(float) + models[i].coordTex.size() * sizeof(float) + models[i].normals.size() * sizeof(float)));
            glEnableVertexAttribArray(3);

            glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(models[i].coordVert.size() * sizeof(float) + models[i].coordTex.size() * sizeof(float) + models[i].normals.size() * sizeof(float) + models[i].positions.size() * sizeof(float)));
            glEnableVertexAttribArray(4);

            glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(models[i].coordVert.size() * sizeof(float) + models[i].coordTex.size() * sizeof(float) + models[i].normals.size() * sizeof(float) + models[i].positions.size() * sizeof(float) + models[i].scales.size() * sizeof(float)));
            glEnableVertexAttribArray(5);

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            const std::string pathTex = "../Mini-libs/veget/Textures/" + models[i].pathTex + ".png";

            models[i].tex = loadTexture(pathTex.c_str(), 1);
        }
    }

    size_t VegetGenerator::createTrunk(std::string specie, std::vector<glm::vec3> &skeleton, float *trunkRadius, VertexBuffer *model)
    {
        float height, trunkDiameter, ratioTopBottom;

        height = (rand() % ((int)params[specie].heightMax - (int)params[specie].heightMin + 1) + (int)params[specie].heightMin) / 100.0f;
        trunkDiameter = (rand() % (params[specie].radiusMax - params[specie].radiusMin + 1) + params[specie].radiusMin) / 100.0f;
        ratioTopBottom = params[specie].ratioTopBottom;

        *trunkRadius = trunkDiameter/2;

        const float bottomDiameter = trunkDiameter;
        const float topDiameter = bottomDiameter * ratioTopBottom;
        const float segHeight = height / nbSeg;

        std::vector<Circle> circles;

        for(size_t i = 0 ; i < nbSeg + 1 ; i++)
        {
            const float coef = (topDiameter - bottomDiameter) / (nbSeg+1);
            const float diameter = coef * i + bottomDiameter;
            const float radius = diameter / 2;
            glm::vec3 center;

            center.x = (rand() % (50 - 10 + 1) + 10) / 100.0f;
            center.y = (rand() % (50 - 10 + 1) + 10) / 100.0f;
            center.z = i * segHeight;

            skeleton.push_back(center);

            Circle c;

            const float alpha = 360.0f / res;
            float angle = 0.0f;

            for(size_t j = 0 ; j < res ; j++)
            {
                glm::vec3 v;

                v.x = center.x + radius * cos(angle * PI/180);
                v.y = center.y + radius * sin(angle * PI/180);
                v.z = center.z;

                c.vertices.push_back(v);

                angle += alpha;
            }

            circles.push_back(c);
        }

        size_t nbVertices = 0;

        for(size_t i = 0 ; i < circles.size()-1 ; i++)
        {
            glm::vec3 center1 = glm::vec3(0.0f);
            glm::vec3 center2 = glm::vec3(0.0f);

            for(size_t j = 0; j < res; j++)
            {
                center1 += circles[i].vertices[j];
            }
            center1 /= res;

            for(size_t j=0; j<res; j++)
            {
                center2 += circles[i+1].vertices[j];
            }
            center2 /= res;

            const float deltaTex = 0.4f / res;
            float coordTexX = 0.05f;
            const float coordTexY = 0.5f * segHeight / trunkDiameter;

            for(size_t j = 0 ; j < res ; j++)
            {
                const size_t index1 = j;
                size_t index2;

                if(j != res-1)
                {
                    index2 = j+1;
                }
                else
                {
                    index2 = 0;
                }

                model->coordVert.push_back(circles[i].vertices[index1].x);
                model->coordVert.push_back(circles[i].vertices[index1].y);
                model->coordVert.push_back(circles[i].vertices[index1].z);

                model->coordVert.push_back(circles[i+1].vertices[index1].x);
                model->coordVert.push_back(circles[i+1].vertices[index1].y);
                model->coordVert.push_back(circles[i+1].vertices[index1].z);

                model->coordVert.push_back(circles[i].vertices[index2].x);
                model->coordVert.push_back(circles[i].vertices[index2].y);
                model->coordVert.push_back(circles[i].vertices[index2].z);

                //////////////////////////////

                model->coordVert.push_back(circles[i].vertices[index2].x);
                model->coordVert.push_back(circles[i].vertices[index2].y);
                model->coordVert.push_back(circles[i].vertices[index2].z);

                model->coordVert.push_back(circles[i+1].vertices[index1].x);
                model->coordVert.push_back(circles[i+1].vertices[index1].y);
                model->coordVert.push_back(circles[i+1].vertices[index1].z);

                model->coordVert.push_back(circles[i+1].vertices[index2].x);
                model->coordVert.push_back(circles[i+1].vertices[index2].y);
                model->coordVert.push_back(circles[i+1].vertices[index2].z);

                ///////////////////////////////////////////////////////////////

                model->coordTex.push_back(coordTexX);
                model->coordTex.push_back(0.0f);

                model->coordTex.push_back(coordTexX);
                model->coordTex.push_back(coordTexY);

                model->coordTex.push_back(coordTexX + deltaTex);
                model->coordTex.push_back(0.0f);

                //////////////////////////////

                model->coordTex.push_back(coordTexX + deltaTex);
                model->coordTex.push_back(0.0f);

                model->coordTex.push_back(coordTexX);
                model->coordTex.push_back(coordTexY);

                model->coordTex.push_back(coordTexX + deltaTex);
                model->coordTex.push_back(coordTexY);

                coordTexX += deltaTex;

                ///////////////////////////////////////////////////////////////

                model->normals.push_back(glm::normalize(circles[i].vertices[index1] - center1).x);
                model->normals.push_back(glm::normalize(circles[i].vertices[index1] - center1).y);
                model->normals.push_back(glm::normalize(circles[i].vertices[index1] - center1).z);

                model->normals.push_back(glm::normalize(circles[i+1].vertices[index1] - center2).x);
                model->normals.push_back(glm::normalize(circles[i+1].vertices[index1] - center2).y);
                model->normals.push_back(glm::normalize(circles[i+1].vertices[index1] - center2).z);

                model->normals.push_back(glm::normalize(circles[i].vertices[index2] - center1).x);
                model->normals.push_back(glm::normalize(circles[i].vertices[index2] - center1).y);
                model->normals.push_back(glm::normalize(circles[i].vertices[index2] - center1).z);

                //////////////////////////////

                model->normals.push_back(glm::normalize(circles[i].vertices[index2] - center1).x);
                model->normals.push_back(glm::normalize(circles[i].vertices[index2] - center1).y);
                model->normals.push_back(glm::normalize(circles[i].vertices[index2] - center1).z);

                model->normals.push_back(glm::normalize(circles[i+1].vertices[index1] - center2).x);
                model->normals.push_back(glm::normalize(circles[i+1].vertices[index1] - center2).y);
                model->normals.push_back(glm::normalize(circles[i+1].vertices[index1] - center2).z);

                model->normals.push_back(glm::normalize(circles[i+1].vertices[index2] - center2).x);
                model->normals.push_back(glm::normalize(circles[i+1].vertices[index2] - center2).y);
                model->normals.push_back(glm::normalize(circles[i+1].vertices[index2] - center2).z);

                nbVertices += 6;
            }
        }

        return nbVertices;
    }

    void VegetGenerator::addItem(Item item)
    {
        items.push_back(item);
    }
}
