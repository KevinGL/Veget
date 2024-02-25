#include "../sdlglutils/sdlglutils.h"
#include "veget.h"

namespace Veget
{
    bool interPlaneSeg(Plane plane, Segment seg, glm::vec3 &inter)
    {
        const float x0 = seg.p1.x;
        const float y0 = seg.p1.y;
        const float z0 = seg.p1.z;

        const float xV = seg.p2.x - seg.p1.x;
        const float yV = seg.p2.y - seg.p1.y;
        const float zV = seg.p2.z - seg.p1.z;

        const float t = -(plane.a * x0 + plane.b * y0 + plane.c * z0 + plane.d) / (plane.a * xV + plane.b * yV + plane.c * zV);

        const glm::vec3 candidate = glm::vec3(x0 + xV * t, y0 + yV * t, z0 + zV * t);

        glm::vec3 middle = seg.p1 + seg.p2;
        middle /= 2;

        if(glm::length(candidate - middle) <= glm::length(seg.p2 - seg.p1) / 2)
        {
            inter = candidate;
            return true;
        }

        return false;
    }
}
