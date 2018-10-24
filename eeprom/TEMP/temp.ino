
//defining all numbers to display full in 4x9
uint8_t numbers [10][4][9] = {
  { //0
    {1,1,1,1},
    {1,1,1,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,1,1,1},
    {1,1,1,1}
  },
    { //1
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0}
  },
  { //2
    {1,1,1,1},
    {1,1,1,1},
    {0,0,1,1},
    {0,0,1,1},
    {1,1,1,1},
    {1,1,0,0},
    {1,1,0,0},
    {1,1,1,1},
    {1,1,1,1}
  },
  { // 3
    {1,1,1,1},
    {1,1,1,1},
    {0,0,0,1},
    {0,0,0,1},
    {1,1,1,1},
    {1,1,1,1},
    {0,0,0,1},
    {1,1,1,1},
    {1,1,1,1}
  },
    { // 4
    {1,0,0,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,1,1,1},
    {1,1,1,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1}
  },
  { // 5
    {1,1,1,1},
    {1,1,1,1},
    {1,0,0,0},
    {1,0,0,0},
    {1,1,1,1},
    {1,1,1,1},
    {0,0,0,1},
    {1,1,1,1},
    {1,1,1,1}
  },  { // 6
    {1,1,1,1},
    {1,1,1,1},
    {1,1,0,0},
    {1,1,0,0},
    {1,1,1,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,1,1,1}
  },  { // 7
    {1,1,1,1},
    {1,1,1,1},
    {0,0,1,1},
    {0,0,1,1},
    {1,1,1,1},
    {0,0,1,1},
    {0,0,1,1},
    {0,0,1,1},
    {0,0,1,1}
  },  { // 8
    {1,1,1,1},
    {1,1,1,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,1,1,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,1,1,1},
    {1,1,1,1},
  },
  { // 9
    {1,1,1,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,1,1,1},
    {1,1,1,1},
    {0,0,1,1},
    {0,0,1,1},
    {0,0,1,1},
    {0,0,1,1}
  }
};

// 4x9 Display based on the MAPPING.xlsx
int display_1 [36] = {
  LED_0,LED_1, LED_2, LED_3,
  LED_17,LED_16,LED_15,LED_14,
  LED_18,LED_19,LED_20,LED_21,
  LED_35,LED_34,LED_33,LED_32,
  LED_36,LED_37,LED_38,LED_39,
  LED_53,LED_52,LED_51,LED_50,
  LED_54,LED_55,LED_56,LED_57,
  LED_71,LED_70,LED_69,LED_68,
  LED_72,LED_73,LED_74,LED_75
  };

//get the matrix and put it on the display.
void showNumber(int i) {
  int[][] matrix = getNumberForDisplay(i);
   turnLEDS_ON(matrix, display_1);
}

//Return the Matrix for each number
int[][] getNumberForDisplay(int i) {
  return numbers[i];
}

//Loop through matrix and turn actually LEDS on.
void turnLEDS_ON(int[][] matrix, int[] display) {
  int index = 0;
  int n = 9;
  int m = 4;
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {

      if(matrix[n][m]== 1) {

        //turn on the LED
        int LED_INDEX = display[index]

        PUT_LED_ON.
        
      }
      index++;
    }
  }
}



