//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit_Language.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TLanguageType LanguageType = ltRussian;
AnsiString    LanguageString[108][2] = {
                                      {"   ����   "                                 , "   File   "    }, // --- 0
                                      {"   ���������   "                            , "   Options   " }, // --- 1
                                      {"   � ��������� ..."                         , "   About ..."  }, // --- 2
                                      {"  ����� "                                   , " Exit "        }, // --- 3
                                      {"������ ��� ��������� (�������)"             , "Calc detection zones (statics)" }, // --- 4
                                      {"������ ������������ ������"                 , "Run in static mode" }, // --- 5
                                      {"������������� ������"                       , "Data extrapolation" }, // --- 6
                                      {"����������� �����"                          , "Elevation lines" }, // --- 7
                                      {"������ ��� ��������� (��������)"            , "Calc detection zones (dynamics)" }, // --- 8
                                      {"������ ������������� ������"                , "Run in dynamic mode" }, // --- 9
                                      {"����"                                       , "Stop" }, // --- 10
                                      {"���������� �������������"                   , "Stop model" }, // --- 11
                                      {"��������� �����������"                      , "Save image" }, // --- 12
                                      {"������� �������"                            , "Delete image" }, // --- 13
                                      {"������������� ������� ..."                  , "Rename image" }, // --- 14
                                      {"����������� ��������� �� ���������"         , "Restore default settings" }, // --- 15
                                      {"���"                                        , "Radar" }, // --- 16
                                      {"��������� ���"                              , "Radar features" }, // --- 17
                                      {"�����������"                                , "Display" }, // --- 18
                                      {"��������� �����������"                      , "Display parameters" }, // --- 19
                                      {"C���� ���������������"                      , "Propagation medium" }, // --- 20
                                      {"�-�� �����������"                           , "Detection perf" }, // --- 21 //Characteristics
                                      {"����"                                       , "Target" }, // --- 22
                                      {"��������� ����"                             , "Target parameters" }, // --- 23
                                      {"��������� ���������"                        , "Load parameters" }, // --- 24
                                      {"��������� ���������"                        , "Save parameters" }, // --- 25
                                      {"����� ..."                                  , "Exit ..." }, // --- 26
                                      {"�������� ��� ���������:"                    , "List of detection zones:" }, // --- 27
                                      {"������ �������"                             , "Antenna height" }, // --- 28
                                      {"���� ����� ���� ��"                         , "Elevation beamwidth" }, // --- 29
                                      {"������ ���� ��"                             , "Half-power beamwidth" }, // --- 30
                                      {"����� �����"                                , "Wavelength" }, // --- 31
                                      {"����� ���"                                  , "Antenna pattern (AP)" }, // --- 32
                                      {"������� ���������"                          , "Range (S/N = 1)" }, // --- 33
                                      {"������ ������� ���������"                   , "Calc range (S/N = 1)" }, // --- 34
                                      {"���������� ��������"                        , "Peak power" }, // --- 35
                                      {"�-� ��� �� ��������"                        , "Antenna gain (Tr)" }, // --- 36
                                      {"�-� ��� �� �����"                           , "Antenna gain (Rr)" }, // --- 37
                                      {"�-� ������ �� ���������"                    , "Factor compression" }, // --- 38
                                      {"�-� ���� ���������"                         , "Noise factor" }, // --- 39
                                      {"������ ����������� ���������"               , "Receiver passband" }, // --- 40
                                      {"������ ��. ���������"                       , "Receiver passband" }, // --- 41
                                      {"������ ��� ���������"                       , "Transmitting loss factor" }, // --- 42
                                      {"������ ��� ������"                          , "Receiving loss factor" }, // --- 43
                                      {"������ ��� ���������"                       , "Processing loss" }, // --- 44
                                      {"��"                                         , "dB" }, // --- 45
                                      {"���"                                        , "MHz" }, // --- 46
                                      {"���"                                        , "kW" }, // --- 47
                                      {"��"                                         , "km" }, // --- 48
                                      {"c"                                          , "s" }, // --- 49
                                      {"c�"                                         , "sm" }, // --- 50
                                      {"��"                                         , "deg" }, // --- 51
                                      {"�"                                          , "m" }, // --- 52
                                      {"��"                                         , "Hz" }, // --- 53
                                      {"��� ����"                                   , "Target CS" }, // --- 54
                                      {"������ ���������� �������"                  , "Model of signal fluctuation" }, // --- 55
                                      {"��� ����������"                             , "No fluctuation" }, // --- 56
                                      {"�������� 1"                                 , "Swerling 1" }, // --- 57
                                      {"�������� 2"                                 , "Swerling 2" }, // --- 58
                                      {"�������� 3"                                 , "Swerling 3" }, // --- 59
                                      {"�������� 4"                                 , "Swerling 4" }, // --- 60
                                      {"�������. ������ ������ �����������"         , "Auto Display height" }, // --- 61
                                      {"������ �����������"                         , "Display height" }, // --- 62
                                      {"���� ������� ��������. ���"                 , "Vertical axis tick spacing" }, // --- 63
                                      {"�������. ������ ��������. ���������"        , "Auto Horizontal range" }, // --- 64
                                      {"�������������� ���������"                   , "Horizontal range" }, // --- 65
                                      {"���� ������� ��������. ���"                 , "Horizontal axis tick spacing" }, // --- 66
                                      {"���� ���� ������"                           , "Background color" }, // --- 67
                                      {"���� ������ �����"                          , "Axis line color" }, // --- 68
                                      {"���� ���������� �����"                      , "Dot line color" }, // --- 69
                                      {"���� �������"                               , "Font color" }, // --- 70
                                      {"��������� ����������� ���"                  , "Elevation axis settings" }, // --- 71
                                      {"������ ����������"                          , "Asperity height" }, // --- 72
                                      {"��������� ����"                             , "Sea state, numbers" }, // --- 73
                                      {"����. ������������ ������ � ���������"      , "Atmospheric-absorption loss" }, // --- 74
                                      {"����. ������ ������"                        , "Other loss factor" }, // --- 75
                                      {"����������� ������ �����"                   , "Effective Earth radius" }, // --- 76
                                      {"�������� ����"                              , "Sea disturbance" }, // --- 77
                                      {"���������"                                  , "Amplitude" }, // --- 78
                                      {"�������"                                    , "Frequency" }, // --- 79
                                      {"����"                                       , "Phase" }, // --- 80
                                      {"����������� �����������"                    , "Probability of detection" }, // --- 81
                                      {"����������� ������� �����������"            , "False alarm detection prob." }, // --- 82
                                      {"���� ��������� ��� ������ Pd"               , "Detection zones for other Pd" }, // --- 83
                                      {"��������� ����������� ���"                  , "Elevation axis settings" }, // --- 84
                                      {"�������� ����������� �������"               , "List of elevation marks" }, // --- 85
                                      {"������"                                     , "Cancel" }, // --- 86
                                      {"��"                                         , "Yes" }, // --- 87
                                      {"�������� ����"                              , "Angle value" }, // --- 88
                                      {"���������� �������"                         , "Show mark " }, // --- 89
                                      {"���������"                                  , "Apply" }, // --- 90
                                      {"������"                                     , "Cancel" }, // --- 91
                                      {"  ������� �������"                          , "Delete mark" }, // --- 92
                                      {"  �������� �������"                         , "Add mark" }, // --- 93
                                      {"�������"                                    , "Table" }, // --- 94
                                      {"��������� ������� � ����"                   , "Save table in a file" }, // --- 95
                                      {"������ ����"                                , "Target altitude" }, // --- 96
                                      {"��������� ����������� ����"                 , "Detection range" }, // --- 97
                                      {"��� '�����-������������'"                   , "SRI 'Kvant Radar System'"}, // -----98
                                      {"������ 2.0"                                 , "Version 2.0"}, // -----99
                                      {"� ��� '�������� ������� � ����������'"      , "� Digital systems and technologies"}, // -----100
                                      {"��� '�������� ������� � ����������'"        , "Digital systems and technologies"}, // -----101
                                      {"��������� ��������� :"                      , "Program algorithms :"}, // -----102
                                      {"��������� ��������� :"                      , "Program interface :"}, // -----103
                                      {"����� ���������� �o� ��������� (*.zv)|*.zv" , " File parametrs (*.zv)|*.zv"}, // -----104
                                      {"������� ���� ���������� �o� ���������"      , " Open file"}, // -----105
                                      {"����� ���������� �o� ��������� (*.zv)|*.zv" , " File paramatrs (*.zv)|*.zv"}, // -----106
                                      {"��������� ���� ���������� �o� ���������"    , " Save file"} // -----107
                                     };