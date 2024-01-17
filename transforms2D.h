#pragma once

#include "utils/glm_utils.h"


namespace transforms2D
{
    // Translate matrix
    inline glm::mat3 Translate(float tx, float ty)
    {
        // TODO(student): Implement the translation matrix

        return glm::transpose(
            glm::mat3(1, 0, tx,
                0, 1, ty,
                0, 0, 1)
        );

    }

    // Scale matrix
    inline glm::mat3 Scale(float sX, float sY)
    {
        // TODO(student): Implement the scaling matrix
        return glm::transpose(
            glm::mat3(sX, 0, 0,
                0, sY, 0,
                0, 0, 1)
        );

    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        // TODO(student): Implement the rotation matrix
        return glm::transpose(
            glm::mat3(cos(radians), -sin(radians), 0,
                sin(radians), cos(radians), 0,
                0, 0, 1)
        );

    }
}   // namespace transform2D
