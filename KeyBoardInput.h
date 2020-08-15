#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H
#pragma once



void KeyBoardInput(char c,

				   bool &A_or_B_File_Ext,  int &New_Image_X, int &Points_Visible, float &Downstream_Gain, bool &Sequence_On, bool &Stop_Program,  int &First_Sequence_Image_X,  int &Test_Mode,  bool &Video_On,

				   bool &Pause_A, float &Gain_A, float &Black_Level_A, float &Chroma_Level_A, float &Gamma_Level_A,

				   bool &img_On_A,  bool &img_proc_On_A, bool &Show_Proc_A,

                   bool &Reset_All_Filters_A,   bool &Filter_Ones_On_A, bool &Filter_3Taps_On_A,
				   bool &Filter_Ones2x2_A,      bool &Filter_Ones3x3_A, bool &Filter_Ones4x4_A, bool &Filter_Ones5x5_A, bool &Filter_Ones6x6_A, bool &Filter_Ones7x7_A,
                   bool &Filter_1p1_A,          bool &Filter_2p2_A,     bool &Filter_3p3_A,     bool &Filter_4p4_A,

				   bool &Pause_B,          float &Gain_B,          float &Black_Level_B,   float &Chroma_Level_B, float &Gamma_Level_B,

				   bool &img_On_B,         bool &img_proc_On_B,    bool &Show_Proc_B,

                   bool &Reset_All_Filters_B,   bool &Filter_Ones_On_B, bool &Filter_3Taps_On_B,
				   bool &Filter_Ones2x2_B,      bool &Filter_Ones3x3_B, bool &Filter_Ones4x4_B, bool &Filter_Ones5x5_B, bool &Filter_Ones6x6_B, bool &Filter_Ones7x7_B,
                   bool &Filter_1p1_B,          bool &Filter_2p2_B,     bool &Filter_3p3_B,     bool &Filter_4p4_B,

                    bool &Constellation_On,  bool &Lumen_Pulse_Upper_On, bool &LP_62_On, bool &LP_63_On, bool &TRX_On, bool &Seam_Flip,

                    bool &Fader_Stopped,
                    bool &White_On,
                   bool &LED_Correction_On
				   );



#endif /* _KEYBOARDINPUT */
