#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

void Veget::Init()
{
    std::ifstream datas("../Mini-libs/veget/DataTrees.txt");

    while(1)
    {
        std::string line;

        if(!getline(datas, line))
        {
            break;
        }

        ParamsTrees p;

        char type[line.length()];
        char pathTex[line.length()];

        sscanf(line.c_str(), "%s : heightMin=%d | heightMax=%d | trunkRadius=%f | ratioHeight=%f | coefTex=%f | tex=%s\n",
               type,
               &p.heightMin,
               &p.heightMax,
               &p.trunkRadius,
               &p.ratioHeight,
               &p.coefTex,
               pathTex);

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

    datas.close();

    isInitialized = true;
}

void Veget::AddTreesInArea(std::vector<glm::vec3> area, TREE_TYPE type, const unsigned int nbTrees, const unsigned int resH, const unsigned int resV)
{
    if(!isInitialized)
    {
        Init();
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

    std::cout << "Your trees are positionned :)" << std::endl << std::endl;


    for(size_t i=0; i<trees.size(); i++)
    {
        createTree(type, resH, resV, &trees[i]);
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

void Veget::createTree(TREE_TYPE type, const unsigned int resH, const unsigned int resV, Tree *tree)
{
    ParamsTrees params = getParamsTree(type);

    const float height = (float)(rand()%(params.heightMax - params.heightMin + 1) + params.heightMin);

    std::vector<glm::vec3> skeleton;

    createSkeleton(type, resV, height, skeleton);

    /*for(size_t i=0; i<skeleton.size(); i++)
    {
        std::cout << skeleton[i].x << " " << skeleton[i].y << " " << skeleton[i].z << std::endl;
    }*/

    createTrunk(params.trunkRadius, params.ratioHeight, resH, resV, height, params.coefTex, skeleton, tree);

    Veget_Tex vt = getVegetTex(type);

    tree->tex.push_back(vt.barkTex);
    tree->tex.push_back(vt.barkNormalTex);
    tree->tex.push_back(vt.branchTex);
}

void Veget::createSkeleton(TREE_TYPE type, const unsigned int resV, const float height, std::vector<glm::vec3> &skeleton)
{
    const float lgSeg = height/resV;

    skeleton.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

    int deltaAngle;

    if(type == TREE_PINE)
    {
        deltaAngle = 20;
    }

    for(size_t i=0; i<resV; i++)
    {
        glm::vec3 axeRot;
        const float angleZ = (float)(rand() % 360);

        axeRot.x = cos(angleZ * PI/180);
        axeRot.y = sin(angleZ * PI/180);
        axeRot.z = 0.0f;

        const float angleX = (float)(rand() % (2 * deltaAngle + 1) - deltaAngle);

        glm::mat4 matrixRot = glm::rotate(angleX, axeRot.x, axeRot.y, axeRot.z);

        glm::vec4 segment = glm::vec4(0.0f, 0.0f, lgSeg, 1.0f);

        segment = matrixRot * segment;

        const size_t indexPrec = skeleton.size() - 1;

        glm::vec3 suiv = skeleton[indexPrec] + glm::vec3(segment.x, segment.y, segment.z);

        skeleton.push_back(suiv);
    }
}

void Veget::createTrunk(const float trunkRadius, const float ratioHeight, const unsigned int resH, const unsigned int resV, const float height, const float multiTex, std::vector<glm::vec3> skeleton, Tree *tree)
{
    const float paramExp = log(ratioHeight) / height;

    std::vector<Veget_Circle> circles;

    for(size_t i=0; i<skeleton.size(); i++)
    {
        Veget_Circle circle;

        const float radius = trunkRadius * exp(paramExp * skeleton[i].z);
        glm::vec3 center = skeleton[i];

        float angle = 0.0f;
        const float alpha = 360.0f / resH;

        for(size_t j=0; j<resH; j++)
        {
            glm::vec3 point(radius * cos(angle * PI/180), radius * sin(angle * PI/180), 0.0f);

            point += center;

            circle.points.push_back(point);

            angle += alpha;
        }

        circles.push_back(circle);
    }

    const float partTexH = 1.0f / resH;
    const float partTexV = multiTex / resV;

    for(size_t i=0; i<circles.size()-1; i++)
    {
        for(size_t j=0; j<resH-1; j++)
        {
            glm::vec3 point = circles[i].points[j];

            tree->coordVertex.push_back(point.x + tree->pos.x);
            tree->coordVertex.push_back(point.y + tree->pos.y);
            tree->coordVertex.push_back(point.z + tree->pos.z);

            ////////////////////////////////////////////////

            point = circles[i+1].points[j];

            tree->coordVertex.push_back(point.x + tree->pos.x);
            tree->coordVertex.push_back(point.y + tree->pos.y);
            tree->coordVertex.push_back(point.z + tree->pos.z);

            ////////////////////////////////////////////////

            point = circles[i].points[j+1];

            tree->coordVertex.push_back(point.x + tree->pos.x);
            tree->coordVertex.push_back(point.y + tree->pos.y);
            tree->coordVertex.push_back(point.z + tree->pos.z);

            ////////////////////////////////////////////////////////////////////////////////////////////////

            point = circles[i].points[j+1];

            tree->coordVertex.push_back(point.x + tree->pos.x);
            tree->coordVertex.push_back(point.y + tree->pos.y);
            tree->coordVertex.push_back(point.z + tree->pos.z);

            ////////////////////////////////////////////////

            point = circles[i+1].points[j];

            tree->coordVertex.push_back(point.x + tree->pos.x);
            tree->coordVertex.push_back(point.y + tree->pos.y);
            tree->coordVertex.push_back(point.z + tree->pos.z);

            ////////////////////////////////////////////////

            point = circles[i+1].points[j+1];

            tree->coordVertex.push_back(point.x + tree->pos.x);
            tree->coordVertex.push_back(point.y + tree->pos.y);
            tree->coordVertex.push_back(point.z + tree->pos.z);

            ////////////////////////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////////////////////////

            glm::vec2 coordTex = glm::vec2(j * partTexH, i * partTexV);

            tree->coordTex.push_back(coordTex.x);
            tree->coordTex.push_back(coordTex.y);

            ////////////////////////////////////////////////

            coordTex = glm::vec2(j * partTexH, (i+1) * partTexV);

            tree->coordTex.push_back(coordTex.x);
            tree->coordTex.push_back(coordTex.y);

            ////////////////////////////////////////////////

            coordTex = glm::vec2((j+1) * partTexH, i * partTexV);

            tree->coordTex.push_back(coordTex.x);
            tree->coordTex.push_back(coordTex.y);

            ////////////////////////////////////////////////////////////////////////////////////////////////

            coordTex = glm::vec2((j+1) * partTexH, i * partTexV);

            tree->coordTex.push_back(coordTex.x);
            tree->coordTex.push_back(coordTex.y);

            ////////////////////////////////////////////////

            coordTex = glm::vec2(j * partTexH, (i+1) * partTexV);

            tree->coordTex.push_back(coordTex.x);
            tree->coordTex.push_back(coordTex.y);

            ////////////////////////////////////////////////

            coordTex = glm::vec2((j+1) * partTexH, (i+1) * partTexV);

            tree->coordTex.push_back(coordTex.x);
            tree->coordTex.push_back(coordTex.y);

            ////////////////////////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////////////////////////

            glm::vec3 normal = glm::normalize(circles[i].points[j] - skeleton[i]);

            tree->normals.push_back(normal.x);
            tree->normals.push_back(normal.y);
            tree->normals.push_back(normal.z);

            ////////////////////////////////////////////////

            normal = glm::normalize(circles[i+1].points[j] - skeleton[i+1]);

            tree->normals.push_back(normal.x);
            tree->normals.push_back(normal.y);
            tree->normals.push_back(normal.z);

            ////////////////////////////////////////////////

            normal = glm::normalize(circles[i].points[j+1] - skeleton[i]);

            tree->normals.push_back(normal.x);
            tree->normals.push_back(normal.y);
            tree->normals.push_back(normal.z);

            ////////////////////////////////////////////////////////////////////////////////////////////////

            normal = glm::normalize(circles[i].points[j+1] - skeleton[i]);

            tree->normals.push_back(normal.x);
            tree->normals.push_back(normal.y);
            tree->normals.push_back(normal.z);

            ////////////////////////////////////////////////

            normal = glm::normalize(circles[i+1].points[j] - skeleton[i+1]);

            tree->normals.push_back(normal.x);
            tree->normals.push_back(normal.y);
            tree->normals.push_back(normal.z);

            ////////////////////////////////////////////////

            normal = glm::normalize(circles[i+1].points[j+1] - skeleton[i+1]);

            tree->normals.push_back(normal.x);
            tree->normals.push_back(normal.y);
            tree->normals.push_back(normal.z);
        }
    }

    for(size_t i=0; i<tree->coordVertex.size()/3; i++)
    {
        tree->colors.push_back(1.0f);
        tree->colors.push_back(1.0f);
        tree->colors.push_back(1.0f);
    }

    for(size_t i=0; i<tree->coordVertex.size()/3; i++)
    {
        tree->numtex.push_back(0);      //Bark
    }
}
