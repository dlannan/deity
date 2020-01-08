# Microsoft Developer Studio Project File - Name="System" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=System - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "System.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "System.mak" CFG="System - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "System - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "System - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/DEITY2/Code/System", WGBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "System - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /Gz /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "System - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Gz /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "System - Win32 Release"
# Name "System - Win32 Debug"
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Comm\Apollo.h
# End Source File
# Begin Source File

SOURCE=.\Types\Binary_types.h
# End Source File
# Begin Source File

SOURCE=.\Comm\Comm_types.h
# End Source File
# Begin Source File

SOURCE=.\Api\cpu_types.h
# End Source File
# Begin Source File

SOURCE=.\Types\Crc_types.h
# End Source File
# Begin Source File

SOURCE=.\Comm\Dll_func.h
# End Source File
# Begin Source File

SOURCE=.\Debugging\Error_types.h
# End Source File
# Begin Source File

SOURCE=.\Types\File_types.h
# End Source File
# Begin Source File

SOURCE=.\Types\Func_types.h
# End Source File
# Begin Source File

SOURCE=.\Debugging\Logging.h
# End Source File
# Begin Source File

SOURCE=.\Geometry\Math_types.h
# End Source File
# Begin Source File

SOURCE=.\Geometry\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\Types\Nodelist_types.h
# End Source File
# Begin Source File

SOURCE=.\Types\Packet_types.h
# End Source File
# Begin Source File

SOURCE=.\Debugging\Profile.h
# End Source File
# Begin Source File

SOURCE=.\Api\render_types.h
# End Source File
# Begin Source File

SOURCE=.\Types\String_types.h
# End Source File
# Begin Source File

SOURCE=.\System.h
# End Source File
# Begin Source File

SOURCE=.\Types\Timer_types.h
# End Source File
# Begin Source File

SOURCE=.\Types\Util_types.h
# End Source File
# Begin Source File

SOURCE=.\Geometry\Vertex.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Debugging\Error_types.cpp
# End Source File
# Begin Source File

SOURCE=.\Debugging\Profile.cpp
# End Source File
# End Target
# End Project
