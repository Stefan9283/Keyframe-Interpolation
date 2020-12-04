//
// Created by Stefan on 04-Dec-20.
//

#include <KeyFrames.h>

glm::mat4 KeyFrames::getTransform() {
    cout << glfwGetTime() - internal_time << ": ";
    double time = glfwGetTime() - internal_time;
    glm::mat4 t = glm::mat4(1), r= glm::mat4(1), s= glm::mat4(1);
    s = glm::scale(glm::mat4(1), interpScale(time));
    r = glm::mat4_cast(interpRotation(time));
    t = glm::translate(glm::mat4(1), interpPosition(time));
    if(time > last_time) playing = false;
    glm::mat4 model = t * r * s;
    return model;
}

glm::vec3 KeyFrames::interpPosition(double time) {

    if(pos.size() == 0)
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

    cout << index << "\n";

    double a = (time - pos[index].second) / (pos[index+1].second - pos[index].second);

    return glm::mix(pos[index].first, pos[index+1].first, a);
}

glm::vec3 KeyFrames::interpScale(double time) {
    if(scal.size() == 0)
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

    cout << index << "\n";

    double a = (time - scal[index].second) / (scal[index+1].second - scal[index].second);

    return glm::mix(scal[index].first, scal[index+1].first, a);
}

glm::quat KeyFrames::interpRotation(double time) {
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

    glm::quat interp = glm::slerp(rot[index].first, rot[index+1].first, (float)a);
    //cout << glm::to_string(interp) << " " << glm::length(interp) << " interp\n";
    return interp;
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

                //cout << glm::to_string(rotation) << "\n";
                //cout << glm::to_string(keyframe.first) << " " << glm::to_string(glm::eulerAngles(kf.first))  << " " << glm::to_string(kf.first) << " rot\n";

                kf.second = keyframe.second;
                rot.push_back(kf);
            }
            else { cout << "typo? (pushVec3)\n"; return; }


            if(keyframe.second > last_time) last_time = keyframe.second;
}

void KeyFrames::startPlaying() {
    playing = true;
    internal_time = glfwGetTime();
}

bool KeyFrames::IsPlaying() {
    return playing;
}
