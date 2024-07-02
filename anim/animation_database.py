class AnimationDatabase:
    def __init__(self):
        self.animations = {}  # Key: animation name, Value: animation data

    def load_bvh_files(self, file_paths):
        for path in file_paths:
            # Assuming a function `load_bvh_file` exists that loads a BVH file and returns its data
            animation_name, animation_data = load_bvh_file(path)
            self.animations[animation_name] = animation_data

    def get_animation(self, name):
        return self.animations.get(name, None)

    def get_frame(self, animation_name, frame_index):
        animation = self.get_animation(animation_name)
        if animation is not None:
            # Assuming animation_data is a list of frames
            return animation['frames'][frame_index]
        return None

# Example function to load a BVH file, to be implemented
def load_bvh_file(file_path):
    # Load the file, extract name and data
    animation_name = "Extracted from file path or file content"
    animation_data = {
        "frames": [],  # Populate with frame data
        # Include other relevant data such as joint hierarchy
    }
    return animation_name, animation_data