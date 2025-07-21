#pragma once
#include "../Components/Components.hpp"
#include <glm/glm.hpp>

struct Gizmo {
    enum Type {
        TRANSLATE,
        ROTATE,
        SCALE
    };
    enum class AxisSelected {
        X,
        Y,
        Z,
        NONE
    };
    enum class PlaneSelected {
        ZY,
        XZ,
        XY,
        NONE
    };
    Type transformation = TRANSLATE;
    AxisSelected axis = AxisSelected::NONE;
    PlaneSelected plane = PlaneSelected::NONE;
    Transform* transform;

    glm::mat4 axisMatrices[3];
    glm::mat4 planeMatrices[3];

    glm::vec3 startPosition = glm::vec3(0.0f);
    glm::vec3 cursorOffset = glm::vec3(0.0f);
};


class Gizmos {
    public:
        static Gizmo globalGizmo;
        static glm::vec3 clickStart;
        static void CalculateGizmoTransform(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, Gizmo& gizmo = globalGizmo);
        static void SetGizmoLineMatrix(glm::mat4& modelMatrix, const Gizmo::AxisSelected& axis, const glm::vec3& pos, const float& lineLength, const float& lineWidth);
        static void SetGizmoPlaneMatrix(glm::mat4& modelMatrix, const Gizmo::PlaneSelected& plane, const glm::vec3& translation, const float& translationDistance, glm::vec3& scale);
        static void SetGizmoMatrices(Gizmo& gizmo);
    };