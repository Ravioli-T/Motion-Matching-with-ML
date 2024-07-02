import pygame
import sys

pygame.init()

width, height = 800, 600
background = (0, 0, 0)
ch = (0, 255, 0)
size = 50
speed = 5

screen = pygame.display.set_mode((width, height))
pygame.display.set_caption('带有碰撞检测的角色移动')


# 角色类
class Character:
    def __init__(self, x, y):
        self.rect = pygame.Rect(x, y, size, size)

    def move(self, dx, dy):
        # 更新角色的位置并进行边界检查
        new_x = self.rect.x + dx
        new_y = self.rect.y + dy

        if 0 <= new_x <= width - size:
            self.rect.x = new_x
        if 0 <= new_y <= height - size:
            self.rect.y = new_y

    def draw(self, surface):
        pygame.draw.rect(surface, ch, self.rect)


# 创建一个角色实例
character = Character(width // 2, height // 2)

# 主循环
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    keys = pygame.key.get_pressed()
    dx = dy = 0
    if keys[pygame.K_LEFT]:
        dx -= speed
    if keys[pygame.K_RIGHT]:
        dx += speed
    if keys[pygame.K_UP]:
        dy -= speed
    if keys[pygame.K_DOWN]:
        dy += speed

    character.move(dx, dy)

    # 绘制所有内容
    screen.fill(background)
    character.draw(screen)
    pygame.display.flip()

    # 限制帧率
    pygame.time.Clock().tick(60)

pygame.quit()
sys.exit()
