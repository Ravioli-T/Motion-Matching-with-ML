#include "bvh_loader.h"
#include <fstream>
#include <sstream>

BVHData LoadBVH(const char* fileName) {
    BVHData bvhData;
    std::ifstream file(fileName);
    std::string line;

    if (!file.is_open()) {
        TraceLog(LOG_ERROR, "Failed to open BVH file");
        return bvhData;
    }

    while (std::getline(file, line)) {
        // Parse hierarchy, motion data, etc.
        if (line.find("Frame Time:") != std::string::npos) {
            std::istringstream ss(line);
            std::string token;
            ss >> token >> token; // Skip "Frame" and "Time:"
            ss >> bvhData.frameTime;
        }
        else if (line.find("ROOT") != std::string::npos || line.find("JOINT") != std::string::npos) {
            BVHJoint joint;
            std::istringstream ss(line);
            std::string token;
            ss >> token >> joint.name;
            bvhData.joints.push_back(joint);
        }
        else if (line.find("MOTION") != std::string::npos) {
            break;
        }
    }

    // Parse frames data
    while (std::getline(file, line)) {
        if (line.empty() || line.find("Frame Time:") != std::string::npos) {
            continue;
        }

        std::istringstream ss(line);
        BVHFrame frame;
        float value;
        while (ss >> value) {
            frame.data.push_back(value);
        }
        bvhData.frames.push_back(frame);
    }

    file.close();
    return bvhData;
}

ModelAnimation ConvertBVHToModelAnimation(const BVHData& bvhData, Model* model) {
    ModelAnimation anim = { 0 };
    anim.boneCount = model->boneCount;
    anim.frameCount = bvhData.frames.size();
    anim.bones = model->bones;
    anim.framePoses = (Transform**)RL_MALLOC(anim.frameCount * sizeof(Transform*));

    for (int i = 0; i < anim.frameCount; i++) {
        anim.framePoses[i] = (Transform*)RL_MALLOC(anim.boneCount * sizeof(Transform));
        for (int j = 0; j < anim.boneCount; j++) {
            anim.framePoses[i][j].translation = Vector3{ 0.0f, 0.0f, 0.0f };
            anim.framePoses[i][j].rotation = Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
            anim.framePoses[i][j].scale = Vector3{ 1.0f, 1.0f, 1.0f };
        }
    }

    for (int i = 0; i < bvhData.frames.size(); i++) {
        BVHFrame frame = bvhData.frames[i];
        for (int j = 0; j < bvhData.joints.size(); j++) {
            BVHJoint joint = bvhData.joints[j];
            int baseIndex = j * 3;
            if (baseIndex + 2 < frame.data.size()) {
                anim.framePoses[i][j].translation.x = frame.data[baseIndex];
                anim.framePoses[i][j].translation.y = frame.data[baseIndex + 1];
                anim.framePoses[i][j].translation.z = frame.data[baseIndex + 2];
            }
        }
    }

    return anim;
}

void UnloadBVHData(BVHData bvhData) {
    // Clean up BVH data if needed
}

void UnloadModelAnimationData(ModelAnimation anim) {
    for (int i = 0; i < anim.frameCount; i++) {
        RL_FREE(anim.framePoses[i]);
    }
    RL_FREE(anim.framePoses);
}
