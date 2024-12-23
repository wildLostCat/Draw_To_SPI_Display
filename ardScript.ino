#include <SPI.h>
#include <TFT_eSPI.h>
#include <stdlib.h>
#include <vector>
#include <ArduinoJson.h>


// 320x240
#define WIDTH 320
#define HEIGHT 240
#define BALL_RADIUS 3
#define MAX_BUFF_LEN 255


class Ball{
  public:
    int x;
    int y;
    int r = BALL_RADIUS;
    int32_t ball_color = TFT_PINK;
    

    Ball(int x_cord, int y_cord){
      x = x_cord;
      y = y_cord;
    }
};


JsonDocument doc;
TFT_eSPI tft = TFT_eSPI();
std::vector<Ball> positions;
char c;
char str[MAX_BUFF_LEN];
int idx = 0; 


void setup() {
  Serial.begin(921600);
  

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_WHITE);
}


void loop() {
  if (Serial.available()>0){
    deserializeJson(doc, Serial);
    

    for (int i=0; i<doc.size(); i++){
      if (doc[0] == "c"){
        positions.clear();
        break;
      }
      else{
        int x = doc[i][0];
        int y = doc[i][1];
        positions.push_back(Ball(x, y));
      }
    }
    
    doc.clear();
    drawCircles();
  }
}


void drawCircles(){
  if (positions.size()>0)
    for (int ball = 0; ball<positions.size(); ball++){
      int x = positions[ball].x;
      int y = positions[ball].y;
      int r = positions[ball].r;

      tft.fillCircle(x, y, r, positions[ball].ball_color);
    }
  else{
    tft.fillScreen(TFT_WHITE);
  }
}

