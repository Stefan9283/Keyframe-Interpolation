//
// Created by Stefan on 04-Dec-20.
//

#include <KeyFrames.h>

void KeyFrames::startPlaying() {
    playing = true;
    internal_time = glfwGetTime();

    if(!pos.empty()) { last_position = pos[0];}
    if(!rot.empty()) { last_rotation = rot[0];}
    tr_acceleration = 0.0f, rot_acceleration = 0.0f;
    velocity = 0.0f;
}
bool KeyFrames::IsPlaying() const {
    return playing;
}
void KeyFrames::pushVec3(std::pair<glm::vec3, double> keyframe, char type) {
    if(type == 's') scal.push_back(keyframe);
    else if(type == 't') pos.push_back(keyframe);
    else if(type == 'r') {
        glm::mat4 rotation = glm::mat4(1);
        rotation = glm::rotate(rotation, glm::radians(keyframe.first.x), glm::vec3(1.0f, 0.0f, 0.0f));
        rotation = glm::rotate(rotation, glm::radians(keyframe.first.y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotation = glm::rotate(rotation, glm::radians(keyframe.first.z), glm::vec3(0.0f, 0.0f, 1.0f));

        std::pair<glm::quat, double> kf;
        kf.first = glm::quat_cast(rotation);

        kf.second = keyframe.second;
        rot.push_back(kf);
    }
    else { cout << "typo? (pushVec3)\n"; return; }


    if(keyframe.second > last_time) last_time = keyframe.second;
}
glm::mat4 KeyFrames::getTransform() {
    glm::vec3 t, s;
    glm::quat r;

    double time = glfwGetTime() - internal_time;

    switch (tmode) {
        case 0:
            t = LinearInterpPosition(time);
            if(time > last_time) playing = false;
            break;
        case 1:
            t = cubicInterpPosition(time);
            if(time > last_time) playing = false;
            break;
        case 2:
            if(!pos.empty() && pos[pos.size() - 1].second < time)
                t = SpringInterpPosition(time);
            else t = cubicInterpPosition(time);
            if(time > last_time + dampTime) {
                playing = false;
            }
            break;
    }
    switch (rmode) {
        case 0:
            r = LinearInterpRotation(time);
            if(time > last_time) playing = false;
            break;
        case 1:
            r = cubicInterpRotation(time);
            if(time > last_time) playing = false;
            break;
        case 2:
            if(!rot.empty() && rot[rot.size() - 1].second < time)
                r = SpringInterpRotation(time);
            else r = cubicInterpRotation(time);
            if(time > last_time + dampTime) {
                playing = false;
            }
            break;
    }
    switch (smode) {
        case 0:
            s = LinearInterpScale(time);
            if(time > last_time) playing = false;
            break;
        case 1:
            s = cubicInterpScale(time);
            if(time > last_time) playing = false;
            break;
        case 2:
            if(!scal.empty() && scal[scal.size() - 1].second < time)
                s = SpringInterpScale(time);
            else s = cubicInterpScale(time);
            if(time > last_time + dampTime) {
                playing = false;
            }
            break;
    }

    return glm::translate(glm::mat4(1), t) * glm::mat4_cast(r) * glm::scale(glm::mat4(1), s);
}

// Linear Interpolation
glm::vec3 KeyFrames::LinearInterpPosition(double time) {

    if(pos.empty())
        return glm::vec3(0.0f);
    if(pos.size() == 1)
        return pos[0].first;

    if(time > pos[pos.size() - 1].second)
        time = pos[pos.size() - 1].second;

    int index = 0;
    for(int i=0; i<pos.size()-1; i++)
    {
        index = i;
        if(pos[i+1].second > time)
            break;
    }


    double a = (time - pos[index].second) / (pos[index+1].second - pos[index].second);

    return glm::mix(pos[index].first, pos[index+1].first, a);
}
glm::vec3 KeyFrames::LinearInterpScale(double time) {
    if(scal.empty())
        return glm::vec3(1.0f);
    if(scal.size() == 1)
        return scal[0].first;

    if(time > scal[scal.size() - 1].second)
        time = scal[scal.size() - 1].second;

    int index = 0;
    for(int i=0; i<scal.size()-1; i++)
    {
        index = i;
        if(scal[i+1].second > time)
            break;
    }


    double a = (time - scal[index].second) / (scal[index+1].second - scal[index].second);

    return glm::mix(scal[index].first, scal[index+1].first, a);
}
glm::quat KeyFrames::LinearInterpRotation(double time) {
    if(rot.empty())
        return glm::quat(1,0,0,0);
    if(rot.size() == 1)
        return rot[0].first;

    if(time > rot[rot.size() - 1].second)
        time = rot[rot.size() - 1].second;

    int index = 0;
    for(int i=0; i<rot.size()-1; i++)
    {
        index = i;
        if(rot[i+1].second > time)
            break;
    }

    double a = (time - rot[index].second) / (rot[index+1].second - rot[index].second);

    return glm::slerp(rot[index].first, rot[index+1].first, (float)a);
}

// Cubic Spline Interpolation
glm::mat4 KeyFrames::getCubicTransform() {
    double time = glfwGetTime() - internal_time;
    glm::mat4 t = glm::mat4(1), r= glm::mat4(1), s= glm::mat4(1);
    s = glm::scale(glm::mat4(1), cubicInterpScale(time));
    r = glm::mat4_cast(cubicInterpRotation(time));
    t = glm::translate(glm::mat4(1), cubicInterpPosition(time));
    if(time > last_time) playing = false;
    glm::mat4 model = t * r * s;
    return model;
}
float cubicInterp(float f0, float f1, float d0, float d1, float p) {
    //assert(p>=0 && p<=1);
    // f(x) = a * x ^ 3 + b * x ^ 2 + c * x + d
    float d = f0;
    float c = d0;
    float a = d1 - d0 - 2 * (f1 - c - d);
    float b = f1 - a - d - c;

    return a * (p * p * p) + b * (p * p) + c * (p) + d;
}
float cubicAccelerationFunc(float p, float d0, float d1) {
    //assert(p>=0 && p<=1);
    float f0 = 0, f1 = 1;
    // f(x) = a * x ^ 3 + b * x ^ 2 + c * x + d
    float d = f0;
    float c = d0;
    float a = d1 - d0 - 2 * (f1 - c - d);
    float b = f1 - a - d - c;

    return a * (p * p * p) + b * (p * p) + c * (p) + d;
}
float quinticAccelerationFunc(float f0, float f1, float p, float d0, float d1, float dd0, float dd1) {
    float
    delta = 1,
    d = (dd0/2) * delta * delta,
    e = d0 * delta,
    f = 0,
    A = 1 - d - e - f,
    B = d1 * delta - 2 * d - e,
    C = dd1 * delta * delta - 2 * d,
    b = - 15 * A + 7 * B  - C,
    a = (B - 3 * A - b) / 2,
    c = A - a - b;

    float res = a * (p * p * p * p * p) + b * (p * p * p * p) + c * (p * p * p) + d * (p * p) + e * (p) + f;
    return res;
}
glm::vec3 cubInterpVec3(glm::vec3 v0, glm::vec3 v1, float d0, float d1, float p) {
    float x, y, z;
    x = cubicInterp(v0.x, v1.x, d0, d1, p);
    y = cubicInterp(v0.y, v1.y, d0, d1, p);
    z = cubicInterp(v0.z, v1.z, d0, d1, p);
    return glm::vec3(x, y, z);
}
glm::quat cubInterpQuat(glm::quat q0, glm::quat q1, float d0, float d1, float p) {
    float x,y,z,w;
    x = cubicInterp(q0.x, q1.x, d0, d1, p);
    y = cubicInterp(q0.y, q1.y, d0, d1, p);
    z = cubicInterp(q0.z, q1.z, d0, d1, p);
    w = cubicInterp(q0.w, q1.w, d0, d1, p);
    return glm::quat(w, x, y, z);
}
glm::vec3 KeyFrames::cubicInterpPosition(double time) {
    if(pos.empty())
        return glm::vec3(0.0f);
    if(pos.size() == 1)
        return pos[0].first;

    if(time > pos[pos.size() - 1].second)
        time = pos[pos.size() - 1].second;

    int index = 0;
    for(int i=0; i<pos.size()-1; i++)
    {
        index = i;
        if(pos[i+1].second > time)
            break;
    }

    double a = (time - pos[index].second) / (pos[index+1].second - pos[index].second);

    float ds = 0, df = 0;
    if(index == 0)
        ds = d0t;
    if(index + 1 == pos.size() - 1)
        df = d1t;

    if(index != pos.size() - 2)
        a = cubicAccelerationFunc((float)a, ds, df);
    else a = quinticAccelerationFunc(pos[index].second, pos[index+1].second, (float)a, 0, 3, 1, 1);


    glm::vec3 res = cubInterpVec3(pos[index].first, pos[index+1].first, ds, df, (float)a);

    if(time != last_position.second)
        tr_acceleration = glm::length(res - last_position.first)/(time - last_position.second);

    if(time != last_position.second) {
        last_position.first = res;
        last_position.second = time;
    }

    return res;
}
glm::vec3 KeyFrames::cubicInterpScale(double time) {
    if(scal.empty())
        return glm::vec3(1.0f);
    if(scal.size() == 1)
        return scal[0].first;

    if(time > scal[scal.size() - 1].second)
        time = scal[scal.size() - 1].second;

    int index = 0;
    for(int i=0; i<scal.size()-1; i++)
    {
        index = i;
        if(scal[i+1].second > time)
            break;
    }

    double a = (time - scal[index].second) / (scal[index+1].second - scal[index].second);

    float ds = 0, df = 0;
    if(index == 0)
        ds = d0s;
    if(index + 1 == scal.size() - 1)
        df = d1s;

    if(index != scal.size() - 2)
        a = cubicAccelerationFunc((float)a, ds, df);
    else a = quinticAccelerationFunc(scal[index].second, scal[index+1].second, (float)a, 0, 3, 1, 1);

    glm::vec3 res = cubInterpVec3(scal[index].first, scal[index+1].first, ds, df, (float)a);

    if(time != last_scaling.second) {
        scal_acceleration = glm::length(res - last_scaling.first)/(time - last_scaling.second);

        last_scaling.first = res;
        last_scaling.second = time;
    }

    return res;
}
glm::quat KeyFrames::cubicInterpRotation(double time) {
    if(rot.empty())
        return glm::quat(1,0,0,0);
    if(rot.size() == 1)
        return rot[0].first;

    if(time > rot[rot.size() - 1].second)
        time = rot[rot.size() - 1].second;

    int index = 0;
    for(int i=0; i<rot.size()-1; i++)
    {
        index = i;
        if(rot[i+1].second > time)
            break;
    }

    double a = (time - rot[index].second) / (rot[index+1].second - rot[index].second);

    float ds = 0, df = 0;
    if(index == 0)
        ds = d0r;
    if(index + 1 == rot.size() - 1)
        df = d1r;

    if(index != rot.size() - 2)
        a = cubicAccelerationFunc((float)a, ds, df);
    else a = quinticAccelerationFunc(rot[index].second, rot[index+1].second, (float)a, 0, 3, 1, 1);


    glm::quat res = glm::normalize(cubInterpQuat(rot[index].first, rot[index+1].first, ds, df, (float)a));

    if(time != last_rotation.second) {
        rot_acceleration = glm::length(res - last_rotation.first)/(time - last_rotation.second);

        last_rotation.first = res;
        last_rotation.second = time;
    }

    return res;

}

// Spring Dampening Interpolation
glm::mat4 KeyFrames::getSpringTransform() {
    double time = glfwGetTime() - internal_time;

    glm::mat4 t = glm::mat4(1), r= glm::mat4(1), s= glm::mat4(1);

    if(!scal.empty() && scal[scal.size() - 1].second < time)
        s = glm::scale(glm::mat4(1), SpringInterpScale(time));
    else s = glm::scale(glm::mat4(1), cubicInterpScale(time));

    if(!rot.empty() && rot[rot.size() - 1].second < time)
        r = glm::mat4_cast(SpringInterpRotation(time));
    else r = glm::mat4_cast(cubicInterpRotation(time));


    if(!pos.empty() && pos[pos.size() - 1].second < time)
        t = glm::translate(glm::mat4(1), SpringInterpPosition(time));
    else t = glm::translate(glm::mat4(1), cubicInterpPosition(time));


    if(time > last_time + dampTime) {
        playing = false;
    }
    glm::mat4 model = t * r * s;
    return model;
}
glm::vec3 KeyFrames::SpringInterpPosition(double time) {
    glm::vec3 CubicPos = cubicInterpPosition(time);

    if(pos.empty()) return CubicPos;
    if(pos.size() == 1) return CubicPos;

    float delta = ct / mt / 2.0f;
    float omega = glm::sqrt(kt/mt);

    //assert(omega - delta >= 0);

    time = glfwGetTime() - internal_time - pos[pos.size() - 1].second;

    glm::vec3 sol = CubicPos + glm::normalize(CubicPos - pos[pos.size() - 2].first)
            * (float)(tr_acceleration * glm::exp(- delta * time) * glm::sin(omega * time));

    return sol;
}
glm::vec3 KeyFrames::SpringInterpScale(double time) {
    glm::vec3 CubicScal = cubicInterpScale(time);

    if(scal.empty()) return CubicScal;
    if(scal.size() == 1) return CubicScal;

    float delta = cs / ms / 2.0f;
    float omega = glm::sqrt(ks/ms);

    //assert(omega - delta >= 0);

    time = glfwGetTime() - internal_time - scal[scal.size() - 1].second;

    glm::vec3 sol = CubicScal + glm::normalize(CubicScal - scal[scal.size() - 2].first)
                               * (float)(scal_acceleration * glm::exp(- delta * time) * glm::sin(omega * time));

    return sol;
}
glm::quat KeyFrames::SpringInterpRotation(double time) {
    glm::quat CubicRot = cubicInterpRotation(time);

    if(rot.empty()) return CubicRot;
    if(rot.size() == 1) return CubicRot;


    float delta = cr / mr / 2.0f;
    float omega = glm::sqrt(kr/mr);

    //assert(omega - delta >= 0);

    time = glfwGetTime() - internal_time - rot[rot.size() - 1].second;

    glm::quat sol = CubicRot + glm::normalize(CubicRot - rot[rot.size() - 2].first)
                                * (float)(rot_acceleration * glm::exp(- delta * time) * glm::sin(omega * time));

    return sol;
}
