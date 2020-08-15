// #pragma once



   #include "opencv2/opencv.hpp"
// #include "opencv2/core/ocl.hpp""
// #include "opencv2/core/cuda.hpp""
// #include "opencv2/cudaimgproc.hpp""
// #include "/usr/local/include/opencv2/opencv.hpp"
// #include "/home/jim/Desktop/includes/opencv.hpp"
// #include "opencv2/highgui/highgui.hpp"
// #include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "ftd2xx.h"
#include<thread>
#include <chrono>
#include<pthread.h>
#include <unistd.h>
#include<ctime>



#include "defines_MPZ.h"
#include "measure.h"
#include "process_filters.h"
#include "process_other.h"

#include "file_io.h"
#include "maps_and_headers.h"
#include "KeyBoardInput.h"
#include "display_images_text.h"




using namespace std;
using namespace cv;

// Define a pixel opencv
typedef Vec<uint16_t, 3> Pixel_Type;





// Define a pixel  c++
//typedef Point3_<uint16_t> Pixel;
// c++
// typedef std::vector<int> int_vec_t;


/*
struct Image_File_Struct {
   char     File_Name[50];
   int      Gain;
   int      Black_Level;
 //  Vec3w    Color_8bit;
};
*/




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



// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v{}")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v{}")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from left & right
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v{}")
{
    s.erase(0, s.find_first_not_of(t));
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// copying versions

inline std::string ltrim_copy(std::string s, const char* t = " \t\n\r\f\v{}")
{
    return ltrim(s, t);
}

inline std::string rtrim_copy(std::string s, const char* t = " \t\n\r\f\v{}")
{
    return rtrim(s, t);
}

inline std::string trim_copy(std::string s, const char* t = " \t\n\r\f\v{}")
{
    return trim(s, t);
}




// Yaml Parser
int Read_YAML_Data(const char *Filename,  int ix, int jx,  std::string  FileData[][10]   )
{

    std::ifstream infile;
    std::string input_line;

    size_t  found_comma, found_colon, found_comment;
    bool    valid_getline;
    size_t  value_cnt = 0,
    line_cnt = 0;

    infile.open(Filename);

    valid_getline = true;
    while(valid_getline)
    {
        if(std::getline(infile, input_line) )  // end of file returns 0
        {
            found_comment = input_line.find_first_of("#");  // is there a comment
            found_colon = input_line.find_first_of(":");    // is there a colon
            while( ( ( found_colon == input_line.npos) || (found_comment < found_colon) ) && (valid_getline)  ) // no colon or comment less than colon
            {
                if(getline(infile, input_line) )      // end of file returns 0         // keep grabbing lines until line has a colon
                {
                    found_colon = input_line.find_first_of(":");    // is there a comment
                    found_comment = input_line.find_first_of("#");  // is there a colon
                }
                else valid_getline = false;   // end of file
            }

            if(valid_getline)
            {
                value_cnt = 0;
                FileData[line_cnt][value_cnt++] = trim_copy( input_line.substr (0,found_colon) );  // parameter is before the colon

                if(found_comment != input_line.npos)input_line =  input_line.substr (0,found_comment);  // get rid of post comment
                input_line =  input_line.substr(found_colon+1);  // get rid of parameter from string

                found_comma = input_line.find_first_of(",");    // look for commas
                if( found_comma==input_line.npos )FileData[line_cnt][value_cnt++]  = trim_copy(input_line); // no comma not found
                else  // comma found
                {
                    while (found_comma!=input_line.npos)              // goes through at least once
                    {
                        FileData[line_cnt][value_cnt++]  = trim_copy(input_line.substr (0,found_comma) );      // next data
                        input_line = input_line.substr (found_comma+1); // get rid of next data from string
                        found_comma=input_line.find_first_of(",");      // check for another comma
                    }
                    FileData[line_cnt][value_cnt++] = trim_copy(input_line); // final data
                }
            }
        }
        else valid_getline = false;  // end of file

        line_cnt++;
    }

    infile.close();
    return line_cnt-1;
}



/*
inline
void   Fader_Main(int &Current_A_Or_B,  float &Fader_Coef, float &Fade_Direction,  float &Fade_Rate )
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
*/







bool FTDI_On = true;
int Test_Mode = 0;

bool Fader_Stopped = false;

int Read_Sample_Point_Location_Array_SS(std::string FileName,  int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS]);


int main(){

//    pthread_t thread1, thread2;

    // FTDI hardware stuff
	DWORD dwError, dwPriClass;
	DWORD bytes_written;
	FT_HANDLE ftHandle;
	FT_STATUS ftStatus;
	UCHAR Mask = 0xff;
	UCHAR Mode;
	DWORD EventDWord;
	DWORD RxBytes;
	DWORD TxBytes;
	DWORD BytesWritten;
	DWORD BytesReceived;


    time_t time_time;
    struct tm * time_X;


    // array containing sample point locations read from file
    int Sample_Points[SAMPLE_ROWS][SAMPLE_COLS];
    int Num_Of_Samples_Per_Row[SAMPLE_ROWS] ;

    // array containing the panel map
    // int Panel_Map[PANEL_MAP_SIZE] ;
    //  int Panel_Map_W_Gaps[PANEL_MAP_W_GAPS_SIZE] ;

    int Sculpture_Map[Sculpture_Size_Pixels];



    int Enclosure_Info[NUM_OF_ENCLOSURES][NUM_OF_ENCLOSURE_PARAMETERS];


    // for synchronizing
    uint8_t	 *RxBuffer;
    RxBuffer = new uint8_t [RxBuffer_Size];   // only receives 64 for syncing


// these are for testing only
    uint16_t *Grab_Buffer;
	Grab_Buffer =  new uint16_t [Sculpture_Size_RGB]; //  33408  11136 * 3
    uint16_t *Map_Buffer;
	Map_Buffer =  new uint16_t [Sculpture_Size_RGB];  //  33408  11136 * 3
// this one fills in partial chassis and leaves spaces for enclosure and panel headers
    uint16_t *Map_Buffer_W_Gaps;
	Map_Buffer_W_Gaps =  new uint16_t [Buffer_W_Gaps_Size_RGB];  //  39348  13116 * 3



    uint16_t *Grab_Buffer_RGBW;
	Grab_Buffer_RGBW =  new uint16_t [Sculpture_Size_RGBW];  //  16 bit   4 * 11136    44544
    uint16_t *Map_Buffer_RGBW;
	Map_Buffer_RGBW =  new uint16_t [Sculpture_Size_RGBW];   //  16 bit   4 * 11136    44544
// this one fills in partial chassis and leaves spaces for enclosure and panel headers
    uint16_t *Map_Buffer_W_Gaps_RGBW;
	Map_Buffer_W_Gaps_RGBW =  new uint16_t [Buffer_W_Gaps_Size_RGBW_Extra];   //  16 bit   4 * 13116  52464


    uint16_t Light_Sensor;


    Mat img_in_A, img_in_B;
    Mat img_proc_A, img_proc_B;
    Mat image_proc_sampled_A, image_proc_sampled_B;
    Mat img_proc_u16_A, img_proc_u16_B;

    Mat Mat_Sum;

    Mat Mat_Sum_sampled;

    Mat Mat_Sum_Display;

    Mat text_window;

    Mat upper_LP_window;



    UMat YUV_Gain_A, YUV_Gain_B;

    UMat img_in_A_U, img_in_B_U ;

    UMat    img_proc_A_U, img_proc_B_U ;

    UMat    Mat_Sum_U;



    printf("here0x");


    VideoCapture cap_A, cap_B ;


    // timing measurement stuff
    Prog_Durations Program_Timer;
    Prog_Durations Video_Loop_Timer;
    Prog_Durations Within_Loop_Timer;

    Prog_Durations Temp_Time_Test;

    Prog_Durations New_1;
    Prog_Durations New_2;
    Prog_Durations New_3;
    Prog_Durations New_4;
    Prog_Durations New_5;
    Prog_Durations New_6;
    Prog_Durations New_7;
    Prog_Durations New_8;
    Prog_Durations New_9;
    Prog_Durations New_10;
    Prog_Durations New_11;
    Prog_Durations New_12;

    Prog_Durations New_1B;
    Prog_Durations New_2B;
    Prog_Durations New_3B;


    float d1, d2, d3, d4,d5, d6, d7, d8, d9, d10, d11, d12, max_Within_Loop_Timer, max_stored, d1f,  min_Within_Loop_Timer ;

    float time_accum_avg, time_accum;


    int cnt;
    int tread;

    int Points_Visible;

    bool Pause_A, Points_Visible_A, Filter_On_Off_A;
    bool Pause_B, Points_Visible_B, Filter_On_Off_B;

    bool F_1p1_A, F_2p2_A,  F_3p3_A, F_4p4_A, Filter_3Taps_On_A ;
    bool F_1p1_B, F_2p2_B,  F_3p3_B, F_4p4_B, Filter_3Taps_On_B ;

    bool Ones2x2_A, Ones3x3_A, Ones4x4_A, Ones5x5_A, Ones6x6_A, Ones7x7_A, Ones_On_A ;
    bool Ones2x2_B, Ones3x3_B, Ones4x4_B, Ones5x5_B, Ones6x6_B, Ones7x7_B, Ones_On_B ;

    bool Reset_All_Filters_A, Reset_All_Filters_B;

    bool Constellation_On = false ;
    bool Lumen_Pulse_Upper_On = false ;

    bool LP_62_On  ;
    bool LP_63_On ;
    bool TRX_On    ;

    bool Seam_Flip;

     bool Video_On;




    bool img_On_A, img_proc_On_A, img_On_B, img_proc_On_B;

	bool Show_Proc_A = true;
	bool Show_Proc_B = true;

    bool Grab  = false ;

    int32_t loopcnt = 0;


    float Gain_A;
    float Black_Level_A ;
    float C_Gain_A ;
    float Luma_Level_A ;
    float G_Gain_A; // Gamma
    float Opacity_A;
    float Mix_Key_Type_A;
    int A_Fader_State;
    float Key_Start_Time_A;

    float   Fader_Coef_A = 1.0;
    float    Sign_Fader_A;

    float Gain_B;
    float Black_Level_B;
    float C_Gain_B;
    float Luma_Level_B;
    float G_Gain_B; // Gamma
    float Opacity_B;
    float Mix_Key_Type_B;
    int B_Fader_State;
    float Key_Start_Time_B;

    float   Fader_Coef_B = 0.0;
    float   Sign_Fader_B;




    float   Downstream_Gain;

    float   Temp_Gain;

    float   Fader_Coef = 1.0 ;
    float   Fade_Rate;
    float   Fade_Direction  = 1.0;
    int     New_Image_Selected = 0;   // finishes after fade limit
    int     Current_A_Or_B = Image_A;   // Fading 0   A 1    B 2
    bool     A_or_B_File_Ext ;


    A_Fader_State = 2;
    B_Fader_State = 0;



    int Start_Key_Time, End_Key_Time;
    bool Start_Key = false;
    bool End_Key = false;



    int     First_Sequence_Image_X;

    int Num_of_Image_Files_Read;


    float Red_62_A, Green_62_A, Blue_62_A;
    float Red_62_B, Green_62_B, Blue_62_B;


    uint8_t Red_62_Combined , Green_62_Combined, Blue_62_Combined ;


    bool    New_Sequenced_Image_Selected;
    int     Current_Sequencer_Selection;
    int     Sequence_Duration_Counter;

    bool    New_Key_Image_Selected;
    int     Current_Key_Sequencer_Selection;

    bool New_Keyboard_Image;
    bool New_Sequenced_Image;

    int     Preview;

    bool    Sequence_On;

    float   Duartion_Mod;

    bool    Stop_Program;



    bool    White_On;
    bool LED_Correction_On;
    float R_Correction;
    float G_Correction;
    float B_Correction;
    float W_Correction;




    int64_t Current_Hour;
    int64_t Current_Minute;
    int64_t Current_Second;
    int64_t Time_Turn_On_Total ;  // 8:30 pm
    int64_t Time_Turn_Off_Total ;            // 3  am
    int64_t Time_Current_Total ;
    int64_t Time_Turn_Color_Total ;



    int64_t Turn_On_Time ;
    int64_t Turn_On_Time_Trigger ;
    int64_t Turn_On_Time_Delayed ;


    int64_t Turn_Color_Time ;
    int64_t Turn_Color_Time_Trigger ;
    int64_t Turn_Color_Time_Delayed ;


    int64_t Turn_Off_Time ;
    int64_t Turn_Off_Time_Trigger ;
    int64_t Turn_Off_Time_Delayed ;


    int64_t Hours_On;  // 24 hour
    int64_t Mins_On ;

    int64_t Hours_Color;  // 24 hour
    int64_t Mins_Color ;
    int64_t Night_Color ;
    int64_t Loop_Back_Image;

    int64_t  Hours_Off ;
    int64_t Mins_Off ;






    char    Path[200] =  "../" ; //    /Salesforce/BuildingLoaderTop/";
    char    File1[200] ;

    char    CurFile[200];

   	int  New_Image_A = 1;
   	int  New_Image_B = 2;

    char c = -1;

    // initialize the frame buffersw
 //   img_in_A.create(IMAGE_ROWS,IMAGE_COLS,CV_8UC3);
    img_proc_A.create(IMAGE_ROWS,IMAGE_COLS,CV_32FC3);  // note tested CV_32FC3 vs CV_16SC3  and they were almost identical
 //   img_in_B.create(IMAGE_ROWS,IMAGE_COLS,CV_8UC3);
    img_proc_B.create(IMAGE_ROWS,IMAGE_COLS,CV_32FC3);




    text_window = Mat::zeros( 180, 1024, CV_8UC3 );
    upper_LP_window = Mat::zeros( 70, 256, CV_8UC3 );



    YUV_Gain_A.create(IMAGE_ROWS,IMAGE_COLS,CV_32FC3);
    YUV_Gain_B.create(IMAGE_ROWS,IMAGE_COLS,CV_32FC3);

    img_proc_A_U.create(IMAGE_ROWS,IMAGE_COLS,CV_32FC3);
    img_proc_B_U.create(IMAGE_ROWS,IMAGE_COLS,CV_32FC3);

    Mat_Sum_U.create(IMAGE_ROWS,IMAGE_COLS,CV_32FC3);


    printf("here0");

    std::string Files_Info[8][10] = {};
    int data_sets;

    std::string  grab_map_info_location_file, sculpture_map_info_location_file, enclosure_map_info_location_file, images_info_location_file, sequencer_info_location_file, movies_location;

    data_sets  = Read_YAML_Data("defaults1.yml", 8, 3,  Files_Info   );


    for(int jjj= 0; jjj < data_sets; jjj++ )
    {
        if( Files_Info[jjj][0].compare("grab_map") == 0)grab_map_info_location_file =  Files_Info[jjj][1] + Files_Info[jjj][2] ;
        else if( Files_Info[jjj][0].compare("sculpture_map") == 0)sculpture_map_info_location_file =  Files_Info[jjj][1] + Files_Info[jjj][2] ;
        else if( Files_Info[jjj][0].compare("enclosure_map") == 0)enclosure_map_info_location_file =  Files_Info[jjj][1] + Files_Info[jjj][2] ;
        else if( Files_Info[jjj][0].compare("images_info_location") == 0)images_info_location_file =  Files_Info[jjj][1] ;
        else if( Files_Info[jjj][0].compare("sequencer_info_location") == 0)sequencer_info_location_file =  Files_Info[jjj][1] ;
        else if( Files_Info[jjj][0].compare("movies_location") == 0)movies_location =  Files_Info[jjj][1] ;
    }

    for(int jjj= 0; jjj < data_sets; jjj++ )
    {
        if( Files_Info[jjj][0].compare("images_info_filename") == 0)images_info_location_file +=   Files_Info[jjj][1]  ;
        else if( Files_Info[jjj][0].compare("sequencer_info_filename") == 0)sequencer_info_location_file +=   Files_Info[jjj][1]  ;
    }


    printf("here1");

    tread = Read_Sample_Point_Location_Array_SS(grab_map_info_location_file,  Sample_Points, Num_Of_Samples_Per_Row);
    tread = Read_Map_SS(sculpture_map_info_location_file,  Sculpture_Map);  // SP6   SP  all points



        cout << "sculpture_map_info_location_file" << grab_map_info_location_file << endl  << endl; // asctime(time_X);
        cout << "sculpture_map_info_location_file" << sculpture_map_info_location_file << endl; // asctime(time_X);






//


    printf("here1");



//    tread = Read_Map_SS(sculpture_map_info_location_file,  Panel_Map_W_Gaps);  // SP6   SP  all points
//    tread = Read_Enclosure_Info_SS(enclosure_map_info_location_file,   Enclosure_Info);

    printf("here2");

    // this section is pathetic make better at some point  do with structure or class

    char Image_File_Names[Max_Num_Of_Image_Files][Max_File_Name_Length];
    float Image_Parameters[Max_Num_Of_Image_Files][Max_Num_Of_Image_Parameters];

    char Sequencr_File_Names[Max_Num_Of_Image_Files][Max_File_Name_Length];
    float Sequencer_Parameters[Max_Num_Of_Image_Files][Max_Num_Of_Image_Parameters];


    tread = Read_Image_Info_SS(images_info_location_file, Image_File_Names, Image_Parameters );

    for(int p=0; p<tread; p++)
    {
        printf("%s  %.2f %.2f  %.2f      \n",   Image_File_Names[p] ,  Image_Parameters[p][Gain_X] , Image_Parameters[p][Black_Level_X], Image_Parameters[p][Chroma_X]);

    }
     printf("\nREAD %d files  \n",  tread);

    Num_of_Image_Files_Read = tread;


    tread = Read_Image_Info_SS(sequencer_info_location_file, Sequencr_File_Names, Sequencer_Parameters );

    for(int p=0; p<tread; p++)
    {
        printf("%s  %.2f %.2f  %.2f     %.1f  \n",   Sequencr_File_Names[p] ,  Sequencer_Parameters[p][Gain_X] , Sequencer_Parameters[p][Black_Level_X], Sequencer_Parameters[p][Chroma_X],
                                   Sequencer_Parameters[p][Duration_X]  );
    }
     printf("\nREAD %d files  \n",  tread);

    int Num_of_Sequencer_Image_Files_Read = tread;


    sleep(1);






     // FTDI_Init();  could not get working

    if(FTDI_On)
    {
        ftStatus = FT_Open(0, &ftHandle);
        if(ftStatus != FT_OK)
        {
            // FT_Open failed return;
            printf("FT_Open FAILED! \r\n");
            exit(0);
        }
        else
        {
            printf("\n made it to FTDI initialize \n");
            //set interface into FT245 Synchronous FIFO mode
            Mode = 0x00; //reset mode
            ftStatus = FT_SetBitMode(ftHandle, Mask, Mode);
            Mode = 0x40; //Sync FIFO mode
            ftStatus = FT_SetBitMode(ftHandle, Mask, Mode);
            if (ftStatus != FT_OK) printf("FT_SetBitMode FAILED! \r\n"); // FT_SetBitMode FAILED!
            FT_SetLatencyTimer(ftHandle, 2);
            FT_SetUSBParameters(ftHandle,0x10000, 0x10000);
            FT_SetFlowControl(ftHandle, FT_FLOW_RTS_CTS, 0x0, 0x0);
            FT_Purge(ftHandle, FT_PURGE_RX);

            ftStatus = FT_GetStatus(ftHandle,&RxBytes,&TxBytes,&EventDWord);
            FT_Purge(ftHandle, FT_PURGE_RX);
        }
    }



    strcpy(File1, Path);
    strcat(File1, "Movies/black-MPZ.mp4");
	cap_B.open(File1);


    strcpy(File1, Path);
    strcat(File1, "Movies/black-MPZ.mp4");
    cap_A.open(File1);


    Video_Loop_Timer.Start_Delay_Timer();
    cnt = 0;


    Points_Visible = 0;

    // temporary keyboard controls
    Pause_A = false;
    Points_Visible_A = false;
    Pause_B = false;
    Points_Visible_B = false;


    img_On_A  = true;
    img_proc_On_A  = true;
    img_On_B  = true;
    img_proc_On_B  = true;



    Reset_All_Filters_A = false;
    Ones3x3_A = true;
    Ones5x5_A = true;
    Ones7x7_A = false;
    Filter_On_Off_A = true;
    Filter_3Taps_On_A = true;
    Ones_On_A = true;

    Reset_All_Filters_B = false;
    Ones3x3_B = true;
    Ones5x5_B = true;
    Ones7x7_B = false;
    Filter_On_Off_B = true;
    Filter_3Taps_On_B = true;
    Ones_On_B = true;


	Gain_A = 0;
	Black_Level_A = .5;
	C_Gain_A = 1.;
	Luma_Level_A = 1.;
	G_Gain_A = .25;

	Gain_B = 0;
	Black_Level_B = .5;
	C_Gain_B = 1.;
	Luma_Level_B = 1.;
	G_Gain_B = .25;


	Downstream_Gain = .1;



		White_On = true;


	LED_Correction_On = true;

   R_Correction =   .9;
   G_Correction =  1.0;
   B_Correction =  .3;
   W_Correction =  1.0;


	Red_62_A = 0;
	Green_62_A = 0;
	Blue_62_A = 0;
    Red_62_B = 0;
    Green_62_B = 0;
    Blue_62_B = 0;


    LP_62_On  =true;
    LP_63_On = false;
    TRX_On   = false ;


    Video_On = true;

    Seam_Flip = false;





    Sequence_Duration_Counter = 0;
    Current_Sequencer_Selection = 0;
    New_Sequenced_Image_Selected = false;

    Sequence_On = false;

    bool Sequence_On_Delayed = false;


    Stop_Program = false;


    loopcnt = 0;



    //time_t time_time;
    //struct tm * time_X;




    // Applying time()
    time (&time_time);

    // Using localtime()
    time_X = localtime(&time_time);

    Current_Hour = time_X->tm_hour;
    Current_Minute = time_X->tm_min;
    Current_Second = time_X->tm_sec;

    cout << "Current Day, Date and Time is = " << Current_Hour << " " << Current_Minute; // asctime(time_X);



    Hours_On     = 19; // 20;  // 24 hour  8:30
    Mins_On       = 45;
    Hours_Color   = 01;      // 3 am
    Mins_Color   =  01;
    Hours_Off    = 6;      // 3 am
    Mins_Off     = 15;


    Night_Color = 4; // 11;
    Loop_Back_Image = 1;
    First_Sequence_Image_X = 0;



    Time_Turn_On_Total =  Hours_On * 3600 + Mins_On * 60  ;  // 8:30 pm
    Time_Turn_Color_Total = Hours_Color * 3600 +  Mins_Color * 60  ;            // assume after midnight
    Time_Turn_Off_Total = Hours_Off * 3600 +  Mins_Off * 60  ;            // assume after midnight


    Time_Current_Total  =  Current_Hour * 3600 + Current_Minute * 60 + Current_Second;


    // if the program is started after the turn on time then the sequencer is enabled
    if(Time_Current_Total >=  Time_Turn_On_Total)
    {
        Sequence_On = true;
        First_Sequence_Image_X = 0;
    }


    while( (cnt!=4) || !Stop_Program)
    {

        if(FTDI_On)FT_GetStatus(ftHandle,&RxBytes,&TxBytes,&EventDWord);

        if ((RxBytes >= 64) || !FTDI_On)
        {
        if(FTDI_On)
        {
            ftStatus = FT_Read(ftHandle,RxBuffer,RxBytes,&BytesReceived);
            if(BytesReceived != 64)printf("\n\n XXXXXXX MISSED FRAME XXXXXXXxx  BytesReceived %ld \n\n", BytesReceived  );
        }


        // write to xilinx stuff

            //  IMPORTANT NOTE   MISSION STREET XILINX IS SENDING AN UPDATE EVERY 1/60 of a second  so ignore every other one
            // OR change the xilinx

            if( ( (RxBytes == 64) || !FTDI_On) )
            {

                New_1.Start_Delay_Timer();
                New_2.Start_Delay_Timer();
                New_3.Start_Delay_Timer();
                New_4.Start_Delay_Timer();
                New_5.Start_Delay_Timer();
                New_6.Start_Delay_Timer();
                New_7.Start_Delay_Timer();
                New_8.Start_Delay_Timer();
                New_9.Start_Delay_Timer();
                New_10.Start_Delay_Timer();
                New_11.Start_Delay_Timer();
                New_12.Start_Delay_Timer();
                // check the program cycle time  (total interval is 33.3 ms)
                Program_Timer.End_Delay_Timer();
                Program_Timer.Start_Delay_Timer();
                // Check the loop time not including the tick from hardware
                Within_Loop_Timer.Start_Delay_Timer();
                loopcnt++;


         //       ftStatus = FT_GetStatus(ftHandle,&RxBytes,&TxBytes,&EventDWord);
         //       if((ftStatus == FT_OK) && (TxBytes == 0))ftStatus = FT_Write(ftHandle, (char*)Grab_Buffer_RGBW, Buffer_W_Gaps_Size_RGBW_Bytes_Extra, &BytesWritten);

                Light_Sensor = (uint16_t)RxBuffer[0] * 256 + (uint16_t)RxBuffer[1] ;


                /// time processing for triggers
                time (&time_time);
                time_X = localtime(&time_time);
                Current_Hour = time_X->tm_hour;
                Current_Minute = time_X->tm_min;
                Current_Second = time_X->tm_sec;
                Time_Current_Total = Current_Hour * 3600 + Current_Minute * 60 + Current_Second;
                /// Turn On Time passed
                Turn_On_Time =   (Time_Current_Total ==  Time_Turn_On_Total  ) ;
                Turn_On_Time_Trigger = (!Turn_On_Time_Delayed & Turn_On_Time);
                Turn_On_Time_Delayed = Turn_On_Time;
                /// Turn On Color time passed
                Turn_Color_Time =   (Time_Current_Total ==  Time_Turn_Color_Total  ) ;
                Turn_Color_Time_Trigger = (!Turn_Color_Time_Delayed & Turn_Color_Time);
                Turn_Color_Time_Delayed = Turn_Color_Time;
                /// Turn Off Time passed
                Turn_Off_Time =   (Time_Current_Total ==  Time_Turn_Off_Total  ) ;
                Turn_Off_Time_Trigger = (!Turn_Off_Time_Delayed & Turn_Off_Time);
                Turn_Off_Time_Delayed = Turn_Off_Time;



                /// automatic timing triggers
                if(Turn_On_Time_Trigger)
                {
                    Sequence_On = true;
                    First_Sequence_Image_X = 0;
                }
                else if(Turn_Color_Time_Trigger)
                {
                    Sequence_On = false;
                    New_Image_Selected = Night_Color;
                }
                else if(Turn_Off_Time_Trigger)
                {
                    Sequence_On = false;
                    New_Image_Selected = 0;
                }



                if(Sequence_On && !Sequence_On_Delayed)  /// Sequence just initiated manually or by timing trigger
                {

                    Sequence_Duration_Counter   = 0;
                    Current_Sequencer_Selection = First_Sequence_Image_X;  /// which image is 1st  (add to yml file)
                    New_Sequenced_Image_Selected = true;
                }
                else if(Sequence_On)        /// Sequence already on
                {
                    Duartion_Mod = (Test_Mode >= 2) ?   5. : Sequencer_Parameters[Current_Sequencer_Selection][Duration_X] ;
                    Sequence_Duration_Counter++;


                    if((Sequence_Duration_Counter/30.) >= Duartion_Mod)
                    {
                        Current_Sequencer_Selection++;
                        New_Sequenced_Image_Selected = true;
                        if(Current_Sequencer_Selection > Num_of_Sequencer_Image_Files_Read)Current_Sequencer_Selection = Loop_Back_Image;  /// which image to loop to  (add to yml file)
                    }



                    if(Sequencer_Parameters[Current_Sequencer_Selection][Mix_Type_X]   == 1 )  ///  new sequenced is a background
                    {
                        Start_Key_Time =  30 * Sequencer_Parameters[Current_Sequencer_Selection + 1][Key_Start_Time_X] ;
                        End_Key_Time   =  Start_Key_Time  + 30 * Sequencer_Parameters[Current_Sequencer_Selection + 1][Duration_X] ;

                        if(Sequence_Duration_Counter ==  Start_Key_Time)
                        {
                           // printf("\n made it to start key\n\n");
                            Current_Key_Sequencer_Selection = Current_Sequencer_Selection + 1;
                            New_Key_Image_Selected = true;
                            Start_Key = true;
                        }
                        else Start_Key = false;

                        if(Sequence_Duration_Counter ==  End_Key_Time)
                        {
                            New_Key_Image_Selected = true;
                            End_Key = true;
                        }
                        else  End_Key = false;
                    }
                }


                // look for change in Sequence Enabled
                Sequence_On_Delayed = Sequence_On;


                New_Keyboard_Image = ( ( New_Image_Selected < Num_of_Image_Files_Read) && ( New_Image_Selected >= 0) );
                New_Sequenced_Image = (New_Sequenced_Image_Selected  &&  (Current_Sequencer_Selection < Num_of_Sequencer_Image_Files_Read) ) ;



                New_1.End_Delay_Timer();



                /**********************************************  IMAGE A STUFF  **********************************/
                /**********************************************  IMAGE A STUFF  **********************************/


                /*************** CODE FOR WHEN A NEW IMAGE IS SELECTED  ****************/
            //    if (    (  New_Keyboard_Image || New_Sequenced_Image )  && (Current_A_Or_B == Image_B) )
              //  if (    (  New_Keyboard_Image || New_Sequenced_Image )  && ( (B_Fader_State == Image_Full_On)  || (A_Fader_State == Image_Partial_On)  || (A_Fader_State == Image_Partial_On_Negative)  )  )

                if    ( (  New_Keyboard_Image || New_Sequenced_Image )  && (B_Fader_State == Image_Full_On)  )
                {

                  //  printf("\n made it to A NEW  \n\n");

                  //  printf("\n made it to HERE1  \n"); // , Current_Sequencer_Selection);

                    if(New_Sequenced_Image_Selected && ( (A_Fader_State == 8) || (A_Fader_State == 12)   )  )Current_Sequencer_Selection++;
                    if(Current_Sequencer_Selection > Num_of_Sequencer_Image_Files_Read)Current_Sequencer_Selection = Loop_Back_Image;

                    if(New_Sequenced_Image_Selected)New_Image_A = Current_Sequencer_Selection;
                    else New_Image_A = New_Image_Selected;

                    strcpy(File1, Path);
                    strcat(File1, "Movies/");

                    if(New_Sequenced_Image_Selected)strcpy(CurFile,  Sequencr_File_Names[New_Image_A] ) ;
                    else strcpy(CurFile,  Image_File_Names[New_Image_A] ) ;

                    strcat(File1, CurFile);
              //      strcat(File1,  ".mov" ) ;

                    if(New_Sequenced_Image_Selected)
                    {
                        Gain_A          = Sequencer_Parameters[New_Image_A][Gain_X];
                        C_Gain_A        = Sequencer_Parameters[New_Image_A][Chroma_X];
                        Black_Level_A   = Sequencer_Parameters[New_Image_A][Black_Level_X];
                        G_Gain_A        = Sequencer_Parameters[New_Image_A][Gamma_X];
                        Opacity_A       = Sequencer_Parameters[New_Image_A][Opacity_X] ;
                        Mix_Key_Type_A  = Sequencer_Parameters[New_Image_A][Mix_Type_X] ;
                        Key_Start_Time_A  = Sequencer_Parameters[New_Image_A+1][Key_Start_Time_X] ;
                    }
                    else
                    {
                        Gain_A          = Image_Parameters[New_Image_A][Gain_X];
                        C_Gain_A        = Image_Parameters[New_Image_A][Chroma_X];
                        Black_Level_A   = Image_Parameters[New_Image_A][Black_Level_X];
                        G_Gain_A        = Image_Parameters[New_Image_A][Gamma_X];
                    }

                    if(cap_A.isOpened())cap_A.release();
                    cap_A.open(File1);
                    if(!cap_A.isOpened()){  // Check if capture opened successfully
                    cout << "Error opening video stream or file  44444"   << endl;
                    return -1;
                    }

                    // Start the Fader to A
                    Preview = Image_A ;
                    A_or_B_File_Ext = true;

                    A_Fader_State = Image_Fading_To_Full_On;
                    B_Fader_State = Image_Fading_To_Off_From_Full_On;
                    Sequence_Duration_Counter = 0;


                    New_Image_Selected = 1000;
                    New_Sequenced_Image_Selected = false;

                }
                /************* CODE FOR WHEN A NEW IMAGE IS SELECTED DONE ********/



                else if((New_Key_Image_Selected) && (B_Fader_State == Image_Full_On)  )
                {

                   // printf("\n made it to A NEW KEY A \n\n");

                  //  printf("\n made it to HERE1  \n"); // , Current_Sequencer_Selection);

                    if(Start_Key)
                    {
                        New_Image_A = Current_Key_Sequencer_Selection;

                        strcpy(File1, Path);
                        strcat(File1, "Movies/");
                        strcpy(CurFile,  Sequencr_File_Names[New_Image_A] ) ;
                        strcat(File1, CurFile);

                        Gain_A          = Sequencer_Parameters[New_Image_A][Gain_X];
                        C_Gain_A        = Sequencer_Parameters[New_Image_A][Chroma_X];
                        Black_Level_A   = Sequencer_Parameters[New_Image_A][Black_Level_X];
                        G_Gain_A        = Sequencer_Parameters[New_Image_A][Gamma_X];
                        Opacity_A       = Sequencer_Parameters[New_Image_A][Opacity_X] ;
                        Mix_Key_Type_A  = Sequencer_Parameters[New_Image_A][Mix_Type_X] ;
                        Key_Start_Time_A  = Sequencer_Parameters[New_Image_A][Key_Start_Time_X] ;

                        if(cap_A.isOpened())cap_A.release();
                        cap_A.open(File1);
                        if(!cap_A.isOpened()){  // Check if capture opened successfully
                        cout << "Error opening video stream or file  44444"   << endl;
                        return -1;
                        }

                        if ( Mix_Key_Type_A == 3 ) A_Fader_State = Image_Fading_To_Partial_On_Negative;    // fade to a negative limit
                        else if(   Mix_Key_Type_A == 2 ) A_Fader_State = Image_Fading_To_Partial_On ;
                    }

                    else if(End_Key)
                    {
                        if( Mix_Key_Type_A == 3 ) A_Fader_State = Image_Fading_To_Off_From_Partial_On_Negative;
                        else if(  Mix_Key_Type_A == 2 ) A_Fader_State = Image_Fading_To_Off_From_Partial_On;
              //          Current_Sequencer_Selection++;  // skip the keyed image
                    }

                    Preview = Image_A ;
                    A_or_B_File_Ext = true;
                    New_Key_Image_Selected = false;
                }





                /*********************  ACCESSED EVERY FRAME *********************/
                if(!Pause_A)cap_A >> img_in_A ; // img_in_1;  // have to cimage_proc_1_sampledapture at 8 bit
                if (img_in_A.empty())      // If the frame is empty, break immediately
                {
                    cout << "Error opening video stream or file  5555"   << endl;
                    break;
                }

                // inversion for keying
                if( (Mix_Key_Type_A  == 3) & Sequence_On )bitwise_not ( img_in_A, img_in_A );

                // loop the video file
                if (cap_A.get(CAP_PROP_POS_FRAMES) == (cap_A.get(CAP_PROP_FRAME_COUNT)  ) )
                {
                    cap_A.set(CAP_PROP_POS_FRAMES, 0);
                    // check the video loop time
                    Video_Loop_Timer.End_Delay_Timer();
                    Video_Loop_Timer.Start_Delay_Timer();
                    printf("\nVideo Loop Time %6.2f \n\n",Video_Loop_Timer.time_delay/1000);
                }


                if(Seam_Flip)Shift_Image_Horizontal(img_in_A, IMAGE_COLS/2);   // Change_Seam(img_in_A, 1);

                // 8 bit IN  32 float OUT  Filter Gain and Black Level
                img_in_A.copyTo(img_in_A_U);  // UMatOnes_On_A

                Process_Image_U(img_in_A_U, img_in_A_U, YUV_Gain_A, Filter_On_Off_A,
                            F_1p1_A, F_2p2_A,  F_3p3_A,  F_4p4_A,  Filter_3Taps_On_A,
                            Ones2x2_A,  Ones3x3_A,  Ones4x4_A,  Ones5x5_A,  Ones6x6_A,  Ones7x7_A,  Ones_On_A,
                            Gain_A, Black_Level_A, C_Gain_A, G_Gain_A); // G_Gain_A  ) ;

                img_in_A_U.copyTo(img_proc_A);  // convert back to Mat
                New_2.End_Delay_Timer();
                /************************  ACCESSED EVERY FRAME ***********************/





                /************************************  IMAGE A STUFF DONE ***********************************/
                /************************************  IMAGE A STUFF DONE ***********************************/






                /********************************************  IMAGE B STUFF  **********************************/
                /********************************************  IMAGE B STUFF  **********************************/


                /*************** CODE FOR WHEN A NEW IMAGE IS SELECTED  ****************/
                // if (    (  New_Keyboard_Image || New_Sequenced_Image )  && (Current_A_Or_B == Image_A) )
                if (    (  New_Keyboard_Image || New_Sequenced_Image )  &&  ( (A_Fader_State == Image_Full_On)  ) )
                {


                  //  printf("\n made it to B NEW  \n\n");


                    if(New_Sequenced_Image_Selected && ( (B_Fader_State == 8) || (B_Fader_State == 12)  )  )Current_Sequencer_Selection++;
                    if(Current_Sequencer_Selection > Num_of_Sequencer_Image_Files_Read)Current_Sequencer_Selection = Loop_Back_Image;


                    if(New_Sequenced_Image_Selected)New_Image_B = Current_Sequencer_Selection;
                    else New_Image_B = New_Image_Selected;
                    if(cap_B.isOpened())cap_B.release();

                    strcpy(File1, Path);
                    strcat(File1, "Movies/");

                    if(New_Sequenced_Image_Selected)strcpy(CurFile,  Sequencr_File_Names[New_Image_B] ) ;
                    else strcpy(CurFile,  Image_File_Names[New_Image_B] ) ;

                    strcat(File1, CurFile);
              //      strcat(File1,  ".mov" ) ;

                    if(New_Sequenced_Image_Selected)
                    {
                        Gain_B          = Sequencer_Parameters[New_Image_B][Gain_X];
                        C_Gain_B        = Sequencer_Parameters[New_Image_B][Chroma_X];
                        Black_Level_B   = Sequencer_Parameters[New_Image_B][Black_Level_X];
                        G_Gain_B        = Sequencer_Parameters[New_Image_B][Gamma_X];
                        Opacity_B       = Sequencer_Parameters[New_Image_B][Opacity_X] ;
                        Mix_Key_Type_B  = Sequencer_Parameters[New_Image_B][Mix_Type_X] ;
                        Key_Start_Time_B  = Sequencer_Parameters[New_Image_B][Key_Start_Time_X] ;
                    }
                    else
                    {
                        Gain_B      = Image_Parameters[New_Image_B][Gain_X];
                        C_Gain_B   = Image_Parameters[New_Image_B][Chroma_X];
                        Black_Level_B   = Image_Parameters[New_Image_B][Black_Level_X];
                        G_Gain_B        = Image_Parameters[New_Image_B][Gamma_X];
                    }

                    if(cap_B.isOpened())cap_B.release();
                    cap_B.open(File1);
                    if(!cap_B.isOpened()){  // Check if capture opened successfully
                    cout << "Error opening video stream or file  44444"   << endl;
                    return -1;
                    }

                    // Start the Fader to A

                    Preview = Image_B ;
                    A_or_B_File_Ext = false;

                    B_Fader_State = Image_Fading_To_Full_On;
                    A_Fader_State = Image_Fading_To_Off_From_Full_On;
                    Sequence_Duration_Counter = 0;

                    New_Image_Selected = 1000;
                    New_Sequenced_Image_Selected = false;
                }
                /************** CODE FOR WHEN A NEW IMAGE IS SELECTED DONE ********/


                else if((New_Key_Image_Selected) && (A_Fader_State == Image_Full_On)  )
                {

                  //  printf("\n made it to B NEW KEY B \n\n");

                  //  printf("\n made it to HERE1  \n"); // , Current_Sequencer_Selection);

                    if(Start_Key)
                    {
                        New_Image_B = Current_Key_Sequencer_Selection;

                        strcpy(File1, Path);
                        strcat(File1, "Movies/");
                        strcpy(CurFile,  Sequencr_File_Names[New_Image_B] ) ;
                        strcat(File1, CurFile);

                        Gain_B          = Sequencer_Parameters[New_Image_B][Gain_X];
                        C_Gain_B        = Sequencer_Parameters[New_Image_B][Chroma_X];
                        Black_Level_B   = Sequencer_Parameters[New_Image_B][Black_Level_X];
                        G_Gain_B        = Sequencer_Parameters[New_Image_B][Gamma_X];
                        Opacity_B       = Sequencer_Parameters[New_Image_B][Opacity_X] ;
                        Mix_Key_Type_B  = Sequencer_Parameters[New_Image_B][Mix_Type_X] ;
                        Key_Start_Time_B  = Sequencer_Parameters[New_Image_B][Key_Start_Time_X] ;

                        if(cap_B.isOpened())cap_B.release();
                        cap_B.open(File1);
                        if(!cap_B.isOpened()){  // Check if capture opened successfully
                        cout << "Error opening video stream or file  44444"   << endl;
                        return -1;
                        }

                        if(   Mix_Key_Type_B == 3 ) B_Fader_State = Image_Fading_To_Partial_On_Negative;    // fade to a negative limit
                        else if( Mix_Key_Type_B == 2 )  B_Fader_State = Image_Fading_To_Partial_On ;
                    }

                    else if(End_Key)
                    {
                        if(   Mix_Key_Type_B == 3 ) B_Fader_State = Image_Fading_To_Off_From_Partial_On_Negative;
                        else if(  Mix_Key_Type_B == 2 ) B_Fader_State = Image_Fading_To_Off_From_Partial_On;
                  //      Current_Sequencer_Selection++;  // skip the keyed image
                    }

                    New_Key_Image_Selected = false;
                    Preview = Image_B ;
                    A_or_B_File_Ext = false;
                }




                /**************************  ACCESSED EVERY FRAME ***************/
                if(!Pause_B)cap_B >> img_in_B ; // img_in_1;  // have to cimage_proc_1_sampledapture at 8 bit
                if (img_in_B.empty())      // If the frame is empty, break immediately
                {
                    cout << "Error opening video stream or file  5555"   << endl;
                    break;
                }

                // inversion for keying
                if( (Mix_Key_Type_B  == 3) & Sequence_On )bitwise_not ( img_in_B, img_in_B );

                // loop the video file
                if (cap_B.get(CAP_PROP_POS_FRAMES) == (cap_B.get(CAP_PROP_FRAME_COUNT)  ) )
                {
                    cap_B.set(CAP_PROP_POS_FRAMES, 0);
                    // check the video loop time
                    Video_Loop_Timer.End_Delay_Timer();
                    Video_Loop_Timer.Start_Delay_Timer();
                    printf("\nVideo Loop Time %6.2f \n\n",Video_Loop_Timer.time_delay/1000);
                }


                if(Seam_Flip)Shift_Image_Horizontal(img_in_B, IMAGE_COLS/2);   // Change_Seam(img_in_B, 1);

                // 8 bit IN  32 float OUT  Filter Gain and Black Level
                img_in_B.copyTo(img_in_B_U);  // UMatOnes_On_B

                Process_Image_U(img_in_B_U, img_in_B_U, YUV_Gain_B, Filter_On_Off_B,
                            F_1p1_B, F_2p2_B,  F_3p3_B,  F_4p4_B,  Filter_3Taps_On_B,
                            Ones2x2_B,  Ones3x3_B,  Ones4x4_B,  Ones5x5_B,  Ones6x6_B,  Ones7x7_B,  Ones_On_B,
                            Gain_B, Black_Level_B, C_Gain_B, G_Gain_B); // G_Gain_B  ) ;



                img_in_B_U.copyTo(img_proc_B);  // convert back to Mat


                New_3.End_Delay_Timer();

                /**************************  ACCESSED EVERY FRAME DONE *********/


                /************************************  IMAGE B STUFF DONE  ***********************************/
                /************************************  IMAGE B STUFF DONE  ***********************************/





                /************************************  FADER  ***********************************/

                Fade_Rate = (Test_Mode >= 1) ? .02 : .0035;  // .01 : .0035;

               //     if(!Fader_Stopped)Fader_Main_Orig( Current_A_Or_B,  Fader_Coef, Fade_Direction, Fade_Rate);



           //     if(!Fader_Stopped)Fader_Main_Rev2( Current_A_Or_B,  Fader_Coef_A, Fader_Coef_B ,
        //                                            Fade_Direction, Fade_Rate,  Mix_Key_Type_A, Mix_Key_Type_B, Opacity_A, Opacity_B, Sign_Fader_A, Sign_Fader_B);

        //        addWeighted( img_in_A_U,  Sign_Fader_A*Fader_Coef_A,   img_in_B_U,  Sign_Fader_B*Fader_Coef_B, 0.0, Mat_Sum_U);



                Fader_Main_Rev3( A_Fader_State, B_Fader_State, Fader_Coef_A, Fader_Coef_B, Fade_Rate,  Opacity_A, Opacity_B );

                addWeighted( img_in_A_U,  Fader_Coef_A,   img_in_B_U,  Fader_Coef_B, 0.0, Mat_Sum_U);



                Mat_Sum_U.copyTo(Mat_Sum);  // convert back to Mat


                /************************************  FADER DONE  ***********************************/





                // for full size displayOnes_On_A
                Mat_Sum.convertTo(Mat_Sum_Display, CV_8UC3);

                // output gain
                Temp_Gain = Video_On ? Downstream_Gain : 0 ;
                Mat_Sum *= Temp_Gain ;

                // // for small output display
                resize(Mat_Sum, Mat_Sum_sampled, Size(), .33, .33, INTER_NEAREST);
                Mat_Sum_sampled.convertTo(Mat_Sum_sampled, CV_8UC3);




                /************************************  SUBSAMPLE AND ADD HEADERS AND DOWNLOAD  *********************/

                // convert to 16 bit for mapping and sending to Hardware
                Mat_Sum.convertTo(img_proc_u16_A, CV_16UC3,256);  // .35 ms

                // sub samples based on map and converts to RGBW
                // WOULD UMat speed this up ??
                //    Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_U(img_proc_1_u16_U, Grab_Buffer_RGBW, Sample_Points, Num_Of_Samples_Per_Row);  // .31 ms
               // Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert(img_proc_u16_A, Grab_Buffer_RGBW, Sample_Points, Num_Of_Samples_Per_Row);  // .31 ms

              //  Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_rev2(img_proc_u16_A, Grab_Buffer_RGBW, Sample_Points, Num_Of_Samples_Per_Row, 5);


                /// int16 version
                Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_rev3(img_proc_u16_A, Grab_Buffer_RGBW, Sample_Points, Num_Of_Samples_Per_Row, 5,
                                                                        White_On, LED_Correction_On, R_Correction, G_Correction, B_Correction, W_Correction);


                Panel_Mapper(Grab_Buffer_RGBW, Map_Buffer_RGBW, Sculpture_Map, 4  );



                uint8_t shortloopcnt;
                shortloopcnt++;
                if(shortloopcnt >= 64)shortloopcnt = 0;



           //     New_4.End_Delay_Timer();

                // testfastest
                // if(Grab == true)Grab_Test(  Grab_Buffer, Map_Buffer, Map_Buffer_W_Gaps, Map_Buffer_W_Gaps_RGBW, Grab );
                // Grab = false;


                /// moved to top of code
                ftStatus = FT_GetStatus(ftHandle,&RxBytes,&TxBytes,&EventDWord);
                if((ftStatus == FT_OK) && (TxBytes == 0))ftStatus = FT_Write(ftHandle, (char*)Map_Buffer_RGBW, Buffer_W_Gaps_Size_RGBW_Bytes_Extra, &BytesWritten);



                /************************************  CAPTURE AND ADD HEADERS DONE ***********************************/


                if(Preview == Image_A)  img_proc_A.convertTo(img_proc_A, CV_8UC3);
                else img_proc_B.convertTo(img_proc_B, CV_8UC3);
                Display_Mat_Select( "Preview", img_proc_A, img_proc_B,  (Preview == Image_A)  , 1, Preview_Location_X, Preview_Location_Y);




              //   if(Points_Visible > 0)Add_Visible_Sample_Locations_From_CSV_Map_2(Mat_Sum_Display, Sample_Points, Num_Of_Samples_Per_Row, Points_Visible);

                if(Points_Visible > 0)Add_Visible_Sample_Locations_From_CSV_Map_rev3(Mat_Sum_Display, Sample_Points, Num_Of_Samples_Per_Row, Points_Visible, 5);

               Display_Mat("Final_Output",Mat_Sum_Display,Output_Location_X,Output_Location_Y);

                New_4.End_Delay_Timer();


               Display_Mat("Final Output Small",Mat_Sum_sampled,Output_Small_Location_X,Output_Small_Location_Y);


                New_5.End_Delay_Timer();




                Display_Text_Mat("text window", text_window, 20, 680,
                    Pause_A,  Gain_A,   C_Gain_A ,  Black_Level_A, G_Gain_A,   F_1p1_A,   F_2p2_A,   F_3p3_A,   F_4p4_A,   Ones2x2_A,   Ones3x3_A,   Ones4x4_A,   Ones5x5_A,   Ones6x6_A,   Ones7x7_A,
                    Pause_B,   Gain_B,   C_Gain_B , Black_Level_B, G_Gain_B,  F_1p1_B,   F_2p2_B,   F_3p3_B,   F_4p4_B,   Ones2x2_B,   Ones3x3_B,   Ones4x4_B,   Ones5x5_B,   Ones6x6_B,   Ones7x7_B,
                    CurFile,   Downstream_Gain ,   Video_On, Fader_Coef,   Fade_Rate,  Current_A_Or_B,    A_or_B_File_Ext,
                    Test_Mode,   Sequence_On,  Current_Sequencer_Selection ,  Sequence_Duration_Counter ,    Duartion_Mod,
                    Constellation_On,   Lumen_Pulse_Upper_On,   LP_62_On,   LP_63_On,   TRX_On,  Red_62_Combined,  Green_62_Combined,  Blue_62_Combined,
                    Current_Hour,  Current_Minute,  Current_Second,   Hours_On,  Mins_On,   Hours_Off,  Mins_Off,   max_stored, Light_Sensor, White_On, LED_Correction_On);







                New_6.End_Delay_Timer();

                // Press  ESC on keyboard to exit etc                if(cnt == 4)printf("   VEC   %d  %d  %d\n",Pixel_Vec[0], Pixel_Vec[1], Pixel_Vec[2]);
                 c=(char)waitKey(1);  // part of openCV needs to be at least 1

                New_7.End_Delay_Timer();


                int dummy;

                if(Stop_Program);


              //   else if(c==27)break;

                else    KeyBoardInput(  c,

                A_or_B_File_Ext, New_Image_Selected, Points_Visible,  Downstream_Gain, Sequence_On,  Stop_Program, First_Sequence_Image_X,  Test_Mode,   Video_On,

                Pause_A,  Gain_A,  Black_Level_A,  C_Gain_A, G_Gain_A,

                img_On_A,   img_proc_On_A,  Show_Proc_A,

                Reset_All_Filters_A,    Ones_On_A,  Filter_3Taps_On_A,
                Ones2x2_A,       Ones3x3_A,  Ones4x4_A,  Ones5x5_A,  Ones6x6_A,  Ones7x7_A,
                F_1p1_A,           F_2p2_A,      F_3p3_A,      F_4p4_A,

                Pause_B,           Gain_B,           Black_Level_B,    C_Gain_B, G_Gain_B,

                img_On_B,          img_proc_On_B,     Show_Proc_B,

                Reset_All_Filters_B,    Ones_On_B,  Filter_3Taps_On_B,
                Ones2x2_B,       Ones3x3_B,  Ones4x4_B,  Ones5x5_B,  Ones6x6_B,  Ones7x7_B,
                F_1p1_B,           F_2p2_B,      F_3p3_B,      F_4p4_B,

                Constellation_On,  Lumen_Pulse_Upper_On, LP_62_On, LP_63_On, TRX_On, Seam_Flip,

                Fader_Stopped,  White_On, LED_Correction_On
               );


                Within_Loop_Timer.End_Delay_Timer();

                // print debug stuff
                cnt++;
                if(cnt >= 66)cnt=0;

                New_8.End_Delay_Timer();

                if(cnt == 5)
                {
                    d1  = New_1.time_delay;
                    d2  = New_2.time_delay - New_1.time_delay;
                    d3  = New_3.time_delay - New_2.time_delay;
                    d4  = New_4.time_delay - New_3.time_delay;
                    d5  = New_5.time_delay - New_4.time_delay;
                    d6  = New_6.time_delay - New_5.time_delay;
                    d7  = New_7.time_delay - New_6.time_delay;
                    d8  = New_8.time_delay - New_7.time_delay;
                    d9  = New_9.time_delay - New_8.time_delay;
                    d10 = New_10.time_delay - New_9.time_delay;
                    d11  = New_11.time_delay - New_10.time_delay;
                    d12  = New_12.time_delay - New_11.time_delay;
                    max_Within_Loop_Timer = Within_Loop_Timer.time_delay;
                    min_Within_Loop_Timer = Within_Loop_Timer.time_delay;
                }
                else if(Within_Loop_Timer.time_delay > max_Within_Loop_Timer)
                {
                    d1  = New_1.time_delay;
                    d2  = New_2.time_delay - New_1.time_delay;
                    d3  = New_3.time_delay - New_2.time_delay;
                    d4  = New_4.time_delay - New_3.time_delay;
                    d5  = New_5.time_delay - New_4.time_delay;
                    d6  = New_6.time_delay - New_5.time_delay;
                    d7  = New_7.time_delay - New_6.time_delay;
                    d8  = New_8.time_delay - New_7.time_delay;
                    d9  = New_9.time_delay - New_8.time_delay;
                    d10 = New_10.time_delay - New_9.time_delay;
                    d11  = New_11.time_delay - New_10.time_delay;
                    d12  = New_12.time_delay - New_11.time_delay;
                    max_Within_Loop_Timer = Within_Loop_Timer.time_delay;
                }
                else if(Within_Loop_Timer.time_delay < min_Within_Loop_Timer)
                {
                    min_Within_Loop_Timer = Within_Loop_Timer.time_delay;
                }

                if(cnt != 4)time_accum += Within_Loop_Timer.time_delay;
                else if(cnt == 4)
                {
                    time_accum_avg = time_accum / 66. ;
                    time_accum = Within_Loop_Timer.time_delay ;
                }

                if(cnt == 4)
                {
                    printf("%.2f   %.1f | %.1f | %.1f   %.1f  %.1f  %.1f  %.1f  %.1f  %.1f  %.1f  %.1f  %.1f \n",
                                Program_Timer.time_delay, max_Within_Loop_Timer, min_Within_Loop_Timer, time_accum_avg, d1, d2, d3, d4, d5, d6, d7, d8, New_8.time_delay  );

            //       printf("Opacity %.1f %.1f    mix %.1f %.1f   faders %.1f %.1f   signs %.1f %.1f  \n", Opacity_A, Opacity_B, Mix_Key_Type_A, Mix_Key_Type_B, Fader_Coef_A, Fader_Coef_B, Sign_Fader_A, Sign_Fader_B  );
           //         printf("Fade_Direction %.1f   AorB %d    FA %.1f   FB %.1f  \n", Fade_Direction, Current_A_Or_B, Fader_Coef_A, Fader_Coef_B );
           //         printf("A_Fader_State = %d   B_Fader_State = %d \n\n", A_Fader_State, B_Fader_State);

                    max_stored = max_Within_Loop_Timer;
                }


            }

        }

    }

      // When everything done, release the video capture object
      cap_A.release();
      cap_B.release();

      // Closes all the frames
      destroyAllWindows();

      FT_Close(ftHandle); // !!

      return 0;
}




// misc things tried !!!!




    // pthread_t t1,t2;


/*
        for(int yy=0; (yy < 280); yy++)
            for(int xx=0; (xx < 1024); xx++)
            {
                image_proc_1_sampled.at<Vec3f>(Point(xx,yy)) =  img_proc_1.at<Vec3f>(Point(xx,yy)) ;
            }
*/




/*
	int err = pthread_create(&thread1, NULL, &threadFunc, NULL);
	// Check if thread is created sucessfuly
	if (err)
	{img_1_On
		std::cout << "Thread creation failed : " << strerror(err);
		return err;
	}
	else
		std::cout << "Thread Created with ID : " << thread1 << std::endl;
*/


         //        Alpha1.convertTo(Alpha1, CV_32FC3); //
         //       multiply(Alpha1, img_proc_2, img_proc_2);
        //        img_proc_2.convertTo(img_proc_2, -1, 1./255., 0);


                //  double resize to give approx building resolution
                // resize(img_proc_1, img_proc_1, Size(), 0.25, 0.2p5, INTER_LINEAR  );
                // resize(img_proc_1, img_proc_1, Size(), 4, 4, INTER_NEAREST);
                // display the filtered image

                // display the raw input image
                //resize(frame2, frame2, Size(), 0.5, 0.5, INTER_LINEAR  );
                //resize(frame2, frame2, Size(), 2, 2, INTER_NEAREST  );

/*
                // filter options -- faster to do the filters inline ???
                img_in_1.convertTo(img_proc_1, CV_32FC3);
                Filter_3Taps(img_proc_1, Filter_1p1, Filter_2p2, Filter_3p3, Filter_4p4, Filter_3Taps_On);
                Filter_Blurs(img_proc_1, Ones2x2, Ones3x3, Ones4x4, Ones5x5, Ones6x6, Ones7x7, Ones_On);
                img_proc_1.convertTo(img_proc_1, -1, gain, black_level);
*/

                // gamma curve check,
                //multiply(img_in_1F,img_in_1F,img_in_1F);


                // convert to floatw
          //      img_in_2.convertTo(img_proc_2, CV_32FC3);

                // filter options -- faster to do the filters inline ???
                // Filter_Combo(img_proc_2, Filter_On_Off, Filter_2p2, Filter_3p3, Filter_4p4);

         //       Filter_3Taps(img_proc_2, Filter_1p1, Filter_2p2, Filter_3p3, Filter_4p4, Filter_3Taps_On);
         //       Filter_Blurs(img_proc_2, Ones2x2, Ones3x3, Ones4x4, Ones5x5, Ones6x6, Ones7x7, Ones_On);

                //  contrast brightness
         //       float gain2 = 1.1;
         //       float black_level2 = -32;
         //       img_proc_2.convertTo(img_proc_2, -1, gain2, black_level2);
