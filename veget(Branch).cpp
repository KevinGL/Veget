#include <dirent.h>
#include <conio.h>
#include <sstream>
#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

namespace Veget
{
    size_t VegetGenerator::createBranchs(std::string specie, std::vector<glm::vec3> skeleton, const float trunkRadius, VertexBuffer *model)
    {
        float beginBranch;
        float angleBranch;
        float ratioTopBottom;

        beginBranch = params[specie].beginBranch;
        angleBranch = params[specie].angleBranch;
        ratioTopBottom = params[specie].ratioTopBottom;

        std::vector<glm::vec3> baseBranchs;

        const float gapBranchs = 0.2f;

        const float height = skeleton[skeleton.size()-1].z - skeleton[0].z;
        const float zBegin = skeleton[0].z + beginBranch * height;

        for(float z = zBegin ; z < skeleton[skeleton.size()-1].z ; z += gapBranchs)
        {
            Plane plane;

            plane.a = 0.0f;
            plane.b = 0.0f;
            plane.c = 1.0f;
            plane.d = -z;

            for(size_t i = 0 ; i < skeleton.size()-1 ; i++)
            {
                Segment seg;

                seg.p1 = skeleton[i];
                seg.p2 = skeleton[i+1];

                /*std::cout << seg.p1.x << " " << seg.p1.y << " " << seg.p1.z << std::endl;
                std::cout << seg.p2.x << " " << seg.p2.y << " " << seg.p2.z << std::endl << std::endl;*/

                glm::vec3 inter;

                if(interPlaneSeg(plane, seg, inter))
                {
                    baseBranchs.push_back(inter);
                }
            }
        }

        size_t nbVertices = 0;

        const float ratioBranchTrunk = params[specie].ratioBranchTrunk;
        const float lgMax = ratioBranchTrunk * height;
        size_t index = 0;

        const float htMin = baseBranchs[0].z;
        const float htMax = baseBranchs[baseBranchs.size()-1].z;
        const float htMed = (htMin + htMax) / 2;

        for(const glm::vec3 base : baseBranchs)
        {
            const float angleZ = rand() % 360;
            float lg;
            const float lgMin = 0.5f * lgMax;
            const float angleY = rand() % 21 - 10 + params[specie].angleBranch;
            const std::string shape = params[specie].shape;

            if(shape == "Rect")
            {
                if(index <= baseBranchs.size() / 4)
                {
                    const float coef = (lgMax - lgMin) / (baseBranchs.size() / 4);
                    lg = coef * index + lgMin;
                }

                else
                if(index > baseBranchs.size() / 4 && index <= 3 * baseBranchs.size() / 4)
                {
                    lg = lgMax;
                }

                else
                if(index > 3 * baseBranchs.size() / 4)
                {
                    const float coef = -(lgMax - lgMin) / (baseBranchs.size() / 4);
                    const float ord = -coef * baseBranchs.size() + lgMin;
                    lg = coef * index + ord;
                }
            }

            else
            if(shape == "Sphere")
            {
                const float deltaZ = fabs(base.z - htMed);
                const float radius = (htMax - htMin) / 2;

                lg = sqrt(pow(radius, 2) - pow(deltaZ, 2));
            }

            else
            if(shape == "SemiSphere")
            {
                const float deltaZ = fabs(base.z - htMin);
                const float radius = htMax - htMin;

                lg = sqrt(pow(radius, 2) - pow(deltaZ, 2));
            }

            nbVertices += createBranch(base, trunkRadius * ratioBranchTrunk, ratioTopBottom, lg, angleZ, angleY, model);

            index++;
        }
    }

    size_t VegetGenerator::createBranch(const glm::vec3 base, const float radius, const float ratioTopBottom, const float lg, const float angleZ, const float angleY, VertexBuffer *model)
    {
        const float bottomDiameter = 2 * radius;
        const float topDiameter = 0.0f;//bottomDiameter * ratioTopBottom;
        const float segLg = lg / nbSeg;

        std::vector<Circle> circles;
        std::vector<glm::vec3> skeleton;

        glm::vec3 center = base;

        const glm::mat4 rotationZ = glm::rotate(angleZ, 0.0f, 0.0f, 1.0f);
        const glm::mat4 rotationY = glm::rotate(angleY, 0.0f, 1.0f, 0.0f);

        for(size_t i = 0 ; i < nbSeg + 1 ; i++)
        {
            const float coef = (topDiameter - bottomDiameter) / (nbSeg+1);
            const float diameter = coef * i + bottomDiameter;
            const float circleRadius = diameter / 2;

            Circle circle;

            const float alpha = 360.0f / res;
            float angle = 0.0f;

            for(size_t j = 0 ; j < res ; j++)
            {
                glm::vec4 v;

                v.x = 0.0f;
                v.y = circleRadius * cos(angle * PI/180);
                v.z = circleRadius * sin(angle * PI/180);
                v.w = 1.0f;

                v = rotationY * v;
                v = rotationZ * v;

                v.x += center.x;
                v.y += center.y;
                v.z += center.z;

                circle.vertices.push_back(glm::vec3(v.x, v.y, v.z));

                angle += alpha;
            }

            circles.push_back(circle);

            glm::vec4 segVec;

            const int randGap = (int)(100 * lg / 8);

            segVec.x = segLg;
            segVec.y = (rand() % (randGap + 1) - randGap/2) / 100.0f;
            segVec.z = (rand() % (randGap + 1) - randGap/2) / 100.0f;
            segVec.w = 1.0f;

            segVec = rotationY * segVec;
            segVec = rotationZ * segVec;

            skeleton.push_back(center);

            center.x += segVec.x;
            center.y += segVec.y;
            center.z += segVec.z;
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

                model->coordTex.push_back(coordTex);
                model->coordTex.push_back(0.0f);

                model->coordTex.push_back(coordTex);
                model->coordTex.push_back(1.0f);

                model->coordTex.push_back(coordTex + deltaTex);
                model->coordTex.push_back(0.0f);

                //////////////////////////////

                model->coordTex.push_back(coordTex + deltaTex);
                model->coordTex.push_back(0.0f);

                model->coordTex.push_back(coordTex);
                model->coordTex.push_back(1.0f);

                model->coordTex.push_back(coordTex + deltaTex);
                model->coordTex.push_back(1.0f);

                coordTex += deltaTex;

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


                ///////////////////////////////////////////////////////////////

                nbVertices += 6;
            }
        }

        nbVertices += createLeaves(skeleton, lg, rotationZ, model);

        return nbVertices;
    }
}
