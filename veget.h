#include <GL/gl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../glm/glm.hpp"
#include "../glm/gtx/transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

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
    TREE_UNDEFINED,
};

struct ParamsTrees
{
    TREE_TYPE type;
    int heightMin;
    int heightMax;
    float trunkRadius;
    float ratioHeight;
    float coefTex;
};

struct Tree
{
    std::vector<float> coordVertex;
    std::vector<float> coordTex;
    std::vector<float> normals;
    std::vector<float> colors;
    std::vector<int> numtex;
    GLuint VBO;
    GLuint VAO;
    glm::vec3 pos;
    std::vector<GLuint> tex;
};

struct TreesArea
{
    std::vector<Tree> trees;
    std::vector<glm::vec2> perimeter;
    glm::vec2 pos;
};

struct Veget_Circle
{
    std::vector<glm::vec3> points;
};

struct Veget_Tex
{
    TREE_TYPE treeType;
    GLuint barkTex;
    GLuint barkNormalTex;
    GLuint branchTex;
};

class Veget
{
    private :

    //std::vector<Tree> trees;
    std::vector<TreesArea> treesAreas;
    std::vector<Veget_Tex> textures;
    std::vector<ParamsTrees> paramsTrees;
    const float PI = 3.1415926535897932384626433832795f;
    bool isInitialized = false;

    void Init();
    bool inPolygon(glm::vec3 point, std::vector<glm::vec3> polygon);
    std::vector<glm::vec3> getClosest3Points(glm::vec3 point, std::vector<glm::vec3> polygon);
    float getProjectionZ(glm::vec2 point, std::vector<glm::vec3> triangle);
    void createTree(TREE_TYPE type, const unsigned int resH, const unsigned int resV, Tree *tree);
    void createSkeleton(TREE_TYPE type, const unsigned int resV, const float height, std::vector<glm::vec3> &skeleton);
    void createTrunk(const float trunkRadius, const float ratioHeight, const unsigned int resH, const unsigned int resV, const float height, const float multiTex, std::vector<glm::vec3> skeleton, Tree *tree);
    ParamsTrees getParamsTree(TREE_TYPE type);
    Veget_Tex getVegetTex(TREE_TYPE type);

    public :

    void AddTreesArea(std::vector<glm::vec3> area, TREE_TYPE type, const unsigned int nbTrees, const unsigned int resH, const unsigned int resV);
    void Draw(const GLuint shaderId, const glm::vec3 posCam);
    void DrawByArea(const GLuint shaderId, const glm::vec3 posCam, const size_t indexArea);

    size_t getNbAreas()
    {
        return treesAreas.size();
    }

    glm::vec2 getAreaPos(const size_t index)
    {
        return treesAreas[index].pos;
    }
};
