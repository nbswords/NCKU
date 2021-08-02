'''這是一個類似電子經典遊戲 Pong的遊戲  
主要內容就是用下方的橫版不斷將球往上頂回去 
若是讓球掉下去就結束了
使用方向鍵左右遊玩  一次移動只能按左或者按右
'''

'''
總共建立了 3個 Class  Ball , Paddle , Pong
in class Ball's init function create 7 variables to accept parameters
(x,y座標  vx,vy 速度 width,height 長寬 color 顏色)
use function "render" and "@Property" decorator to draw the ball
and third function "update" to update is movement

pseudocode of Class Ball :
  init ()
    accept x,y,height,vx,vy,color variables
  render()
    use pygame.draw to draw ellipse
  @property
    rect()
      回傳 pygame.Rect 
  update():
    持續讓
    x 和 y += 他們自己的位移(vx vy)
   
in class Paddle's init function create 6 variables to accept parameters
(x,y座標  speed 速度 width,height 長寬 color 顏色)
在Paddle中將vx vy合併成速度是為了方便操作
同樣使用@Property render update 
但為了給使用者操作  多了一個key handler 
利用pygame.KEYLEFT , pygame.KEYRIGHT來達成

init () ,render() ,@property 和Class Ball 類似 不再贅述
 key_handler()
   如果使用者輸入了按鍵 
       且輸入的是向左鍵
         則給予其負值的速度
       若輸入的是向右鍵
         給予其正值的速度
   否則若是兩個都有輸入
       讓速度=0
in Class Pong
  首先指定顏色
  init()
   設定視窗大小、球和橫板的大小和顏色
   並且初始化分數
  play()
    play bgm
    set clock=system clock
    如果有輸入按鍵
        若是這個按鍵是關掉遊戲 就關掉
        否則若輸入的是left或者right
        就一直持續輸入key left
  呼叫　collsion和draw
  
  draw()
    self.screen.fill(白色)
    pygame.font 分數的字體大小設為48
    將分數設為紅色
    印出分數
    ball.update
    ball.render
    paddle.update
    pallde.render
        
'''

import pygame
import sys
import math
from livewires import games

'''Load BGM'''
bgm = games.load_sound("bgm.wav")

'''Ball的屬性 '''
class Ball(object):
    def __init__(self, x, y, width, height, vx, vy, color):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.vx = vx
        self.vy = vy
        self.color = color
    '''用pygame畫出橢圓'''    
    def render(self, screen):
        pygame.draw.ellipse(screen, self.color, self.rect)
    '''更新球的路徑'''
    def update(self):
        self.x += self.vx
        self.y += self.vy

    @property
    def rect(self):
        return pygame.Rect(self.x, self.y, self.width, self.height)

'''橫板的屬性'''
class Paddle(object):
    def __init__(self, x, y, width, height, speed, color):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.vx = 0
        self.speed = speed
        self.color = color

    def render(self, screen):
        pygame.draw.rect(screen, self.color, self.rect)

    def update(self):
        self.x += self.vx
    '''用上下左右操控橫板 *2是為了讓移動的速度更快一些'''
    def key_handler(self, event):
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                self.vx = -self.speed *2
            elif event.key == pygame.K_RIGHT:
                self.vx = self.speed * 2
        elif event.key in (pygame.K_LEFT, pygame.K_RIGHT):
                self.vx = 0

    @property
    def rect(self):
        return pygame.Rect(self.x, self.y, self.width, self.height)



class Pong(object):
    COLORS = {
               "BLACK": (0,   0,   0),
               "WHITE": (255, 255, 255),
               "RED": (255,   0,   0)  }

    def __init__(self):
        pygame.init()
        (WIDTH, HEIGHT) = (640, 480)
        self.screen = pygame.display.set_mode((WIDTH, HEIGHT))
        pygame.display.set_caption("Pong")
        self.ball = Ball(5, 5, 50, 50, 5, 5, Pong.COLORS["BLACK"])
        self.paddle = Paddle(WIDTH / 2, HEIGHT - 50, 100,
                             10, 3, Pong.COLORS["BLACK"])
        self.score = 0

    def play(self):
        '''播放bgm'''
        bgm.play()
        clock = pygame.time.Clock()
        while True:
            clock.tick(50)
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()
                if event.type in (pygame.KEYDOWN, pygame.KEYUP):
                    self.paddle.key_handler(event)

            self.collision_handler()
            self.draw()
    def collision_handler(self):
        '''每次球碰到橫板的時候  反彈並且分數+1'''
        if self.ball.rect.colliderect(self.paddle.rect):
            self.ball.vy *= -1
            self.score += 1
        '''球碰到邊界也要反彈'''
        if self.ball.x + self.ball.width >= self.screen.get_width():
            self.ball.vx = -(math.fabs(self.ball.vx))
        elif self.ball.x <= 0:
            self.ball.vx = math.fabs(self.ball.vx)
        '''若是球掉到下面  遊戲就結束'''
        if self.ball.y + self.ball.height >= self.screen.get_height():
            pygame.quit()
            sys.exit()
        elif self.ball.y <= 0:
            self.ball.vy = math.fabs(self.ball.vy)
        '''如果橫板碰到牆壁 就阻止它繼續向右或者左'''
        if self.paddle.x + self.paddle.width >= self.screen.get_width():
            self.paddle.x = self.screen.get_width() - self.paddle.width
        elif self.paddle.x <= 0:
            self.paddle.x = 0
    '''分數的字體顏色設為紅色 背景設為白色'''
    def draw(self):
        self.screen.fill(Pong.COLORS["WHITE"])

        font = pygame.font.Font(None, 48)
        score_text = font.render("Score: " + str(self.score), True,
                                 Pong.COLORS["RED"])
        self.screen.blit(score_text, (0, 0))

        self.ball.update()
        self.ball.render(self.screen)
        self.paddle.update()
        self.paddle.render(self.screen)
        
        pygame.display.update()


if __name__ == "__main__":
    Pong().play()
