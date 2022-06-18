class Display
{
  public:
    Display(void);
    void begin(void);
    void end(void);
    void eol();
    uint8_t * getLineBuffer(int j);
};
