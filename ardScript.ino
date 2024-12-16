#include <SPI.h>
#include <TFT_eSPI.h>
#include <stdlib.h>
#include <vector>

// 320x240
#define WIDTH 320
#define HEIGHT 240
#define BALL_RADIUS 5
#define BALL_COUNT 10
#define MAX_BUFF_LEN 255


class Ball{
  public:
    int x;
    int y;
    int r = BALL_RADIUS;

    Ball(int x_cord, int y_cord){
      x = x_cord;
      y = y_cord;
    }
};


TFT_eSPI tft = TFT_eSPI();
std::vector<Ball> positions;
char c;
char str[MAX_BUFF_LEN];
int idx = 0; 


void setup() {
  Serial.begin(9600);
  

  tft.init();
  tft.setRotation(3);
  tft.setTextSize(3);
  tft.setTextColor(TFT_BLACK);
  tft.fillScreen(TFT_WHITE);
}

bool reading_x = true; 
void loop() {

  if (Serial.available()>0){
    c = Serial.read();
    // if (c == 'c'){
    //   positions.clear();
    // }
    if (c != '\n'){ //Get input
      str[idx++] = c;
    }
    else {
      str[idx] = '\0';
      idx = 0;

      //Filter the input
      //clear display
      if (str[0] == 'c'){
        positions.clear();
        drawCircles();
      }
      else{
        writeNewPos();
      }
    }
  }
}


void drawCircles(){
  if (positions.size()>0)
    for (int ball = 0; ball<positions.size(); ball++){
      int x = positions[ball].x;
      int y = positions[ball].y;
      int r = positions[ball].r;

      tft.fillCircle(x, y, r, TFT_VIOLET);
    }
  else{
    tft.fillScreen(TFT_WHITE);
  }
}


void genPositions(){
  for (int ball = 0; ball<BALL_COUNT; ball++){
    int x = random(WIDTH);
    int y = random(HEIGHT);

    positions.push_back(Ball(x, y));
  }
}

void writeNewPos(){
  //New Circle positions
  String x;
  String y;
  bool reading_x;
  for (int i=0; i<MAX_BUFF_LEN; i++){ 
    if (str[i] == '\0' || str[i] == '\n'){
      break;
    }

    if (str[i]=='x'){
      reading_x = true;
      continue;
    }
    else if (str[i]=='y'){
      reading_x = false;
      continue;
    }

    if (reading_x){
      x += str[i];
    }
    else{
      y += str[i];
    }
  }

  positions.push_back(Ball(x.toInt(), y.toInt()));
  drawCircles();  
  // delay(500);
}