//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"

#include "esp_system.h"
#include <Arduino.h>
#include "display.h"
#include "CompositeColorOutput.h"
#include "esp_pm.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/rtc.h"

#include "soc/syscon_reg.h"
#include "soc/rtc_cntl_struct.h"
#define USE_ATARI_COLORS
CompositeColorOutput composite(CompositeColorOutput::NTSC);

unsigned char **gb_buffer_vga;

Display::Display(void){}

esp_pm_lock_handle_t powerManagementLock;
int xres=340;
int yres=240;
int lineNum=0;
//uint16_t* line = (uint16_t*)heap_caps_malloc(320*1*sizeof(uint16_t), MALLOC_CAP_DMA);
//uint16_t* line = (uint16_t*)malloc(340*sizeof(uint16_t));
//blocks[j]= (uint16_t*)heap_caps_malloc(ILI9341_TFTREALWIDTH*lines_per_block*sizeof(uint16_t), MALLOC_CAP_DMA);
// this line is exposed to the VIC afer each line "end of line" eol() is called from the vic -> this last line it is redrawn into fomat for the video_out
uint8_t* line = (uint8_t*)(unsigned char*)malloc(xres);
void Display::begin(void)
{
  //highest clockspeed needed
  
  esp_pm_lock_create(ESP_PM_CPU_FREQ_MAX, 0, "compositeCorePerformanceLock", &powerManagementLock);
  esp_pm_lock_acquire(powerManagementLock);
  
  //initializing DMA buffers and I2S


  int xres=340;
//  int xres=336; // max of the composite out
  int yres=240;
//// logFreeHeap();
 gb_buffer_vga = (unsigned char**)malloc(yres* sizeof(uint8_t*));
//    backbuffer = (char**)malloc(yres * sizeof(char*));
    //not enough memory for z-buffer implementation
    //zbuffer = (char**)malloc(yres * sizeof(char*));
//    gb_buffer_vga[0] = (unsigned char*)malloc(xres);
    for(int y = 0; y < yres; y++)
//      for(int y = 0; y < yres; y=y+4)
    {
//      gb_buffer_vga[y] = gb_buffer_vga[0];
      gb_buffer_vga[y] = (unsigned char*)malloc(xres/2);
//      gb_buffer_vga[y] = (unsigned char*)line;
//      gb_buffer_vga[y+1] =  gb_buffer_vga[y];
//      gb_buffer_vga[y+2] = gb_buffer_vga[y];
//      gb_buffer_vga[y+3] = gb_buffer_vga[y];
    }
 char** gbuffer= (char**)gb_buffer_vga;
 composite.sendFrameHalfResolution( &gbuffer);
 composite.init();
}

//uint16_t * ILI9341_t3DMA::getLineBuffer(int j)
uint8_t * Display::getLineBuffer(int j)
{
  lineNum=j;
//  uint16_t * block=blocks[j>>6];  
//  return(&block[(j&0x3F)*ILI9341_TFTREALWIDTH]);
//  return (uint16_t*) line;
  return line;//(uint8_t*) gb_buffer_vga[j];
}

void Display::eol() {
//  uint8_t** lines = (uint8_t**) *&gb_buffer_vga;
//  uint8_t* l = (uint8_t*) lines[lineNum];

  uint8_t* l = (uint8_t*) gb_buffer_vga[lineNum];
  // rewrite the "full line (aka 8bit per pixel)" into 4-BIT color format;
  uint8_t* p = line;
  uint8_t* e = p + xres;
  while (p<e) {
//      Serial.println(l[n]);

//    l[n] = 6;//l[n];  
      *l++ = *p++|*p++<<4;
  }
}

void Display::end(void)
{
     esp_intr_disable(RawCompositeVideoBlitter::_isr_handle);
       // reset conf
       int ch=1;
    I2S0.conf.val = 1;
    I2S0.conf.val = 0;
    I2S0.conf.tx_right_first = 1;
    I2S0.conf.tx_mono = (ch == 2 ? 0 : 1);

    I2S0.conf2.lcd_en = 1;
    I2S0.fifo_conf.tx_fifo_mod_force_en = 1;
    I2S0.sample_rate_conf.tx_bits_mod = 16;
    I2S0.conf_chan.tx_chan_mod = (ch == 2 ? 0 : 1);
    rtc_clk_apll_enable(0,0,0,0,0);
//    rtc_clk_apll_disable();
//    RTCCNTL.ana_conf.plla_force_pd = 0;    
         esp_pm_lock_release(powerManagementLock);
         periph_module_disable(PERIPH_I2S0_MODULE);
         dac_output_disable(DAC_CHANNEL_1);    


  int yres=240;

// gb_buffer_vga = (unsigned char**)malloc(yres * sizeof(unsigned char*));
//    backbuffer = (char**)malloc(yres * sizeof(char*));
    //not enough memory for z-buffer implementation
    //zbuffer = (char**)malloc(yres * sizeof(char*));
    for(int y = 0; y < yres; y++)
    {
      Serial.printf("free %d",y);
      free(gb_buffer_vga[y]);// = (unsigned char*)malloc(xres);
//      backbuffer[y] = (char*)malloc(xres);
      //zbuffer[y] = (char*)malloc(xres);
    }
    free(gb_buffer_vga);
}
