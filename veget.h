#include <GL/gl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
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

struct Circle
{
    std::vector<glm::vec3> points;
};

class Veget
{
    private :

    std::vector<Tree> trees;
    std::vector<GLuint> textures;
    const float PI = 3.1415926535897932384626433832795f;

    bool inPolygon(glm::vec3 point, std::vector<glm::vec3> polygon);
    std::vector<glm::vec3> getClosest3Points(glm::vec3 point, std::vector<glm::vec3> polygon);
    float getProjectionZ(glm::vec2 point, std::vector<glm::vec3> triangle);
    void createTree(TREE_TYPE type, const unsigned int resH, const unsigned int resV, Tree *tree);
    void createSkeleton(TREE_TYPE type, const unsigned int resV, const float height, std::vector<glm::vec3> &skeleton);
    void createTrunk(const float trunkRadius, const float ratioHeight, const unsigned int resH, const unsigned int resV, const float height, std::vector<glm::vec3> skeleton, Tree *tree);

    public :

    void AddTreesInArea(std::vector<glm::vec3> area, TREE_TYPE type, const unsigned int nbTrees, const unsigned int resH, const unsigned int resV);
};
