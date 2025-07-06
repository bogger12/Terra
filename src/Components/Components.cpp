#include "Components.hpp"

const std::map<std::string, Components::ComponentAdder> Components::ComponentFactories = {
    { "Transform", [](entt::registry& reg, entt::entity& e) {
        if (reg.any_of<Transform>(e)) reg.emplace_or_replace<Transform>(e); 
    }},
    { "ModelData", [](entt::registry& reg, entt::entity&e) {
        if (reg.any_of<ModelData>(e)) reg.emplace_or_replace<ModelData>(e); 
    }},
    { "RenderingData", [](entt::registry& reg, entt::entity& e) {
        if (reg.any_of<RenderingData>(e)) reg.emplace_or_replace<RenderingData>(e); 
    }},
    { "ModelWrapper", [](entt::registry& reg, entt::entity& e) {
        if (reg.any_of<ModelWrapper>(e)) reg.emplace_or_replace<ModelWrapper>(e); 
    }},
    { "LightTag", [](entt::registry& reg, entt::entity& e) {
        if (reg.any_of<LightTag>(e)) reg.emplace_or_replace<LightTag>(e); 
    }},
    { "DirectionalLight", [](entt::registry& reg, entt::entity& e) {
        if (reg.any_of<DirectionalLight>(e)) reg.emplace_or_replace<DirectionalLight>(e); 
    }},
    { "PointLight", [](entt::registry& reg, entt::entity& e) {
        if (reg.any_of<PointLight>(e)) reg.emplace_or_replace<PointLight>(e); 
    }},
    { "SpotLight", [](entt::registry& reg, entt::entity& e) {
        if (reg.any_of<SpotLight>(e)) reg.emplace_or_replace<SpotLight>(e); 
    }}
};