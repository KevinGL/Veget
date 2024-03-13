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

    /*enum VEGET_TYPE
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
    };*/

    struct Item
    {
        glm::vec3 pos;
        float scale = 0.0f;
        std::string type = "";
    };

    struct Params
    {
        bool tree;
        unsigned int heightMin;
        unsigned int heightMax;
        unsigned int radiusMin;
        unsigned int radiusMax;
        float ratioTopBottomTrunk;
        float ratioTopBottomBranch;
        float ratioBranchTrunkRadius;
        float ratioBranchTrunkLength;
        float ratioLeavesBranch;
        std::string texKey;
        float beginBranch;
        float angleBranch;
        std::string shape;
        float branchsWeigth;
        unsigned int torsion;
        unsigned int torsionBranchs;
        bool multiTrunk;
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
        std::vector<float> positions;
        std::vector<float> scales;
        std::vector<float> angles;
        std::string specie;
        std::string pathTex;
        GLuint tex;
        BoundBox bbox;
    };

    struct Circle
    {
        std::vector<glm::vec3> vertices;
    };

    class VegetGenerator
    {
        private :

        std::vector<VertexBuffer> models;
        std::map<std::string, GLuint> textures;
        unsigned int res = 8;
        unsigned int nbSeg = 10;
        std::vector<size_t> nbVerticesByTree;
        std::vector<Item> items;
        std::map<std::string, Params> params;

        void InitParams();
        void LoadTextures();
        void createModels();
        std::vector<float> getPositionsFromSpecie(const std::string specie);
        std::vector<float> getScalesFromSpecie(const std::string specie);
        void createTrunk(const std::string specie, std::vector<glm::vec3> &skeleton, float *trunkRadius, VertexBuffer *model);
        void createBranchs(const std::string specie, std::vector<glm::vec3> skeleton, const float trunkRadius, VertexBuffer *model);
        void createBranch(const float branchsWeight, const float ratioLeavesBranch, const int torsion, const glm::vec3 base, const float radius, const float ratioTopBottom, const float lg, const float angleZ, const float angleY, VertexBuffer *model);
        void createLeaves(const std::vector<glm::vec3> skeleton, const float ratioLeavesBranch, const glm::mat4 rotZ, const glm::mat4 rotY, VertexBuffer *model);
        void createGrass(VertexBuffer *model);
        float getCurve(const std::string curve, const size_t index, const float zBase);

        public :

        void Init();
        void Finalize();
        void addItem(Item item);
        void drawVB(const GLuint shader, const glm::vec3 camPos = glm::vec3(0.0f), const glm::vec3 camTarget = glm::vec3(0.0f), const bool frustumCulling = false);
        BoundBox getBoundBox(const size_t index);
        size_t getnbInstances()
        {
            return items.size();
        }
        glm::vec3 getPos(const size_t index);
    };

    bool interPlaneSeg(Plane plane, Segment seg, glm::vec3 &inter);
    std::string trim(const std::string str);
}
