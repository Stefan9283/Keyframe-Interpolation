//
// Created by Stefan on 04-Dec-20.
//

#include "Common.h"

#ifndef INTERPOLATIONS_KEYFRAMES_H
#define INTERPOLATIONS_KEYFRAMES_H



class KeyFrames {

    vector<pair<glm::vec3, float>> pos;
    vector<pair<glm::vec3, float>> scal;
    vector<pair<glm::quat, float>> rot;

    void pushVec3(pair<glm::vec3, float> keyframe, char type);

    glm::mat4 getTransform(float time);


    glm::vec3 interpPosition(float time);
    glm::vec3 interpScale(float time);
    glm::quat interpRotation(float time);

};

#endif //INTERPOLATIONS_KEYFRAMES_H
