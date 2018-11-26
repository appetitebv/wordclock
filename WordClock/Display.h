#ifndef Display_h
#define Display_h
class Display {
  public:
    Display();
    void displayTemperature(uint8_t temperature);
    uint8_t NUMBER_load_reference(unsigned char number);
  private:
    void displayNumberAtPosition(uint8_t number, uint8_t position);
    static uint8_t displayMapping[9][9];
    static uint8_t numberMapping[10][9][4];
    static uint8_t numberMappingRows();
    static uint8_t numberMappingCols();
};
#endif
