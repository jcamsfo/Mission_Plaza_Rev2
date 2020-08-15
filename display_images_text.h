
#ifndef DISPLAY_IMAGES_TEXT_H
#define DISPLAY_IMAGES_TEXT_H
#pragma once


using namespace std;
using namespace cv;


typedef Vec<uint16_t, 3> Pixel_Type;




void Display_Mat(char Window_Name[100], cv::Mat src, int x, int y);


void Display_Mat_Select(char Window_Name[100], cv::Mat srcA, cv::Mat srcB,  bool Image_A_Or_B, bool Image_On, int x, int y);




void Add_Visible_Sample_Points_Grid(cv::Mat src);

void Add_Visible_Sample_Locations_From_CSV_Map(cv::Mat src, int SP[SAMPLE_ROWS][SAMPLE_COLS]);

void Add_Visible_Sample_Locations_From_CSV_Map_2(cv::Mat src, int SP[SAMPLE_ROWS][SAMPLE_COLS], int SPNUM[SAMPLE_ROWS], int Display_Type );

void Add_Visible_Sample_Locations_From_CSV_Map_rev3(cv::Mat src, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS], int Display_Type, int Vertical_Spacing);


void Create_Quarter_Scale_Rect_Image_For_Display(cv::Mat src, cv::Mat src2 );


void Create_Half_Scale_Rect_Image_For_Display(cv::Mat src, cv::Mat src2 );







void Display_Text_Mat(char Window_Name[100], cv::Mat text_window, int x, int y,

bool Pause_A, float Gain_A, float C_Gain_A , float Black_Level_A, float G_Gain_A, bool F_1p1_A, bool F_2p2_A, bool F_3p3_A, bool F_4p4_A, bool Ones2x2_A, bool Ones3x3_A, bool Ones4x4_A, bool Ones5x5_A, bool Ones6x6_A, bool Ones7x7_A,
bool Pause_B, float Gain_B, float C_Gain_B , float Black_Level_B, float G_Gain_B, bool F_1p1_B, bool F_2p2_B, bool F_3p3_B, bool F_4p4_B, bool Ones2x2_B, bool Ones3x3_B, bool Ones4x4_B, bool Ones5x5_B, bool Ones6x6_B, bool Ones7x7_B,
char CurFile[200] , float Downstream_Gain , bool Video_On, float Fader_Coef, float Fade_Rate, int Current_A_Or_B,  bool A_or_B_File_Ext,
int Test_Mode, bool Sequence_On, int Current_Sequencer_Selection , int Sequence_Duration_Counter , float  Duartion_Mod,
bool Constellation_On, bool Lumen_Pulse_Upper_On, bool LP_62_On, bool LP_63_On, bool TRX_On, uint8_t Red_62_Combined, uint8_t Green_62_Combined, uint8_t Blue_62_Combined,
int64_t Current_Hour, int64_t Current_Minute, int64_t Current_Second,  int64_t Hours_On, int64_t Mins_On,  int64_t Hours_Off, int64_t Mins_Off, float max_Within_Loop_Timer, uint16_t Light_Sensor, bool White_On, bool LED_Correction_On);



void Display_and_Fill_Upper_LP(char Window_Name[100], cv::Mat upper_LP_window, int x, int y, int Red, int Green, int Blue );

#endif /* DISPLAY_IMAGES_TEXT_H */
