//
// Created by Stefan on 04-Dec-20.
//

#include <KeyFrames.h>


glm::mat4 KeyFrames::getTransform(float time) {
    glm::mat4 t, r, s;
    s = glm::scale(glm::mat4(1), interpScale(time));
    r = glm::mat4_cast(interpRotation(time));
    t = glm::translate(glm::mat4(1), interpPosition(time));
    return t * r * s;
}

glm::vec3 KeyFrames::interpPosition(float time) {
    return glm::vec3(1);
}

glm::vec3 KeyFrames::interpScale(float time) {
    return glm::vec3(1);
}

glm::quat KeyFrames::interpRotation(float time) {
    return glm::quat();
}

void KeyFrames::pushVec3(std::pair<glm::vec3, float> keyframe, char type) {
    if(type == 's') scal.push_back(keyframe);
        else if(type == 'p') pos.push_back(keyframe);
            else if(type == 'r') {
                glm::mat4 rotation = glm::mat4(1);
                rotation = glm::rotate(rotation, keyframe.first.x, glm::vec3(1.0f, 0.0f, 0.0f));
                rotation = glm::rotate(rotation, keyframe.first.y, glm::vec3(0.0f, 1.0f, 0.0f));
                rotation = glm::rotate(rotation, keyframe.first.z, glm::vec3(0.0f, 0.0f, 1.0f));
                std::pair<glm::quat, float> kf;
                kf.first = glm::quat_cast(rotation);
                kf.second = keyframe.second;
                rot.push_back(kf);
            }
            else cout << "typo? (pushVec3)\n";
}
