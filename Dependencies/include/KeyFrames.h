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
    glm::vec3 LinearInterpPosition(double time);
    glm::vec3 LinearInterpScale(double time);
    glm::quat LinearInterpRotation(double time);

    double last_time;
    double internal_time;
    bool playing = false;

public:
    double velocity, tr_acceleration, rot_acceleration;
    pair<glm::vec3, double> last_position;
    pair<glm::quat, double> last_rotation;
    float d0s = 0, d1s = 0;
    float d0t = 0, d1t = 0;
    float d0r = 0, d1r = 0;

    float dampTime = 2.0f;

    // spring constants
    float mt = 1, ct = 5;
    float kt = 100;
    float ms = 1, cs = 5;
    float ks = 100;
    float mr = 1, cr = 5;
    float kr = 13;

    int tmode = 2, smode = 2, rmode = 2;

    void pushVec3(pair<glm::vec3, double> keyframe, char type);
    glm::mat4 getCubicTransform();
    glm::mat4 getSpringTransform();


    glm::mat4 getTransform();

    void startPlaying();
    bool IsPlaying() const;
};

#endif //INTERPOLATIONS_KEYFRAMES_H
