//#include "go.h"

#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include <WiFi.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
//#include "webpages.h"
#include "Teensy64.h"

extern "C" {
  #include "emuapi.h"
  #include "iopins.h"
}

#include "esp_event.h"

#include "keyboard_osd.h"

#ifdef HAS_SND
#include "AudioPlaySystem.h"
#endif

#include "c64.h"
//#include "cpu.h"

//#include "ili9341_t3dma.h"
//ILI9341_t3DMA tft = ILI9341_t3DMA(PIN_NUM_CS, PIN_NUM_DC, -1, PIN_NUM_MOSI, PIN_NUM_CLK, PIN_NUM_MISO, TPIN_NUM_CS, TPIN_NUM_IRQ);

//#include "display.h"
//Display
Display tft = Display();
//#include "cpu.h"


#ifdef HAS_SND
AudioPlaySystem audio;
#endif

//static void spi_task(void *args)
//{
//  while(true) {
////    tft.refresh();
//  } 
//}
//
static void input_task(void *args)
{
  while(true) {
//    if ((emu_ReadKeys() & (MASK_KEY_USER1+MASK_KEY_USER2)) == (MASK_KEY_USER1+MASK_KEY_USER2)) {  
//      printf("rebooting\n");
//      esp_restart();    
//    }
//
//    uint16_t bClick = emu_DebounceLocalKeys();
//    if (bClick & MASK_KEY_USER2) { 
//      printf("%d\n",emu_SwapJoysticks(1)); 
//      emu_SwapJoysticks(0);
//    }
//    else {
//      emu_Input(bClick);
//    }
#ifdef HAS_SND      
    audio.step();
#endif  
    vTaskDelay(1);
  } 
}

//static void main_step() {
////  if (menuActive()) {
////    uint16_t bClick = emu_DebounceLocalKeys();
////    int action = handleMenu(bClick);
////    char * filename = menuSelection();
////    if (action == ACTION_RUNTFT) {
////#ifdef HAS_SND      
////      audio.begin();
////      audio.start();
////#endif                 
////      toggleMenu(false); 
////      tft.fillScreenNoDma( RGBVAL16(0x00,0x00,0x00) );
////      xTaskCreatePinnedToCore(input_task, "inputthread", 4096, NULL, 2, NULL, 0);
////      emu_Init(filename);        
////    }       
////    //vTaskDelay(20 / portTICK_PERIOD_MS); 
////  }
////  else {         
//    emu_Step();     
////  }
//}


void setupOTA() {
    ArduinoOTA
    .onStart([]() {
    Serial.println("OTA START");
//    server->end();
    audio.stop();
    Serial.println("AUDIO STOPPED");
    tft.end();
    Serial.println("DISPLAY END");
//    free(cpu.RAM);
//    Serial.println("RAM FREE");

    
    
//    logFreeHeap();

         
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
  ArduinoOTA.setHostname("ESP32-C64_SIMULATOR");
  ArduinoOTA.begin();
}

void setupWiFi_by_manager() {
    WiFiManager manager;    
     
    bool success = manager.autoConnect("ESP32_AP","password");
 
    if(!success) {
        Serial.println("Failed to connect");
    } 
    else {
        Serial.println("Connected");
        Serial.println("\n\nNetwork Configuration:");
        Serial.println("----------------------");
        Serial.print("         SSID: "); Serial.println(WiFi.SSID());
        Serial.print("  Wifi Status: "); Serial.println(WiFi.status());
        Serial.print("Wifi Strength: "); Serial.print(WiFi.RSSI()); Serial.println(" dBm");
        Serial.print("          MAC: "); Serial.println(WiFi.macAddress());
        Serial.print("           IP: "); Serial.println(WiFi.localIP());
        Serial.print("       Subnet: "); Serial.println(WiFi.subnetMask());
        Serial.print("      Gateway: "); Serial.println(WiFi.gatewayIP());
        Serial.print("        DNS 1: "); Serial.println(WiFi.dnsIP(0));
        Serial.print("        DNS 2: "); Serial.println(WiFi.dnsIP(1));
        Serial.print("        DNS 3: "); Serial.println(WiFi.dnsIP(2));
        Serial.println();

    }
//    free(&manager);
}














void setup(void)
{
  Serial.begin(115200);
  setupWiFi_by_manager();
  setupOTA();
//  printf("Starting emulator\n");
//  
  tft.begin();
//  tft.flipscreen(true);  
//  tft.start();
//  tft.refresh();
// 
//  emu_init(); 
  #ifdef HAS_SND      
      audio.begin();
      audio.start();
  #endif  

 c64_Init();
 xTaskCreatePinnedToCore(input_task, "inputthread", 4096, NULL, 2, NULL, 0);
//  xTaskCreatePinnedToCore(spi_task, "spithread", 4096, NULL, 1, NULL, 0);
  //vTaskPrioritySet(NULL, tskIDLE_PRIORITY+1);     
}

void loop(void)
{
  ArduinoOTA.handle();
  unsigned long t = esp_timer_get_time();
  c64_Step();
//  
//  main_step();
//  printf("%d\n",(int)((esp_timer_get_time()-t)/1000));  
} 
