

#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "ftd2xx.h"
#include <thread>
#include <string>
#include <string.h>

#include "defines_MPZ.h"
#include "file_io.h"



using namespace std;




int Read_Sample_Point_Location_Array_SS(std::string FileName,  int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS])
{
    char line_buff[SAMPLE_LINE_BUFFSIZE]; // a buffer to park the whole line
    char small_buff[20]; // a buffer to temporarily park the single number data
    ifstream infile(FileName);
    stringstream ss;
    int line_tot;
    int row_cnt = 0;
    int Total_Read = 0;

    cout << endl << "heck" << endl ;

    // assume no more than 30 blank lines inbetween real data
    for(int row = 0; row < SAMPLE_ROWS + 30 ; ++row )
    {
        // read a full line of input into the buffer (newline is
        //  automatically discarded)
        infile.getline( line_buff,  SAMPLE_LINE_BUFFSIZE );
        // copy the entire line into the stringstream
        ss << line_buff;
        line_tot = 0;
        for( int col = 0; col < SAMPLE_MAX_LINE_SIZE;  ++col )
        {
            ss.getline( small_buff, 100, ',' );
             if  (isdigit(small_buff[0]) )
             {
                SP[row_cnt][line_tot] = atoi( small_buff  ) ;
                line_tot++;
                Total_Read++;
             }
        }
        // dont count empty rows
        if(line_tot == 0)row_cnt = 0;
        else
        {
            Num_Of_Samples_Per_Row[row_cnt] = line_tot ;
            row_cnt++;
        }

        ss << "";  // This erases the previous contents.
        ss.clear(); // This clears the 'eof' flag.
    }
   infile.close();
    return Total_Read;
}





int Read_Map_SS(std::string FileName,  int *PM)
{
    char line_buff[PM_LINE_BUFF_SIZE]; // a buffer to temporarily park the data
    char small_buff[20]; // a buffer to temporarily park the data
    ifstream infile(FileName);
    stringstream ss;
    int inc = 0;
    bool done = false;

    cout << endl << "hell" << endl ;


    while(!done)
    {
        // read a full line of input into the buffer (newline is
        //  automatically discarded)
        infile.getline( line_buff,  PM_LINE_BUFF_SIZE );
        // copy the entire line into the stringstream
        ss << line_buff;

        for( int col = 0; col < PM_MAX_LINE_SIZE; ++col )
        {
            ss.getline( small_buff, 100, ',' );
            if  (isdigit(small_buff[0]) )
             {
                PM[inc] = atoi( small_buff ) ;
                inc++;
                if(inc >= PANEL_MAP_SIZE)done = true;
             }
        }
        ss << "";  // This erases the previous contents.
        ss.clear(); // This clears the 'eof' flag.
    }
   infile.close();

    cout << endl << "hello2" << endl ;

    return inc;
}



int Read_Enclosure_Info_SS(std::string FileName,   int Info[NUM_OF_ENCLOSURES][NUM_OF_ENCLOSURE_PARAMETERS])
{
    char line_buff[SAMPLE_LINE_BUFFSIZE]; // a buffer to park the whole line
    char small_buff[20]; // a buffer to temporarily park the single number data
    ifstream infile(FileName);
    stringstream ss;
    int line_tot;
    int row_cnt = 0;
    int Total_Read = 0;
    bool done = false;
    while(!done)
    {
        // read a full line of input into the buffer (newline is
        //  automatically discarded)
        infile.getline( line_buff,  SAMPLE_LINE_BUFFSIZE );
        // copy the entire line into the stringstream
        ss << line_buff;
        line_tot = 0;
        for( int col = 0; col < SAMPLE_MAX_LINE_SIZE;  ++col )
        {
            ss.getline( small_buff, 100, ',' );
             if  (isdigit(small_buff[0]) )
             {
                Info[row_cnt][line_tot] = atoi( small_buff  ) ;
                Total_Read++;
                line_tot++;
                if(Total_Read == (NUM_OF_ENCLOSURES * NUM_OF_ENCLOSURE_PARAMETERS)  )done = true;
             }
        }

        if(line_tot != 0)row_cnt++;

        ss << "";  // This erases the previous contents.
        ss.clear(); // This clears the 'eof' flag.
    }
   infile.close();
    return Total_Read;
}





int Read_Image_Info(char *FileName, char Image_File_Names[Max_Num_Of_Image_Files][Max_File_Name_Length],  float Image_Parameters[Max_Num_Of_Image_Files][Max_Num_Of_Image_Parameters])
{
    char line_buff[1000]; // a buffer to park the whole line
    char small_buff[20];  // a buffer to temporarily park the single number data
    int Test_I;
    ifstream infile(FileName);
    stringstream ss;
    int Total_Read = 0;
    bool done = false;

    int Parameter_Number = 0;
    int Image_Number = 0;

    bool Image_File_Read = false;
    bool Ready = false;

    Image_Number = 0;
    Total_Read = 0;
    while(!done)
    {
        // read a full line of input into the buffer (newline is
        //  automatically discarded)
        infile.getline( line_buff,  1000 );
        // copy the entire line into the stringstream
        ss << line_buff;
        Image_File_Read = false;
        Parameter_Number = 0;
        for( int col = 0; col < 1000;  ++col )
        {
            ss.getline( small_buff, 100, ',' );

             if  (isalnum(small_buff[0]) )
             {

                if(strcmp ("start",small_buff) == 0) Ready = true;

                else if(Ready)
                {
                    if(!Image_File_Read)
                    {
                        if(strcmp ("done",small_buff) == 0) done = true;
                        else
                        {
                       //     cout << small_buff  << " " << Image_Number <<" " << endl ;
                            strcpy(Image_File_Names[Image_Number], small_buff);
                            Image_Number++;
                            Total_Read++;
                            Image_File_Read = true;
                        }
                    }
                    else if (isdigit(small_buff[0]) )
                    {
                        Test_I = atoi( small_buff  ) ;
                       cout << Test_I << " " << Parameter_Number << " " << Image_Number-1 << endl  ;
                        if(Parameter_Number <= 3)Image_Parameters[Image_Number-1][Parameter_Number] = ( (float)Test_I) / 1000. ;
                        else Image_Parameters[Image_Number-1][Parameter_Number] = ( (float)Test_I) ;
                        Parameter_Number++;
                    }
                    else if(strcmp ("done",small_buff) == 0) done = true;
                }
             }
        }
        ss << "";  // This erases the previous contents.
        ss.clear(); // This clears the 'eof' flag.
    }
    cout << "image number: " << Total_Read << endl ;
    infile.close();
    return Total_Read;
}



int Read_Image_Info_SS(std::string FileName, char Image_File_Names[Max_Num_Of_Image_Files][Max_File_Name_Length],  float Image_Parameters[Max_Num_Of_Image_Files][Max_Num_Of_Image_Parameters])
{
    char line_buff[1000]; // a buffer to park the whole line
    char small_buff[20];  // a buffer to temporarily park the single number data
    int Test_I;
    ifstream infile(FileName);
    stringstream ss;
    int Total_Read = 0;
    bool done = false;

    int Parameter_Number = 0;
    int Image_Number = 0;

    bool Image_File_Read = false;
    bool Ready = false;

    Image_Number = 0;
    Total_Read = 0;
    while(!done)
    {
        // read a full line of input into the buffer (newline is
        //  automatically discarded)
        infile.getline( line_buff,  1000 );
        // copy the entire line into the stringstream
        ss << line_buff;
        Image_File_Read = false;
        Parameter_Number = 0;
        for( int col = 0; col < 1000;  ++col )
        {
            ss.getline( small_buff, 100, ',' );

             if  (isalnum(small_buff[0]) )
             {

                if(strcmp ("start",small_buff) == 0) Ready = true;

                else if(Ready)
                {
                    if(!Image_File_Read)
                    {
                        if(strcmp ("done",small_buff) == 0) done = true;
                        else
                        {
                       //     cout << small_buff  << " " << Image_Number <<" " << endl ;
                            strcpy(Image_File_Names[Image_Number], small_buff);
                            Image_Number++;
                            Total_Read++;
                            Image_File_Read = true;
                        }
                    }
                    else if (isdigit(small_buff[0]) )
                    {
                        Test_I = atoi( small_buff  ) ;
                       cout << Test_I << " " << Parameter_Number << " " << Image_Number-1 << endl  ;
                        if(Parameter_Number <= 3)Image_Parameters[Image_Number-1][Parameter_Number] = ( (float)Test_I) / 1000. ;
                        else Image_Parameters[Image_Number-1][Parameter_Number] = ( (float)Test_I) ;
                        Parameter_Number++;
                    }
                    else if(strcmp ("done",small_buff) == 0) done = true;
                }
             }
        }
        ss << "";  // This erases the previous contents.
        ss.clear(); // This clears the 'eof' flag.
    }
    cout << "image number: " << Total_Read << endl ;
    infile.close();
    return Total_Read;
}



int Read_Image_Info_2_SS(std::string FileName,  char Image_File_Names[Max_Num_Of_Image_Files][Max_File_Name_Length],  float Image_Parameters[Max_Num_Of_Image_Files][Max_Num_Of_Image_Parameters])
{
    char line_buff[1000]; // a buffer to park the whole line
    char small_buff[20];  // a buffer to temporarily park the single number data
    int Test_I;
    ifstream infile(FileName);
    stringstream ss;
    int Total_Read = 0;
    bool done = false;

    int Parameter_Number = 0;
    int Image_Number = 0;

    bool Image_File_Read = false;
    bool Ready = false;

    Image_Number = 0;
    Total_Read = 0;
    while(!done)
    {
        infile.getline( line_buff,  1000 ); // read a full line of input into the buffer (newline is automatically discarded)
        ss << line_buff;                    // copy the entire line into the stringstream
        Image_File_Read = false;
        Parameter_Number = 0;
        for( int col = 0; col < 1000;  ++col )
        {
            ss.getline( small_buff, 100, ',' );

             if  (isalnum(small_buff[0]) )
             {
                if(strcmp ("start",small_buff) == 0) Ready = true;
                else if(Ready)
                {
                    if(!Image_File_Read)
                    {
                        if(strcmp ("done",small_buff) == 0) done = true;
                        else
                        {
                            strcpy(Image_File_Names[Image_Number], small_buff);
                            Image_Number++;
                            Total_Read++;
                            Image_File_Read = true;
                        }
                    }
                    else if (isdigit(small_buff[0]) )
                    {
                        Test_I = atoi( small_buff  ) ;
                        cout << Test_I << " " << Parameter_Number << " " << Image_Number-1 << endl  ;
                        if(Parameter_Number <= 3)Image_Parameters[Image_Number-1][Parameter_Number] = ( (float)Test_I) / 1000. ;
                        else Image_Parameters[Image_Number-1][Parameter_Number] = ( (float)Test_I) ;
                        Parameter_Number++;
                    }
                    else if(strcmp ("done",small_buff) == 0) done = true;
                }
             }
        }
        ss << "";  // This erases the previous contents.
        ss.clear(); // This clears the 'eof' flag.
    }
    cout << "image number: " << Total_Read << endl ;
    infile.close();
    return Total_Read;
}



int Read_Sequence_Info_SS(std::string FileName, char Image_File_Names[Max_Num_Of_Image_Files][Max_File_Name_Length],  float Image_Parameters[Max_Num_Of_Image_Files][Max_Num_Of_Image_Parameters])
{
    char line_buff[1000]; // a buffer to park the whole line
    char small_buff[20];  // a buffer to temporarily park the single number data
    int Test_I;
    ifstream infile(FileName);
    stringstream ss;
    int Total_Read = 0;
    bool done = false;

    int Parameter_Number = 0;
    int Image_Number = 0;

    bool Image_File_Read = false;
    bool Ready = false;

    Image_Number = 0;
    Total_Read = 0;
    while(!done)
    {
        // read a full line of input into the buffer (newline is
        //  automatically discarded)
        infile.getline( line_buff,  1000 );
        // copy the entire line into the stringstream
        ss << line_buff;
        Image_File_Read = false;
        Parameter_Number = 0;
        for( int col = 0; col < 1000;  ++col )
        {
            ss.getline( small_buff, 100, ',' );

             if  (isalnum(small_buff[0]) )
             {

                if(strcmp ("start",small_buff) == 0) Ready = true;

                else if(Ready)
                {
                    if(!Image_File_Read)
                    {
                        if(strcmp ("done",small_buff) == 0) done = true;
                        else
                        {
                       //     cout << small_buff  << " " << Image_Number <<" " << endl ;
                            strcpy(Image_File_Names[Image_Number], small_buff);
                            Image_Number++;
                            Total_Read++;
                            Image_File_Read = true;
                        }
                    }
                    else if (isdigit(small_buff[0]) )
                    {
                        Test_I = atoi( small_buff  ) ;
                       cout << Test_I << " " << Parameter_Number << " " << Image_Number-1 << endl  ;
                        if(Parameter_Number <= 3)Image_Parameters[Image_Number-1][Parameter_Number] = ( (float)Test_I) / 1000. ;         // gain and blacl level

                        else Image_Parameters[Image_Number-1][Parameter_Number] = ( (float)Test_I) ;
                        Parameter_Number++;
                    }
                    else if(strcmp ("done",small_buff) == 0) done = true;
                }
             }
        }
        ss << "";  // This erases the previous contents.
        ss.clear(); // This clears the 'eof' flag.
    }
    cout << "image number: " << Total_Read << endl ;
    infile.close();
    return Total_Read;
}




void Grab_Test(  uint16_t *Grab_Buffer, uint16_t *Map_Buffer, uint16_t *Map_Buffer_W_Gaps, uint16_t *Map_Buffer_W_Gaps_RGBW, bool Grab )
{
    ofstream myFile ("unmapped32.raw", ios::out | ios::binary);
    ofstream myFile2 ("mapped32.raw", ios::out | ios::binary);
    ofstream myFile3 ("mapped_w_headers32.raw", ios::out | ios::binary);
    ofstream myFile4 ("mapped_w_headers_RGBW32.raw", ios::out | ios::binary);

    int totall = Sculpture_Size_RGB;
    int totall_RGBW = Sculpture_Size_RGBW;

    int totall2 = Buffer_W_Gaps_Size_RGB;
    int totall2_RGBW = Buffer_W_Gaps_Size_RGBW;

    int totall3 = Buffer_W_Gaps_Size_RGBW;

    myFile.write ( (char*)Grab_Buffer, totall * sizeof(*Grab_Buffer));

    myFile2.write ( (char*)Map_Buffer, totall * sizeof(*Map_Buffer));

    myFile3.write ( (char*)Map_Buffer_W_Gaps, totall2 * sizeof(*Map_Buffer_W_Gaps));

    myFile4.write ( (char*)Map_Buffer_W_Gaps_RGBW, totall2_RGBW * sizeof(*Map_Buffer_W_Gaps_RGBW));

    myFile.close();
    myFile2.close();
    myFile3.close();
    myFile4.close();

 //   myFile.write ( (char*)Pixel_Vec_All, totall * sizeof(*Pixel_Vec_All));

    printf("PIXELS9999999999999999999 ---  %d  %d  %d    \n", *(Grab_Buffer + 0 ), *(Grab_Buffer +  1), *(Grab_Buffer + 2) ) ;
}




