#include <dirent.h>
#include <sstream>
#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

namespace Trees
{
    glm::vec3 TreesGenerator::getPos(const size_t index)
    {
        return positions[index];
    }

    void TreesGenerator::drawVB(const size_t index, const GLuint shader)
    {
        for(size_t i=0; i<vb.textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + 3 * i);
            glBindTexture(GL_TEXTURE_2D, vb.textures[i].bark);

            std::ostringstream os;

            os << 3 * i;

            std::string local = "tex[" + os.str() + "]";

            glUniform1i(glGetUniformLocation(shader, local.c_str()), 3 * i);

            ///////////////////////////////////////////////////////////////////

            os.str("");

            glActiveTexture(GL_TEXTURE0 + 3 * i + 1);
            glBindTexture(GL_TEXTURE_2D, vb.textures[i].barkNormal);

            os << 3 * i + 1;

            local = "tex[" + os.str() + "]";

            glUniform1i(glGetUniformLocation(shader, local.c_str()), 3 * i + 1);

            ///////////////////////////////////////////////////////////////////

            os.str("");

            glActiveTexture(GL_TEXTURE0 + 3 * i + 2);
            glBindTexture(GL_TEXTURE_2D, vb.textures[i].branch);

            os << 3 * i + 2;

            local = "tex[" + os.str() + "]";

            glUniform1i(glGetUniformLocation(shader, local.c_str()), 3 * i + 2);
        }

        glBindVertexArray(vb.VAO);

        glDrawArrays(GL_TRIANGLES, index * nbVerticesByTree, nbVerticesByTree);

        //std::cout << vb.coordVert[0] << " " << vb.coordVert[1] << " " << vb.coordVert[2] << std::endl;
    }
}
