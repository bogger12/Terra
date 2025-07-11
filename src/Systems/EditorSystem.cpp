#include "EditorSystem.hpp"
#include "../Utilities/Debug.hpp"
#include "../Systems/RenderSystem.hpp"

Gizmo EditorSystem::globalGizmo;



glm::vec3 RayPlaneIntersection(const glm::vec3& l0, const glm::vec3& l, const glm::vec3& p0, glm::vec3& n) {
    // glm::vec3 l0 = rayOrigin, l = ray, p0 = planePos, n = planeNormal;
    float t;

    float denom = glm::dot(n, l);
    if (denom < 0) { n = -n; denom = -denom; } // Allow for intersection from both sides

    if (denom > 1e-6) {
        t = glm::dot(p0 - l0, n) / denom;
    } else t = 0;
    return l0 + l * t; // = p
}

void EditorSystem::CalculateGizmoTransform(const glm::vec3& rayOrigin, const glm::vec3& pointerRayDirection, Gizmo& gizmo) {

    // ray pos on plane calc
    // pos distance from gizmo calc
    // set axis of transform to corresponding component of pos (eg x to x)


    glm::vec3 normalXYaxis = glm::normalize(glm::vec3( 0, 0, 1 )), normalZaxis = glm::normalize(glm::vec3( 1, 0, 0 ));

    glm::vec3& gizmoPos = gizmo.transform->position;

    glm::vec3 pointXY = RayPlaneIntersection(rayOrigin, pointerRayDirection, gizmoPos, normalXYaxis);
    glm::vec3 pointZ = RayPlaneIntersection(rayOrigin, pointerRayDirection, gizmoPos, normalZaxis);

    // X axis
    float xDistance = (pointXY - gizmoPos).x;
    float yDistance = (pointXY - gizmoPos).y;
    float zDistance = (pointZ - gizmoPos).z;

    if (true) { // X move
        gizmoPos.x = pointXY.x;
        gizmoPos.y = pointXY.y;
    }

    // Debug::LogStream() << "origin: " << rayOrigin.x << " " << rayOrigin.y << " " << rayOrigin.z << std::endl;
    // Debug::LogStream() << "pointerRayDirection: " << pointerRayDirection.x << " " << pointerRayDirection.y << " " << pointerRayDirection.z << " " << glm::length(pointerRayDirection) << std::endl;
    // Debug::LogStream() << "hitpoint: " << pointXY.x << " " << pointXY.y << std::endl;

    RenderSystem::DrawLine(rayOrigin+glm::vec3(0.07,-1,0), pointXY, glm::vec3(0.0,0.0,1.0));
    // RenderSystem::DrawRay(rayOrigin+glm::vec3(-0.07,-1,0), pointerRayDirection, 10, glm::vec3(1.0,0.0,0.0));

}