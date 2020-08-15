#ifndef DEFINES_H
#define DEFINES_H
#pragma once




// should these be defines or constants ????
// **************************   Sculpture Size Definitions   ******************************/

constexpr int Sculpture_Size_Pixels = 11136;         //  4 * 16 bit words per pixel

constexpr int Sculpture_Size_RGBW = 44544;              //  16 bit   4 * 11136
constexpr int Sculpture_Size_RGBW_Bytes = 89088;        //  16 bit   8 * 11136

constexpr int Buffer_W_Gaps_Size_RGBW = 52464;          //  16 bit   4 * 13116
constexpr int Buffer_W_Gaps_Size_RGBW_Bytes = 104928;   //  16 bit   8 * 13116

constexpr int Buffer_W_Gaps_Size_RGBW_Extra = 55000;        //  16 bit   4 * 13116 with extra
constexpr int Buffer_W_Gaps_Size_RGBW_Bytes_Extra = 110000; //  16 bit   8 * 13116 with extra


// Not Used currently   FOR RGB Sculpture vs RGBW sculpture
constexpr int Sculpture_Size_RGB = 33408;               //  16 bit   3 * 11136
constexpr int Sculpture_Size_RGB_Bytes = 66816;         //  16 bit   6 * 11136

constexpr int Buffer_W_Gaps_Size_RGB = 39348;           //  16 bit   3 * 13116
constexpr int Buffer_W_Gaps_Size_RGB_Bytes = 78696;     //  16 bit   6 * 13116

// **************************   Sculpture Size Definitions Done  ************************/




// *******************   FTDI USB Size Definitions   ***********************/
// 1K multiples
// 12288 "pixels"   49152 words  98304 bytes
constexpr int TxBuffer_Size_Words = 49152;   //  FTDI needs multiple of 1024
constexpr int TxBuffer_Size_Bytes = 98304;  //  FTDI needs multiple of 1024
constexpr int RxBuffer_Size = 1024;
// *******************   FTDI USB Size Definitions  DONE  ***************/




// For Info Array Declarations
constexpr int Max_Num_Of_Image_Files = 100;
constexpr int Max_File_Name_Length = 25;
constexpr int Max_Num_Of_Image_Parameters = 50;




enum Corrections {Gain_X = 0, Black_Level_X = 1, Chroma_X = 2, Red_Val = 3, Green_Val = 4, Blue_Val = 5,  Duration = 6  };



enum Colors62 {Red_Val_62 = 7,  Green_Val_62 = 8,  Blue_Val_62 = 9, RGB_Gain_62 = 10 };
enum Colors63 {Red_Val_63 = 11, Green_Val_63 = 12, Blue_Val_63 = 13, RGB_Gain_63 = 14 };
enum Colors64 {Red_Val_64 = 15, Green_Val_64 = 16, Blue_Val_64 = 17, RGB_Gain_64 = 18 };
enum Colors65 {Red_Val_65 = 19, Green_Val_65 = 20, Blue_Val_65 = 21, RGB_Gain_65 = 22 };

enum ColorsFilament {Red_Val_Filament = 13, Green_Filament = 24, Blue_Filament = 25, RGB_Gain_Filament = 26 };
enum ColorsConstellation {Red_Val_Constellation = 27, Green_Val_Constellation = 28, Blue_Val_Constellation = 29, White_Val_Constellation = 30, RGBW_Gain_Constellation = 31 };





// Dissolve defines
constexpr float ToA = 1.0;
constexpr float ToB = -1.0;


constexpr int Image_Fading = 0;
constexpr int Image_A = 1;
constexpr int Image_B = 2;





constexpr int IMAGE_ROWS = 280;
constexpr int IMAGE_COLS = 1024;


constexpr int SAMPLE_ROWS = 67;
constexpr int SAMPLE_COLS = 256;            // not every row is 256 long
constexpr int SAMPLE_LINE_BUFFSIZE = 3000;  // text row length
constexpr int SAMPLE_MAX_LINE_SIZE = 500;   // elements per row


constexpr int PANEL_MAP_SIZE    = 11136; // full building
constexpr int PM_LINE_BUFF_SIZE = 3000;  // text row length
constexpr int PM_MAX_LINE_SIZE  = 2000;  // max elements per row


constexpr int PANEL_MAP_W_GAPS_SIZE     = 11136; // full building
constexpr int PM_W_GAP_LINE_BUFF_SIZE   = 3000;  // text row length
constexpr int PM_W_GAP_MAX_LINE_SIZE    = 2000;   // max elements per row

constexpr int NUM_OF_ENCLOSURES = 32;
constexpr int NUM_OF_ENCLOSURE_PARAMETERS = 17;
constexpr int MAX_NUM_OF_PANELS = 6;


constexpr int RGB  = 3;
constexpr int RGBW = 4;


constexpr int Mapped_Enclosure_Address  = 1;
constexpr int First_Pixel_Location = 2;
constexpr int Panel1_Offset  = 3;
constexpr int Green_DAC = 9;
constexpr int Panel1_LR  = 10;







#endif /* DEFINES_H */
