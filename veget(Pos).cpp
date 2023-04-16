#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

void Veget::AddTreesGroup(std::vector<glm::vec3> area, std::vector<TREE_TYPE> types, const unsigned int nbTrees, const unsigned int resH, const unsigned int resV)
{
    if(!isInitialized)
    {
        Init();
    }

    if(types.size() == 0)
    {
        std::cout << "You have not indicated types of trees, aborting" << std::endl;
    }

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

    const unsigned int precision = 100;

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

    TreesGroup tg;

    for(size_t i=0; i<pointsInPoly.size(); i++)
    {
        std::vector<glm::vec3> triangleOfThisPoint = getClosest3Points(pointsInPoly[i], area);

        //std::cout << i << " : " << pointsInPoly[i].x << " " << pointsInPoly[i].y << " => (" << triangleOfThisPoint[0].x << " " << triangleOfThisPoint[0].y << ") (" << triangleOfThisPoint[1].x << " " << triangleOfThisPoint[1].y << ") (" << triangleOfThisPoint[2].x << " " << triangleOfThisPoint[2].y << ") => ";

        const float Z = getProjectionZ(glm::vec2(pointsInPoly[i].x, pointsInPoly[i].y), triangleOfThisPoint);

        //std::cout << Z << std::endl;

        Tree tree;

        tree.pos = glm::vec3(pointsInPoly[i].x, pointsInPoly[i].y, Z);

        tg.trees.push_back(tree);
    }

    std::cout << "Your trees are positionned :)" << std::endl << std::endl;


    for(size_t i=0; i<tg.trees.size(); i++)
    {
        size_t indexType = rand() % types.size();

        TREE_TYPE type = types[indexType];

        createTree(type, resH, resV, &tg.trees[i]);
    }

    for(size_t i=0; i<area.size(); i++)
    {
        tg.perimeter.push_back(glm::vec2(area[i].x, area[i].y));
    }

    tg.pos = glm::vec2(0.0f);

    for(size_t i=0; i<tg.perimeter.size(); i++)
    {
        tg.pos += tg.perimeter[i];
    }

    tg.pos /= tg.perimeter.size();

    treesGroups.push_back(tg);
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

    glm::vec2 test = glm::vec2(polygon[second].x, polygon[second].y) - glm::vec2(polygon[first].x, polygon[first].y);

    distMin = 1000000000000.0f;
    unsigned int third;

    for(size_t i=0; i<polygon.size(); i++)
    {
        const float dist = sqrt(pow(polygon[i].x - point.x, 2) + pow(polygon[i].y - point.y, 2));

        glm::vec2 test2 = glm::vec2(polygon[i].x, polygon[i].y) - glm::vec2(polygon[first].x, polygon[first].y);

        if(distMin > dist && i != first && i != second && glm::dot(test, test2) > -1.0f && glm::dot(test, test2) < 1.0f)
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

ParamsTrees Veget::getParamsTree(TREE_TYPE type)
{
    for(size_t i=0; i<paramsTrees.size(); i++)
    {
        if(paramsTrees[i].type == type)
        {
            return paramsTrees[i];
        }
    }
}

Veget_Tex Veget::getVegetTex(TREE_TYPE type)
{
    for(size_t i=0; i<textures.size(); i++)
    {
        if(textures[i].treeType == type)
        {
            return textures[i];
        }
    }
}

void Veget::AddTreesPositions(std::vector<glm::vec3> positions, std::vector<TREE_TYPE> types, const unsigned int resH, const unsigned int resV)
{
    if(!isInitialized)
    {
        Init();
    }

    if(types.size() == 0)
    {
        std::cout << "You have not indicated types of trees, aborting" << std::endl;
    }

    std::cout << "Add of trees ..." << std::endl;

    TreesGroup tg;

    for(size_t i=0; i<positions.size(); i++)
    {
        Tree tree;

        tree.pos = positions[i];

        tg.trees.push_back(tree);
    }

    std::cout << "Your trees are positionned :)" << std::endl << std::endl;

    for(size_t i=0; i<tg.trees.size(); i++)
    {
        size_t indexType = rand() % types.size();

        TREE_TYPE type = types[indexType];

        createTree(type, resH, resV, &tg.trees[i]);
    }

    for(size_t i=0; i<positions.size(); i++)
    {
        tg.perimeter.push_back(glm::vec2(positions[i].x, positions[i].y));
    }

    tg.pos = glm::vec2(0.0f);

    for(size_t i=0; i<tg.perimeter.size(); i++)
    {
        tg.pos += tg.perimeter[i];
    }

    tg.pos /= tg.perimeter.size();

    treesGroups.push_back(tg);
}
