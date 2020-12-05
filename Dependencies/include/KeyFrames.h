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
    double velocity, acceleration;
    float d0 = 0, d1 = 0;
    float dampTime = 2.0f;

    // spring constants
    float m = 1, c = 5;
    float k = 100;

    int tmode = 2, smode = 2, rmode = 2;

    void pushVec3(pair<glm::vec3, double> keyframe, char type);
    glm::mat4 getCubicTransform();
    glm::mat4 getSpringTransform();


    glm::mat4 getTransform();

    void startPlaying();
    bool IsPlaying() const;
};

#endif //INTERPOLATIONS_KEYFRAMES_H
