#pragma once
#include <glm/glm.hpp>

void GetFrustumPlanes(const glm::mat4& transform, glm::vec4* planes);
