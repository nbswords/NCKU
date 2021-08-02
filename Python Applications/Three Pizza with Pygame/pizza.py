from livewires import games
import random
games.init(screen_width=640, screen_height=480,
           fps=50)
           
class Pizza(games.Sprite):

 def update(self):
  """ 碰到視窗邊界的時候反彈"""
  if self.right > games.screen.width or self.left < 0:
    self.dx = -self.dx
  if self.bottom > games.screen.height or self.top < 0:
    self.dy = -self.dy
  self.check_collide()
  """披薩互撞也反彈"""
 def check_collide(self):
     for pizza in self.overlapping_sprites:
       pizza.handle_collide()

 def handle_collide(self):
     self.dx = -self.dx
     self.dy = -self.dy


def main():

 """背景"""
 space_image = games.load_image("space.jpg", transparent=False)
 games.screen.background = space_image

 """三個披薩 初始位置隨機 , 移動速度dx dy在1-5之間隨機 (避免移動太快會眼花撩亂因此設1-5)"""

 pizza_image1 = games.load_image("pizza.bmp")
 the_pizza1 = Pizza(image=pizza_image1,
                    x=random.randrange(games.screen.width),
                    y=random.randrange(games.screen.height),
                    dx=random.randrange(1,5),
                    dy=random.randrange(1,5) )

 pizza_image2 = games.load_image("pizza.bmp")
 the_pizza2 = Pizza(image=pizza_image2,
                    x=random.randrange(games.screen.width),
                    y=random.randrange(games.screen.height),
                    dx=random.randrange(1,5),
                    dy=random.randrange(1,5) )

 pizza_image3 = games.load_image("pizza.bmp")
 the_pizza3 = Pizza(image=pizza_image3,
                    x=random.randrange(games.screen.width),
                    y=random.randrange(games.screen.height),
                    dx=random.randrange(1,5),
                    dy=random.randrange(1,5) )

 games.screen.add(the_pizza1)
 games.screen.add(the_pizza2)
 games.screen.add(the_pizza3)
 games.screen.mainloop()


# kick it off
main()
