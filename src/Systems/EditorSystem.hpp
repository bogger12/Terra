#pragma once
#include "../Components/Components.hpp"
#include <glm/glm.hpp>

struct Gizmo {
    enum Type {
        TRANSLATE,
        ROTATE,
        SCALE
    };
    enum AxisSelected {
        NONE,
        X,
        Y,
        Z
    };
    Type transformation = TRANSLATE;
    AxisSelected axis = NONE;
    Transform* transform;
};


class EditorSystem {
    public:
        static Gizmo globalGizmo;
        static void CalculateGizmoTransform(const glm::vec3& rayOrigin, const glm::vec3& pointerRayDirection, Gizmo& gizmo = globalGizmo);
        // static glm::vec3 PointerToRay(const glm::vec3& pointerPos);
};