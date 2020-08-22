
#ifndef MAPS_AND_HEADERS_H
#define MAPS_AND_HEADERS_H
#pragma once




/*
void Panel_Mapper(uint16_t *Vid_Grabbed_Local, uint16_t *Vid_Mapped_Local, int *Panel_Map_Local  );
void Panel_Mapper_W_Gaps(uint16_t *Vid_Grabbed_Local, uint16_t *Vid_Mapped_Local, int *Panel_Map_Local  );
void Add_Headers (uint16_t *Vid_Mapped_Local, int Enclosure_Info[NUM_OF_ENCLOSURES][NUM_OF_ENCLOSURE_PARAMETERS] );
void Add_HeadersX (uint16_t *Vid_Mapped_Local, int Enclosure_Info[NUM_OF_ENCLOSURES][NUM_OF_ENCLOSURE_PARAMETERS] );

void Panel_Mapper_RGBW(uint16_t *Vid_Grabbed_Local, uint16_t *Vid_Mapped_Local, int *Panel_Map_Local  );
void Panel_Mapper_W_Gaps_RGBW(uint16_t *Vid_Grabbed_Local, uint16_t *Vid_Mapped_Local, int *Panel_Map_Local  );
void Add_Headers_RGBW(uint16_t *Vid_Mapped_Local, int Enclosure_Info[NUM_OF_ENCLOSURES][NUM_OF_ENCLOSURE_PARAMETERS] );
void Add_Headers_RGBWX(uint16_t *Vid_Mapped_Local, int Enclosure_Info[NUM_OF_ENCLOSURES][NUM_OF_ENCLOSURE_PARAMETERS] );
*/

void Panel_Mapper(uint16_t *Vid_Grabbed_Local, uint16_t *Vid_Mapped_Local, int *Panel_Map_Local, int Words_Per_Pixel  );



void Add_Headers(uint16_t *Vid_Mapped_Local, int Enclosure_Info[NUM_OF_ENCLOSURES][NUM_OF_ENCLOSURE_PARAMETERS]  );


void Add_DMX(uint16_t *Vid_Mapped_Local, int Enclosure_Info[NUM_OF_ENCLOSURES][NUM_OF_ENCLOSURE_PARAMETERS],
                bool Constel_On, bool LP_Upper_On, bool LP_62_On, bool LP_63_On, bool TRX_On,

                uint16_t RedConstellation, uint16_t GreenConstellation,  uint16_t BlueConstellation, uint16_t WhiteConstellation,
                uint8_t Red6465, uint8_t Green6465,  uint8_t Blue6465,
                uint8_t LP_62_Red, uint8_t LP_62_Green,  uint8_t LP_62_Blue,
                uint8_t LP_63_Red, uint8_t LP_63_Green,  uint8_t LP_63_Blue,
                uint8_t TRX_Red, uint8_t TRX_Green,  uint8_t TRX_Blue );



#endif /* MAPS_AND_HEADERS */
