
#include "opencv2/opencv.hpp"
// #include "opencv2/highgui/highgui.hpp"
// #include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "ftd2xx.h"
#include <thread>
#include <unistd.h>


#include "defines_MPZ.h"
#include "measure.h"
#include "process_filters.h"
#include "file_io.h"
#include "process_other.h"



using namespace std;
using namespace cv;


typedef Vec<uint8_t, 3> Pixel_Type_8b;

typedef Vec<uint16_t, 3> Pixel_Type;
typedef Vec<uint16_t, 3> Pixel_Type_u16;


typedef Vec<float, 3> Pixel_Type_F;



    class Parallel_process : public cv::ParallelLoopBody
    {

    private:
        cv::Mat img;
        cv::Mat& retVal;

    public:
        Parallel_process(cv::Mat inputImgage, cv::Mat& outImage)
            : img(inputImgage), retVal(outImage){}

        virtual void operator()(const cv::Range& range) const
        {
            for(int i = range.start; i < range.end; i++)
            {
                // Your code here
            }
        }
    };



/// A_Fader_State States:

/// 0 -> A Off
/// 1 -> A Fading Up To Full Limit
/// 2 -> A Full On
/// 3 -> A Fading Down to 0

/// 4 -> NOT USED

/// 5 -> A Fading Up to Partial Limit
/// 6 -> A At Partial Limit
/// 7 -> A Fading to 0 from Partial Limit

/// 8 -> A OFF AFTER 7

/// 9 -> A Fading to NEGATIVE Partial Limit
/// 10 -> A At Partial NEGATIVE Limit
/// 11 -> A Fading to 0 from NEGATIVE Partial Limit

/// 12 -> A OFF AFTER 11




void   Fader_Main_Rev3( int &A_Fader_State, int &B_Fader_State, float &Fader_Coef_A, float &Fader_Coef_B, float &Fade_Rate,  float &Opacity_A, float &Opacity_B )
{

    float Fader_A_Upper_Limit_Level, Fader_A_Lower_Limit_Level, Fader_B_Upper_Limit_Level, Fader_B_Lower_Limit_Level ;

    /**************************  A SIDE CALCULATIONS  ***********************/
    /// set limits based on STATE
    Fader_A_Upper_Limit_Level = 1.0 ; /// default limit
    if(A_Fader_State == 5)Fader_A_Upper_Limit_Level = Opacity_A/ 100. ;
    else if(A_Fader_State == 11)Fader_A_Upper_Limit_Level = 0.0 ;
    Fader_A_Lower_Limit_Level = 0.0 ;  /// default limit
    if(A_Fader_State == 9)Fader_A_Lower_Limit_Level = -1 * Opacity_A/ 100. ;

    if( (A_Fader_State == 1) || (A_Fader_State == 5) || (A_Fader_State == 11) )  /// POSITIVE DIRECTION (upward slope)
    {
        Fader_Coef_A +=  Fade_Rate;
        if(Fader_Coef_A >=  Fader_A_Upper_Limit_Level  )
        {
            Fader_Coef_A = Fader_A_Upper_Limit_Level;
            /// STATE CHANGE 1 -> 2  5 -> 6   11 -> 0
            A_Fader_State++;
        }
    }
    else if( (A_Fader_State == 3) || (A_Fader_State == 7) || (A_Fader_State == 9) )   /// NEGATIVE DIRECTION (downward slope)
    {
        Fader_Coef_A -=  Fade_Rate;
        if(Fader_Coef_A <=  Fader_A_Lower_Limit_Level)
        {
            Fader_Coef_A = Fader_A_Lower_Limit_Level;
            /// STATE CHANGE 3 -> 0  7 -> 0   9 -> 10
            A_Fader_State++;
        }
    }

    /**************************  B SIDE CALCULATIONS  ***********************/
    /// set limits based on STATE
    Fader_B_Upper_Limit_Level = 1.0 ; /// default limit
    if(B_Fader_State == 5)Fader_B_Upper_Limit_Level = Opacity_B/ 100. ;
    else if(B_Fader_State == 11)Fader_B_Upper_Limit_Level = 0.0 ;
    Fader_B_Lower_Limit_Level = 0.0 ;  /// default limit
    if(B_Fader_State == 9)Fader_B_Lower_Limit_Level = -1 * Opacity_B/ 100. ;

    if( (B_Fader_State == 1) || (B_Fader_State == 5) || (B_Fader_State == 11) )   /// POSITIVE DIRECTION (upward slope)
    {
        Fader_Coef_B +=  Fade_Rate;
        if(Fader_Coef_B >=  Fader_B_Upper_Limit_Level  )
        {
            Fader_Coef_B = Fader_B_Upper_Limit_Level;
            /// STATE CHANGE 1 -> 2  5 -> 6   11 -> 0
            B_Fader_State++;
        }
    }
    else if( (B_Fader_State == 3) || (B_Fader_State == 7) || (B_Fader_State == 9) )   /// NEGATIVE DIRECTION (downward slope)
    {
        Fader_Coef_B -=  Fade_Rate;
        if(Fader_Coef_B <=  Fader_B_Lower_Limit_Level)
        {
            Fader_Coef_B = Fader_B_Lower_Limit_Level;
            /// STATE CHANGE 3 -> 0  7 -> 0   9 -> 10
            B_Fader_State++;
        }
    }
}





void   Fader_Main_Orig(int &Current_A_Or_B,  float &Fader_Coef, float &Fade_Direction,  float &Fade_Rate )
{
    Fader_Coef += Fade_Rate * Fade_Direction;

    if(Fader_Coef >= 1.0)
    {
        Fader_Coef = 1.0;
        Current_A_Or_B = Image_A;
    }

    else if(Fader_Coef <= 0.0)
    {
        Fader_Coef = 0.0;
        Current_A_Or_B = Image_B;
    }
}




void   Fader_Main_Rev2(int &Current_A_Or_B,  float &Fader_Coef_A, float &Fader_Coef_B,  float &Fade_Direction,  float &Fade_Rate, float &Mix_Key_Type_A, float &Mix_Key_Type_B, float &Opacity_A, float &Opacity_B, float &Sign_Fader_A, float &Sign_Fader_B )
{

    float Fader_A_Upper_Clip_Level, Fader_A_Lower_Clip_Level, Fader_B_Upper_Clip_Level, Fader_B_Lower_Clip_Level ;


        // default clips
        Fader_A_Upper_Clip_Level = 1.0 ;
        Fader_A_Lower_Clip_Level = 0.0 ;
        Fader_B_Upper_Clip_Level = 1.0 ;
        Fader_B_Lower_Clip_Level = 0.0 ;


        // Video_A side
        if( ( Mix_Key_Type_B == 3) )
        {
            Fader_Coef_A =  1.0;
            Sign_Fader_A =  1.0;
            if(Fade_Direction > 0)
            {
                Fader_B_Lower_Clip_Level = 1 - Opacity_B/ 100. ;
                Fader_B_Upper_Clip_Level = 1.0 ;
            }
            else
            {
                Fader_B_Lower_Clip_Level = 0.0 ;
                Fader_B_Upper_Clip_Level =  Opacity_B/ 100. ;
            }

        }
        else // normal case
        {
            Fader_Coef_A += Fade_Rate * Fade_Direction;
            if( ( Mix_Key_Type_A == 3) )Sign_Fader_A =  -1.0;
            else Sign_Fader_A =  1.0;
        }



        // Video_B side
        if( ( Mix_Key_Type_A == 3)  )
        {
            Fader_Coef_B =  1.0;
            Sign_Fader_B =  1.0;
            if(Fade_Direction < 0)
            {
                Fader_A_Lower_Clip_Level = 1 - Opacity_A/ 100. ;
                Fader_A_Upper_Clip_Level = 1.0 ;
            }
            else
            {
                Fader_A_Lower_Clip_Level = 0.0 ;
                Fader_A_Upper_Clip_Level =  Opacity_A/ 100. ;
            }

        }
        else  // normal case
        {
            Fader_Coef_B -= Fade_Rate * Fade_Direction;
            Sign_Fader_A =  1.0;
            if(  ( Mix_Key_Type_B == 3) )Sign_Fader_B =  -1.0 ;
            else Sign_Fader_B =  1.0 ;
        }




        // limiter
        if(Fader_Coef_A >= Fader_A_Upper_Clip_Level)
        {
            Fader_Coef_A = Fader_A_Upper_Clip_Level;
            Current_A_Or_B = Image_A;
        }
        else if(Fader_Coef_A <= Fader_A_Lower_Clip_Level)
        {
            Fader_Coef_A = Fader_A_Lower_Clip_Level;
            Current_A_Or_B = Image_B;
        }


        // limiter
        if(Fader_Coef_B >= Fader_B_Upper_Clip_Level)
        {
            Fader_Coef_B = Fader_B_Upper_Clip_Level;
            Current_A_Or_B = Image_B;
        }
        else if(Fader_Coef_B <= Fader_B_Lower_Clip_Level)
        {
            Fader_Coef_B = Fader_B_Lower_Clip_Level;
            Current_A_Or_B = Image_A;
        }
}



/*

// stores a rectangular image 70 x 256  TEST
void Save_Samples_From_Grid_To_Buffer(cv::Mat src, cv::Mat src2 )
{
    //  Pixel pixel = temp.at<Pixel>(2, 2);
    //  Pixel_Vec = temp.at<Pixel>(2, 2);void Panel_Mapper(uint16_t *Vid_Grabbed_Local, uint16_t *Vid_Mapped_Local, int *Panel_Map_Local  )


    Vec3s vv;
    uint16_t  a,b ;

    for(int yy=2; (yy < IMAGE_ROWS); yy += 4)
        for(int xx=2; (xx < IMAGE_COLS); xx += 4)
        {
        src2.at<Vec3f>(Point( (xx-2)/4, (yy-2)/4  )) = src.at<Vec3f>(Point(xx,yy)) ;
        vv = src.at<Vec3f>(Point(xx,yy)) ;
        }
}





void Save_Samples_From_CSV_Map_To_Buffer(cv::Mat src, uint16_t *Buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS])
{
    int xx_arr, yy_arr, yy, xx;
    int inc = 0;
    Pixel_Type Pixel_Vec;
    for(yy_arr=0; (yy_arr < SAMPLE_ROWS); yy_arr++)  // up to 67
    {
        yy = 2 + (4 * yy_arr);  // up to 280
        for(xx_arr=0; xx_arr <  Num_Of_Samples_Per_Row[yy_arr]; xx_arr++)
        {
            xx = SP[yy_arr][xx_arr];
            Pixel_Vec = src.at<Pixel_Type>(yy, xx);
            *(Buffer + inc++) = Pixel_Vec[2];
            *(Buffer + inc++) = Pixel_Vec[1];
            *(Buffer + inc++) = Pixel_Vec[0];
        }
    }
}



// Saves Sample RGB W= 0080;
void Save_Samples_From_CSV_Map_To_Buffer_RGBW(cv::Mat src, uint16_t *Buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS])
{
    int xx_arr, yy_arr, yy, xx;
    int inc = 0;
    Pixel_Type Pixel_Vec;
    for(yy_arr=0; (yy_arr < SAMPLE_ROWS); yy_arr++)  // up to 67
    {
        yy = 2 + (4 * yy_arr);  // up t// Define a pixel  c++
//typedef Point3_<uint16_t> Pixel;
// c++o 280
        for(xx_arr=0; xx_arr <  Num_Of_Samples_Per_Row[yy_arr]; xx_arr++)
        {
            xx = SP[yy_arr][xx_arr];
            Pixel_Vec = src.at<Pixel_Type>(yy, xx);
            *(Buffer + inc++) = Pixel_Vec[2];
            *(Buffer + inc++) = Pixel_Vec[1];
            *(Buffer + inc++) = Pixel_Vec[0];
            *(Buffer + inc++) = 0x0080;
        }
    }
}

*/


// converts to RGBW (Creates W)
void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert(cv::Mat src, uint16_t *Buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS])
{
    uint16_t r,g,b,w,Sub_Val;
    int xx_arr, yy_arr, yy, xx;
    int inc = 0;
    Pixel_Type Pixel_Vec;
    for(yy_arr=0; (yy_arr < SAMPLE_ROWS); yy_arr++)  // up to 67
    {
        yy = 2 + (4 * yy_arr);  // up to 280
        for(xx_arr=0; xx_arr <  Num_Of_Samples_Per_Row[yy_arr]; xx_arr++)
        {
            xx = SP[yy_arr][xx_arr];
            Pixel_Vec = src.at<Pixel_Type>(yy, xx);
            r=Pixel_Vec[2];
            g=Pixel_Vec[1];
            b=Pixel_Vec[0];

            if((r<=g) &  (r<=b))Sub_Val = r;
            else if((g<=r) &  (g<=b))Sub_Val = g;
            else Sub_Val = b;

            *(Buffer + inc++) = r-Sub_Val;
            *(Buffer + inc++) = g-Sub_Val;
            *(Buffer + inc++) = b-Sub_Val;
            *(Buffer + inc++) = Sub_Val;
        }
    }
}


// converts to RGBW (Creates W)
void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_rev2(cv::Mat src, uint16_t *Buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS],int  Vertical_Spacing)
{
    uint16_t r,g,b,w,Sub_Val;
    int xx_arr, yy_arr, yy, xx;
    int inc = 0;
    Pixel_Type Pixel_Vec;
    for(yy_arr=0; (yy_arr < SAMPLE_ROWS); yy_arr++)  // up to 67
    {
        yy = 2 + (Vertical_Spacing * yy_arr);  // up to 280
        for(xx_arr=0; xx_arr <  Num_Of_Samples_Per_Row[yy_arr]; xx_arr++)
        {
            xx = SP[yy_arr][xx_arr];
            Pixel_Vec = src.at<Pixel_Type>(yy, xx);
            r=Pixel_Vec[2];
            g=Pixel_Vec[1];
            b=Pixel_Vec[0];

            if((r<=g) &  (r<=b))Sub_Val = r;
            else if((g<=r) &  (g<=b))Sub_Val = g;
            else Sub_Val = b;

            *(Buffer + inc++) = r-Sub_Val;
            *(Buffer + inc++) = g-Sub_Val;
            *(Buffer + inc++) = b-Sub_Val;
            *(Buffer + inc++) = Sub_Val;
        }
    }
}



// converts to RGBW (Creates W)
void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_rev3(cv::Mat src, uint16_t *Buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS],int  Vertical_Spacing,

                                                            bool White_On, bool LED_Correction_On, float Red_Correct, float Green_Correct, float Blue_Correct, float White_Correct )
{

    float rf,gf,bf,wf,Sub_Val;
    int xx_arr, yy_arr,xx, yy;
    int inc = 0;
    Pixel_Type_u16 Pixel_Vec;


    float Red_Correct_Mod = LED_Correction_On ? Red_Correct : 1.0 ;
    float Green_Correct_Mod = LED_Correction_On ? Green_Correct : 1.0 ;
    float Blue_Correctt_Mod = LED_Correction_On ? Blue_Correct : 1.0 ;

    for(yy_arr=0; (yy_arr < SAMPLE_ROWS); yy_arr++)  // up to 67
    {
        yy = 2 + (Vertical_Spacing * yy_arr);  // up to 280
        for(xx_arr=0; xx_arr <  Num_Of_Samples_Per_Row[yy_arr]; xx_arr++)
        {
            xx = SP[yy_arr][xx_arr];
            Pixel_Vec =  src.at<Pixel_Type_u16>(yy, xx);
            rf = (float) Pixel_Vec[2];
            gf =(float) Pixel_Vec[1];
            bf =(float) Pixel_Vec[0];


            if((rf<=gf) &  (rf<=bf))Sub_Val = rf;
            else if((gf<=rf) &  (gf<=bf))Sub_Val = gf;
            else Sub_Val = bf;

            if(!White_On)Sub_Val = 0.0;

            rf = Red_Correct_Mod * (rf-Sub_Val) ;
            if(rf > 65535) rf = 65535;
            gf = Green_Correct_Mod *  (gf-Sub_Val) ;
            if(gf > 65535) gf = 65535;
            bf = Blue_Correctt_Mod *  (bf-Sub_Val) ;
            if(bf > 65535) bf = 65535;


            *(Buffer + inc++) = (uint16_t) rf;
            *(Buffer + inc++) = (uint16_t) gf;
            *(Buffer + inc++) = (uint16_t) bf;
            *(Buffer + inc++) = (uint16_t) Sub_Val;
        }
    }
}



// converts to RGBW (Creates W)  USING UMat instead of Mat
void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_U(cv::UMat src, uint16_t *Buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS])
{
    uint16_t r,g,b,w,Sub_Val;
    int xx_arr, yy_arr, yy, xx;
    int inc = 0;
    Pixel_Type Pixel_Vec;
    for(yy_arr=0; (yy_arr < SAMPLE_ROWS); yy_arr++)  // up to 67
    {
        yy = 2 + (4 * yy_arr);  // up to 280
        for(xx_arr=0; xx_arr <  Num_Of_Samples_Per_Row[yy_arr]; xx_arr++)
        {
            xx = SP[yy_arr][xx_arr];

            // Pixel_Vec = src.at<Pixel_Type>(yy, xx);

            src.getMat(ACCESS_READ).at<Pixel_Type>(yy, xx);


            r=Pixel_Vec[2];
            g=Pixel_Vec[1];
            b=Pixel_Vec[0];

            // generate white pixel
            if((r<=g) &  (r<=b))Sub_Val = r;
            else if((g<=r) &  (g<=b))Sub_Val = g;
            else Sub_Val = b;

            *(Buffer + inc++) = r-Sub_Val;
            *(Buffer + inc++) = g-Sub_Val;
            *(Buffer + inc++) = b-Sub_Val;
            *(Buffer + inc++) = Sub_Val;
        }
    }
  //  src_Mul.setTo(Scalar(.0039, .0039,  .0039)); // scale factor

//    multiply(src,src_Mul,src);          // scale
}



/*

void Simple_Test(cv::Mat src)
{

    for(int i=0; i<src.rows; i++)
        for(int j=0; j<src.cols; j++)src.at<Vec3f>(i,j) *= .1;
}


void Simple_Test2(cv::Mat src)
{
    src.forEach<Vec3f>([] (Vec3f &p, const int *)
    {
        p *= .1;
    }   );
}

void Simple_Test3(cv::Mat src)
{
    src.convertTo(src, -1, .1, 0);
}


//cv::Mat m(100, 100, CV_8UC3); //3-channel
//m = Scalar(5, 10, 15);  //Scalar.val[0-2] used
//or

//cv::Mat m(100, 100, CV_8UC3); //3-channel
//m.setTo(Scalar(5, 10, 15));  //Scalar.val[0-2] usedFader



// TRY A BUNCH OF COLOR LEVEL METHODS FOR SPEED TESTING

// expects 32bit in
void Color_Levels_1(cv::Mat src)
{
    Mat splitChannels[3];
    cvtColor(src, src, cv::COLOR_BGR2YUV);
    split(src, splitChannels);
    splitChannels[2].convertTo(splitChannels[2], -1, 2, 0);
    splitChannels[1].convertTo(splitChannels[1], -1, 2, 0);
    merge( splitChannels, 3, src );
    cvtColor(src, src, cv::COLOR_YUV2BGR);
}


// expects 32bit in
void Color_Levels_1B(cv::Mat src, cv::Mat src_Mul, float Chroma_Level, float Luma_Level )
{
    cvtColor(src, src, cv::COLOR_BGR2YUV);
    src_Mul.setTo(Scalar(Luma_Level, Chroma_Level, Chroma_Level));
    multiply(src,src_Mul,src);
    cvtColor(src, src, cv::COLOR_YUV2BGR);
}



// expects 32bit in
void Color_Levels_Local_F(Pixel_Type_F &pixel)
{
    float Chroma_Level = 2;
    float y,by,ry,gy;
    y = pixel[0] * .114  + pixel[1]  * .587 + pixel[2] * .285;
    by = (pixel[0] - y ) * Chroma_Level;
    gy = (pixel[1] - y ) * Chroma_Level;
    ry = (pixel[2] - y ) * Chroma_Level;
    pixel[0]  = y + by;
    pixel[1]  = y + gy;
    pixel[2]  = y + ry;
}

// expects 32bit in
void Color_Levels_2_F(cv::Mat src)
{

struct Operator
{
  void operator ()(Pixel_Type_F &pixel, const int * position) const
  {

    // Perform a simple threshold operation
    Color_Levels_Local_F(pixel);
  }
};

    src.forEach<Pixel_Type_F>(Operator());
}




void Process_Image(cv::Mat& Vid_In, cv::Mat& Vid_In_F, bool Filter_On_Off,
                    bool On1p1, bool On2p2, bool On3p3, bool On4p4, bool On3tap,
                    bool On2x2, bool On3x3, bool On4x4, bool On5x5, bool On6x6, bool On7x7, bool OnOnes,
                    float gain, float black_level )
{
                    // convert to float
    Vid_In.convertTo(Vid_In_F, CV_32FC3);
    Filter_3Taps(Vid_In_F, On1p1, On2p2, On3p3, On4p4, On3tap);
    Filter_Blurs(Vid_In_F, On2x2, On3x3, On4x4, On5x5, On6x6, On7x7, OnOnes);
    Vid_In_F.convertTo(Vid_In_F, -1, gain, black_level);
}


*/






void Change_Seam(cv::Mat& Vid_In, bool Normal)
{


    Mat Mat_Temp;
    Pixel_Type_8b Pixel_Vec;
    Mat_Temp = Vid_In.clone();  // convert back to Mat
    for(int iy=0; iy<IMAGE_ROWS; iy++)
    {
            for(int ix=0; ix<IMAGE_COLS; ix++)
            {
                Pixel_Vec = Vid_In.at<Vec3b>(Point(ix,iy))  ;
                if(ix < 512)Mat_Temp.at<Vec3b>(Point(ix+512,iy)) = Pixel_Vec ;
                else Mat_Temp.at<Vec3b>(Point(ix-512,iy)) = Pixel_Vec ;
            }
    }

    Vid_In = Mat_Temp.clone();
}




void Shift_Image_Horizontal(cv::Mat& Vid_In, int Initial_Location )
{
    // note by incrementing the location the movie can shift around in real time.
    // need to figure out how to do this.  Think I need an image class or structure

	int ROI_Width_1 = IMAGE_COLS - Initial_Location ;
    int ROI_Height 	= IMAGE_ROWS;
	int ROI_Width_2 = IMAGE_COLS - ROI_Width_1;

	static int Current_Location;

	Rect Rect_Before_1(0, 0, ROI_Width_1, ROI_Height);
	Rect Rect_Before_2(ROI_Width_1, 0, ROI_Width_2, ROI_Height);

	Rect Rect_After_1(ROI_Width_2, 0, ROI_Width_1, ROI_Height);
	Rect Rect_After_2(0, 0, ROI_Width_2, ROI_Height);


    Mat ROI_Before_1 = Vid_In(Rect_Before_1).clone();
    Mat ROI_Before_2 = Vid_In(Rect_Before_2).clone();

    Mat ROI_After_1  = Vid_In(Rect_After_2);            // Get the header to the destination position
    Mat ROI_After_2  = Vid_In(Rect_After_1);            // Get the header to the destination position

    ROI_Before_1.copyTo(ROI_After_2);
    ROI_Before_2.copyTo(ROI_After_1);
}






// expects 32bit in

void Color_Levels_1B_U(cv::UMat src, cv::UMat src_Mul, float Chroma_Level, float Luma_Level )
{
    cvtColor(src, src, cv::COLOR_BGR2YUV);
    src_Mul.setTo(Scalar(Luma_Level, Chroma_Level, Chroma_Level));
    multiply(src,src_Mul,src);
    cvtColor(src, src, cv::COLOR_YUV2BGR);
}


void RGB_Gamma_Correction(cv::UMat src, float Gamma_Correction )
{
    UMat src_squared;
    multiply(src,src,src_squared);  // square
    addWeighted( src, 1 - Gamma_Correction , src_squared, .0039*Gamma_Correction, 0.0, src);
}







void Process_Image_U(cv::UMat& Vid_In, cv::UMat& Vid_In_F,    cv::UMat& YUV_Gain,        bool Filter_On_Off,
                    bool On1p1, bool On2p2, bool On3p3, bool On4p4, bool On3tap,
                    bool On2x2, bool On3x3, bool On4x4, bool On5x5, bool On6x6, bool On7x7, bool OnOnes,
                    float gain, float black_level, float Chroma_Level, float Gamma_Correction )
{
    // convert to float
    Vid_In.convertTo(Vid_In_F, CV_32FC3);

    Filter_3Taps_U(Vid_In_F, On1p1, On2p2, On3p3, On4p4, On3tap);
    Filter_Blurs_U(Vid_In_F, On2x2, On3x3, On4x4, On5x5, On6x6, On7x7, OnOnes);

    // I like to do the black_level befor scaling which is why this is done in 2 steps
    Vid_In_F.convertTo(Vid_In_F, -1, 1,  black_level * 64 );  // BLACK LEVEL

    Vid_In_F.convertTo(Vid_In_F, -1, 1, 0); // GAIN

    Vid_In_F.convertTo(Vid_In_F, -1, gain, 0);

    RGB_Gamma_Correction(Vid_In_F,  Gamma_Correction);

    Color_Levels_1B_U(Vid_In_F, YUV_Gain, Chroma_Level, 1);

}



// new Stuff MAKE MORE EFFICIENT  copy a whole block

/*
    Pixel_Type_8b Pixel_Vec;
    Mat Mat_Temp, Mat_Temp2;
    Vid_In.copyTo(Mat_Temp);  // convert back to Mat
    Mat_Temp2.create(IMAGE_ROWS,IMAGE_COLS,CV_8UC3);


    for(int iy=0; iy<IMAGE_ROWS; iy++)
    {
            for(int ix=0; ix<IMAGE_COLS; ix++)
            {
                Pixel_Vec = Mat_Temp.at<Vec3b>(Point(ix,iy))  ;
                if(ix < 512)Mat_Temp2.at<Vec3b>(Point(ix+512,iy)) = Pixel_Vec ;
                else Mat_Temp2.at<Vec3b>(Point(ix-512,iy)) = Pixel_Vec ;
            }
    }
    Mat_Temp2.copyTo(Vid_In);  // convert back to Mat
*/

//   Pixel_Vec = UMat_Temp.getMat(ACCESS_READ).at<Pixel_Type_F>(6, 6);
//   UMat_Temp = Vid_In_F.clone();void RGB_Gamma_Correction(cv::UMat src, float Gamma_Correction )

// new Stuff MAKE MORE EFFICIENT  DONE







/*

// stores sample points based on csv file read  non  rectangular
void Save_Samples_From_CSV_Map_To_Vec_Buffer(cv::Mat src, Pixel_Type *Buf_Vec, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS])
{
    int xx_arr, yy_arr, yy;
    int inc = 0;
    Pixel_Type Pixel_Vec;

    for(yy_arr=0; (yy_arr < SAMPLE_ROWS); yy_arr++)
    {
        xx_arr = 0;
        yy = 2 + (4 * yy_arr) ;
        for(int xx=0; (xx < IMAGE_COLS); xx++)
        {
            if( SP[yy_arr][xx_arr] ==  xx)
            {
                Pixel_Vec = src.at<Pixel_Type>(yy, xx); // ORDER IS (Y,X) AND BGR  !!!!!!!!!!!!!!!!!!!!!!!!!
                Buf_Vec[inc++] = Pixel_Vec;
                // if((xx_arr == 0) && (yy_arr == 1))printf("PIXELS22 ---  %d  %d  %d    \n",Pixel_Vec[0],Pixel_Vec[1],Pixel_Vec[2]);
                xx_arr++;
            }
        }
    }
   // printf("PIXELS66666666666 ---  %d  %d  %d    \n", *(Buffer + 0 ), *(Buffer +  1), *(Buffer + 2) ) ;
}
*/
