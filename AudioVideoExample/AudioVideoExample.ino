//code by bitluni give me a shout-out if you like it
#include <soc/rtc.h>
#include "AudioSystem.h"
#include "AudioOutput.h"
#include "Graphics.h"
#include "Image.h"
#include "CompositeOutput.h"
#include "GameControllers.h"
#include "Sprites.h"
#include "Font.h"

//lincude graphics and sounds
namespace font88
{
#include "gfx/font.h"
}
Font font(8, 8, font88::pixels);
#include "gfx/sprites.h"
#include "sfx/sounds.h"

////////////////////////////
//controller pins
const int LATCH = 16;
const int CLOCK = 17;
const int CONTROLLER_DATA_PIN = 18;
GameControllers controllers;

////////////////////////////
//audio configuration
const int SAMPLING_RATE = 44100;
const int BUFFER_SIZE = 2000;
AudioSystem audioSystem(SAMPLING_RATE, BUFFER_SIZE);
AudioOutput audioOutput;

///////////////////////////
//Video configuration
//PAL MAX, half: 324x268 full: 648x536
//NTSC MAX, half: 324x224 full: 648x448
const int XRES = 320;
const int YRES = 144;
Graphics graphics(XRES, YRES);
CompositeOutput composite(CompositeOutput::PAL, XRES * 2, YRES * 2);

void compositeCore(void *data)
{
  while (true)
  {
    composite.sendFrameHalfResolution(&graphics.frame);
  }
}

void setup()
{
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_240M);              //highest cpu frequency

  //initialize composite output and graphics
  composite.init();
  graphics.init();
  graphics.setFont(font);
  xTaskCreatePinnedToCore(compositeCore, "c", 1024, NULL, 1, NULL, 0);

  //initialize audio output
  audioOutput.init(audioSystem);

  //initialize controllers
  controllers.init(LATCH, CLOCK);
  controllers.setController(0, GameControllers::NES, CONTROLLER_DATA_PIN); //first controller

  //Play first sound in loop (music)
  sounds.play(audioSystem, 0, 1, 1, true);
}

void loop()
{
  //fill audio buffer
  audioSystem.calcSamples();
  
  //read controllers
  controllers.poll();
  
  //play sounds on A or B buttons
  if(controllers.pressed(0, GameControllers::A))
    sounds.play(audioSystem, 1);
  if(controllers.pressed(0, GameControllers::B))
    sounds.play(audioSystem, 2);

  graphics.begin();
  //draw some sprites
  int t = (millis() / 16) & 511;
  sprites.draw(graphics, 0, -t / 8, 0);
  sprites.draw(graphics, 1, 320 - t / 2, 80 + t / 8);
  sprites.draw(graphics, 2, 100 + t, 150 - t / 4);
  
  //print some text
  graphics.setCursor(8, 8);
  graphics.print(millis());
  graphics.print("\nOh hi, press A or B for sounds...");
  graphics.end();
}


