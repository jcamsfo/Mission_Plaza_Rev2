

#include <iostream>
#include <cstdio>

#include "KeyBoardInput.h"


void KeyBoardInput(char c,

				   bool &A_or_B_File_Ext,  int &New_Image_X, int &Points_Visible, float &Downstream_Gain, bool &Sequence_On, bool &Stop_Program,  int &First_Sequence_Image_X,  int &Test_Mode, bool &Video_On,

				   bool &Pause_A, float &Gain_A, float &Black_Level_A, float &Chroma_Level_A,  float &Gamma_Level_A,

				   bool &img_On_A,  bool &img_proc_On_A, bool &Show_Proc_A,

                   bool &Reset_All_Filters_A,   bool &Filter_Ones_On_A, bool &Filter_3Taps_On_A,
				   bool &Filter_Ones2x2_A,      bool &Filter_Ones3x3_A, bool &Filter_Ones4x4_A, bool &Filter_Ones5x5_A, bool &Filter_Ones6x6_A, bool &Filter_Ones7x7_A,
                   bool &Filter_1p1_A,          bool &Filter_2p2_A,     bool &Filter_3p3_A,     bool &Filter_4p4_A,

				   bool &Pause_B, float &Gain_B, float &Black_Level_B, float &Chroma_Level_B,  float &Gamma_Level_B,

				   bool &img_On_B,  bool &img_proc_On_B, bool &Show_Proc_B,

                   bool &Reset_Bll_Filters_B, bool &Filter_Ones_On_B,  bool &Filter_3Taps_On_B,
				   bool &Filter_Ones2x2_B, bool &Filter_Ones3x3_B, bool &Filter_Ones4x4_B, bool &Filter_Ones5x5_B, bool &Filter_Ones6x6_B, bool &Filter_Ones7x7_B,
                   bool &Filter_1p1_B, bool &Filter_2p2_B, bool &Filter_3p3_B, bool &Filter_4p4_B,

                   bool &Constellation_On,  bool &Lumen_Pulse_Upper_On, bool &LP_62_On, bool &LP_63_On, bool &TRX_On, bool &Seam_Flip,

                   bool &Fader_Stopped,
                   bool &White_On,
                   bool &LED_Correction_On
				   )
{


    static char previous_c = -1;
    static char previous_2ago = -1;

    if(c != -1)printf("prv_c= %c %d  c= %d %c \n",previous_c, previous_c , c, c);



    if( ( (previous_c == -30) || (previous_c == -31) ) )
    {
        if(c == 'x')
        {
            Stop_Program = true;
            Video_On = false;
        }
        else if(c == 'i')Video_On = !Video_On;
    }


    else if(c == 'a')Fader_Stopped = !Fader_Stopped;      // start on 's'


    else if(c == 't')
    {
        Test_Mode++ ;
        if(Test_Mode >= 3)Test_Mode = 0;  // wrap around
    }


    else if(  (previous_2ago == 'i') & isdigit(previous_c)   & isdigit(c) )
    {

        New_Image_X = c - '0' +  10 * (previous_c - '0') ;
    }


    else if(  (previous_2ago == 's') & isdigit(previous_c)   & isdigit(c) )
    {

        First_Sequence_Image_X = c - '0' +  10 * (previous_c - '0') ;
        Sequence_On = true; // start on 's'
    }





    else if(c == 'd')
    {
        Points_Visible++; // display sample points
        if(Points_Visible > 2)Points_Visible = 0;
    }


		else if(c == '.')
		{
			Downstream_Gain += .01;
			if(Downstream_Gain >= 1.)Downstream_Gain = 1. ;
		}
		else if(c == ',')
		{
			Downstream_Gain -= .01;
			if(Downstream_Gain <= 0)Downstream_Gain = 0. ;
		}


        else if(c == 'i')Sequence_On = false; // start on 's'
     //   else if(c == 's')Sequence_On = true; // start on 's'


    else if(c == 'r')Seam_Flip = !Seam_Flip; // start on 's'


     else if(c == 'y')White_On = !White_On; //

     else if(c == 'v')LED_Correction_On = !LED_Correction_On; //

	else if(A_or_B_File_Ext)
	{
		if(previous_c == 'f')
		{
			if(c == '2')Filter_Ones2x2_A = !Filter_Ones2x2_A; //  {1,1} filter on off
			else if(c == '3')Filter_Ones3x3_A = !Filter_Ones3x3_A; //  {1,1,1} filter on off
			else if(c == '4')Filter_Ones4x4_A = !Filter_Ones4x4_A; //  1,1,1,1} filter on off
			else if(c == '5')Filter_Ones5x5_A = !Filter_Ones5x5_A; //  {1,1,1,1,1} filter on off
			else if(c == '6')Filter_Ones6x6_A = !Filter_Ones6x6_A; //  {1,1,1,1,1,1} filter on off
			else if(c == '7')Filter_Ones7x7_A = !Filter_Ones7x7_A; //  {1,1,1,1,1,1,1} filter on off
		}


		if(previous_c == 'F')
		{
			if(c == '1')Filter_1p1_A = !Filter_1p1_A; //  {1,1} filter on off
			else if(c == '2')Filter_2p2_A = !Filter_2p2_A; //  {1,1} filter on off
			else if(c == '3')Filter_3p3_A = !Filter_3p3_A; //  {1,1,1} filter on off
			else if(c == '4')Filter_4p4_A = !Filter_4p4_A; //  1,1,1,1} filter on off
		}



		else if(c == 'p')Pause_A = !Pause_A; // pause on 'p'

		// else if(c == 's')Pause_A = false; // start on 's'



		else if(c == ']')
		{
			Gain_A += .01;
			if(Gain_A >= 1.5)Gain_A = 1.5 ;
		}
		else if(c == '[')
		{
			Gain_A -= .01;
			if(Gain_A <= 0)Gain_A = 0. ;
		}


		else if(c == 'm')
		{
			Black_Level_A += .02;
			if(Black_Level_A >= 1.)Black_Level_A = 1. ;
		}
		else if(c == 'n')
		{
			Black_Level_A -= .02;
			if(Black_Level_A <=-1)Black_Level_A = -1. ;
		}



		else if(c == 'l')
		{
			Chroma_Level_A += .05;
			if(Chroma_Level_A >= 2.)Chroma_Level_A = 2. ;
		}
		else if(c == 'k')
		{
			Chroma_Level_A -= .05;
			if(Chroma_Level_A <= 0)Chroma_Level_A = 0. ;
		}

		else if(c == 'j')
		{
			Gamma_Level_A += .05;
			if(Gamma_Level_A >= 1.)Gamma_Level_A = 1. ;
		}
		else if(c == 'h')
		{
			Gamma_Level_A -= .05;
			if(Gamma_Level_A <= 0)Gamma_Level_A = 0. ;
		}


		else if(c == 'q')img_On_A = !img_On_A; // display sample points
		else if(c == 'w')img_proc_On_A = !img_proc_On_A; // display sample points

		else if(c == 'X')
		{
			Reset_All_Filters_A = false;
			Filter_1p1_A = false;
			Filter_2p2_A = false;
			Filter_3p3_A = false;
			Filter_4p4_A = false;
			Filter_Ones2x2_A = false;
			Filter_Ones3x3_A = false;
			Filter_Ones4x4_A = false;
			Filter_Ones5x5_A = false;
			Filter_Ones6x6_A = false;
			Filter_Ones7x7_A = false;
			Filter_3Taps_On_A = true;
			Filter_Ones_On_A  = true;
		}
	}

	else
	{
		if(previous_c == 'f')
		{
			if(c == '2')Filter_Ones2x2_B = !Filter_Ones2x2_B; //  {1,1} filter on off
			else if(c == '3')Filter_Ones3x3_B = !Filter_Ones3x3_B; //  {1,1,1} filter on off
			else if(c == '4')Filter_Ones4x4_B = !Filter_Ones4x4_B; //  1,1,1,1} filter on off
			else if(c == '5')Filter_Ones5x5_B = !Filter_Ones5x5_B; //  {1,1,1,1,1} filter on off
			else if(c == '6')Filter_Ones6x6_B = !Filter_Ones6x6_B; //  {1,1,1,1,1,1} filter on off
			else if(c == '7')Filter_Ones7x7_B = !Filter_Ones7x7_B; //  {1,1,1,1,1,1,1} filter on off
		}


		if(previous_c == 'F')
		{
			if(c == '1')Filter_1p1_B = !Filter_1p1_B; //  {1,1} filter on off
			else if(c == '2')Filter_2p2_B = !Filter_2p2_B; //  {1,1} filter on off
			else if(c == '3')Filter_3p3_B = !Filter_3p3_B; //  {1,1,1} filter on off
			else if(c == '4')Filter_4p4_B = !Filter_4p4_B; //  1,1,1,1} filter on off
		}



		else if(c == 'p')Pause_B = !Pause_B; // pause on 'p'




		else if(c == ']')
		{
			Gain_B += .01;
			if(Gain_B >= 1.5)Gain_B = 1.5 ;
		}
		else if(c == '[')
		{
			Gain_B -= .01;
			if(Gain_B <= 0)Gain_B = 0. ;
		}


		else if(c == 'm')
		{
			Black_Level_B += .02;
			if(Black_Level_B >= 1.)Black_Level_B = 1. ;
		}
		else if(c == 'n')
		{
			Black_Level_B -= .02;
			if(Black_Level_B <= -1)Black_Level_B = -1. ;
		}


		else if(c == 'l')
		{
			Chroma_Level_B += .05;
			if(Chroma_Level_B >= 2.)Chroma_Level_B = 2. ;
		}
		else if(c == 'k')
		{
			Chroma_Level_B -= .05;
			if(Chroma_Level_B <= 0)Chroma_Level_B = 0. ;
		}

		else if(c == 'j')
		{
			Gamma_Level_B += .05;
			if(Gamma_Level_B >= 1.)Gamma_Level_B = 1. ;
		}
		else if(c == 'h')
		{
			Gamma_Level_B -= .05;
			if(Gamma_Level_B <= 0)Gamma_Level_B = 0. ;
		}


		else if(c == 'q')img_On_B = !img_On_B; // display sample points
		else if(c == 'w')img_proc_On_B = !img_proc_On_B; // display sample points


        else if (c == 'v')Show_Proc_B = !Show_Proc_B;

		else if(c == 'X')
		{
			Reset_Bll_Filters_B = false;
			Filter_1p1_B = false;
			Filter_2p2_B = false;
			Filter_3p3_B = false;
			Filter_4p4_B = false;
			Filter_Ones2x2_B = false;
			Filter_Ones3x3_B = false;
			Filter_Ones4x4_B = false;
			Filter_Ones5x5_B = false;
			Filter_Ones6x6_B = false;
			Filter_Ones7x7_B = false;
			Filter_3Taps_On_B = true;
			Filter_Ones_On_B  = true;
		}
	}


    if(c != -1)
    {
        previous_2ago = previous_c;
        previous_c = c;
    }

}



