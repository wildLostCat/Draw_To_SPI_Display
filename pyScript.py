import pygame as pg
import serial


#Scales
WIDTH = 320
HEIGHT = 240
BALL_RADIUS = 5
# BALL_COUNT = 100
MAX_BUFF_SIZE = 255
FPS = 60


# COLORS
BLACK = (0, 0, 0)

#pygame init
pg.init()
pg.display.set_caption("TFT Monitor")

clock = pg.time.Clock()
screen = pg.display.set_mode((WIDTH, HEIGHT))

#Serial init
controller = serial.Serial(
    port="COM6",
    baudrate=9600,
    timeout=1,
    write_timeout=1
)

#Balls
ball_positions = set()


def write_positions(positions) -> None:
    for ball in positions:
        string = f"x{ball[0]}y{ball[1]}\n"
        controller.write(string.encode())


def clear_display() -> None:
    controller.write("c\n".encode()) 


timer = 0
while 1:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            exit()
        if event.type == pg.KEYDOWN:
            if event.key == pg.K_c:
                ball_positions = set()
                clear_display()
    
    if pg.mouse.get_pressed()[0]:
        pos = pg.mouse.get_pos()
        if pos not in ball_positions:
            ball_positions.add(pos)
        else:
            ball_positions.remove(pos)

    if timer >=180: #Makes the write commands less frequent    
        if len(ball_positions) > 0:
            write_positions(ball_positions)
        timer = 0

    clock.tick(FPS)
    screen.fill(BLACK)
    timer += 1

