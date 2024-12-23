import pygame as pg
import serial
import json
import threading
import time


#Scales
WIDTH = 320
HEIGHT = 240
BALL_RADIUS = 3
MAX_BUFF_SIZE = 255
FPS = 60


# COLORS
BLACK = (0, 0, 0)
BALL_COLOR = (230, 0, 0)

#pygame init
pg.init()
pg.display.set_caption("TFT Monitor")

clock = pg.time.Clock()
screen = pg.display.set_mode((WIDTH, HEIGHT))

#Serial init
controller = serial.Serial(
  port="COM6",
  baudrate=921600,
  timeout=1,
  write_timeout=1
)

#Balls
ball_positions = set()


def updatePositions(pos: tuple, positions: set) -> None:
    if pos in positions:
        return positions.remove(pos)
    else:
        positions.add(pos)
        

def writeToController(command) -> None:
    command_list = [command]
    controller.write(json.dumps(command_list).encode())
    

def drawWindow() -> None:
    for ball in ball_positions:
        pg.draw.circle(screen, BALL_COLOR, ball, BALL_RADIUS)
    
    pg.display.update()


def runWritingThread(command) -> threading.Thread:
    thread = threading.Thread(target=writeToController(command))
    thread.start()
    return thread


thread_running = False
while 1:
    for event in pg.event.get():
        if event.type == pg.QUIT:
          exit()
        if event.type == pg.KEYDOWN:
            if event.key == pg.K_c:
                ball_positions = set()
                thread = runWritingThread('c')
                thread_running = True
    
    if pg.mouse.get_pressed()[0]:
        pos = pg.mouse.get_pos()
        updatePositions(pos, ball_positions) #Update PyGame pos list
        thread = runWritingThread(pos) #Write the new pos to the controller
        thread_running = True


    if thread_running: #Wait for writing thread to finish
      thread.join()
      thread_running = False


    clock.tick(FPS)
    screen.fill(BLACK)
    drawWindow()
    
  