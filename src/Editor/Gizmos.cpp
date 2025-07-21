#include "Gizmos.hpp"
#include "../Utilities/Debug.hpp"
#include "../Systems/RenderSystem.hpp"
#include "../Editor/Editor.hpp"
#include <terraphysics.h>

Gizmo Gizmos::globalGizmo;



void Gizmos::CalculateGizmoTransform(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, Gizmo& gizmo) {

    bool firstAxisSelect = gizmo.axis == Gizmo::AxisSelected::NONE;
    
    gizmo.plane = Gizmo::PlaneSelected::NONE;


    if (Editor::lastClickInsideEditorWindow) {
        glm::vec3& gizmoPos = gizmo.transform->position;

        Gizmos::SetGizmoMatrices(gizmo);

        if (firstAxisSelect) {
            for (int i=0;i<3;i++) {
                glm::vec3 intersect = Physics::RayCylinderIntersection(rayOrigin, rayDirection, 1.0f, 1.0f, gizmo.axisMatrices[i]);
                if (intersect != rayOrigin && gizmo.axis == Gizmo::AxisSelected::NONE) {
                    gizmo.axis = (Gizmo::AxisSelected)(i);
                    gizmo.startPosition = gizmoPos;
                    gizmo.cursorOffset = intersect - gizmoPos;
                }
            };
        }
        
        switch (gizmo.axis) {
            case Gizmo::AxisSelected::X: {
                glm::vec3 normalXYaxis = glm::normalize(glm::vec3( 0, 0, 1 ));
                glm::vec3 pointXY = Physics::RayPlaneIntersection(rayOrigin, rayDirection, gizmoPos, normalXYaxis);
                gizmoPos.x = pointXY.x - gizmo.cursorOffset.x;
                break;
            }
            case Gizmo::AxisSelected::Y: {
                glm::vec3 normalXYaxis = glm::normalize(glm::vec3( 0, 0, 1 ));
                glm::vec3 pointXY = Physics::RayPlaneIntersection(rayOrigin, rayDirection, gizmoPos, normalXYaxis);
                gizmoPos.y = pointXY.y - gizmo.cursorOffset.y;
                break;
            }
            case Gizmo::AxisSelected::Z: {
                glm::vec3 normalZaxis = glm::normalize(glm::vec3( 1, 0, 0 ));
                glm::vec3 pointZ = Physics::RayPlaneIntersection(rayOrigin, rayDirection, gizmoPos, normalZaxis);
                gizmoPos.z = pointZ.z - gizmo.cursorOffset.z;
                break;
            }
            case Gizmo::AxisSelected::NONE:
                break;
        }
    }
    else {
        gizmo.axis = Gizmo::AxisSelected::NONE;
    }

    // Needs to recalc matrices if gizmo position changed
    RenderSystem::RenderGizmos(Gizmos::globalGizmo, rayOrigin, rayDirection);

}

void Gizmos::SetGizmoMatrices(Gizmo& gizmo) {
    glm::vec3 translation = gizmo.transform->position; 

    for (int i=0; i<3; i++) {

        // Set Plane Transform Matrix
        gizmo.planeMatrices[i] = glm::mat4(1.0f);
        glm::vec3 scale = glm::vec3(0.15f);
        float translationDistance = 0.5f;
        Gizmos::SetGizmoPlaneMatrix(gizmo.planeMatrices[i], (Gizmo::PlaneSelected)(i), translation, translationDistance, scale);

        // Set Axis Transform Matrix
        gizmo.axisMatrices[i] = glm::mat4(1.0f);
        float lineLength = 0.5f;
        Gizmos::SetGizmoLineMatrix(gizmo.axisMatrices[i], (Gizmo::AxisSelected)(i), translation, lineLength);
    }
}



void Gizmos::SetGizmoLineMatrix(glm::mat4& modelMatrix, const Gizmo::AxisSelected& axis, const glm::vec3& pos, const float& lineLength) {
    switch (axis) {
        case Gizmo::AxisSelected::X:
            modelMatrix = glm::translate(modelMatrix, pos+glm::vec3(lineLength, 0.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, -glm::pi<float>()/4.0f, glm::vec3(1.0f,0.0f,0.0f));
            modelMatrix = glm::rotate(modelMatrix, -glm::pi<float>()/2.0f, glm::vec3(0.0f,0.0f,1.0f));
            break;
        case Gizmo::AxisSelected::Y:
            modelMatrix = glm::translate(modelMatrix, pos+glm::vec3(0.0f, lineLength, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, glm::pi<float>()/4.0f, glm::vec3(0.0f,1.0f,0.0f));
            break;
        case Gizmo::AxisSelected::Z:
            modelMatrix = glm::translate(modelMatrix, pos+glm::vec3(0.0f, 0.0f, lineLength));
            modelMatrix = glm::rotate(modelMatrix, -glm::pi<float>()/4.0f, glm::vec3(0.0f,0.0f,1.0f));
            modelMatrix = glm::rotate(modelMatrix, -glm::pi<float>()/2.0f, glm::vec3(1.0f,0.0f,0.0f));
            break;
        case Gizmo::AxisSelected::NONE:
            break;
    }
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, lineLength, 0.05f));
}

void Gizmos::SetGizmoPlaneMatrix(glm::mat4& modelMatrix, const Gizmo::PlaneSelected& plane, const glm::vec3& translation, const float& translationDistance, glm::vec3& scale) {
    switch (plane) {
        case Gizmo::PlaneSelected::ZY: {
            glm::vec3 localTranslation = glm::vec3(0.0f, 1.0f, 1.0f) * translationDistance;
            modelMatrix = glm::translate(modelMatrix, translation+localTranslation);
            modelMatrix = glm::rotate(modelMatrix, glm::pi<float>()/2.0f, glm::vec3(0.0f,1.0f,0.0f));
            break;
        }
        case Gizmo::PlaneSelected::XZ: {
            glm::vec3 localTranslation = glm::vec3(1.0f, 0.0f, 1.0f) * translationDistance;
            modelMatrix = glm::translate(modelMatrix, translation+localTranslation);
            modelMatrix = glm::rotate(modelMatrix, -glm::pi<float>()/2.0f, glm::vec3(1.0f,0.0f,0.0f));
            break;
        }
        case Gizmo::PlaneSelected::XY: {
            glm::vec3 localTranslation = glm::vec3(1.0f, 1.0f, 0.0f) * translationDistance;
            modelMatrix = glm::translate(modelMatrix, translation+localTranslation);
            break;
        }
        case Gizmo::PlaneSelected::NONE:
            break;
    }
    modelMatrix = glm::scale(modelMatrix, scale);
}