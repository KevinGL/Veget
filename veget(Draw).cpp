#include <dirent.h>
#include <sstream>
#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

namespace Veget
{
    glm::vec3 VegetGenerator::getPos(const size_t index)
    {
        return positions[index];
    }

    void VegetGenerator::drawVB_By_Index(const size_t index, const GLuint shader)
    {
        for(size_t i = 0; i < vb.textures.size() ; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, vb.textures[i].tex);

            std::ostringstream os;

            os << i;

            const std::string local = "tex[" + os.str() + "]";

            glUniform1i(glGetUniformLocation(shader, local.c_str()), i);
        }

        glBindVertexArray(vb.VAO);

        glDrawArrays(GL_TRIANGLES, index * nbVerticesByTree, nbVerticesByTree);

        //std::cout << vb.coordVert[0] << " " << vb.coordVert[1] << " " << vb.coordVert[2] << std::endl;

        //std::cout << nbVerticesByTree << std::endl;
    }

    void VegetGenerator::drawVB(const GLuint shader, const glm::vec3 camPos, const glm::vec3 camTarget, const bool frustumCulling)
    {
        for(size_t i = 0; i < vb.textures.size() ; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, vb.textures[i].tex);

            std::ostringstream os;

            os << i;

            const std::string local = "tex[" + os.str() + "]";

            glUniform1i(glGetUniformLocation(shader, local.c_str()), i);
        }

        glBindVertexArray(vb.VAO);

        if(!frustumCulling)
        {
            glDrawArrays(GL_TRIANGLES, 0, vb.coordVert.size()/3);
        }

        else
        {
            for(size_t i = 0 ; i < positions.size() ; i++)
            {
                const glm::vec2 camToTree = glm::normalize(glm::vec2(positions[i].x, positions[i].y) - glm::vec2(camPos.x, camPos.y));
                const glm::vec2 camToTarget = glm::normalize(glm::vec2(camTarget.x, camTarget.y) - glm::vec2(camPos.x, camPos.y));
                const float dist = glm::length(positions[i] - camPos);

                if(glm::dot(camToTree, camToTarget) >= 0.0f && dist <= 100.0f)
                {
                    glDrawArrays(GL_TRIANGLES, i * nbVerticesByTree, nbVerticesByTree);
                }
            }
        }
    }
}
