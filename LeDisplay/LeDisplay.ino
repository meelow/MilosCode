// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>

#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 6

// Color definitions
#define COL_BLACK_32    0x000000
#define COL_RED_32      0xFE0000
#define COL_BLUE_32     0x00FE00
#define COL_GREEN_32    0x0000FE


#define COL_MAX_RED_8   0xFE
#define COL_MAX_GREEN_8 0xFE
#define COL_MAX_BLUE_8  0xFE


/*
// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.
*/
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(6, 6, 6,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);


byte serial_value = 0;

void setup() {
  matrix.begin();
  matrix.show();
  
  // initialize the serial communication:
  Serial.begin(9600);
}


uint32_t HSVtoRGB(float h, float s, float v)
{
  //https://gist.github.com/hdznrrd/656996
  int i;
  float f,p,q,t;
  byte r,g,b;
  
  h = max(0.0, min(360.0, h));
  s = max(0.0, min(100.0, s));
  v = max(0.0, min(100.0, v));
  
  s /= 100;
  v /= 100;
  
  if(s == 0) {
    // Achromatic (grey)
    r = g = b = round(v*255);
    return Adafruit_NeoPixel::Color( r,  g,  b);
  }
 
  h /= 60; // sector 0 to 5
  i = floor(h);
  f = h - i; // factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  switch(i) {
    case 0:
      r = round(255*v);
      g = round(255*t);
      b = round(255*p);
      break;
    case 1:
      r = round(255*q);
      g = round(255*v);
      b = round(255*p);
      break;
    case 2:
      r = round(255*p);
      g = round(255*v);
      b = round(255*t);
      break;
    case 3:
      r = round(255*p);
      g = round(255*q);
      b = round(255*v);
      break;
    case 4:
      r = round(255*t);
      g = round(255*p);
      b = round(255*v);
      break;
    default: // case 5:
      r = round(255*v);
      g = round(255*p);
      b = round(255*q);
    }
  return Adafruit_NeoPixel::Color( r,  g,  b);
}

void ani_testimage(uint16_t steps, uint16_t delaytime)
{
   for(uint16_t j=0; j<1; j++)
  {
	   uint32_t c1 =matrix.Color(50,200,0);
	   uint32_t c2 = matrix.Color(0,50,200);
	  for(uint16_t i=0; i<matrix.numPixels(); i++) {
		  matrix.setPixelColor(i, i%2 ? c1 : c2);
	  }
	  matrix.show();
	  delay(delaytime);
	  for(uint16_t i=0; i<matrix.numPixels(); i++) {
		  matrix.setPixelColor(i, i%2 ? c2 : c1);
	  }
	  matrix.show();
	  delay(delaytime);
  }
}

void fillScreen( uint32_t c )
{
  for( uint16_t i=0; i<matrix.numPixels(); i++)
  {
	matrix.setPixelColor(i, c);
  }
  matrix.show();
}

void fillLine(uint8_t line, uint32_t color)
{
  uint8_t base = line*6;
  for( uint8_t i=0; i<6; i++)
  {
    matrix.setPixelColor(base+i,color);
  }
  matrix.show();
}


/** animation that lets a pixel run across the canvas in forward direction with repeats using
  * a delaytime.
  * @param forward: false or true
  * @param repeats: repeat the forward/backward movement
  * @param delaytime: time between moving the pixels
  */ 
void ani_chase(bool forward, uint16_t repeats, uint16_t delaytime)
{
  if( forward==true )
  {
 	  Serial.write("forward: ");
	  Serial.println( forward, DEC );
    for( uint16_t run=0; run<repeats; run++)
    {
      for( uint16_t i=0; i<=36; i++)
      {
        matrix.fillScreen( matrix.Color(0,0,0));
        matrix.setPixelColor( i, matrix.Color(254, 0, 0));
        delay(delaytime);
        matrix.show();
      } // end chasing
    } // end repeats
  } // if forward
  else
  {
    for( uint16_t run=0; run<repeats; run++)
    {
      for( uint16_t i=36; i>0; i--)
      {
        // Serial.write("backward: ");
        // Serial.println( forward, DEC );    
        matrix.fillScreen( matrix.Color(0,0,0));
        matrix.setPixelColor( i-1, matrix.Color(254, 0, 0));
        delay(delaytime);
        matrix.show();
      } // end chasing
    } // end repeats
  } // end backwards
}

void ani_chaseLine(bool forward, uint16_t repeats, uint16_t delaytime)
{
  if( forward==true )
  {
 	  Serial.write("forward: ");
	  Serial.println( forward, DEC );
    for( uint16_t run=0; run<repeats; run++)
    {
      for( uint16_t i=0; i<=6; i++)
      {
        matrix.fillScreen( matrix.Color(0,0,0));
        fillLine( i, matrix.Color(254, 0, 0));
        delay(delaytime);
        matrix.show();
      } // end chasing
    } // end repeats
  } // if forward
  else
  {
    for( uint16_t run=0; run<repeats; run++)
    {
      for( uint16_t i=6; i>0; i--)
      {
        // Serial.write("backward: ");
        // Serial.println( forward, DEC );    
        matrix.fillScreen( matrix.Color(0,0,0));
        fillLine( i-1, matrix.Color(254, 0, 0));
        delay(delaytime);
        matrix.show();
      } // end chasing
    } // end repeats
  } // end backwards
}

void ani_walkSaturation(uint16_t delayTime)
{
	for( uint8_t sat=0; sat<99; sat++)
	{
		fillScreen(HSVtoRGB(0, sat, 99));
		matrix.show();
		delay(delayTime);
	}
	delay(delayTime*5);
	for( uint8_t sat=100; sat>1; sat--)
	{
		fillScreen(HSVtoRGB(0, sat-1, 99));
		matrix.show();
		delay(delayTime);
	}	  
}

void ani_walkValue(uint16_t delayTime)
{
	for( uint8_t value=0; value<99; value++)
	{
		fillScreen(HSVtoRGB(0, 99, value));
		matrix.show();
		delay(delayTime);
	}
	delay(delayTime*5);
	for( uint8_t value=100; value>1; value--)
	{
		fillScreen(HSVtoRGB(0, 99, value));
		matrix.show();
		delay(delayTime);
	}
}

void ani_walkHue(uint16_t delayTime)
{
	for( uint16_t hue=0; hue<360; hue++ )
	{
		fillScreen(HSVtoRGB(hue, 99, 99));
		matrix.show();
		delay(delayTime);
	}
}

void loop() 
{

  static byte i;

  
  // print out what was received:
  if (Serial.available()) 
  {
    // read the most recent byte (which will be from 0 to 255):
    serial_value = Serial.read();
	if( serial_value!=' ' )
	{
		Serial.write("read: ");
		Serial.write(serial_value);
		Serial.write("\n");
    // short testimage:
    ani_testimage(2, 150);
	}
  }
  
  // handler:
  switch(serial_value)
  {
    case '1' : 
	  fillScreen(COL_RED_32);
	  matrix.show();
	  break;
	case '2' : 
	  fillScreen(COL_GREEN_32);
	  matrix.show();
	  break;
	case '3' : 
	  fillScreen(COL_BLUE_32);
 	  // Serial.write("Color: ");Serial.println( Adafruit_NeoPixel::Color(0,0,254), HEX );
	  matrix.show();
	  break;
	case '4' : 
	  fillScreen(Adafruit_NeoPixel::Color( 254,  0,  0));
	  matrix.show();
	  break;
	case '5' : 
	  fillScreen(Adafruit_NeoPixel::Color( 0,  254,  0));
	  matrix.show();
	  break;
	case '6' : 
	  fillScreen(Adafruit_NeoPixel::Color( 0,  0,  254));
 	  // Serial.write("Color: ");Serial.println( Adafruit_NeoPixel::Color(0,0,254), HEX );
	  matrix.show();
	  break;
	case ' ':
	  break;
	case 'c':
      ani_chase(true, 1, 30);
	  ani_chase(false,1, 30);  
	  // Serial.write("speed: ");
	  // Serial.println( i, DEC );
	  break;
	case 'w' : 
      ani_walkHue(20);
	  break;
	case 'e' : 
	  ani_walkSaturation(30);
	  break;
	case 'r' : 
	  ani_walkValue(30);
	  break;
	case 'm':
		for( uint16_t colorBase=0; colorBase<360; colorBase+=24)
		{
			for( uint8_t line=0; line<6; line++ )
			{
				fillLine(line, HSVtoRGB(colorBase+line*4,99,99));
				matrix.show();
				delay(20);
			}
		}
	  break;
	case 'n':
      ani_chaseLine(true, 1, 30);
	  ani_chaseLine(false,1, 30);  
	  break;
	case 'b':
		for( uint16_t colorBase=0; colorBase<360; colorBase+=36)
		{
			for( uint8_t pixel=0; pixel<36; pixel++ )
			{
				matrix.setPixelColor(pixel,HSVtoRGB(colorBase+pixel,99,99));
				matrix.show();
				delay(15);
			}
		}
	  
	  break;
	default :
	  fillScreen(Adafruit_NeoPixel::Color(128,128,0));
	  matrix.show();
	  break;
  }

}
