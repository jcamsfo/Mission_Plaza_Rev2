

#include "opencv2/opencv.hpp"
#include "defines_MPZ.h"


#include "display_images_text.h"


using namespace std;
using namespace cv;




void Display_Mat(char Window_Name[100], cv::Mat src, int x, int y)
{
    namedWindow(Window_Name);
    moveWindow(Window_Name, x,y);
    imshow( Window_Name, src );
}



void Display_Mat_Select(char Window_Name[100], cv::Mat srcA, cv::Mat srcB,  bool Image_A_Or_B, bool Image_On, int x, int y)
{
    if(Image_A_Or_B && Image_On)
    {
        namedWindow(Window_Name);
        moveWindow(Window_Name, x,y);
        imshow( Window_Name, srcA );
    }
    else if(Image_On)
    {
        namedWindow(Window_Name);
        moveWindow(Window_Name, x,y);
        imshow( Window_Name, srcB );
    }
    else destroyWindow(Window_Name);
 }




/*
// adds visible sample points to a matrix every 4 pixels
// always expects an 8 CV_8UC3
void Add_Visible_Sample_Points_Grid(cv::Mat src)
{
    int rowsY = src.rows;
    int colsX = src.cols;
    for(int yy=2; (yy < rowsY); yy += 4)
        for(int xx=2; (xx < colsX); xx += 4)
            src.at<Vec3b>(Point(xx,yy)) =  {0,0,0}; // { val[0]+64, val[1]+64, val[2]+64 };
}


// adds visible sample points to a matrix based on csv file read  NON-RECTANGULAR points
// always expects an 8 CV_8UC3
void Add_Visible_Sample_Locations_From_CSV_Map(cv::Mat src, int SP[SAMPLE_ROWS][SAMPLE_COLS])
{
    int xx_arr, yy_arr;
    int rowsY = src.rows;
    int colsX = src.cols;
    for(int yy=2; (yy < rowsY); yy += 4)
    {
        xx_arr = 0;
        yy_arr = (yy-2)>>2;
        for(int xx=0; (xx < colsX); xx++)
        {
            if(( SP[yy_arr][xx_arr] ==  xx) && (yy_arr < SAMPLE_ROWS) )
            {
                src.at<Vec3b>(Point(xx,yy)) =  {0,0,0};
                xx_arr++;
            }
        }
    }
}
*/


// adds visible sample points to a matrix based on csv file read  NON-RECTANGULAR points
// always expects an 8 CV_8UC3
void Add_Visible_Sample_Locations_From_CSV_Map_2(cv::Mat src, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS], int Display_Type)
{
    int xx_arr, yy_arr;
    int rowsY = src.rows;
    int colsX = src.cols;
    Pixel_Type Pixel_Vec;
    Vec3s vv;

    for(int yy=2; (yy < rowsY); yy += 4)
    {
        xx_arr = 0;
        yy_arr = (yy-2)>>2;
        for(int xx=0; (xx < colsX); xx++)
        {
            if(( SP[yy_arr][xx_arr] ==  xx) && (yy_arr < SAMPLE_ROWS) && (xx_arr < Num_Of_Samples_Per_Row[yy_arr]) )
            {
                Pixel_Vec = src.at<Vec3b>(Point(xx,yy))  ;
                uint8_t R0 = (Pixel_Vec[0] <= 100) ? Pixel_Vec[0] + 100 : 0 ;
                uint8_t G0 = (Pixel_Vec[1] <= 100) ? Pixel_Vec[1] + 100 : 0;
                uint8_t B0 = (Pixel_Vec[2] <= 100) ? Pixel_Vec[2] + 100 : 0 ;

                if(Display_Type == 1 )src.at<Vec3b>(Point(xx,yy)) =  {  0, 0, 0 }  ;
                else src.at<Vec3b>(Point(xx,yy)) =   { R0, G0, B0 } ;
                xx_arr++;
            }
        }
    }
}



// adds visible sample points to a matrix based on csv file read  NON-RECTANGULAR points
// always expects an 8 CV_8UC3
void Add_Visible_Sample_Locations_From_CSV_Map_rev3(cv::Mat src, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS], int Display_Type, int Vertical_Spacing)
{
    int xx_arr, yy_arr;
    int rowsY = src.rows;
    int colsX = src.cols;
    Pixel_Type Pixel_Vec;
    Vec3s vv;

    for(int yy=2; (yy < rowsY); yy += Vertical_Spacing)
    {
        xx_arr = 0;
      //  yy_arr = (yy-2)>>2;
        yy_arr = (yy-2)/ Vertical_Spacing  ;  // >>1;
        for(int xx=0; (xx < colsX); xx++)
        {
            if(( SP[yy_arr][xx_arr] ==  xx) && (yy_arr < SAMPLE_ROWS) && (xx_arr < Num_Of_Samples_Per_Row[yy_arr]) )
            {
                Pixel_Vec = src.at<Vec3b>(Point(xx,yy))  ;
                uint8_t R0 = (Pixel_Vec[0] <= 100) ? Pixel_Vec[0] + 100 : 0 ;
                uint8_t G0 = (Pixel_Vec[1] <= 100) ? Pixel_Vec[1] + 100 : 0;
                uint8_t B0 = (Pixel_Vec[2] <= 100) ? Pixel_Vec[2] + 100 : 0 ;

                if(Display_Type == 1 )src.at<Vec3b>(Point(xx,yy)) =  {  0, 0, 0 }  ;
                else src.at<Vec3b>(Point(xx,yy)) =   { R0, G0, B0 } ;
                xx_arr++;
            }
        }
    }
}



// converts a rectangular image to 1/4 size for   1024x280 -> 256x70
// always expects an CV_8UC3 or a CV_32FC3
// note if just for viewing the openCV scale function might be better
void Create_Quarter_Scale_Rect_Image_For_Display(cv::Mat src, cv::Mat src2 )
{
    int rowsY = src.rows;
    int colsX = src.cols;
    // printf("\n type1 =  %d   \n",src.depth() );

    if(src.depth() ==  CV_8U)
    {
        for(int yy=2; (yy < rowsY); yy += 4)
            for(int xx=2; (xx < colsX); xx += 4)
                src2.at<Vec3b>(Point( (xx-2)/4, (yy-2)/4  )) = src.at<Vec3b>(Point(xx,yy)) ;
    }
    else if(src.depth() ==  CV_32F)
    {
        for(int yy=2; (yy < rowsY); yy += 4)
            for(int xx=2; (xx < colsX); xx += 4)
                src2.at<Vec3f>(Point( (xx-2)/4, (yy-2)/4  )) = src.at<Vec3f>(Point(xx,yy)) ;
    }
}


void Create_Half_Scale_Rect_Image_For_Display(cv::Mat src, cv::Mat src2 )
{
    int rowsY = src.rows;
    int colsX = src.cols;
    // printf("\n type1 =  %d   \n",src.depth() );

    if(src.depth() ==  CV_8U)
    {
        for(int yy=1; (yy < rowsY); yy += 2)
            for(int xx=1; (xx < colsX); xx += 2)
                src2.at<Vec3b>(Point( (xx-1)/2, (yy-1)/2  )) = src.at<Vec3b>(Point(xx,yy)) ;
    }
    else if(src.depth() ==  CV_32F)
    {
        for(int yy=1; (yy < rowsY); yy += 2)
            for(int xx=1; (xx < colsX); xx += 2)
                src2.at<Vec3f>(Point( (xx-1)/2, (yy-1)/2  )) = src.at<Vec3f>(Point(xx,yy)) ;
    }
}









void Display_Text_Mat(char Window_Name[100], cv::Mat text_window, int x, int y,

bool Pause_A, float Gain_A, float C_Gain_A , float Black_Level_A, float G_Gain_A ,
    bool F_1p1_A, bool F_2p2_A, bool F_3p3_A, bool F_4p4_A,
    bool Ones2x2_A, bool Ones3x3_A, bool Ones4x4_A, bool Ones5x5_A, bool Ones6x6_A, bool Ones7x7_A,

bool Pause_B, float Gain_B, float C_Gain_B , float Black_Level_B, float G_Gain_B ,
    bool F_1p1_B, bool F_2p2_B, bool F_3p3_B, bool F_4p4_B,

    bool Ones2x2_B, bool Ones3x3_B, bool Ones4x4_B, bool Ones5x5_B, bool Ones6x6_B, bool Ones7x7_B,
char CurFile[200] , float Downstream_Gain , bool Video_On, float Fader_Coef, float Fade_Rate, int Current_A_Or_B,  bool A_or_B_File_Ext,
int Test_Mode, bool Sequence_On, int Current_Sequencer_Selection , int Sequence_Duration_Counter , float  Duartion_Mod,
bool Constellation_On, bool Lumen_Pulse_Upper_On, bool LP_62_On, bool LP_63_On, bool TRX_On, uint8_t Red_62_Combined, uint8_t Green_62_Combined, uint8_t Blue_62_Combined,
int64_t Current_Hour, int64_t Current_Minute, int64_t Current_Second,  int64_t Hours_On, int64_t Mins_On,  int64_t Hours_Off, int64_t Mins_Off, float max_Within_Loop_Timer, uint16_t Light_Sensor, bool White_On, bool LED_Correction_On)

{



    int Font_Type = FONT_HERSHEY_DUPLEX;
    text_window.setTo(Scalar(30, 30, 30));

    putText(text_window, format("Pause_A %d Gain %.1f   Chroma %.1f  Black %.2f  Gamma %.1f ", Pause_A, Gain_A, C_Gain_A , Black_Level_A, G_Gain_A) , Point(10,10), Font_Type, .4, Scalar(200,200,200), 1);
    putText(text_window, format("1p1-%d 2p2-%d 3p3-%d 4p4-%d", F_1p1_A, F_2p2_A, F_3p3_A, F_4p4_A) , Point(10,30), Font_Type, .4, Scalar(200,200,200), 1);
    putText(text_window, format("2x2-%d 3x3-%d 4x4-%d 5x5-%d 6x6-%d 7x7-%d", Ones2x2_A, Ones3x3_A, Ones4x4_A, Ones5x5_A, Ones6x6_A, Ones7x7_A) , Point(10,50), Font_Type, .4, Scalar(200,200,200), 1);

    putText(text_window, format("Pause_A %d Gain %.1f   Chroma %.1f  Black %.2f  Gamma %.1f ", Pause_B, Gain_B, C_Gain_B , Black_Level_B, G_Gain_B) , Point(500,10), Font_Type, .4, Scalar(200,200,200), 1);
    putText(text_window, format("1p1-%d 2p2-%d 3p3-%d 4p4-%d", F_1p1_A, F_2p2_B, F_3p3_B, F_4p4_B) , Point(310,30), Font_Type, .4, Scalar(200,200,200), 1);
    putText(text_window, format("2x2-%d 3x3-%d 4x4-%d 5x5-%d 6x6-%d 7x7-%d", Ones2x2_B, Ones3x3_B, Ones4x4_B, Ones5x5_B, Ones6x6_B, Ones7x7_B) , Point(310,50), Font_Type, .4, Scalar(200,200,200), 1);


    putText(text_window, format("Image File: %s    DS Gain %.2f  Video_On %d", CurFile, Downstream_Gain, Video_On ) , Point(10,90), Font_Type, .4, Scalar(200,200,200), 1);

    putText(text_window, format("Fader_Coef %.3f  Rate %.4f  A-Or_B %d  A_or_B_File_Ext %d ", Fader_Coef, Fade_Rate, Current_A_Or_B,  A_or_B_File_Ext) , Point(10,110), Font_Type, .4, Scalar(200,200,200), 1);

    putText(text_window, format("Test_Mode %d  Seq_On %d  Img# %d  Seq_Dur %d  Total Dur %.0f ", Test_Mode, Sequence_On, Current_Sequencer_Selection , Sequence_Duration_Counter/30 , Duartion_Mod) ,
                                                    Point(10,130), Font_Type, .4, Scalar(200,200,200), 1);

    putText(text_window, format("Const  M6465 A62  B63 GTRX  %d  %d  %d  %d  %d", Constellation_On, Lumen_Pulse_Upper_On, LP_62_On, LP_63_On, TRX_On ) , Point(10,160), Font_Type, .4, Scalar(200,200,200), 1);

    putText(text_window, format("Level62 RGB  %d  %d  %d", Red_62_Combined, Green_62_Combined, Blue_62_Combined ) , Point(300,160), Font_Type, .4, Scalar(200,200,200), 1);

    putText(text_window, format("Time %d:%d:%d   on %d:%d   off %d:%02d", Current_Hour, Current_Minute, Current_Second,  Hours_On, Mins_On,  Hours_Off, Mins_Off  ) , Point(500,160), Font_Type, .4, Scalar(200,200,200), 1);

    putText(text_window, format("proc-dur %.2f", max_Within_Loop_Timer) , Point(760,160), Font_Type, .4, Scalar(200,200,200), 1);

    putText(text_window, format("WhiteOn %d   LEDCorrectionOn %d  sensor %d  ", White_On, LED_Correction_On, Light_Sensor) , Point(500,130), Font_Type, .4, Scalar(200,200,200), 1);

    namedWindow(Window_Name);
    moveWindow(Window_Name, x,y);
    imshow( Window_Name, text_window );
    }


    void Display_and_Fill_Upper_LP(char Window_Name[100], cv::Mat upper_LP_window, int x, int y, int Red, int Green, int Blue )
{
    int Font_Type = FONT_HERSHEY_DUPLEX;
    upper_LP_window.setTo(Scalar(Blue, Green, Red));

    namedWindow(Window_Name);
    moveWindow(Window_Name, x,y);
    imshow( Window_Name, upper_LP_window );
    }



