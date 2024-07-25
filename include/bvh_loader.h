#ifndef BVH_LOADER_H
#define BVH_LOADER_H

#include <raylib.h>
#include <vector>
#include <string>

struct BVHJoint {
    std::string name;
    Vector3 offset;
    std::vector<int> channels;
    std::vector<BVHJoint> children;
};

struct BVHFrame {
    std::vector<float> data;
};

struct BVHData {
    std::vector<BVHJoint> joints;
    std::vector<BVHFrame> frames;
    float frameTime;
};

BVHData LoadBVH(const char* fileName);
ModelAnimation ConvertBVHToModelAnimation(const BVHData& bvhData, Model* model);
void UnloadBVHData(BVHData bvhData);
void UnloadModelAnimationData(ModelAnimation anim);

#endif // BVH_LOADER_H
