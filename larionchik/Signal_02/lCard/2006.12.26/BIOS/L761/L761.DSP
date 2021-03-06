{ ********************************************************************* }
{  �᭮���� �ணࠬ�� ��� ࠡ��� � ���⮩ L-761                  			}
{ *********************************************************************	}
.MODULE/ABS=0/SEG=int_pm_user                 main_program;

#include "const.h"
#include "var.h"

{ ********************************************************************  }
{ !!!!!!!!!!!!!!!!!!!! ������� ��������� ���������� !!!!!!!!!!!!!!!!!!! }
{ *********************************************************************	}
{ 	��ࠡ��稪 RESET'� ��� DSP															}
StartLab:
	JUMP Start; NOP; NOP; NOP;

{ *********************************************************************	}
{	��ࠡ��稪 ���뢠��� IRQ2 - ������� �� ��                       		}
{	� ॣ���� SE - �ਧ��� ������ ����ࠡ�⠭��� �������					}
{ *********************************************************************	}
	SE = 0x1; RTI; NOP; NOP;

{ *********************************************************************	}
{ 	��ࠡ��稪 ���뢠��� IRQL1     	                              		}
{ *********************************************************************	}
	RTI; NOP; NOP; NOP;

{ *********************************************************************	}
{ 	��ࠡ��稪 ���뢠��� IRQL0   	                              		}
{ *********************************************************************	}
	RTI; NOP; NOP; NOP;

{ *********************************************************************	}
{ 	��ࠡ��稪 ���뢠��� SPORT0 Transmit											}
{ *********************************************************************	}
	RTI; NOP; NOP; NOP;

{ *********************************************************************	}
{ 	��ࠡ��稪 ���뢠��� SPORT0 Receive (���)									}
{ *********************************************************************	}
	JUMP(I7); NOP; NOP; NOP;

{ *********************************************************************	}
{ 	��ࠡ��稪 ���뢠��� IRQE															}
{ *********************************************************************	}
	RTI; NOP; NOP; NOP;

{ *********************************************************************	}
{	��ࠡ��稪 ���뢠��� BDMA															}
{ *********************************************************************	}
	RTI; NOP; NOP; NOP;

{ *********************************************************************	}
{	��ࠡ��稪 ���뢠��� SPORT1 Transmit (���)									}
{ *********************************************************************	}
	SB = 0x1; RTI; NOP; NOP;

{ *********************************************************************	}
{ ��ࠡ��稪 ���뢠��� SPORT1 Receive (���)										}
{ *********************************************************************	}
	SB = 0x1; RTI; NOP; NOP;

{ *********************************************************************	}
{	��ࠡ��稪 ���뢠��� Timer														}
{ *********************************************************************	}
	RTI; NOP; NOP; NOP;

{ *********************************************************************	}
{	��ࠡ��稪 ���뢠��� Powerdown													}
{ *********************************************************************	}
	RTI; NOP; NOP; NOP;

{ After DSP RESET we appear to be here                                  }
Start:
	AR = 0x5555; DM(TMode1) = AR;
	AR = 0xAAAA; DM(TMode2) = AR;

{ Set Wait State for ALL External Devices                   				}
	AR = 0x1;							{ 0000 0000 0000 0011 						}
	DM(Dm_Wait_Reg) = AR;			{ 0x3FFE - Waitstate Control Register 	}

{ ��⠭���� ���																			}
	AR = 0x167; CALL SendUartData;

{  SPORT0 - disable, SPORT1 - disable, SPORT1 - serial port 		  		}
	AR = 0x0400;
	DM(Sys_Ctrl_Reg) = AR;				{ 0x3FFF - System Control Register 		}

{ ******************************************************************** 	}
{ * Set SPORT0 for receive of ADC samples                           	* 	}
{ * SCLK0 and Receive Frame - external, word = 14 bits               * 	}
{ Serial Clock Divide Modulus 														}
	AR = 9; 								{ may be any : SCLK0 - external 			}
	DM(Sport0_Sclkdiv) = AR;		{ 0x3FF5 - Serial Clock Divide Modulus	}

{ Receive Frame Sync Divide Modulus 												}
	AR = 1000; 				{ may be any number: receive frame is external 	}
	DM(Sport0_Rfsdiv) = AR; { 0x3FF4 - Receive Frame Sync Divide Modulus	}

{ Control word for SPORT0 : SCLK0 - external										}
{ low level, normal external receive frame on each word (14 bit)			}
{ low level, alternate internal transmit frame on each word(14 bit) - not used}
	AR = 0x2EDD; 						{ 0010 1110 1101 1101						}
	DM(Sport0_Ctrl_Reg) = AR;		{ 0x3FF6 - SPORT0 Control Register		}

{  AutoBuffering is off																	}
	AR = 0x0;							{ 0000 0000 0000 0000						}
	DM(Sport0_Autobuf_Ctrl) = AR;	{ 0x3FF3 - SPORT0 Autobufer Ctrl	Reg.	}
{ ******************************************************************** 	}

{ ********************************************************************	}
{ 	Set SPORT1 for transmit digital codes to DAC                    	 	}
{ 	SCLK1 and Transmit Frame - internal, word = 16 bits                	}
{ 	Serial Clock Divide Modulus 														}
	AR=DM(SCLK1_DIV); 				{ SCLK1 - internal 							}
	DM(Sport1_Sclkdiv) = AR;		{ 0x3FF1 - Serial Clock Divide Modulus	}

{ Receive Frame Sync Divide Modulus 												}
	AR = DM(DacRate);		{ ��।���� ����� �뢮�� ����⮢ � ���'�	}
	DM(Sport1_Rfsdiv) = AR;	{ 0x3FF0 - Receive Frame Sync Divide Modulus	}

{ Control word for SPORT1 : SCLK1 - internal										}
{ high level, alternate internal receive frame on each word(16 bit) 		}
{ high level, alternate external transmit frame on each word(16 bit)		}
	AR = 0x7D0F;						{ 0111 1101 0000 1111						}
	DM(Sport1_Ctrl_Reg) = AR;		{ 0x3FF2 - SPORT1 Control Register		}
{ ********************************************************************	}

{ IRQ0,IRQ1,IRQ2 - negative front, disable Interrupt Nesting 				}
	ICNTL=0x07;

{ ���⨬ �� ������ �� ���뢠���													}
	IFC=0xFF; NOP;

{ ��⠭���� 䫠�� FLx � ��室��� ���ﭨ�										}
	SET FL0, SET FL1, SET FL2;

{ ᤥ���� 䫠�� PFx: PF0-PF2, PF6, PF7 - �室��, ��⠫�� - ��室��	}
	AR = 0x38; DM(Prog_Flag_Comp_Sel_Ctrl) = AR;

{ �� 㬮�砭�� �㬠��, �� ࠡ�⠥� � ADSP-2184									}
	DMOVLAY = 0x1; PMOVLAY = 0x1;
{ ����稬 �஡�� ०�� 㬭���⥫�													}
	DIS M_MODE;

{ ��ᨬ ���ப���஫��� AVR (RESET)												}
	CALL Reset_AVR;
{ ��� ����砭�� ��� AVR � �祭�� �ਡ����⥫쭮 340 ��					}
			CNTR = 1000;
			DO WaitResetLoop UNTIL CE;
				CNTR = 10000;
				DO NestingLoop UNTIL CE;
				NestingLoop: NOP;
			WaitResetLoop: NOP;

{ ��⠭���� �� 㬮�砭�� 8-�� ������� ०�� ࠡ���							}
	AR = 0x10A; CALL SendUartData;
	AR = 0x68; CALL SendUartData;

{ ��⠭���� �� 㬮�砭�� ����� ᡮ� ������ � ��� ࠢ��� 100 ���		}
	AR=0x108; CALL SendUartData;
	AY0=0xFF; AR=DM(AdcRate); AR=AR AND AY0;CALL SendUartData;
	SR0=DM(AdcRate); SR=LSHIFT SR0 BY -8 (LO); AR=SR0; CALL SendUartData;

{ ⥯��� ��⠭���� �� 㬮�砭�� ������஢�� ����প� ࠢ��� 10 ���		}
	AR=0x106; CALL SendUartData;
	AY0=0xFF; AR=DM(Inter_Kadr_Delay); AR=AR AND AY0; CALL SendUartData;
	SR0=DM(Inter_Kadr_Delay); SR=LSHIFT SR0 BY -8 (LO); AR=SR0; CALL SendUartData;

{ �ந��樠�����㥬 ����室��� 㪠��⥫� � ��६���� ��� � ���			}
	CALL InitAdcPointers;
	CALL InitDacPointers;

{ SPORT0 enable, SPORT1 enable, SPORT1 - serial port							}
	AR = 0x1C00;						{ 0001 1100 0000 0000 						}
	DM(Sys_Ctrl_Reg) = AR;        { 0x3FFF - System Control Register 		}

{ ࠧ�訬 ���뢠��� IRQ2 (�������)												}
	IMASK = DM(IMASK_VALUE); NOP;

{ �᫨ ����饭� ࠡ�� ���, � �����⨬ AVR, 									}
{ 									�맢�� ���뢠��� INT1 ��� ���ப���஫���	}
{ ��᫥ ��� AVR� ��� ����娢���� ��祬�-� ��易⥫쭮 ����			}
{ 										    ��� �� ���� ࠧ �������� ��� :((((( }
/*	AR=DM(AdcEnable); NONE = PASS AR; IF NE*/ CALL AVR_INT1;

{ ����� ����� �᭮���� �ணࠬ��, �� ���� ��������� ��⮢� � ࠡ��	}
	AR=0x1; DM(Ready)=AR;

{ ===================================================================== }
{ �᭮���� 横� �ணࠬ��																}
{ ===================================================================== }
MainLoop:
	CALL PutAcdData;
DacSampleLab:
	CALL CheckDacSample;
	CALL SendingDataToDac;

	CALL CommandProcessing;

	JUMP MainLoop;

{ *********************************************************************	}
{ ��楤�� ��।�� ��⮢�� ������ �� FIFO ���� ��� � ��					}
{ *********************************************************************	}
PutAcdData:
{ � ࠡ�� ��� ࠧ�襭�?																}
	AR = DM(AdcEnable); NONE= PASS AR;
	IF EQ RTS;								{ �᫨ ���, � ���� �멤��			}

{ 㪠��⥫� ���� ���																	}
	DM(AdcFifoPointer)=I3;

{ 	�஢�ਬ ࠧ�襭� �� ������� ���뢠��� � PC �� ����� ���� ���	}
	AR=DM(EnableAdcIrq); NONE= PASS AR; IF EQ RTS;

{ � �� ��� �� ���뫠�� ����� � ��?												}
	AR=I3;									{ ����稬 '������' FIFO ���			}
	AY0=I0;  								{ ����稬 '墮��' FIFO ���				}
	AR=AR-AY0;
	AY0=DM(AdcFifoLength);				{ ⥪��� ����� FIFO ���� ���		}
	IF LT AR=AR+AY0;			{ ⥯��� � AR ��⨭��� ࠧ��� ����� I3 � I0	}
	AY0=DM(AdcIrqStep);					{ 蠣 � �����஢���� ���뢠���		}
	NONE=AR-AY0;    						{ �ࠢ��� ��								}
	IF LT RTS;								{ �᫨ �����, � �멤��				}

{ ���� � ���� ���, ��稭�� � ���ண� ����� ���뢠��						}
{											��।��� ���樨 AdcIrqStep ������		}
	DM(IrqDataAddress)=I0;
	MODIFY(I0, M0);

{ � ⥯��� ����� ������� ���뢠��� � PC �� ����� ���� ���				}
	RESET FL2; NOP; NOP; SET FL2;

	RTS;

{ *********************************************************************	}
{ ������⭮� ��⠭������� �஢�� �� ���'�										}
{ *********************************************************************	}
CheckDacSample:
{ ����� �� ���-�ਧ��� ������⭮���� �뢮�� �� ���						}
	AR = DM(DAC_Value); AR = TSTBIT 15 OF AR; IF EQ RTS;

{ � ��⮪���� ࠡ�� ��� ࠧ�襭�?													}
	AR = DM(EnableDacStream); NONE = PASS AR;
	IF NE JUMP FinishOfDacSample;			{ �᫨ ��, � ���� �멤��		}

{  SPORT0 - enable, SPORT1 - disable, SPORT1 - serial port		  			}
	AR=0x1400;
	DM(Sys_Ctrl_Reg)=AR;				{ 0x3FFF - System Control Register 		}

{ �� ����� ��⪨ DacSampleLab ���� �ய����	��� CALL SetDacSample		}
	AR = ^DacSampleLab; I5 = AR;
	AR = ^SetDacSample;
	CALL ModifyCall;

{ ��⠭���� ���ᨬ����� ����� �뢮�� ����⮢ �� ��� (122.88 ���)		}
	AR = 19;
	DM(Sport1_Rfsdiv) = AR;	{ 0x3FF0 - Receive Frame Sync Divide Modulus	}

{ ��ᨬ 䫠��� �����襭�� ������⭮� �뤠� ����� �� ���				}
	SB=0x0;

{ ���� - ���⨬ ������ �� ���뢠��� SPORT1 Receive						}
	IFC = 0x2;

{ ��⠭���� ᮮ⢥�����騥 ���� � ��६����� ��᪥ ���뢠���				}
	AR = DM(IMASK_VALUE);
	AR = SETBIT 1 OF AR; 	{ ࠧ�訬 ���뢠��� �� SPORT1 Receive		}
	AR = CLRBIT 2 OF AR; 	{ ����⨬ ���뢠��� �� SPORT1 Transmit		}
	DM(IMASK_VALUE) = AR;
{ ⥯��� ����� ��⠭����� ���� ���뢠���										}
	IMASK = AR;

{ SPORT0 enable, SPORT1 enable, SPORT1 - serial port							}
	AR = 0x1C00;						{ 0001 1100 0000 0000 						}
	DM(Sys_Ctrl_Reg) = AR;			{ 0x3FFF - System Control Register 		}

{ ������ ᫮�� ������ � ॣ���� SPORT1												}
	AR = DM(DAC_Value); AY0 = 0x1FFF;
	AR = AR AND AY0; TX1 = AR;

	RTS;

{ *********************************************************************	}
{ �஢�ઠ ����砭�� ������⭮�� �뢮�� �� ���									}
{ *********************************************************************	}
SetDacSample:
{ 䫠��� �����襭�� �뤠� ����� �� ���											}
	AR = SB;	NONE = PASS AR; IF EQ RTS;

{  SPORT0 - enable, SPORT1 - disable, SPORT1 - serial port		  			}
	AR=0x1400;
	DM(Sys_Ctrl_Reg)=AR;				{ 0x3FFF - System Control Register 		}

{ ��ᨬ 䫠��� �����襭�� ������⭮� �뤠� ����� �� ���				}
	SB=0x0;

{ ��⠭���� ᮮ⢥�����騥 ���� � ��६����� ��᪥ ���뢠���				}
	AR = DM(IMASK_VALUE);
	AR = CLRBIT 1 OF AR; 		{ ����⨬ ���뢠��� �� SPORT1 Receive	}
	DM(IMASK_VALUE) = AR;
{ ⥯��� ����� ��⠭����� ���� ���뢠���										}
	IMASK = AR;

{ �� ����� ��⪨ DacSampleLab ���� �ய����	��� CALL CheckDacSample		}
	AR = ^DacSampleLab; I5 = AR;
	AR = ^CheckDacSample;
	CALL ModifyCall;

{ SPORT0 enable, SPORT1 enable, SPORT1 - serial port							}
	AR = 0x1C00;						{ 0001 1100 0000 0000 						}
	DM(Sys_Ctrl_Reg) = AR;			{ 0x3FFF - System Control Register 		}

FinishOfDacSample:
{ ��ᨬ ���-�ਧ��� ������⭮���� �뢮�� �� ���								}
	AR = DM(DAC_Value); AY0 = 0x1FFF;
	AR = AR AND AY0; DM(DAC_Value) = AR;

	RTS;

{ *********************************************************************	}
{ �஢�ઠ �� ��᫥���� ����� ��� �뢮�� �� ���								}
{ *********************************************************************	}
SendingDataToDac:
{ � �����-� ࠡ�� ��� ࠧ�襭�?													}
	AR = DM(EnableDacStream); NONE = PASS AR; IF EQ RTS;

{ �஢�ਬ �뫮 �� ���뢠��� �� ���												}
	AR = SB; NONE = PASS AR; IF EQ RTS;

{ ��ᨬ 䫠��� ���뢠��� �� ���													}
	SB = 0x0;

{ ��।���� �뢮���� ����� � ॣ���� ���� SPORT1 (���)					}
	AR = PM(I4, M4); TX1 = AR;

{ ⥪�騩 㪠��⥫� FIFO ���� ���													}
	DM(DacFifoPointer)=I4;

	RTS;

{ *********************************************************************	}
{ ��ࠡ��稪 ���뢠��� IRQ2 (��ࠡ��稪 ����� �������)						}
{ *********************************************************************	}
CommandProcessing:
{ �஢�ਬ ���� �� ����ࠡ�⠭��� �������?										}
	AR = SE; NONE= PASS AR;
	IF EQ RTS; 							{ �᫨ ���, � ���� �멤��				}

{ ��ᨬ 䫠��� ����ࠡ�⠭��� �������											}
	SE = 0x0;

{ ���⠥� ����� ����㯨�襩 �������												}
	AR = DM(Command);
	AR = PASS AR;
	IF EQ JUMP Test_cmd;       				{ ����� ������� 0					}
	AR = AR - 0x1;
	IF EQ JUMP Load_Control_Table_cmd;		{ ����� ������� 1					}
	AR = AR - 0x1;
	IF EQ JUMP Enable_ADC_cmd;					{ ����� ������� 2					}
	AR = AR - 0x1;
	IF EQ JUMP ADC_Fifo_Config_cmd;    		{ ����� ������� 3   				}
	AR = AR - 0x1;
	IF EQ JUMP Set_ADC_Kadr_cmd; 				{ ����� ������� 4					}
	AR = AR - 0x1;
	IF EQ JUMP Enable_DAC_Stream_cmd;		{ ����� ������� 5					}
	AR = AR - 0x1;
	IF EQ JUMP DAC_Fifo_Config_cmd;    		{ ����� ������� 6   				}
	AR = AR - 0x1;
	IF EQ JUMP Set_DAC_Rate_cmd; 				{ ����� ������� 7					}
	AR = AR - 0x1;
	IF EQ JUMP Ad_Channel_cmd;  				{ ����� ������� 8					}
	AR = AR - 0x1;
	IF EQ JUMP Get_TTL_cmd;   					{ ����� ������� 9					}
	AR = AR - 0x1;
	IF EQ JUMP Put_TTL_cmd;   					{ ����� ������� 10				}
	AR = AR - 0x1;
	IF EQ JUMP Synchro_Config_cmd; 			{ ����� ������� 11				}
	AR = AR - 0x1;
	IF EQ JUMP Enable_IRQ_PC_cmd; 			{ ����� ������� 12				}
	AR = AR - 0x1;
	IF EQ JUMP IRQ_Test_cmd;   				{ ����� ������� 13				}
	AR = AR - 0x1;
	IF EQ JUMP Set_DSP_Type_cmd; 				{ ����� ������� 14				}

ErrorEndOfCommand:
	AR = 0xFFFF; DM(Command) = AR;
	RTS;

{ *********************************************************************	}
{ ���⮢�� �������.                                                   	}
{ ��᫥ ��室� � �祩��	����� ������ TestLoadVar								}
{  													������ ���� �᫮ 0xAA55)		}
{ *********************************************************************	}
Test_cmd:
	AR = 0xAA55; DM(TestLoadVar) = AR;

{ *********************************************************************	}
{ ��室 �� ��ࠡ��稪� ���뢠��� IRQ2.											}
{ ��᫥ ��室� � �祩��	����� ������ Command ������ ���� �᫮ 0			}
{ *********************************************************************	}
EndOfCommand:
	AR = 0x0; DM(Command) = AR;
	RTS;

{ *********************************************************************	}
{ ���� (RESET) ���ப���஫��� AVR 												}
{ *********************************************************************	}
Reset_AVR:
{ ��ᨬ ���ப���஫��� AVR															}
	AR=0x8; DM(Prog_Flag_Data)=AR;
	AR=0x0; DM(Prog_Flag_Data)=AR;
	CNTR=400; CALL Delay;			{ ����প� 400 ������ DSP - 13.6 ���	}
	AR=0x8; DM(Prog_Flag_Data)=AR;

	RTS;

{ *********************************************************************	}
{ ���뢠��� INT1 ��� ���ப���஫���	AVR (����� ���)						}
{ *********************************************************************	}
AVR_INT1:
{ ����⨬ ॣ����																		}
	AR = RX0;

{ ��ᨬ 䫠��� ࠡ��� SPORT0 ��� UART											}
	AR = 0x0; DM(IsSport0InUart) = AR;

{  SPORT0 - disable, SPORT1 - enable, SPORT1 - serial port 		  			}
	AR = 0x0C00;						{ 0001 1100 0000 0000 						}
	DM(Sys_Ctrl_Reg)=AR;				{ 0x3FFF - System Control Register 		}

{ ******************************************************************** 	}
{ * Set SPORT0 for receive of ADC samples                           	* 	}
{ * SCLK0 and Receive Frame - external, word = 14 bits               * 	}
{ Serial Clock Divide Modulus 														}
	AR = 9; 								{ may be any : SCLK0 - external 			}
	DM(Sport0_Sclkdiv) = AR;		{ 0x3FF5 - Serial Clock Divide Modulus	}

{ Receive Frame Sync Divide Modulus 												}
	AR = 1000; 				{ may be any number: receive frame is external 	}
	DM(Sport0_Rfsdiv) = AR; { 0x3FF4 - Receive Frame Sync Divide Modulus	}

{ Control word for SPORT0 : SCLK0 - external										}
{ low level, alternate external receive frame on each word(14 bit)		}
{ low level, alternate internal transmit frame on each word(14 bit) - not used}
	AR = 0x2EDD; 						{ 0010 1110 1101 1101						}
	DM(Sport0_Ctrl_Reg) = AR;		{ 0x3FF6 - SPORT0 Control Register		}
{ ******************************************************************** 	}

{ SPORT0 enable, SPORT1 enable, SPORT1 - serial port							}
	AR = 0x1C00;						{ 0001 1100 0000 0000 						}
	DM(Sys_Ctrl_Reg) = AR;        { 0x3FFF - System Control Register 		}

{ ⥯��� �����⨬ AVR, �맢�� ���뢠��� INT1 ��� ���ப���஫���		}
	AR = 0x8; DM(Prog_Flag_Data) = AR;
	AR = 0x0; DM(Prog_Flag_Data) = AR;
	CNTR = 28; CALL Delay;			{ ����প� 28 ������ DSP - 0.952 ���	}
	AR = 0x8; DM(Prog_Flag_Data) = AR;
	CNTR = 33; CALL Delay;			{ ����প� 33 ������ DSP - 1.0 ���		}

	RTS;

{ *********************************************************************	}
{ ����প� � ������ DSP																	}
{ *********************************************************************	}
Delay:
   DO DelayLoop UNTIL CE;
DelayLoop:   	NOP;
	RTS;

{***********************************************************************}
{   ������� ���஢���� ���뢠���	                          				}
{***********************************************************************}
IRQ_Test_cmd:
{ ��⠭���� ��� �, �����६����, �뢥��� AVR �� ०��� ᨭ�஭���樨		}
	AR = 0x10B; CALL SendUartData;
	AR = 0x0; CALL SendUartData;

{ ࠧ��᪨�㥬 ���뢠��� IRQ2(�������)											}
	AR = DM(IMASK_VALUE);
	AR = CLRBIT 1 OF AR; 		{ ����⨬ ���뢠��� �� SPORT1 Receive	}
	AR = CLRBIT 2 OF AR; 		{ ����⨬ ���뢠��� �� SPORT1 Transmit	}
	AR = CLRBIT 5 OF AR;			{ ����᪨�㥬 SPORT0 Receive (���)			}
	DM(IMASK_VALUE) = AR;
{ ⥯��� ����� ��⠭����� ���� ���뢠���										}
	IMASK = AR;

{ �������� ����প� �� �믮������ ��।�� ��ࠬ���?????					}
	CNTR=100; CALL Delay;			{ ����প� 100 ������ DSP - 3.4 ���		}

{ ���⨬ ������ �� ���뢠���, �஬� IRQ2 (�������)							}
	IFC = 0x7F; NOP;

{ �஢�ਬ �㦭� �� ������� ���뢠��� � ��										}
	AR = DM(EnableAdcIrq); NONE = PASS AR;
{ �� ����� ��⪨ MainLoop �ய�襬 ��� CALL IrqTest ��� CALL PutAcdData	}
	AR = ^MainLoop; I5 = AR;
	AR = ^PcIrqTest; AY0 = ^PutAcdData;
	IF EQ AR = PASS AY0;
	CALL ModifyCall;

{ �����訬 �������																		}
	JUMP EndOfCommand;

{***********************************************************************}
{ �㭪�� �����஢���� ���뢠��� � �� � ���⮩ ~3.4 ��					}
{***********************************************************************}
PcIrqTest:
	CNTR=1000;
	DO Lab UNTIL CE;
		CNTR=100;
		DO Lab1 UNTIL CE;
	Lab1: 	NOP;
Lab:	NOP;

{ ����ਬ ���뢠��� � �� �� ����� ���												}
	RESET FL2; NOP; NOP; SET FL2;

	RTS;

{-----------------------------------------------------------------------}
#include "adc.h"
#include "dac.h"
#include "synchro.h"
#include "adchanel.h"
#include "fifo.h"
#include "ttl.h"
#include "uart.h"
{-----------------------------------------------------------------------}

{***********************************************************************}
{ ����ன�� �ணࠬ�� �� ᮮ⢥�����騩 ⨯ DSP                    		}
{***********************************************************************}
Set_DSP_Type_cmd:
{ ��⠭���� ���																			}
	AR = 0x167; CALL SendUartData;

{  SPORT0 - disable, SPORT1 - disable, SPORT1 - serial port 		  		}
	AR=0x0400;
	DM(Sys_Ctrl_Reg)=AR;				{ 0x3FFF - System Control Register 		}

	IMASK=0x0; NOP;      { ����᪨�㥬 �� ���뢠���							}

{ ������� ⨯ ��⠭��������� �� ���� DSP											}
{ ��� ADSP-2184 �㦭� DSP_Type=0,      	                     			}
{ � ��� ADSP-2185 - DSP_Type=1, 		     	                     			}
{ � ��� ADSP-2186 - DSP_Type=2  		  			                    			}
	AR = DM(DSP_Type); NONE = PASS AR;
	AR = 0x3000;				{ ��� ADSP-2185 ��� ADSP-2186						}
	AY0 = 0x2000;				{ ��� ADSP-2184				 						}
	IF EQ	AR = PASS AY0;
	SI = AR;			{ �� �, �� ��।����� � �㭪�� ����䨪�樨 ���� 	}

{ �������㥬 �� �����।�⢥��� �⥭�� ��� ����� 						}
{					��/� ������ ������, �.�. Reg=DM(xxxx) ��� DM(xxxx)=Reg	}
	I4=0x0; M4=1; L4=0; M5=1;
	AR=^Set_DSP_Type_cmd;
	CNTR=AR;
	DO ReadWriteDMLoop UNTIL CE;
   	I5=I4;
		AR=PM(I4, M4);
		AF= TSTBIT 15 OF AR;
	   IF NE CALL Test14BitDM;
ReadWriteDMLoop: NOP;

{ �������㥬 �� �����।�⢥��� ����� � ॣ����� ����樨 (I0, I1, ...)		}
	I4=0x0; M4=1; L4=0; M5=1;
	AR=^Set_DSP_Type_cmd;
	CNTR=AR;
	DO LoadRegLoop UNTIL CE;
		I5=I4;
		AR=PM(I4, M4);
		AF= TSTBIT 15 OF AR;
		IF EQ CALL Test14BitI;
LoadRegLoop: NOP;

{ ᪮���㥬 ����� �� ����� ����													}
	AY0=DM(DSP_Type);
	AF= PASS AY0;
	IF EQ	JUMP SkipCopyDmData;	{ ��� ADSP-2184			 						}

	I4=0x2800; M4=1; L4=0;
	I5=0x3800; M5=1; L5=0;
	CNTR=0x7E0;
	DO CopyDmData UNTIL CE;
		AR=DM(I4, M4);
CopyDmData:	DM(I5, M5)=AR;

{ ����⨬ �� �⥪�																	}
SkipCopyDmData:
	DIS INTS;	 				{ ����⨬ �� ���뢠���							}

{ ���⨬ �� �⥪� 																		}
ClearStacksSetDSP:
	POP STS, POP PC, POP CNTR, POP LOOP;
	DIS SEC_REG;				{ ��३��� �� ���� ���� ॣ���஢				}
	AR=SSTAT;
	AY0=0x55;
	AR=AR AND AY0;
	AR=AR XOR AY0;
	IF NE JUMP ClearStacksSetDSP;		{ �᫨ �� �� �⥪� ���⨫���, 		}
												{ � ����ਬ								}
	IMASK=0x0; NOP;      { ����᪨�㥬 �� ���뢠���							}
	ENA INTS;  				{ ࠧ�訬 �� ���뢠���								}

{ ��᫥ ��室� � �祩��	����� ������ Command ������ ���� �᫮ 0			}
	AR = 0x0; DM(Command) = AR;

{ �����訬 �������																		}
	JUMP Start;

{***********************************************************************}
{ �஢�ઠ 14 ��� � ॣ���� AR	��� ������樨 Type 3						}
{***********************************************************************}
Test14BitDM:
	   AF= TSTBIT 14 OF AR;
	   IF EQ CALL Test13BitDM;
      RTS;

{***********************************************************************}
{ �஢�ઠ 13 ��� � ॣ���� AR	��� ������樨 Type 3						}
{***********************************************************************}
Test13BitDM:
	   AF= TSTBIT 13 OF AR;
	   IF EQ CALL ModifyReadWriteDM;
      RTS;

{ *********************************************************************	}
{ ������� � PM �� ��������� ����� ������� Reg=DM(0x(3/2)xxx) 			}
{																��� DM(0x(3/2)xxx)=Reg	}
{ �室:                                                     	  			}
{      I5 - ����, �㤠 �����뢠���� �������             					}
{      SI - ���� ���� 0x(3/2)xxx	(��୥�� ���訥 ���� ����)			}
{ *********************************************************************	}
ModifyReadWriteDM:
{ �᫨ �� ������ � Memory-Maped Control Register, � ���� �멤��	}
	SR=LSHIFT AR BY 6 (LO);
	SR=LSHIFT SR0 BY -2 (LO);	{ �뤥��� ���� � ������樨					}
	AY0=0x3FE0;
	NONE=SR0-AY0;
	IF GE RTS;		{ �᫨ �� Memory-Maped Control Register, � �멤��	}

{ ��ନ�㥬 ��� ������� 																}
   AR= CLRBIT 9 OF AR;
   AR= CLRBIT 8 OF AR;
	SR0=AR;
	SR=SR OR LSHIFT SI BY -4 (LO);
	PM(I5,M5)=SR0;							{ ���࠭��� �� 24-��⮢�� ᫮�� 	}
	RTS;

{***********************************************************************}
{ �஢�ઠ 14 ��� � ॣ���� AR	��� ������樨 Type 7						}
{***********************************************************************}
Test14BitI:
	   AF= TSTBIT 14 OF AR;
	   IF EQ CALL Test13BitI;
      RTS;

{***********************************************************************}
{ �஢�ઠ 13 ��� � ॣ���� AR	��� ������樨 Type 7						}
{***********************************************************************}
Test13BitI:
	   AF= TSTBIT 13 OF AR;
	   IF NE CALL Test12BitI;
      RTS;

{***********************************************************************}
{ �஢�ઠ 12 ��� � ॣ���� AR	��� ������樨 Type 7						}
{***********************************************************************}
Test12BitI:
	   AF= TSTBIT 12 OF AR;
	   IF NE CALL Test11BitI;
      RTS;

{***********************************************************************}
{ �஢�ઠ 11 ��� � ॣ���� AR	��� ������樨 Type 7						}
{***********************************************************************}
Test11BitI:
      MR1=AR;
	   AF= TSTBIT 11 OF AR;
	   IF NE CALL Test10BitIP;
      AR=MR1;
	   AF= TSTBIT 11 OF AR;
	   IF EQ CALL Test10BitID;
      RTS;

{***********************************************************************}
{ �஢�ઠ 10 ��� � ॣ���� AR	��� ������樨 Type 7						}
{***********************************************************************}
Test10BitIP:
	   AF= TSTBIT 10 OF AR;
	   IF EQ CALL ModifyLoadNonDataRegister;
      RTS;

{***********************************************************************}
{ �஢�ઠ 10 ��� � ॣ���� AR	��� ������樨 Type 7						}
{***********************************************************************}
Test10BitID:
	   AF= TSTBIT 10 OF AR;
	   IF NE CALL ModifyLoadNonDataRegister;
      RTS;

{ *********************************************************************	}
{ �������㥬 ��� �����।�⢥���� ����� � ������� ॣ���� I0...I7	}
{ ������� � PM �� ��������� ����� ������� Ix=<data>							}
{ �室:                                                     	  			}
{      I5 - ����, �㤠 �����뢠���� �������             					}
{      SI - ���� ���祭�� <data>	(��୥�� ���訥 ����)					}
{ *********************************************************************	}
ModifyLoadNonDataRegister:
{ �뤥�塞 ����� �����᭮�� ॣ���� I0...I7										}
   AX0=PX;
   AY0=0x0F;
   AF=AX0 AND AY0;
   AX0=0x3;
   AF=AX0-AF;
   IF LT RTS;  { �᫨ �� �� ����� ॣ���� I0...I7, � ��室��			}

{ ��ନ�㥬 ��� ������� 																}
   AR= CLRBIT 9 OF AR;
   AR= CLRBIT 8 OF AR;
	SR0=AR;
	SR=SR OR LSHIFT SI BY -4 (LO);
	PM(I5,M5)=SR0;							{ ���࠭��� �� 24-��⮢�� ᫮�� 	}
	RTS;

.ENDMOD;
