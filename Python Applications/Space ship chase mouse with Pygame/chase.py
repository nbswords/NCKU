from livewires import games
import random
games.init(screen_width=640, screen_height=480, fps=50)

class Ship(games.Sprite):

 def update(self):
        if self.x < games.mouse.x:
            self.x += 1
     
        if self.y < games.mouse.y:
            self.y += 1
        if self.x > games.mouse.x:
            self.x -= 1
        if self.y > games.mouse.y:
            self.y -= 1

def main():
    nebula_image = games.load_image("nebula.jpg", transparent=False)
    games.screen.background = nebula_image
    ship_image = games.load_image("ship.bmp")

    the_ship = Ship(
        image=ship_image,
        x=random.randrange(games.mouse.x),
        y=random.randrange(games.mouse.y),
                   )
    games.screen.add(the_ship)
    games.screen.mainloop()

main()
