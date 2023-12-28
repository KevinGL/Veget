#include <dirent.h>
#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

namespace Trees
{
    void TreesGenerator::Init()
    {
        //

        LoadTextures();
    }

    void TreesGenerator::Finalize()
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

        ///////////////////////////////////////////////////

        nbVerticesByTree = res * 6 * nbSeg;
    }

    void TreesGenerator::LoadTextures()
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

            if(!isFile(path) && name != "." && name != "..")
            {
                Texture tex;

                std::string path2 = path + "/Bark.png";

                tex.specie = name;

                tex.bark = loadTexture(path2.c_str(), 1);

                path2 = path + "/Bark_normals.png";
                tex.barkNormal = loadTexture(path2.c_str(), 1);

                path2 = path + "/Branch.png";
                tex.branch = loadTexture(path2.c_str(), 1);

                vb.textures.push_back(tex);
            }
        }

        closedir(dir);
    }

    size_t TreesGenerator::getIndexTex(const std::string specie, const std::string part)
    {
        size_t res = 0;

        for(size_t i=0; i<vb.textures.size(); i++)
        {
            if(vb.textures[i].specie == specie)
            {
                if(part == "bark")
                {
                    res = 3 * i;
                }

                else
                if(part == "barkNorm")
                {
                    res = 3 * i + 1;
                }

                else
                if(part == "branch")
                {
                    res = 3 * i + 2;
                }

                break;
            }
        }

        return res;
    }

    void TreesGenerator::createTrunk(Tree tree)
    {
        float height, trunkDiameter, ratioTopBottom;
        std::string specie;

        if(tree.type == TREE_PINE)
        {
            height = rand() % (30 - 6 + 1) + 6;
            trunkDiameter = (rand() % (80 - 50 + 1) + 50) / 100.0f;
            ratioTopBottom = 0.5;
            specie = "Pine";
        }

        const float bottomDiameter = trunkDiameter;
        const float topDiameter = bottomDiameter * ratioTopBottom;
        const float segHeight = height / nbSeg;

        std::vector<Circle> circles;

        for(size_t i=0; i<nbSeg+1; i++)
        {
            const float coef = (topDiameter - bottomDiameter) / (nbSeg+1);
            const float diameter = coef * i + bottomDiameter;
            const float radius = diameter / 2;
            glm::vec3 center;

            center.x = tree.pos.x + (rand() % (50 - 10 + 1) + 10) / 100.0f;
            center.y = tree.pos.y + (rand() % (50 - 10 + 1) + 10) / 100.0f;
            center.z = tree.pos.z + i * segHeight;

            Circle c;

            const float alpha = 360.0f / res;
            float angle = 0.0f;

            for(size_t j=0; j<res; j++)
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

        for(size_t i=0; i<circles.size()-1; i++)
        {
            glm::vec3 center1 = glm::vec3(0.0f);
            glm::vec3 center2 = glm::vec3(0.0f);

            for(size_t j=0; j<res; j++)
            {
                center1 += circles[i].vertices[j];
            }
            center1 /= res;

            for(size_t j=0; j<res; j++)
            {
                center2 += circles[i+1].vertices[j];
            }
            center2 /= res;

            for(size_t j=0; j<res; j++)
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

                vb.coordTex.push_back((float)index1 / res);
                vb.coordTex.push_back(0.0f);

                vb.coordTex.push_back((float)index1 / res);
                vb.coordTex.push_back(1.0f);

                vb.coordTex.push_back((float)(index2) / res);
                vb.coordTex.push_back(0.0f);

                //////////////////////////////

                vb.coordTex.push_back((float)(index2) / res);
                vb.coordTex.push_back(0.0f);

                vb.coordTex.push_back((float)index1 / res);
                vb.coordTex.push_back(1.0f);

                vb.coordTex.push_back((float)(index2) / res);
                vb.coordTex.push_back(1.0f);

                ///////////////////////////////////////////////////////////////

                vb.normals.push_back(glm::normalize(circles[i].vertices[index1].x - center1).x);
                vb.normals.push_back(glm::normalize(circles[i].vertices[index1].x - center1).y);
                vb.normals.push_back(glm::normalize(circles[i].vertices[index1].x - center1).z);

                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index1].x - center2).x);
                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index1].x - center2).y);
                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index1].x - center2).z);

                vb.normals.push_back(glm::normalize(circles[i].vertices[index2].x - center1).x);
                vb.normals.push_back(glm::normalize(circles[i].vertices[index2].x - center1).y);
                vb.normals.push_back(glm::normalize(circles[i].vertices[index2].x - center1).z);

                //////////////////////////////

                vb.normals.push_back(glm::normalize(circles[i].vertices[index2].x - center1).x);
                vb.normals.push_back(glm::normalize(circles[i].vertices[index2].x - center1).y);
                vb.normals.push_back(glm::normalize(circles[i].vertices[index2].x - center1).z);

                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index1].x - center2).x);
                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index1].x - center2).y);
                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index1].x - center2).z);

                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index2].x - center2).x);
                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index2].x - center2).y);
                vb.normals.push_back(glm::normalize(circles[i+1].vertices[index2].x - center2).z);

                ///////////////////////////////////////////////////////////////

                for(size_t k=0; k<6; k++)
                {
                    vb.indexTex.push_back(getIndexTex(specie, "bark"));
                }
            }
        }
    }

    void TreesGenerator::sendTree(Tree tree)
    {
        createTrunk(tree);

        //

        positions.push_back(tree.pos);
    }
}
