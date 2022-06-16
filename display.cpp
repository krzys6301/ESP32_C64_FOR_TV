//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"

#include "esp_system.h"
#include <Arduino.h>
#include "display.h"
#include "CompositeColorOutput.h"
#include "esp_pm.h"
#define USE_ATARI_COLORS
CompositeColorOutput composite(CompositeColorOutput::NTSC);

unsigned char **gb_buffer_vga;

Display::Display(void)//uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi, uint8_t clk, uint8_t miso,  uint8_t touch_cs,  uint8_t touch_irq)
{
//  _cs   = cs;
//  _dc   = dc;
//  _rst  = rst;
//  _mosi = mosi;
//  _clk  = clk;
//  _miso = miso;
//  _touch_irq = touch_irq;
//  _touch_cs = touch_cs;
}

esp_pm_lock_handle_t powerManagementLock;
void Display::begin(void)
{
  //highest clockspeed needed
  
  esp_pm_lock_create(ESP_PM_CPU_FREQ_MAX, 0, "compositeCorePerformanceLock", &powerManagementLock);
  esp_pm_lock_acquire(powerManagementLock);
  
  //initializing DMA buffers and I2S


  int xres=320;
  int yres=240;
// logFreeHeap();
 gb_buffer_vga = (unsigned char**)malloc(yres * sizeof(unsigned char*));
//    backbuffer = (char**)malloc(yres * sizeof(char*));
    //not enough memory for z-buffer implementation
    //zbuffer = (char**)malloc(yres * sizeof(char*));
//    gb_buffer_vga[0] = (unsigned char*)malloc(xres);
    for(int y = 0; y < yres; y++)
    {
//      gb_buffer_vga[y] = gb_buffer_vga[0];
      gb_buffer_vga[y] = (unsigned char*)malloc(xres);
    }
 char** gbuffer= (char**)gb_buffer_vga;
 composite.sendFrameHalfResolution( &gbuffer);
 composite.init();
}

//uint16_t * ILI9341_t3DMA::getLineBuffer(int j)
uint16_t * getLineBuffer(int j)
{
//  uint16_t * block=blocks[j>>6];  
//  return(&block[(j&0x3F)*ILI9341_TFTREALWIDTH]);
  return (uint16_t*) gb_buffer_vga[j];
}
