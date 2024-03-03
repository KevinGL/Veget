#include <dirent.h>
#include <sstream>
#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

namespace Veget
{
    glm::vec3 VegetGenerator::getPos(const size_t index)
    {
        return items[index].pos;
    }

    void VegetGenerator::drawVB(const GLuint shader, const glm::vec3 camPos, const glm::vec3 camTarget, const bool frustumCulling)
    {
        for(const VertexBuffer model : models)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, model.tex);

            glBindVertexArray(model.VAO);
            glDrawArraysInstanced(GL_TRIANGLES, 0, model.coordVert.size()/3, model.positions.size()/3);

            /*for(size_t i = 0 ; i < model.positions.size() ; i += 3)
            {
                glUniform3f(glGetUniformLocation(shader, "pos"), model.positions[i], model.positions[i+1], model.positions[i+2]);

                const size_t index = i/3;
                glUniform1f(glGetUniformLocation(shader, "scale"), model.scales[index]);
                glUniform1f(glGetUniformLocation(shader, "angle"), model.angles[index]);

                if(!frustumCulling)
                {
                    glDrawArrays(GL_TRIANGLES, 0, model.coordVert.size()/3);
                }

                else
                {
                    const glm::vec3 pos = glm::vec3(model.positions[i], model.positions[i+1], model.positions[i+2]);
                    const float dist = glm::length(camPos - pos);
                    const glm::vec3 toItem = glm::normalize(pos - camPos);
                    const glm::vec3 toTarget = glm::normalize(camTarget - camPos);

                    if(dist <= 100.0f && glm::dot(toItem, toTarget) >= 0.2f)
                    {
                        glDrawArrays(GL_TRIANGLES, 0, model.coordVert.size()/3);
                    }
                }
            }*/
        }
    }
}
