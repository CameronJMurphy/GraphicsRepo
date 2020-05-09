#include "FrustumPlanes.h"

void GetFrustumPlanes(const glm::mat4& transform, glm::vec4* planes)
{
	// right side
	planes[0] = glm::vec4(transform[0][3] - transform[0][0],
		transform[1][3] - transform[1][0],
		transform[2][3] - transform[2][0],
		transform[3][3] - transform[3][0]);
	// left side
	planes[1] = glm::vec4(transform[0][3] + transform[0][0],
		transform[1][3] + transform[1][0],
		transform[2][3] + transform[2][0],
		transform[3][3] + transform[3][0]);
	// top
	planes[2] = glm::vec4(transform[0][3] - transform[0][1],
		transform[1][3] - transform[1][1],
		transform[2][3] - transform[2][1],
		transform[3][3] - transform[3][1]);
	// bottom
	planes[3] = glm::vec4(transform[0][3] + transform[0][1],
		transform[1][3] + transform[1][1],
		transform[2][3] + transform[2][1],
		transform[3][3] + transform[3][1]);
	// far
	planes[4] = glm::vec4(transform[0][3] - transform[0][2],
		transform[1][3] - transform[1][2],
		transform[2][3] - transform[2][2],
		transform[3][3] - transform[3][2]);
	// near
	planes[5] = glm::vec4(transform[0][3] + transform[0][2],
		transform[1][3] + transform[1][2],
		transform[2][3] + transform[2][2],
		transform[3][3] + transform[3][2]);
	// plane normalisation, based on length of normal
	for (int i = 0; i < 6; i++) {
		float d = glm::length(glm::vec3(planes[i]));
		planes[i] /= d;
	}
}