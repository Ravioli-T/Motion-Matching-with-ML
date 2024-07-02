import pygame
import numpy as np
import sys
from anim import bvh

# Load the BVH file
anim = bvh.load("data/dance1_subject1.bvh")

# Create a mapping from joint names to indices
joint_name_to_idx = {joint.name: i for i, joint in enumerate(anim.skel.joints)}

# Define the dimensions of the window
WIDTH, HEIGHT = 800, 600

# Initialize Pygame
pygame.init()

# Create a window with the defined dimensions
window = pygame.display.set_mode((WIDTH, HEIGHT))

# Initial translation values for each joint
translations = np.zeros((len(anim.skel.joints), 3))


# Function to project 3D coordinates to 2D
def project_3d_to_2d(positions):
    center = np.mean(positions, axis=0)
    positions -= center
    fov = 500
    factor = fov / (fov + positions[:, 2])
    x2d = positions[:, 0] * factor + WIDTH / 2
    y2d = -positions[:, 1] * factor + HEIGHT / 2
    return np.column_stack([x2d, y2d])


def render_frame(positions, selected_joint_idx):
    window.fill((0, 0, 0))
    positions_2d = project_3d_to_2d(positions)
    for position in positions_2d:
        pygame.draw.circle(window, (255, 255, 255), position.astype(int), 5)
    for joint in anim.skel.joints:
        if joint.parent is not None and joint.parent != -1:
            start_idx = joint_name_to_idx[joint.name]
            end_idx = joint.parent
            start = positions_2d[start_idx]
            end = positions_2d[end_idx]
            pygame.draw.line(window, (255, 0, 0), start.astype(int), end.astype(int), 2)
    selected_joint_pos = positions_2d[selected_joint_idx]
    pygame.draw.circle(window, (0, 255, 0), selected_joint_pos.astype(int), 10)
    pygame.display.flip()


def main():
    clock = pygame.time.Clock()
    running = True
    anim_playing = False
    paused = False
    frame_idx = 0
    selected_joint_idx = 0

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        keys = pygame.key.get_pressed()
        if keys[pygame.K_RETURN]:
            anim_playing = not anim_playing
            print(f"Animation playing: {anim_playing}")
        if keys[pygame.K_BACKSPACE]:
            paused = not paused
            print(f"Paused: {paused}")
        if keys[pygame.K_SPACE] and paused:
            frame_idx = (frame_idx + 1) % len(anim)
        if keys[pygame.K_TAB]:
            selected_joint_idx = (selected_joint_idx + 1) % len(anim.skel.joints)
            print(f"Selected joint index: {selected_joint_idx}")

        if paused:
            if keys[pygame.K_LEFT]:
                translations[selected_joint_idx][0] -= 5
            if keys[pygame.K_RIGHT]:
                translations[selected_joint_idx][0] += 5
            if keys[pygame.K_UP]:
                translations[selected_joint_idx][1] -= 5
            if keys[pygame.K_DOWN]:
                translations[selected_joint_idx][1] += 5
            if keys[pygame.K_w]:
                translations[selected_joint_idx][2] -= 5
            if keys[pygame.K_s]:
                translations[selected_joint_idx][2] += 5

        if anim_playing and not paused:
            frame_idx = (frame_idx + 1) % len(anim)
            clock.tick(anim.fps)

        render_frame(anim.positions[frame_idx] + translations, selected_joint_idx)
        clock.tick(60)

    pygame.quit()
    sys.exit()


if __name__ == "__main__":
    main()
