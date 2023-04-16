#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

void Veget::Draw(const GLuint shaderId, const glm::vec3 posCam)
{
    for(size_t i=0; i<treesGroups.size(); i++)
    {
        glm::vec2 areaPos = glm::vec2(0.0f, 0.0f);

        for(size_t j=0; j<treesGroups[i].perimeter.size(); j++)
        {
            areaPos += treesGroups[i].perimeter[j];
        }

        areaPos /= treesGroups[i].perimeter.size();

        //FRUSTUM CULLING ZONE A AJOUTER

        //if(...)
        {
            for(size_t j=0; j<treesGroups[i].trees.size(); j++)
            {
                //FRUSTUM CULLING ZONE A AJOUTER

                //if(...)
                {
                    for(size_t k=0; k<treesGroups[i].trees[j].tex.size(); k++)
                    {
                        glActiveTexture(GL_TEXTURE0 + k);
                        glBindTexture(GL_TEXTURE_2D, treesGroups[i].trees[j].tex[k]);

                        std::ostringstream os;

                        os << k;

                        std::string local = "tex[" + os.str() + "]";

                        glUniform1i(glGetUniformLocation(shaderId, local.c_str()), k);
                    }

                    if(treesGroups[i].trees[j].tex[1] != 0)
                    {
                        glUniform1i(glGetUniformLocation(shaderId, "normal_map"), 1);
                    }
                    else
                    {
                        glUniform1i(glGetUniformLocation(shaderId, "normal_map"), 0);
                    }

                    glBindVertexArray(treesGroups[i].trees[j].VAO);

                    glDrawArrays(GL_TRIANGLES, 0, treesGroups[i].trees[j].coordVertex.size()/3);

                    glBindVertexArray(0);
                }
            }
        }
    }
}

void Veget::DrawByGroup(const GLuint shaderId, const glm::vec3 posCam, const size_t indexGroup)
{
    //

    //FRUSTUM CULLING ZONE A AJOUTER

    //if(...)
    {
        for(size_t i=0; i<treesGroups[indexGroup].trees.size(); i++)
        {
            //FRUSTUM CULLING ZONE A AJOUTER

            //if(...)
            {
                for(size_t j=0; j<treesGroups[indexGroup].trees[i].tex.size(); j++)
                {
                    glActiveTexture(GL_TEXTURE0 + j);
                    glBindTexture(GL_TEXTURE_2D, treesGroups[indexGroup].trees[i].tex[j]);

                    std::ostringstream os;

                    os << j;

                    std::string local = "tex[" + os.str() + "]";

                    glUniform1i(glGetUniformLocation(shaderId, local.c_str()), j);
                }

                if(treesGroups[indexGroup].trees[i].tex[1] != 0)
                {
                    glUniform1i(glGetUniformLocation(shaderId, "normal_map"), 1);
                }
                else
                {
                    glUniform1i(glGetUniformLocation(shaderId, "normal_map"), 0);
                }

                glBindVertexArray(treesGroups[indexGroup].trees[i].VAO);

                glDrawArrays(GL_TRIANGLES, 0, treesGroups[indexGroup].trees[i].coordVertex.size()/3);

                glBindVertexArray(0);
            }
        }
    }
}
