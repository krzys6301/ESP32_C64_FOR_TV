class Display
{
  public:
    Display(void);

    //void setFrameBuffer(uint16_t * fb);
    //static uint16_t * getFrameBuffer(void);

    void begin(void);
    void end(void);
//    void flipscreen(bool flip);
//    bool isflipped(void);
//    void start(void);
//    void refresh(void);
//    void refreshPrepare(void);
//    void refreshFinish(void);
    //void stop();
    //void wait(void);  
  uint8_t * getLineBuffer(int j);

//    void fillScreen(uint16_t color);
//    void writeScreen(int width, int height, int stride, uint8_t *buffer, uint16_t *palette16);
//    void writeLine(int width, int height, int stride, uint8_t *buffer, uint16_t *palette16);
//    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
//    void drawSprite(int16_t x, int16_t y, const uint16_t *bitmap);
//    void drawSprite(int16_t x, int16_t y, const uint16_t *bitmap, uint16_t croparx, uint16_t cropary, uint16_t croparw, uint16_t croparh);
//    void drawText(int16_t x, int16_t y, const char * text, uint16_t fgcolor, uint16_t bgcolor, bool doublesize);
//
//    void fillScreenNoDma(uint16_t color) { fillScreen(color); };
//    void drawSpriteNoDma(int16_t x, int16_t y, const uint16_t *bitmap) { drawSprite(x,y,bitmap); };
//    void drawSpriteNoDma(int16_t x, int16_t y, const uint16_t *bitmap, uint16_t croparx, uint16_t cropary, uint16_t croparw, uint16_t croparh) { drawSprite(x,y,bitmap,croparx,cropary,croparw,croparh); };
//    void drawRectNoDma(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) { drawRect(x,y,w,h,color); };
//    void drawTextNoDma(int16_t x, int16_t y, const char * text, uint16_t fgcolor, uint16_t bgcolor, bool doublesize) { drawText(x,y,text,fgcolor,bgcolor,doublesize); };

    /*
    void writeScreenNoDma(const uint16_t *pcolors);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    uint16_t getPixel(int16_t x, int16_t y);
    */
        
     
//    void touchBegin();
//    void readRo(uint16_t * oX, uint16_t * oY, uint16_t * oZ);
//    bool isTouching();
//    void readRaw(uint16_t * oX, uint16_t * oY, uint16_t * oZ);
//    void readCal(uint16_t * oX, uint16_t * oY, uint16_t * oZ);
//    void callibrateTouch(uint16_t xMin,uint16_t yMin,uint16_t xMax,uint16_t yMax);
//    
//  protected:
//    bool flipped=false;
//    void enableTouchIrq();   
};
