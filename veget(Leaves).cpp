#include <dirent.h>
#include <sstream>
#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

namespace Veget
{
    size_t VegetGenerator::createLeaves(std::vector<glm::vec3> skeleton, const float branchLg, const int indexTex, const glm::mat4 rotZ)
    {
        size_t nbVertices = 0;

        const float size = branchLg / 2;

        for(const glm::vec3 v : skeleton)
        {
            const float angleX = rand() % 360;
            const glm::mat4 rotX = glm::rotate(angleX, 1.0f, 0.0f, 0.0f);

            glm::vec4 v4 = rotX * rotZ * glm::vec4(-size/2, 0.0f, 0.0f, 1.0f);
            vb.coordVert.push_back(v.x + v4.x);
            vb.coordVert.push_back(v.y + v4.y);
            vb.coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(-size/2, 0.0f, size, 1.0f);
            vb.coordVert.push_back(v.x + v4.x);
            vb.coordVert.push_back(v.y + v4.y);
            vb.coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(size/2, 0.0f, 0.0f, 1.0f);
            vb.coordVert.push_back(v.x + v4.x);
            vb.coordVert.push_back(v.y + v4.y);
            vb.coordVert.push_back(v.z + v4.z);

            /////////////////

            v4 = rotX * rotZ * glm::vec4(size/2, 0.0f, 0.0f, 1.0f);
            vb.coordVert.push_back(v.x + v4.x);
            vb.coordVert.push_back(v.y + v4.y);
            vb.coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(-size/2, 0.0f, size, 1.0f);
            vb.coordVert.push_back(v.x + v4.x);
            vb.coordVert.push_back(v.y + v4.y);
            vb.coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(size/2, 0.0f, size, 1.0f);
            vb.coordVert.push_back(v.x + v4.x);
            vb.coordVert.push_back(v.y + v4.y);
            vb.coordVert.push_back(v.z + v4.z);

            //////////////////////////////////

            vb.coordTex.push_back(0.5f);
            vb.coordTex.push_back(0.0f);

            vb.coordTex.push_back(0.5f);
            vb.coordTex.push_back(1.0f);

            vb.coordTex.push_back(1.0f);
            vb.coordTex.push_back(0.0f);

            /////////////////

            vb.coordTex.push_back(1.0f);
            vb.coordTex.push_back(0.0f);

            vb.coordTex.push_back(0.5f);
            vb.coordTex.push_back(1.0f);

            vb.coordTex.push_back(1.0f);
            vb.coordTex.push_back(1.0f);

            ////////////////////////////////////////////////////////////////////

            v4 = rotX * rotZ * glm::vec4(0.0f, -size/2, 0.0f, 1.0f);
            vb.coordVert.push_back(v.x + v4.x);
            vb.coordVert.push_back(v.y + v4.y);
            vb.coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(0.0f, -size/2, size, 1.0f);
            vb.coordVert.push_back(v.x + v4.x);
            vb.coordVert.push_back(v.y + v4.y);
            vb.coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(0.0f, size/2, 0.0f, 1.0f);
            vb.coordVert.push_back(v.x + v4.x);
            vb.coordVert.push_back(v.y + v4.y);
            vb.coordVert.push_back(v.z + v4.z);

            /////////////////

            v4 = rotX * rotZ * glm::vec4(0.0f, size/2, 0.0f, 1.0f);
            vb.coordVert.push_back(v.x + v4.x);
            vb.coordVert.push_back(v.y + v4.y);
            vb.coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(0.0f, -size/2, size, 1.0f);
            vb.coordVert.push_back(v.x + v4.x);
            vb.coordVert.push_back(v.y + v4.y);
            vb.coordVert.push_back(v.z + v4.z);

            v4 = rotX * rotZ * glm::vec4(0.0f, size/2, size, 1.0f);
            vb.coordVert.push_back(v.x + v4.x);
            vb.coordVert.push_back(v.y + v4.y);
            vb.coordVert.push_back(v.z + v4.z);

            //////////////////////////////////

            vb.coordTex.push_back(0.5f);
            vb.coordTex.push_back(0.0f);

            vb.coordTex.push_back(0.5f);
            vb.coordTex.push_back(1.0f);

            vb.coordTex.push_back(1.0f);
            vb.coordTex.push_back(0.0f);

            /////////////////

            vb.coordTex.push_back(1.0f);
            vb.coordTex.push_back(0.0f);

            vb.coordTex.push_back(0.5f);
            vb.coordTex.push_back(1.0f);

            vb.coordTex.push_back(1.0f);
            vb.coordTex.push_back(1.0f);

            ////////////////////////////////////////////////////////////////////

            for(size_t i = 0 ; i < 12 ; i++)
            {
                vb.normals.push_back(0.0f);
                vb.normals.push_back(0.0f);
                vb.normals.push_back(0.0f);

                ////

                vb.indexTex.push_back(indexTex);
            }

            nbVertices += 6;
        }

        return nbVertices;
    }
}
