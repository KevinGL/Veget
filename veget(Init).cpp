#include <dirent.h>
#include <conio.h>
#include <SDL2/SDL_image.h>
#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

namespace Veget
{
    void VegetGenerator::Init()
    {
        //

        LoadTextures();
    }

    void VegetGenerator::Finalize()
    {
        glGenVertexArrays(1, &vb.VAO);
        glGenBuffers(1, &vb.VBO);

        glBindVertexArray(vb.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, vb.VBO);

        const size_t totalSize = vb.coordVert.size() * sizeof(float) + vb.coordTex.size() * sizeof(float) + vb.normals.size() * sizeof(float) + vb.indexTex.size() * sizeof(int);

        glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STREAM_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, vb.coordVert.size() * sizeof(float), vb.coordVert.data());
        glBufferSubData(GL_ARRAY_BUFFER, vb.coordVert.size() * sizeof(float), vb.coordTex.size() * sizeof(float), vb.coordTex.data());
        glBufferSubData(GL_ARRAY_BUFFER, vb.coordVert.size() * sizeof(float) + vb.coordTex.size() * sizeof(float), vb.normals.size() * sizeof(float), vb.normals.data());
        glBufferSubData(GL_ARRAY_BUFFER, vb.coordVert.size() * sizeof(float) + vb.coordTex.size() * sizeof(float) + vb.normals.size() * sizeof(float), vb.indexTex.size() * sizeof(int), vb.indexTex.data());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float) , (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float) , (void*)(vb.coordVert.size() * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float) , (void*)(vb.coordVert.size() * sizeof(float) + vb.coordTex.size() * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribIPointer(3, 1, GL_INT, sizeof(int), (void*)(vb.coordVert.size() * sizeof(float) + vb.coordTex.size() * sizeof(float) + vb.normals.size() * sizeof(float)));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
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

    int VegetGenerator::getIndexTexture(const std::string specie)
    {
        for(size_t i = 0 ; i < vb.textures.size() ; i++)
        {
            if(specie == vb.textures[i].specie)
            {
                return i;
            }
        }

        return -1;
    }

    size_t VegetGenerator::createTrunk(Plant plant, std::vector<glm::vec3> &skeleton, float *trunkRadius)
    {
        float height, trunkDiameter, ratioTopBottom;
        std::string specie;
        int indexTex;

        if(plant.type == VEGET_SCOTS_PINE || plant.type == VEGET_UMBRELL_PINE || plant.type == VEGET_MARTITIM_PINE)
        {
            height = rand() % (20 - 6 + 1) + 6;
            trunkDiameter = (rand() % (130 - 80 + 1) + 80) / 100.0f;
            ratioTopBottom = 0.5;
            specie = "Scots_Pine";
            indexTex = getIndexTexture("Pine");
        }

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

            center.x = plant.pos.x + (rand() % (50 - 10 + 1) + 10) / 100.0f;
            center.y = plant.pos.y + (rand() % (50 - 10 + 1) + 10) / 100.0f;
            center.z = plant.pos.z + i * segHeight;

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
            float coordTex = 0.05f;

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

                vb.coordVert.push_back(circles[i].vertices[index1].x);
                vb.coordVert.push_back(circles[i].vertices[index1].y);
                vb.coordVert.push_back(circles[i].vertices[index1].z);

                vb.coordVert.push_back(circles[i+1].vertices[index1].x);
                vb.coordVert.push_back(circles[i+1].vertices[index1].y);
                vb.coordVert.push_back(circles[i+1].vertices[index1].z);

                vb.coordVert.push_back(circles[i].vertices[index2].x);
                vb.coordVert.push_back(circles[i].vertices[index2].y);
                vb.coordVert.push_back(circles[i].vertices[index2].z);

                //////////////////////////////

                vb.coordVert.push_back(circles[i].vertices[index2].x);
                vb.coordVert.push_back(circles[i].vertices[index2].y);
                vb.coordVert.push_back(circles[i].vertices[index2].z);

                vb.coordVert.push_back(circles[i+1].vertices[index1].x);
                vb.coordVert.push_back(circles[i+1].vertices[index1].y);
                vb.coordVert.push_back(circles[i+1].vertices[index1].z);

                vb.coordVert.push_back(circles[i+1].vertices[index2].x);
                vb.coordVert.push_back(circles[i+1].vertices[index2].y);
                vb.coordVert.push_back(circles[i+1].vertices[index2].z);

                ///////////////////////////////////////////////////////////////

                vb.coordTex.push_back(coordTex);
                vb.coordTex.push_back(0.0f);

                vb.coordTex.push_back(coordTex);
                vb.coordTex.push_back(1.0f);

                vb.coordTex.push_back(coordTex + deltaTex);
                vb.coordTex.push_back(0.0f);

                //////////////////////////////

                vb.coordTex.push_back(coordTex + deltaTex);
                vb.coordTex.push_back(0.0f);

                vb.coordTex.push_back(coordTex);
                vb.coordTex.push_back(1.0f);

                vb.coordTex.push_back(coordTex + deltaTex);
                vb.coordTex.push_back(1.0f);

                coordTex += deltaTex;

                ///////////////////////////////////////////////////////////////

                vb.normals.push_back(glm::normalize(circles[i].vertices[index1] - center1).x);
                vb.normals.push_back(glm::normalize(circles[i].vertices[index1] - center1).y);
                vb.normals.push_back(glm::normalize(circles[i].vertices[index1] - center1).z);

                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index1] - center2).x);
                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index1] - center2).y);
                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index1] - center2).z);

                vb.normals.push_back(glm::normalize(circles[i].vertices[index2] - center1).x);
                vb.normals.push_back(glm::normalize(circles[i].vertices[index2] - center1).y);
                vb.normals.push_back(glm::normalize(circles[i].vertices[index2] - center1).z);

                //////////////////////////////

                vb.normals.push_back(glm::normalize(circles[i].vertices[index2] - center1).x);
                vb.normals.push_back(glm::normalize(circles[i].vertices[index2] - center1).y);
                vb.normals.push_back(glm::normalize(circles[i].vertices[index2] - center1).z);

                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index1] - center2).x);
                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index1] - center2).y);
                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index1] - center2).z);

                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index2] - center2).x);
                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index2] - center2).y);
                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index2] - center2).z);

                ///////////////////////////////////////////////////////////////

                for(size_t k = 0 ; k < 6 ; k++)
                {
                    vb.indexTex.push_back(indexTex);
                }

                ///////////////////////////////////////////////////////////////

                nbVertices += 6;
            }
        }

        return nbVertices;
    }

    void VegetGenerator::createPlant(Plant plant)
    {
        int indexTex;

        if(plant.type == VEGET_GRASS)
        {
            indexTex = getIndexTexture("Grass");
        }

        const float size = (rand() % (200 - 100 + 1) + 100) / 100.0f;
        const float angle = rand() % 360;
        const glm::mat4 rotation = glm::rotate(angle, 0.0f, 0.0f, 1.0f);

        {
            glm::vec4 v = rotation * glm::vec4(-size/2, 0.0f, 0.0f, 1.0f);
            vb.coordVert.push_back(v.x + plant.pos.x);
            vb.coordVert.push_back(v.y + plant.pos.y);
            vb.coordVert.push_back(v.z + plant.pos.z);

            v = rotation * glm::vec4(-size/2, 0.0f, size, 1.0f);
            vb.coordVert.push_back(v.x + plant.pos.x);
            vb.coordVert.push_back(v.y + plant.pos.y);
            vb.coordVert.push_back(v.z + plant.pos.z);

            v = rotation * glm::vec4(size/2, 0.0f, 0.0f, 1.0f);
            vb.coordVert.push_back(v.x + plant.pos.x);
            vb.coordVert.push_back(v.y + plant.pos.y);
            vb.coordVert.push_back(v.z + plant.pos.z);

            /////////////

            v = rotation * glm::vec4(size/2, 0.0f, 0.0f, 1.0f);
            vb.coordVert.push_back(v.x + plant.pos.x);
            vb.coordVert.push_back(v.y + plant.pos.y);
            vb.coordVert.push_back(v.z + plant.pos.z);

            v = rotation * glm::vec4(-size/2, 0.0f, size, 1.0f);
            vb.coordVert.push_back(v.x + plant.pos.x);
            vb.coordVert.push_back(v.y + plant.pos.y);
            vb.coordVert.push_back(v.z + plant.pos.z);

            v = rotation * glm::vec4(size/2, 0.0f, size, 1.0f);
            vb.coordVert.push_back(v.x + plant.pos.x);
            vb.coordVert.push_back(v.y + plant.pos.y);
            vb.coordVert.push_back(v.z + plant.pos.z);

            //////////////////////////

            v = rotation * glm::vec4(0.0f, -size/2, 0.0f, 1.0f);
            vb.coordVert.push_back(v.x + plant.pos.x);
            vb.coordVert.push_back(v.y + plant.pos.y);
            vb.coordVert.push_back(v.z + plant.pos.z);

            v = rotation * glm::vec4(0.0f, -size/2, size, 1.0f);
            vb.coordVert.push_back(v.x + plant.pos.x);
            vb.coordVert.push_back(v.y + plant.pos.y);
            vb.coordVert.push_back(v.z + plant.pos.z);

            v = rotation * glm::vec4(0.0f, size/2, 0.0f, 1.0f);
            vb.coordVert.push_back(v.x + plant.pos.x);
            vb.coordVert.push_back(v.y + plant.pos.y);
            vb.coordVert.push_back(v.z + plant.pos.z);

            /////////////

            v = rotation * glm::vec4(0.0f, size/2, 0.0f, 1.0f);
            vb.coordVert.push_back(v.x + plant.pos.x);
            vb.coordVert.push_back(v.y + plant.pos.y);
            vb.coordVert.push_back(v.z + plant.pos.z);

            v = rotation * glm::vec4(0.0f, -size/2, size, 1.0f);
            vb.coordVert.push_back(v.x + plant.pos.x);
            vb.coordVert.push_back(v.y + plant.pos.y);
            vb.coordVert.push_back(v.z + plant.pos.z);

            v = rotation * glm::vec4(0.0f, size/2, size, 1.0f);
            vb.coordVert.push_back(v.x + plant.pos.x);
            vb.coordVert.push_back(v.y + plant.pos.y);
            vb.coordVert.push_back(v.z + plant.pos.z);
        }

        for(size_t i = 0 ; i < 2 ; i++)
        {
            vb.coordTex.push_back(0.0f);
            vb.coordTex.push_back(0.0f);

            vb.coordTex.push_back(0.0f);
            vb.coordTex.push_back(1.0f);

            vb.coordTex.push_back(1.0f);
            vb.coordTex.push_back(0.0f);

            /////////////

            vb.coordTex.push_back(1.0f);
            vb.coordTex.push_back(0.0f);

            vb.coordTex.push_back(0.0f);
            vb.coordTex.push_back(1.0f);

            vb.coordTex.push_back(1.0f);
            vb.coordTex.push_back(1.0f);
        }

        for(size_t i = 0 ; i < 12 ; i++)
        {
            vb.normals.push_back(0.0f);
            vb.normals.push_back(0.0f);
            vb.normals.push_back(0.0f);

            vb.indexTex.push_back(indexTex);
        }
    }

    void VegetGenerator::addPlant(Plant plant)
    {
        if(plant.type == VEGET_SCOTS_PINE && getIndexTexture("Pine") == -1)
        {
            Texture tex;

            tex.specie = "Pine";
            tex.tex = textures["Pine"];

            vb.textures.push_back(tex);
        }

        else
        if(plant.type == VEGET_GRASS && getIndexTexture("Grass") == -1)
        {
            Texture tex;

            tex.specie = "Grass";
            tex.tex = textures["Grass"];

            vb.textures.push_back(tex);
        }

        if(plant.type != VEGET_GRASS && plant.type != VEGET_LAVENDER)
        {
            std::vector<glm::vec3> trunkSkeleton;
            float trunkRadius;

            size_t nbVertices = createTrunk(plant, trunkSkeleton, &trunkRadius);
            nbVertices += createBranchs(plant, trunkSkeleton, trunkRadius);

            nbVerticesByTree.push_back(nbVertices);
        }

        else
        {
            createPlant(plant);
            nbVerticesByTree.push_back(12);
        }

        positions.push_back(plant.pos);
    }
}
