#include <GL/gl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "../glm/glm.hpp"
#include "../glm/gtx/transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

namespace Veget
{
    #define PI 3.1415926535897932384626433832795

    enum VEGET_TYPE
    {
        VEGET_SCOTS_PINE,
        VEGET_UMBRELL_PINE,
        VEGET_MARTITIM_PINE,
        VEGET_OAK,
        VEGET_PLANE,
        VEGET_BIRCH,
        VEGET_CYPRESS,
        VEGET_ARIZONA_CYPRESS,
        VEGET_FIR,
        VEGET_OLIV,
        VEGET_FIG,
        VEGET_POPLAR,
        VEGET_VINE,
        VEGET_SCRUB,
        VEGET_LAVENDER,
        VEGET_GRASS,
        VEGET_UNDEFINED,
    };

    struct Plant
    {
        glm::vec3 pos;
        VEGET_TYPE type = VEGET_UNDEFINED;
    };

    struct Plane
    {
        float a;
        float b;
        float c;
        float d;
    };

    struct Segment
    {
        glm::vec3 p1;
        glm::vec3 p2;
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
        GLuint tex;
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

    class VegetGenerator
    {
        private :

        VertexBuffer vb;
        std::map<std::string, GLuint> textures;
        unsigned int res = 8;
        unsigned int nbSeg = 10;
        std::vector<size_t> nbVerticesByTree;
        std::vector<glm::vec3> positions;

        void LoadTextures();
        size_t createTrunk(Plant plant, std::vector<glm::vec3> &skeleton, float *trunkRadius);
        size_t createBranchs(Plant plant, std::vector<glm::vec3> skeleton, const float trunkRadius);
        size_t createBranch(const glm::vec3 base, const float radius, const float ratioTopBottom, const float lg, const int indexTex, const float angleZ, const float angleY);
        size_t createLeaves(std::vector<glm::vec3> skeleton, const float branchLg, const int indexTex, const glm::mat4 rotZ);
        void createPlant(Plant plant);
        int getIndexTexture(const std::string specie);

        public :

        void Init();
        void Finalize();
        void addPlant(Plant plant);
        void drawVB_By_Index(const size_t index, const GLuint shader);
        void drawVB(const GLuint shader, const glm::vec3 camPos = glm::vec3(0.0f), const glm::vec3 camTarget = glm::vec3(0.0f), const bool frustumCulling = false);
        BoundBox getBoundBox(const size_t index);
        size_t getnbInstances()
        {
            return positions.size();
        }
        glm::vec3 getPos(const size_t index);
    };

    bool interPlaneSeg(Plane plane, Segment seg, glm::vec3 &inter);
}
