#include <dirent.h>
#include <sstream>
#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

namespace Veget
{
    size_t VegetGenerator::createLeaves(std::vector<glm::vec3> skeleton, const float branchLg, const glm::mat4 rotZ, VertexBuffer *model)
    {
        size_t nbVertices = 0;

        const float size = branchLg / 2;

        for(const glm::vec3 v : skeleton)
        {
            const float angleX = rand() % 360;
            const glm::mat4 rotX = glm::rotate(angleX, 1.0f, 0.0f, 0.0f);

            glm::vec4 v4 = rotX * rotZ * glm::vec4(-size/2, 0.0f, 0.0f, 1.0f);
            model->coordVert.push_back(v.x + v4.x);
            model->coordVert.push_back(v.y + v4.y);
            model->coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(-size/2, 0.0f, size, 1.0f);
            model->coordVert.push_back(v.x + v4.x);
            model->coordVert.push_back(v.y + v4.y);
            model->coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(size/2, 0.0f, 0.0f, 1.0f);
            model->coordVert.push_back(v.x + v4.x);
            model->coordVert.push_back(v.y + v4.y);
            model->coordVert.push_back(v.z + v4.z);

            /////////////////

            v4 = rotX * rotZ * glm::vec4(size/2, 0.0f, 0.0f, 1.0f);
            model->coordVert.push_back(v.x + v4.x);
            model->coordVert.push_back(v.y + v4.y);
            model->coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(-size/2, 0.0f, size, 1.0f);
            model->coordVert.push_back(v.x + v4.x);
            model->coordVert.push_back(v.y + v4.y);
            model->coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(size/2, 0.0f, size, 1.0f);
            model->coordVert.push_back(v.x + v4.x);
            model->coordVert.push_back(v.y + v4.y);
            model->coordVert.push_back(v.z + v4.z);

            //////////////////////////////////

            model->coordTex.push_back(0.5f);
            model->coordTex.push_back(0.0f);

            model->coordTex.push_back(0.5f);
            model->coordTex.push_back(1.0f);

            model->coordTex.push_back(1.0f);
            model->coordTex.push_back(0.0f);

            /////////////////

            model->coordTex.push_back(1.0f);
            model->coordTex.push_back(0.0f);

            model->coordTex.push_back(0.5f);
            model->coordTex.push_back(1.0f);

            model->coordTex.push_back(1.0f);
            model->coordTex.push_back(1.0f);

            ////////////////////////////////////////////////////////////////////

            v4 = rotX * rotZ * glm::vec4(0.0f, -size/2, 0.0f, 1.0f);
            model->coordVert.push_back(v.x + v4.x);
            model->coordVert.push_back(v.y + v4.y);
            model->coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(0.0f, -size/2, size, 1.0f);
            model->coordVert.push_back(v.x + v4.x);
            model->coordVert.push_back(v.y + v4.y);
            model->coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(0.0f, size/2, 0.0f, 1.0f);
            model->coordVert.push_back(v.x + v4.x);
            model->coordVert.push_back(v.y + v4.y);
            model->coordVert.push_back(v.z + v4.z);

            /////////////////

            v4 = rotX * rotZ * glm::vec4(0.0f, size/2, 0.0f, 1.0f);
            model->coordVert.push_back(v.x + v4.x);
            model->coordVert.push_back(v.y + v4.y);
            model->coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(0.0f, -size/2, size, 1.0f);
            model->coordVert.push_back(v.x + v4.x);
            model->coordVert.push_back(v.y + v4.y);
            model->coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(0.0f, size/2, size, 1.0f);
            model->coordVert.push_back(v.x + v4.x);
            model->coordVert.push_back(v.y + v4.y);
            model->coordVert.push_back(v.z + v4.z);

            //////////////////////////////////

            model->coordTex.push_back(0.5f);
            model->coordTex.push_back(0.0f);

            model->coordTex.push_back(0.5f);
            model->coordTex.push_back(1.0f);

            model->coordTex.push_back(1.0f);
            model->coordTex.push_back(0.0f);

            /////////////////

            model->coordTex.push_back(1.0f);
            model->coordTex.push_back(0.0f);

            model->coordTex.push_back(0.5f);
            model->coordTex.push_back(1.0f);

            model->coordTex.push_back(1.0f);
            model->coordTex.push_back(1.0f);

            ////////////////////////////////////////////////////////////////////

            for(size_t i = 0 ; i < 12 ; i++)
            {
                model->normals.push_back(0.0f);
                model->normals.push_back(0.0f);
                model->normals.push_back(0.0f);
            }

            nbVertices += 6;
        }

        return nbVertices;
    }
}
