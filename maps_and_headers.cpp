

#include "opencv2/opencv.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "ftd2xx.h"
#include<thread>
#include <chrono>
#include<pthread.h>
#include <unistd.h>


#include "defines_MPZ.h"
#include "measure.h"
#include "process_filters.h"
#include "process_other.h"
#include "file_io.h"
#include "maps_and_headers.h"



using namespace std;
using namespace cv;


// Define a pixel opencv type
typedef Vec<uint16_t, 3> Pixel_Type;

// c++
// typedef std::vector<int> int_vec_t;







void Panel_Mapper(uint16_t *Vid_Grabbed_Local, uint16_t *Vid_Mapped_Local, int *Panel_Map_Local, int Words_Per_Pixel  )
{
    for(int ii=0; ii<Sculpture_Size_Pixels; ii++)
    {
          int iixx = Words_Per_Pixel * ii;
          int Panel_MapxX = Words_Per_Pixel * Panel_Map_Local[ii];
          for(int jj=0;  jj < Words_Per_Pixel; jj++) Vid_Mapped_Local[ Panel_MapxX  + jj]  = Vid_Grabbed_Local[iixx + jj];
    }
}


void Add_Headers(uint16_t *Vid_Mapped_Local, int Enclosure_Info[NUM_OF_ENCLOSURES][NUM_OF_ENCLOSURE_PARAMETERS]  )
{
    // note do we want to clear the buffer first (maybe only once)  ???
   //  for(int ii=0; ii<Buffer_W_Gaps_Size_RGB; ii++)Vid_Mapped_Local[ii] = 0;;

    int Enclosure_Header_Start, Panel1_Header_Location;
    int Offset;

    for(int ii=0; ii<NUM_OF_ENCLOSURES; ii++)
    {
        // ENCLOSURE HEADER AREA
        Enclosure_Header_Start = 4 * (Enclosure_Info[ii][First_Pixel_Location] - 9);  // in pixel lengths
        Vid_Mapped_Local[Enclosure_Header_Start     ] = 0xFFFF;  // sync header 1
        Vid_Mapped_Local[Enclosure_Header_Start + 1 ] = 0xAAAA;  // sync header 2
        Vid_Mapped_Local[Enclosure_Header_Start + 2 ] = ( (uint16_t)Enclosure_Info[ii][Mapped_Enclosure_Address]  ) & 0x001F;  // Mapped Enclosure #  0 - 31

        // Panel HEADER AREA
        Panel1_Header_Location = 4 * (Enclosure_Info[ii][First_Pixel_Location] - 1);
        for(int ix=0; ix < MAX_NUM_OF_PANELS; ix++)
        {
            Offset =  ( 4 * Enclosure_Info[ii][Panel1_Offset + ix] )   ;
            if( (ix == 0) || (Offset > 0) )
            {
                Vid_Mapped_Local[Panel1_Header_Location + Offset + 0]   = 0xFFFF;  // sync header 1
                Vid_Mapped_Local[Panel1_Header_Location + Offset + 1]   = 0x5555;  // sync header 2
                Vid_Mapped_Local[Panel1_Header_Location + Offset + 2]   = ( (uint16_t)ix ) &  0x0007  ;       // 0 - 5
                if(Enclosure_Info[ii][Panel1_LR + ix]  == 1) Vid_Mapped_Local[Panel1_Header_Location + Offset + 2]  |=  0x2000 ;
            }
        }
    }
}







void Add_DMX(uint16_t *Vid_Mapped_Local, int Enclosure_Info[NUM_OF_ENCLOSURES][NUM_OF_ENCLOSURE_PARAMETERS],
                bool Constel_On, bool LP_Upper_On, bool LP_62_On, bool LP_63_On, bool TRX_On,

                uint16_t RedConstellation, uint16_t GreenConstellation,  uint16_t BlueConstellation, uint16_t WhiteConstellation,
                uint8_t Red6465, uint8_t Green6465,  uint8_t Blue6465,
                uint8_t LP_62_Red, uint8_t LP_62_Green,  uint8_t LP_62_Blue,
                uint8_t LP_63_Red, uint8_t LP_63_Green,  uint8_t LP_63_Blue,
                uint8_t TRX_Red, uint8_t TRX_Green,  uint8_t TRX_Blue )
{

    int Enclosure_Header_Start, Panel1_Header_Location;
    int Offset;
    static uint16_t ramp = 0;


    int DMX_LP62_Offset, DMX_LP63_Offset, DMX_TRX_Offset;

    uint16_t Combined1, Combined2, Combined3 ;


    ramp += 100;

    for(int ii=0; ii<NUM_OF_ENCLOSURES; ii++)
    {
        // ENCLOSURE HEADER AREA
        Enclosure_Header_Start = 4 * (Enclosure_Info[ii][First_Pixel_Location] - 9);  // in pixel lengths





        /********* LIGHTS CONTROL FOR DMX INFO SENT TO THE ENCLOSURES  Lumenpulse and Constellation  Maint_Walkway_2  Maint_Walkway_3       ********************/

        // note 62 and 63 have their own dedicated cables  while 64 and 65 come out of the enclosure chassis

        // LumenPulse Bytes     Word 4-{Green , Red}    Word 5-{Red2 , Blue}   Word 6-{Blue2 , Green2}


        Combined1 = Green6465;
        Combined1 <<= 8 ;
        Combined1 =   (Combined1 & 0xff00)  |  (uint16_t) Red6465 ;

        Combined2 = Red6465;
        Combined2 <<= 8 ;
        Combined2 =   (Combined2 & 0xff00)  |  (uint16_t) Blue6465 ;

        Combined3 = Blue6465;
        Combined3 <<= 8 ;
        Combined3 =   (Combined3 & 0xff00)  |  (uint16_t) Green6465 ;

        Vid_Mapped_Local[Enclosure_Header_Start + 4]    =  LP_Upper_On ? Combined1 :  0 ; // 0x0010;   // 1st 2bytes                    Red and Green
        Vid_Mapped_Local[Enclosure_Header_Start + 5]    =  LP_Upper_On ? Combined2 :  0; // 0x0010; //ramp;   // 1st 2bytes
        Vid_Mapped_Local[Enclosure_Header_Start + 6]    =  LP_Upper_On ? Combined3 :  0; // 0x0010; //ramp;   // 1st 2bytes       Blue and next Red


        // constellation DMX start address 1-512 for constellation  typically 1
        Vid_Mapped_Local[Enclosure_Header_Start + 3 ] = 0x0001;   // DMX START ADDRESS 1-512 for constellation

        // constellation Bytes  word 22-RED   23-GREEN  24-BLUE   25-WHITE
        Vid_Mapped_Local[Enclosure_Header_Start + 22 ] = Constel_On ?     RedConstellation          :  0 ;   // 44th byte
        Vid_Mapped_Local[Enclosure_Header_Start + 23 ] = Constel_On ?     GreenConstellation   :  0  ;     // 44th byte
        Vid_Mapped_Local[Enclosure_Header_Start + 24 ] = Constel_On ?     BlueConstellation   :  0  ;      // 44th byte
        Vid_Mapped_Local[Enclosure_Header_Start + 25 ] = Constel_On ?     WhiteConstellation     :  0  ;      // 44th byte
    //    Vid_Mapped_Local[Enclosure_Header_Start + 23 ] =  (ramp<<8)  & 0xff00 ;
   //     Vid_Mapped_Local[Enclosure_Header_Start + 23 ]  |=  (ramp<<1)  & 0x00fe ;

        /********* LIGHTS CONTROL FOR DMX INFO SENT TO THE ENCLOSURES  Lumenpulse and Constellation  Maint_Walkway_2  Maint_Walkway_3  DONE   *******************/

    }



    /***** Footer for Lumenpulse level 62 and lumenpulse level 63  and Traxon   single color for all for now     **/
    /********* LIGHTS CONTROL FOR DMX DIRECT Lumenpulse 62 and 63 and Traxon    ********************/

    // word address offset to USB stream   note 62 and 63 have their own dedicated cables  while 64 and 65 come out of the enclosure chassis
    DMX_LP62_Offset = 52464;
    DMX_LP63_Offset = 52467;
    DMX_TRX_Offset  = 52470;

    Vid_Mapped_Local[DMX_LP62_Offset]   = LP_62_On ? (uint16_t)LP_62_Red  : 0 ;       // LP_Red
    Vid_Mapped_Local[DMX_LP62_Offset+1] = LP_62_On ? (uint16_t)LP_62_Green  : 0 ;
    Vid_Mapped_Local[DMX_LP62_Offset+2] = LP_62_On ? (uint16_t)LP_62_Blue : 0 ;

    Vid_Mapped_Local[DMX_LP63_Offset]   = LP_63_On ? (uint16_t)LP_63_Red  : 0 ;       // LP_Red
    Vid_Mapped_Local[DMX_LP63_Offset+1] = LP_63_On ? (uint16_t)LP_63_Green : 0  ;
    Vid_Mapped_Local[DMX_LP63_Offset+2] = LP_63_On ? (uint16_t)LP_63_Blue : 0 ;

    Vid_Mapped_Local[DMX_TRX_Offset]   = TRX_On ? (uint16_t)TRX_Red  : 0 ;       // LP_Red
    Vid_Mapped_Local[DMX_TRX_Offset+1] = TRX_On ? (uint16_t)TRX_Green  : 0 ;
    Vid_Mapped_Local[DMX_TRX_Offset+2] = TRX_On ? (uint16_t)TRX_Blue : 0;

    /********* LIGHTS CONTROL FOR DMX DIRECT Lumenpulse 62 and 63 and Traxon   DONE ********************/



}






