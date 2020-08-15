#ifndef FILE_IO_H
#define FILE_IO_H
#pragma once



int Read_Sample_Point_Location_Array_SS( std::string FileName,  int SP[SAMPLE_ROWS][SAMPLE_COLS], int *Num_Of_Samples_Per_Row );

int Read_Map_SS( std::string FileName,  int *PM);

int Read_Enclosure_Info_SS( std::string FileName,  int Info[NUM_OF_ENCLOSURES][NUM_OF_ENCLOSURE_PARAMETERS]);


int Read_Image_Info_SS( std::string FileName, char Image_File_Names[Max_Num_Of_Image_Files][Max_File_Name_Length],  float Image_Parameters[Max_Num_Of_Image_Files][Max_Num_Of_Image_Parameters]);

int Read_Image_Info( char *FileName, char Image_File_Names[Max_Num_Of_Image_Files][Max_File_Name_Length],  float Image_Parameters[Max_Num_Of_Image_Files][Max_Num_Of_Image_Parameters]);

int Read_Sequence_Info_SS( std::string FileName, char Image_File_Names[Max_Num_Of_Image_Files][Max_File_Name_Length],  float Image_Parameters[Max_Num_Of_Image_Files][Max_Num_Of_Image_Parameters]);

void Grab_Test(uint16_t *Grab_Buffer, uint16_t *Map_Buffer, uint16_t *Map_Buffer_W_Gaps, uint16_t *Map_Buffer_W_Gaps_RGBW, bool Grab );


// int Read_Image_Info_New(char *FileName, char Image_File_Names[Max_Num_Of_Image_Files][Max_File_Name_Length],  float Image_Parameters[Max_Num_Of_Image_Files][Max_Num_Of_Image_Parameters]);


#endif /* FILE_IO_H */
