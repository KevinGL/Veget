#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

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

    createBranchs(params, resH, resV, skeleton, height, tree);

    Veget_Tex vt = getVegetTex(type);

    tree->tex.push_back(vt.barkTex);
    tree->tex.push_back(vt.barkNormalTex);
    tree->tex.push_back(vt.branchTex);

    glGenVertexArrays(1, &tree->VAO);
    glGenBuffers(1, &tree->VBO);

    glBindVertexArray(tree->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, tree->VBO);

    const size_t bufferSize = (tree->coordVertex.size() + tree->coordTex.size() + tree->normals.size() + tree->colors.size()) * sizeof(float) + tree->numtex.size() * sizeof(int);

    glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STREAM_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, tree->coordVertex.size() * sizeof(float), tree->coordVertex.data());
    glBufferSubData(GL_ARRAY_BUFFER, tree->coordVertex.size() * sizeof(float), tree->coordTex.size() * sizeof(float), tree->coordTex.data());
    glBufferSubData(GL_ARRAY_BUFFER, tree->coordVertex.size() * sizeof(float) + tree->coordTex.size() * sizeof(float), tree->normals.size() * sizeof(float), tree->normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, tree->coordVertex.size() * sizeof(float) + tree->coordTex.size() * sizeof(float) + tree->normals.size() * sizeof(float), tree->colors.size() * sizeof(float), tree->colors.data());
    glBufferSubData(GL_ARRAY_BUFFER, tree->coordVertex.size() * sizeof(float) + tree->coordTex.size() * sizeof(float) + tree->normals.size() * sizeof(float) + tree->colors.size() * sizeof(float), tree->numtex.size() * sizeof(int), tree->numtex.data());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float) , (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float) , (void*)(tree->coordVertex.size() * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float) , (void*)(tree->coordVertex.size() * sizeof(float) + tree->coordTex.size() * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float) , (void*)(tree->coordVertex.size() * sizeof(float) + tree->coordTex.size() * sizeof(float) + tree->normals.size() * sizeof(float)));
    glEnableVertexAttribArray(3);

    glVertexAttribIPointer(4, 1, GL_INT, sizeof(int) , (void*)(tree->coordVertex.size() * sizeof(float) + tree->coordTex.size() * sizeof(float) + tree->normals.size() * sizeof(float) + tree->colors.size() * sizeof(float)));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Veget::createSkeleton(TREE_TYPE type, const unsigned int resV, const float height, std::vector<glm::vec3> &skeleton)
{
    ParamsTrees params = getParamsTree(type);

    const float lgSeg = height/resV;

    skeleton.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

    for(size_t i=0; i<resV; i++)
    {
        glm::vec3 axeRot;
        const float angleZ = (float)(rand() % 360);

        axeRot.x = cos(angleZ * PI/180);
        axeRot.y = sin(angleZ * PI/180);
        axeRot.z = 0.0f;

        const float angleX = (float)(rand() % (2 * params.deltaAngle + 1) - params.deltaAngle);

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
        for(size_t j=0; j<resH; j++)
        {
            const size_t prec = j;
            size_t suiv = j+1;

            if(suiv == resH)
            {
                suiv = 0;
            }

            glm::vec3 point = circles[i].points[prec];

            tree->coordVertex.push_back(point.x + tree->pos.x);
            tree->coordVertex.push_back(point.y + tree->pos.y);
            tree->coordVertex.push_back(point.z + tree->pos.z);

            ////////////////////////////////////////////////

            point = circles[i+1].points[prec];

            tree->coordVertex.push_back(point.x + tree->pos.x);
            tree->coordVertex.push_back(point.y + tree->pos.y);
            tree->coordVertex.push_back(point.z + tree->pos.z);

            ////////////////////////////////////////////////

            point = circles[i].points[suiv];

            tree->coordVertex.push_back(point.x + tree->pos.x);
            tree->coordVertex.push_back(point.y + tree->pos.y);
            tree->coordVertex.push_back(point.z + tree->pos.z);

            ////////////////////////////////////////////////////////////////////////////////////////////////

            point = circles[i].points[suiv];

            tree->coordVertex.push_back(point.x + tree->pos.x);
            tree->coordVertex.push_back(point.y + tree->pos.y);
            tree->coordVertex.push_back(point.z + tree->pos.z);

            ////////////////////////////////////////////////

            point = circles[i+1].points[prec];

            tree->coordVertex.push_back(point.x + tree->pos.x);
            tree->coordVertex.push_back(point.y + tree->pos.y);
            tree->coordVertex.push_back(point.z + tree->pos.z);

            ////////////////////////////////////////////////

            point = circles[i+1].points[suiv];

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

            glm::vec3 normal = glm::normalize(circles[i].points[prec] - skeleton[i]);

            tree->normals.push_back(normal.x);
            tree->normals.push_back(normal.y);
            tree->normals.push_back(normal.z);

            ////////////////////////////////////////////////

            normal = glm::normalize(circles[i+1].points[prec] - skeleton[i+1]);

            tree->normals.push_back(normal.x);
            tree->normals.push_back(normal.y);
            tree->normals.push_back(normal.z);

            ////////////////////////////////////////////////

            normal = glm::normalize(circles[i].points[suiv] - skeleton[i]);

            tree->normals.push_back(normal.x);
            tree->normals.push_back(normal.y);
            tree->normals.push_back(normal.z);

            ////////////////////////////////////////////////////////////////////////////////////////////////

            normal = glm::normalize(circles[i].points[suiv] - skeleton[i]);

            tree->normals.push_back(normal.x);
            tree->normals.push_back(normal.y);
            tree->normals.push_back(normal.z);

            ////////////////////////////////////////////////

            normal = glm::normalize(circles[i+1].points[prec] - skeleton[i+1]);

            tree->normals.push_back(normal.x);
            tree->normals.push_back(normal.y);
            tree->normals.push_back(normal.z);

            ////////////////////////////////////////////////

            normal = glm::normalize(circles[i+1].points[suiv] - skeleton[i+1]);

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

void Veget::createBranchs(ParamsTrees pt, const unsigned int resH, const unsigned int resV, std::vector<glm::vec3> skeleton, const float height, Tree *tree)
{
    const unsigned int heightMin = 100 * (unsigned int)(pt.beginBranchs * height);
    const unsigned int heightMax = 100 * (unsigned int)height;
    const size_t nbBranchs = rand() % (16) + 5;

    for(size_t i=0; i<nbBranchs; i++)
    {
        const float beginBranch = (rand() % (heightMax - heightMin + 1) + heightMin) / 100.0f;

        glm::vec3 beginBranchSkeleton;

        for(size_t i=0; i<skeleton.size()-1; i++)
        {
            if(beginBranch >= skeleton[i].z && beginBranch <= skeleton[i+1].z)
            {
                const float deltaZ = beginBranch - skeleton[i].z;
                const float ratioZ = deltaZ / (skeleton[i+1].z - skeleton[i].z);
                glm::vec3 segment = skeleton[i+1] - skeleton[i];
                const float partLength = glm::length(segment) * ratioZ;

                beginBranchSkeleton = skeleton[i].z + partLength * segment;

                break;
            }
        }

        //std::cout << beginBranchSkeleton.x << " " << beginBranchSkeleton.y << " " << beginBranchSkeleton.z << std::endl;

        float branchLength;

        if(pt.branchsShape == VEGET_SHAPE_RANDOM)
        {
            //
        }
    }
}
