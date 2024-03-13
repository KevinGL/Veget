#include <dirent.h>
#include <conio.h>
#include <sstream>
#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

namespace Veget
{
    void VegetGenerator::createBranchs(const std::string specie, std::vector<glm::vec3> skeleton, const float trunkRadius, VertexBuffer *model)
    {
        const float beginBranch = params[specie].beginBranch;
        const float angleBranch = params[specie].angleBranch;
        const float ratioTopBottomBranch = params[specie].ratioTopBottomBranch;
        const float branchsWeigth = params[specie].branchsWeigth;
        float ratioLeavesBranch = params[specie].ratioLeavesBranch;
        const int torsion = params[specie].torsionBranchs;

        std::vector<glm::vec3> baseBranchs;

        const float gapBranchs = 0.1f;

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

        const float ratioBranchTrunkLength = params[specie].ratioBranchTrunkLength;
        const float ratioBranchTrunkRadius = params[specie].ratioBranchTrunkRadius;
        const float lgMax = ratioBranchTrunkLength * height;
        size_t index = 0;

        const float htMin = baseBranchs[0].z;
        const float htMax = baseBranchs[baseBranchs.size()-1].z;
        const float htMed = (htMin + htMax) / 2;

        for(const glm::vec3 base : baseBranchs)
        {
            const float angleZ = rand() % 360;
            float lg;
            const float angleY = rand() % 21 - 10 + params[specie].angleBranch;
            const std::string shape = params[specie].shape;

            if(shape == "RECT")
            {
                /*const float lgMin = 0.5f * lgMax;

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
                }*/

                lg = lgMax;
            }

            else
            if(shape == "SPHERE")
            {
                const float deltaZ = fabs(base.z - htMed);
                const float radius = fabs(htMax - htMed);

                lg = sqrt(pow(radius, 2) - pow(deltaZ, 2));
            }

            else
            if(shape == "SEMISPHERE")
            {
                const float deltaZ = fabs(base.z - htMin);
                const float radius = htMax - htMin;

                lg = sqrt(pow(radius, 2) - pow(deltaZ, 2));
            }

            else
            if(shape == "CONIC")
            {
                const float lgMin = 0.1f * lgMax;

                const float coef = (lgMax - lgMin) / (htMin - htMax);
                const float ord = -coef * htMin + lgMax;

                lg = coef * base.z + ord;
            }

            else
            if(shape == "FASTIGIATED")
            {
                const float htInterm = htMin + 0.125f * (htMax - htMin);

                if(base.z <= htInterm)
                {
                    const float period = 4 * (htInterm - htMin);
                    const float pulsation = (2 * M_PI) / period;
                    const float phase = -pulsation * htMin;
                    const float amp = lgMax;

                    lg = amp * sin(pulsation * base.z + phase);

                    if(lg < 0)
                    {
                        lg = 0.0f;
                    }

                    //std::cout << "period=" << period << " pulsation=" << pulsation << " phase=" << phase << " amp=" << amp << " z=" << base.z << " => " << lg << " " << leavesSize << std::endl;
                }

                else
                {
                    const float period = 4.1f * (htMax - htInterm);
                    const float pulsation = (2 * M_PI) / period;
                    const float amp = lgMax;
                    const float phase = asin(lgMax / amp) - pulsation * htInterm;

                    lg = amp * sin(pulsation * base.z + phase);

                    if(lg < 0)
                    {
                        lg = 0.0f;
                    }

                    //std::cout << "period=" << period << " pulsation=" << pulsation << " phase=" << phase << " amp=" << amp << " z=" << base.z << " => " << lg << " " << leavesSize << std::endl;
                }
            }

            createBranch(branchsWeigth, ratioLeavesBranch, torsion, base, trunkRadius * ratioBranchTrunkRadius, ratioTopBottomBranch, lg, angleZ, angleY, model);

            index++;
        }
    }

    void VegetGenerator::createBranch(const float branchsWeight, const float ratioLeavesBranch, const int torsion, const glm::vec3 base, const float radius, const float ratioTopBottom, const float lg, const float angleZ, const float angleY, VertexBuffer *model)
    {
        const float bottomDiameter = 2 * radius;
        const float topDiameter = bottomDiameter * ratioTopBottom;
        const float segLg = lg / nbSeg;

        std::vector<Circle> circles;
        std::vector<glm::vec3> skeleton;

        glm::vec3 center = base;

        const glm::mat4 rotationZ = glm::rotate(angleZ, 0.0f, 0.0f, 1.0f);
        const glm::mat4 rotationY = glm::rotate(angleY, 0.0f, 1.0f, 0.0f);

        float distMax = 0.0f;

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

            segVec.x = segLg;

            if(torsion != 0)
            {
                segVec.y = (rand() % torsion - torsion/2) / 100.0f;
                segVec.z = (rand() % torsion - torsion/2) / 100.0f;// + getCurve(branchsCurve, i, base.z);
            }
            else
            {
                segVec.y = 0.0f;
                segVec.y = 0.0f;//getCurve(branchsCurve, i, base.z);
            }

            segVec.w = 1.0f;

            segVec = rotationY * segVec;
            segVec = rotationZ * segVec;

            skeleton.push_back(center);

            const float distXY = sqrt(pow(skeleton[i].x - base.x, 2) + pow(skeleton[i].y - base.y, 2));

            if(distXY > distMax)
            {
                distMax = distXY;
            }

            center.x += segVec.x;
            center.y += segVec.y;
            center.z += segVec.z;
        }

        const float zMax = -base.z;
        const float coef = (zMax / (distMax * distMax)) * branchsWeight;

        for(size_t i = 0 ; i < skeleton.size() ; i++)
        {
            const float distXY = sqrt(pow(skeleton[i].x - base.x, 2) + pow(skeleton[i].y - base.y, 2));
            const float deltaZ = coef * (distXY * distXY);

            skeleton[i].z += deltaZ;

            for(size_t j = 0 ; j < circles[i].vertices.size() ; j++)
            {
                circles[i].vertices[j].z += deltaZ;
            }
        }

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
            const float coordTexY = 0.5f * segLg / bottomDiameter;

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
            }
        }

        createLeaves(skeleton, ratioLeavesBranch, rotationZ, rotationY, model);
    }

    float VegetGenerator::getCurve(const std::string curve, const size_t index, const float zBase)
    {
        float deltaZ;

        if(curve == "LINEAR")
        {
            deltaZ = 0.0f;
        }

        else
        if(curve == "WEEPING")
        {
            const float coef = -zBase / (nbSeg * nbSeg);
            deltaZ = coef * (index * index);
        }

        return deltaZ;
    }
}
