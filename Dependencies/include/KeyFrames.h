//
// Created by Stefan on 04-Dec-20.
//

#include "Common.h"

#ifndef INTERPOLATIONS_KEYFRAMES_H
#define INTERPOLATIONS_KEYFRAMES_H



class KeyFrames {

private:
    float d0 = 0, d1 = 0;
    float dampTime = 2.0f;
    vector<pair<glm::vec3, double>> pos;
    vector<pair<glm::vec3, double>> scal;
    vector<pair<glm::quat, double>> rot;


    // Spring Dampening Interpolation
    glm::vec3 SpringInterpPosition(double time);
    glm::vec3 SpringInterpScale(double time);
    glm::quat SpringInterpRotation(double time);


    // Cubic Interpolation
    glm::vec3 cubicInterpPosition(double time);
    glm::vec3 cubicInterpScale(double time);
    glm::quat cubicInterpRotation(double time);

    // Linear Interpolation
    glm::vec3 interpPosition(double time);
    glm::vec3 interpScale(double time);
    glm::quat interpRotation(double time);
    double last_time;
    double internal_time;
    bool playing = false;

public:

    void pushVec3(pair<glm::vec3, double> keyframe, char type);
    glm::mat4 getTransform();
    glm::mat4 getSpringTransform();

        void startPlaying();
    bool IsPlaying() const;
};

#endif //INTERPOLATIONS_KEYFRAMES_H
