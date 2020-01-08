; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFontMakerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "FontMaker.h"

ClassCount=4
Class1=CFontMakerApp
Class2=CFontMakerDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_FONTMAKER_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_FONTMAKER_DIALOG (English (U.S.))
Class4=LoadProgress
Resource6=IDR_MENU1

[CLS:CFontMakerApp]
Type=0
HeaderFile=FontMaker.h
ImplementationFile=FontMaker.cpp
Filter=N

[CLS:CFontMakerDlg]
Type=0
HeaderFile=FontMakerDlg.h
ImplementationFile=FontMakerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CFontMakerDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=FontMakerDlg.h
ImplementationFile=FontMakerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_FONTMAKER_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CFontMakerDlg

[DLG:IDD_FONTMAKER_DIALOG (English (U.S.))]
Type=1
Class=CFontMakerDlg
ControlCount=0

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=3
Control1=IDC_STATIC,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889

[MNU:IDR_MENU1]
Type=1
Class=CFontMakerDlg
Command1=ID_LOADFONT
Command2=ID_SAVEFONT
Command3=ID_VIEWFONT
CommandCount=3

[CLS:LoadProgress]
Type=0
HeaderFile=LoadProgress.h
ImplementationFile=LoadProgress.cpp
BaseClass=CDialog
Filter=D
LastObject=LoadProgress

