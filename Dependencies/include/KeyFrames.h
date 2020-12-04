//
// Created by Stefan on 04-Dec-20.
//

#include "Common.h"

#ifndef INTERPOLATIONS_KEYFRAMES_H
#define INTERPOLATIONS_KEYFRAMES_H



class KeyFrames {

private:

    vector<pair<glm::vec3, double>> pos;
    vector<pair<glm::vec3, double>> scal;
    vector<pair<glm::quat, double>> rot;


    glm::vec3 interpPosition(double time);
    glm::vec3 interpScale(double time);
    glm::quat interpRotation(double time);
    double last_time;
    double internal_time;
    bool playing = false;

public:
    void pushVec3(pair<glm::vec3, double> keyframe, char type);
    glm::mat4 getTransform();
    void startPlaying();
    bool IsPlaying();
};

#endif //INTERPOLATIONS_KEYFRAMES_H
