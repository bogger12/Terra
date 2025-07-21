#pragma once
#include <glm/glm.hpp>
#include <math.h>

#define eps 0.00001

namespace Physics {


    inline void quadraticSolver(const float& a, const float& b, const float& c, float& x1, float& x2) {
        float discriminant = b*b - 4*a*c;
        float sqrtDiscriminant = sqrt(discriminant);
        
        if (discriminant > 0) {
            x1 = (-b - sqrtDiscriminant) / (2*a);
            x2 = (-b + sqrtDiscriminant) / (2*a);
        } else if (discriminant == 0) {
            x1 = x2 = (-b) / (2*a);
        }
        // No need to handle complex roots
    };

    struct BBox {
        glm::vec3 p1;
        glm::vec3 p2;
        
        BBox(glm::vec3 p1, glm::vec3 p2) 
        : p1(p1), p2(p2) {}
    };
    struct BBox2D {
        glm::vec2 p1;
        glm::vec2 p2;
        
        BBox2D(glm::vec2 p1, glm::vec2 p2) 
        : p1(p1), p2(p2) {}
        BBox2D(glm::vec4 rect) 
        : p1(glm::vec2(rect.x, rect.y)), p2(glm::vec2(rect.z, rect.w)) {}
    };

    inline static bool PointInsideBBox(glm::vec3 pos, BBox bbox) {
        return glm::all(glm::greaterThanEqual(pos, bbox.p1)) && glm::all(glm::lessThanEqual(pos, bbox.p2));
    };

    inline static bool PointInsideBBox2D(glm::vec2 pos, BBox2D bbox) {
        return glm::all(glm::greaterThanEqual(pos, bbox.p1)) && glm::all(glm::lessThanEqual(pos, bbox.p2));
    };

    inline glm::vec3 RayPlaneIntersection(const glm::vec3& l0, const glm::vec3& l, const glm::vec3& p0, glm::vec3& n) {
        // glm::vec3 l0 = rayOrigin, l = ray, p0 = planePos, n = planeNormal;
        float t;

        float denom = glm::dot(n, l);
        if (denom < 0) { n = -n; denom = -denom; } // Allow for intersection from both sides

        if (denom > 1e-6) {
            t = glm::dot(p0 - l0, n) / denom;
        } else t = 0;
        return l0 + l * t; // = p
    };

    // Returns l0 if not hit
    inline glm::vec3 RayQuadIntersection(const glm::vec3& l0, const glm::vec3& l, BBox bbox, glm::vec3 n) {
        // glm::vec3 l0 = rayOrigin, l = ray, p0 = planePos, n = planeNormal;
        assert(glm::any(glm::equal(bbox.p1, bbox.p2))); // Checks if bbox is on a plane (2d)
        glm::vec3 intersect = RayPlaneIntersection(l0, l, bbox.p1, n);
        return PointInsideBBox(intersect, bbox) ? intersect : l0;
    };

    inline glm::vec3 RayTransformedQuadIntersection(const glm::vec3& l0, const glm::vec3& l, BBox bbox, glm::vec3 n, glm::mat4 quadTransform = glm::mat4(1.0)) {
        glm::mat4 inverseTransform = glm::inverse(quadTransform);
        glm::vec3 eye = inverseTransform * glm::vec4(l0, 1.0f);
        glm::vec3 d = glm::normalize(inverseTransform * glm::vec4(l, 0.0f));
        glm::vec3 intersect = RayQuadIntersection(eye, d, bbox, n);
        if (glm::length(intersect - l0) > eps) {
            return glm::vec3(quadTransform*glm::vec4(intersect, 1.0f)); // Return untransformed intersect
        } else return l0;
    }

    inline glm::vec3 RayCylinderIntersection(const glm::vec3& l0, const glm::vec3& l, const float &r, const float &h, glm::mat4 cylinderTransform = glm::mat4(1.0)) {
        glm::mat4 inverseTransform = glm::inverse(cylinderTransform);
        glm::vec3 eye = inverseTransform * glm::vec4(l0, 1.0f);
        glm::vec3 d = glm::normalize(inverseTransform * glm::vec4(l, 0.0f));

        float t1 = 0, t2 = 0;
        float a = d.x*d.x + d.z*d.z;
        float b = 2*(eye.x*d.x + eye.z*d.z);
        float c = eye.x*eye.x + eye.z*eye.z - r*r;
        quadraticSolver(a, b, c, t1, t2);
        // now t1 and t2 are intercepts
        float h1 = eye.y + d.y*t1;
        float h2 = eye.y + d.y*t2;
        if (h1 > h || h1 < -h) t1 = 0.0f;
        if (h2 > h || h2 < -h) t2 = 0.0f;
        float closest;
        if (t1 == 0 || t2 == 0) closest = t1 + t2;
        else closest = (t1 < t2) ? t1 : t2;

        if (closest > 0.0f) {
            glm::vec3 hitObjSpace = eye + d*closest;
            return glm::vec3(cylinderTransform*glm::vec4(hitObjSpace, 1.0f));
        }
        else return l0;
    }
};