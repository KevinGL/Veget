#include "veget.h"

void Veget::AddTreesInArea(std::vector<glm::vec3> area, TREE_TYPE type, const unsigned int nbTrees, const unsigned int precision)
{
    std::cout << "Calcul random position of trees ..." << std::endl;

    float xMoy = 0.0f, yMoy = 0.0f;

    for(size_t i=0; i<area.size(); i++)
    {
        xMoy += area[i].x;
        yMoy += area[i].y;
    }

    xMoy /= area.size();
    yMoy /= area.size();

    ////////////

    float xMin = xMoy, xMax = xMoy;
    float yMin = yMoy, yMax = yMoy;

    for(size_t i=0; i<area.size(); i++)
    {
        if(xMin > area[i].x)
        {
            xMin = area[i].x;
        }
    }

    for(size_t i=0; i<area.size(); i++)
    {
        if(yMin > area[i].y)
        {
            yMin = area[i].y;
        }
    }

    //////

    for(size_t i=0; i<area.size(); i++)
    {
        if(xMax < area[i].x)
        {
            xMax = area[i].x;
        }
    }

    for(size_t i=0; i<area.size(); i++)
    {
        if(yMax < area[i].y)
        {
            yMax = area[i].y;
        }
    }

    ////////////

    const int xMinInt = precision * (int)xMin;
    const int xMaxInt = precision * (int)xMax;
    const int yMinInt = precision * (int)yMin;
    const int yMaxInt = precision * (int)yMax;

    std::vector<glm::vec3> pointsInPoly;

    while(1)
    {
        glm::vec3 p;

        p.x = (float)(rand()%(xMaxInt - xMinInt + 1) + xMinInt)/precision;
        p.y = (float)(rand()%(yMaxInt - yMinInt + 1) + yMinInt)/precision;

        //std::cout << p.x << " " << p.y << std::endl;

        if(inPolygon(p, area))
        {
            pointsInPoly.push_back(p);
        }

        if(pointsInPoly.size() >= nbTrees)
        {
            break;
        }
    }

    for(size_t i=0; i<pointsInPoly.size(); i++)
    {
        std::vector<glm::vec3> triangleOfThisPoint = getClosest3Points(pointsInPoly[i], area);

        //std::cout << pointsInPoly[i].x << " " << pointsInPoly[i].y << " => (" << triangleOfThisPoint[0].x << " " << triangleOfThisPoint[0].y << ") (" << triangleOfThisPoint[1].x << " " << triangleOfThisPoint[1].y << ") (" << triangleOfThisPoint[2].x << " " << triangleOfThisPoint[2].y << ")" << std::endl;

        const float Z = getProjectionZ(glm::vec2(pointsInPoly[i].x, pointsInPoly[i].y), triangleOfThisPoint);

        //std::cout << pointsInPoly[i].x << " " << pointsInPoly[i].y << " " << Z << std::endl;

        Tree tree;

        tree.pos = glm::vec3(pointsInPoly[i].x, pointsInPoly[i].y, Z);

        trees.push_back(tree);
    }


    for(size_t i=0; i<trees.size(); i++)
    {
        createTree(type, &trees[i]);
    }
}

bool Veget::inPolygon(glm::vec3 point, std::vector<glm::vec3> polygon)
{
    unsigned int nbInters = 0;

    for(size_t i=0; i<polygon.size(); i++)
    {
        const size_t index1 = i;

        size_t index2 = i+1;
        if(index2 == polygon.size())
        {
            index2 = 0;
        }

        glm::vec3 p1 = polygon[index1];
        glm::vec3 p2 = polygon[index2];

        if(p2.x - p1.x != 0.0f)
        {
            const float coef = (p2.y - p1.y)/(p2.x - p1.x);
            const float ord = -coef * p1.x + p1.y;

            glm::vec2 inter;

            inter.y = coef * point.x + ord;
            inter.x = point.x;

            glm::vec2 miSeg;

            miSeg.x = (p1.x + p2.x)/2;
            miSeg.y = (p1.y + p2.y)/2;

            const float lgSeg = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
            const float distMiSeg_Inter = sqrt(pow(inter.x - miSeg.x, 2) + pow(inter.y - miSeg.y, 2));

            if(distMiSeg_Inter < lgSeg/2 && inter.y > point.y)
            {
                nbInters++;
            }
        }
    }

    if(nbInters % 2 == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::vector<glm::vec3> Veget::getClosest3Points(glm::vec3 point, std::vector<glm::vec3> polygon)
{
    float distMin = 1000000000000.0f;
    unsigned int first;
    std::vector<glm::vec3> result;

    for(size_t i=0; i<polygon.size(); i++)
    {
        const float dist = sqrt(pow(polygon[i].x - point.x, 2) + pow(polygon[i].y - point.y, 2));

        if(distMin > dist)
        {
            distMin = dist;
            first = i;
        }
    }

    result.push_back(polygon[first]);

    distMin = 1000000000000.0f;
    unsigned int second;

    for(size_t i=0; i<polygon.size(); i++)
    {
        const float dist = sqrt(pow(polygon[i].x - point.x, 2) + pow(polygon[i].y - point.y, 2));

        if(distMin > dist && i != first)
        {
            distMin = dist;
            second = i;
        }
    }

    result.push_back(polygon[second]);

    distMin = 1000000000000.0f;
    unsigned int third;

    for(size_t i=0; i<polygon.size(); i++)
    {
        const float dist = sqrt(pow(polygon[i].x - point.x, 2) + pow(polygon[i].y - point.y, 2));

        if(distMin > dist && i != first && i != second)
        {
            distMin = dist;
            third = i;
        }
    }

    result.push_back(polygon[third]);

    return result;
}

float Veget::getProjectionZ(glm::vec2 point, std::vector<glm::vec3> triangle)
{
    glm::vec3 v1 = triangle[1] - triangle[0];
    glm::vec3 v2 = triangle[2] - triangle[0];
    glm::vec3 normal = glm::cross(v1, v2);

    const float a = normal.x;
    const float b = normal.y;
    const float c = normal.z;
    const float d = -(a * triangle[0].x + b * triangle[0].y + c * triangle[0].z);

    const float x0 = point.x;
    const float y0 = point.y;
    const float z0 = 0.0f;

    const float xV = 0.0f;
    const float yV = 0.0f;
    const float zV = 1.0f;

    const float t = -(a * x0 + b * y0 + c * z0 + d)/(a * xV + b * yV + c * zV);

    return z0 + t * zV;
}

void Veget::createTree(TREE_TYPE type, Tree *tree)
{
    //
}
