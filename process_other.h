#ifndef PROCESS_OTHER_H
#define PROCESS_OTHER_H
#pragma once



using namespace std;
using namespace cv;


typedef Vec<uint16_t, 3> Pixel_Type;


void   Fader_Main_Orig(int &Current_A_Or_B,  float &Fader_Coef, float &Fade_Direction,  float &Fade_Rate );
// void   Fader_Main(int &Current_A_Or_B,  float &Fader_Coef, float &Fade_Direction,  float &Fade_Rate );
void   Fader_Main(int &Current_A_Or_B,  float &Fader_Coef_A, float &Fader_Coef_B, float &Fade_Direction,  float &Fade_Rate );
// void Fader_Main_Rev2(int &Current_A_Or_B,  float &Fader_Coef_A, float &Fader_Coef_B,  float &Fade_Direction,  float &Fade_Rate, float &Fader_End_Value_Percent )
void   Fader_Main_Rev2(int &Current_A_Or_B,  float &Fader_Coef_A, float &Fader_Coef_B,  float &Fade_Direction,  float &Fade_Rate, float &Mix_Key_Type_A, float &Mix_Key_Type_B, float &Opacity_A, float &Opacity_B, float &Sign_Fader_A, float &Sign_Fader_B );

void   Fader_Main_Rev3( int &A_Fader_State, int &B_Fader_State, float &Fader_Coef_A, float &Fader_Coef_B, float &Fade_Rate,  float &Opacity_A, float &Opacity_B );


void Save_Samples_From_Grid_To_Buffer(cv::Mat src, cv::Mat src2 );

void Save_Samples_From_CSV_Map_To_Buffer(cv::Mat src, uint16_t *buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS] );
void Save_Samples_From_CSV_Map_To_Buffer_RGBW(cv::Mat src, uint16_t *buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS] );
void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert(cv::Mat src, uint16_t *buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS] );

void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_rev2(cv::Mat src, uint16_t *Buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS],int  Vertical_Spacing);

void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_rev3(cv::Mat src, uint16_t *Buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS],int  Vertical_Spacing,
                                                            bool White_On, bool LED_Correction_On, float Red_Correct, float Green_Correct, float Blue_Correct, float White_Correct );



void Save_Samples_From_CSV_Map_To_Vec_Buffer(cv::Mat src, Pixel_Type *Buf_Vec, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS]) ;

void Simple_Test(cv::Mat src);

void Simple_Test2(cv::Mat src);

void Simple_Test3(cv::Mat src);


// tested a bunch

void Color_Levels_1(cv::Mat src, float Chroma_Level);
void Color_Levels_2_F(cv::Mat src);
void Color_Levels_1B(cv::Mat src, cv::Mat src_Mul, float Chroma_Level, float Luma_Level );
void Color_Levels_1B_U(cv::UMat src, cv::UMat src_Mul, float Chroma_Level, float Luma_Level );

void RGB_Gamma_Correction(cv::UMat src, float Gamma_Correction );

/*
void Process_Image(cv::Mat& Vid_In, cv::Mat& Vid_In_F, bool Filter_On_Off,
                    bool On1p1, bool On2p2, bool On3p3, bool On4p4, bool On3tap,
                    bool On2x2, bool On3x3, bool On4x4, bool On5x5, bool On6x6, bool On7x7, bool OnOnes,
                    float gain, float black_level );


void Process_Image_UB(cv::UMat& Vid_In, cv::UMat& Vid_In_F, cv::UMat& YUV_Gain,  bool Filter_On_Off,
                    bool On1p1, bool On2p2, bool On3p3, bool On4p4, bool On3tap,
                    bool On2x2, bool On3x3, bool On4x4, bool On5x5, bool On6x6, bool On7x7, bool OnOnes,
                    float gain, float black_level, float Chroma_Level );
*/

void Process_Image_U(cv::UMat& Vid_In, cv::UMat& Vid_In_F, cv::UMat& YUV_Gain,  bool Filter_On_Off,
                    bool On1p1, bool On2p2, bool On3p3, bool On4p4, bool On3tap,
                    bool On2x2, bool On3x3, bool On4x4, bool On5x5, bool On6x6, bool On7x7, bool OnOnes,
                    float gain, float black_level, float Chroma_Level, float Gamma_Level );






void Change_Seam(cv::Mat& Vid_In, bool Normal);

void Shift_Image_Horizontal(cv::Mat& Vid_In, int Initial_Location );



void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_U(cv::UMat src, uint16_t *buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS] );


#endif /* PROCESS_OTHER_H */
