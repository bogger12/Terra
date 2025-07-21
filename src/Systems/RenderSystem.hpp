#pragma once

#include "../Core/WindowManager.hpp"
#include "../Components/Components.hpp"
#include "../Components/Lights.hpp"
#include "../Utilities/Debug.hpp"
#include "../Editor/Gizmos.hpp"
#include <entt/entt.hpp>
#include <camera.h>

class RenderSystem {
    public:
        RenderSystem() = default;
        static void Render(entt::registry &registry, float fov, Camera camera, glm::vec2 projectionRatio);
        static void BindVertexArray(entt::registry &registry, bool reloadVBOs=false);
        static void RenderGizmos(Gizmo& gizmo = Gizmos::globalGizmo, const glm::vec3& rayOrigin = glm::vec3(0.0f), const glm::vec3& pointerRayDirection = glm::vec3(0.0f), const bool recalcGizmoMatrices = true);
        static glm::vec3 PointerToRay(const glm::vec2& pointer);
        static void DrawRay(const glm::vec3& origin, const glm::vec3& direction, const float& length, const glm::vec3& color = glm::vec3(1.0f,0.0f,0.0f));
        static void DrawLine(const glm::vec3& startPos, const glm::vec3& endPos, const glm::vec3& color = glm::vec3(1.0f,0.0f,0.0f));

    private:
        static glm::mat4 viewMatrix;
        static glm::mat4 projectionMatrix;
        
};