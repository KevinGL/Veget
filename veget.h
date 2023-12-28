#include <GL/gl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "../glm/glm.hpp"
#include "../glm/gtx/transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

namespace Trees
{
    #define PI 3.1415926535897932384626433832795

    enum TREE_TYPE
    {
        TREE_PINE,
        TREE_OAK,
        TREE_PLANE,
        TREE_CYPRESS,
        TREE_FIR,
        TREE_OLIV,
        TREE_FIG,
        TREE_POPLAR,
        TREE_VINE,
        TREE_SCRUB,
        TREE_UNDEFINED,
    };

    struct Tree
    {
        glm::vec3 pos;
        TREE_TYPE type = TREE_UNDEFINED;
    };

    struct BoundBox
    {
        float side;
        float height;
        glm::vec3 pos;
    };

    struct Texture
    {
        std::string specie;
        GLuint bark;
        GLuint barkNormal;
        GLuint branch;
    };

    struct VertexBuffer
    {
        GLuint VAO;
        GLuint VBO;
        std::vector<float> coordVert;
        std::vector<float> coordTex;
        std::vector<float> normals;
        std::vector<float> colors;
        std::vector<int> indexTex;
        std::vector<Texture> textures;
        BoundBox bbox;
    };

    struct Circle
    {
        std::vector<glm::vec3> vertices;
    };

    class TreesGenerator
    {
        private :

        VertexBuffer vb;
        //std::map<std::string, Texture> textures;
        unsigned int res = 8;
        unsigned int nbSeg = 5;
        size_t nbVerticesByTree;
        std::vector<glm::vec3> positions;

        void LoadTextures();
        void createTrunk(Tree tree);
        size_t getIndexTex(const std::string specie, const std::string part);

        public :

        void Init();
        void Finalize();
        void sendTree(Tree tree);
        void drawVB(const size_t index, const GLuint shader);
        BoundBox getBoundBox(const size_t index);
        size_t getnbInstances()
        {
            return positions.size();
        }
        glm::vec3 getPos(const size_t index);
    };
}
