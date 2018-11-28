#ifndef Display_h
#define Display_h


//Number height and width of pixels
#define N_HEIGHT 9
#define N_WIDTH 4


class Display {
  
  public:
    Display();
    void displayTemperature(uint8_t temperature);
    struct NUMBER_IN_DISPLAY NUMBER_load_progmem(uint8_t number);
  private:
    void displayNumberAtPosition(uint8_t number, uint8_t position);
    static uint8_t displayMapping[9][9];
    static uint8_t numberMapping[10][N_HEIGHT][N_WIDTH];
    static uint8_t numberMappingRows();
    static uint8_t numberMappingCols();

  
};



struct NUMBER_IN_DISPLAY {
    int matrix[N_HEIGHT][N_WIDTH];
  };



#endif
