#define RES 8
#define DC 7
#define CS 6

#define OLED_SPI_SPEED 8000000ul
#include <GyverOLED.h>
#include <math.h>

GyverOLED<SSD1306_128x64> oled;

float angle = 0.0;
float speed = 0.1;
int segments = 8;
float depthStep = 5.0;
float perspective = 80.0;

void setup() {
  oled.init();
  Wire.setClock(800000L);
}

void loop() {
  oled.clear();
  int centerX = 64;
  int centerY = 32;

  for (int i = 0; i < 10; i++) {
    float scale = 1.0 / (1.0 + (i * depthStep) / perspective);
    float radius = 20.0 * scale;
    float xOffset = sin(angle + i * 0.3) * 10 * scale;
    int xCenter = centerX + xOffset;
    int yCenter = centerY + i * depthStep * scale;

    int prevX = xCenter + radius * cos(0);
    int prevY = yCenter + radius * sin(0);

    for (int j = 1; j <= segments; j++) {
      float theta = (j * 2 * PI) / segments;
      int x = xCenter + radius * cos(theta);
      int y = yCenter + radius * sin(theta);
      oled.line(prevX, prevY, x, y);
      prevX = x;
      prevY = y;
    }

    if (i > 0) {
      int prevCenterX = centerX + sin(angle + (i - 1) * 0.3) * 10 * (1.0 / (1.0 + ((i - 1) * depthStep) / perspective));
      int prevCenterY = centerY + (i - 1) * depthStep * (1.0 / (1.0 + ((i - 1) * depthStep) / perspective));
      float prevRadius = 20.0 * (1.0 / (1.0 + ((i - 1) * depthStep) / perspective));
      for (int j = 0; j < segments; j++) {
        float theta1 = (j * 2 * PI) / segments;
        float theta2 = ((j + 1) * 2 * PI) / segments;
        int x1 = prevCenterX + prevRadius * cos(theta1);
        int y1 = prevCenterY + prevRadius * sin(theta1);
        int x2 = xCenter + radius * cos(theta1);
        int y2 = yCenter + radius * sin(theta1);
        oled.line(x1, y1, x2, y2);
      }
    }
  }

  oled.update();
  angle += speed;
  delay(50);
}
