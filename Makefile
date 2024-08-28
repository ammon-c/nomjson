#---------------------------------------------------------------------
# NMAKE build script for NomJSON, a C++ module for parsing JSON files.
#
# On NMAKE command line, use RELEASE=1 to select release build instead
# of debug build, and use WIN64=1 to select 64-bit build instead
# of 32-bit build.
#---------------------------------------------------------------------

#### TODO:  the vc140.pdb file should go in the obj folder.

!ifndef RELEASE
DIR_SUFFIX=
CPPFLAGS2=   -MTd -Od -Zi -EHsc
!else
DIR_SUFFIX=r
CPPFLAGS2=   -MT -Ox -EHsc
!endif

CPPFLAGS=   -nologo -c -Gs -EHsc -W4 -WX -DWIN32 $(CPPFLAGS2)
!ifndef WIN64
OBJDIR=     obj$(DIR_SUFFIX)
EXEDIR=     bin$(DIR_SUFFIX)
!else
OBJDIR=     obj64$(DIR_SUFFIX)
EXEDIR=     bin64$(DIR_SUFFIX)
!endif

.SUFFIXES:
.SUFFIXES:   .cpp

{.}.cpp{$(OBJDIR)}.obj:
   cl $(CPPFLAGS) -Fo$*.obj $<

all:  $(OBJDIR) $(EXEDIR) $(EXEDIR)\nomjsontest.exe

$(OBJDIR):
   if not exist $(OBJDIR)/$(NULL) mkdir $(OBJDIR)

$(EXEDIR):
   if not exist $(EXEDIR)/$(NULL) mkdir $(EXEDIR)

$(EXEDIR)\nomjsontest.exe:   $(OBJDIR)\nomjsontest.obj $(OBJDIR)\nomjson.obj
   if exist link.tmp del link.tmp
   @echo /OUT:$@                    >> link.tmp
   @echo /DEBUG                     >> link.tmp
   @echo /SUBSYSTEM:CONSOLE         >> link.tmp
   @echo $(OBJDIR)\nomjsontest.obj  >> link.tmp
   @echo $(OBJDIR)\nomjson.obj      >> link.tmp
   @echo user32.lib gdi32.lib comdlg32.lib      >> link.tmp
   @echo shell32.lib advapi32.lib winmm.lib     >> link.tmp
   @echo comctl32.lib kernel32.lib wininet.lib  >> link.tmp
   link /NOLOGO @link.tmp
   if exist link.tmp del link.tmp

$(OBJDIR)\nomjson.obj:     nomjson.cpp nomjson.h trace.h
$(OBJDIR)\nomjsontest.obj: nomjsontest.cpp nomjson.h trace.h

clean:
   echo Cleaning.
   if exist lib.tmp del lib.tmp
   if exist link.tmp del link.tmp
   if exist *.obj del *.obj
   if exist *.lst del *.lst
   if exist *.bak del *.bak
   if exist *.aps del *.aps
   if exist *.res del *.res
   if exist *.map del *.map
   if exist *.exp del *.exp
   if exist *.pdb del *.pdb
   if exist *.ilk del *.ilk
   if exist *.lib del *.lib
   if exist *.exp del *.exp
   if exist *.exe del *.exe
   if exist *.dll del *.dll
   if exist *.vc* del *.vc*
   if exist $(OBJDIR)\*.obj del $(OBJDIR)\*.obj
   if exist $(OBJDIR)\*.res del $(OBJDIR)\*.res
   if exist $(OBJDIR)\$(NULL) rmdir $(OBJDIR)
   if exist $(EXEDIR)\*.exe del $(EXEDIR)\*.exe
   if exist $(EXEDIR)\*.mac del $(EXEDIR)\*.mac
   if exist $(EXEDIR)\*.pdb del $(EXEDIR)\*.pdb
   if exist $(EXEDIR)\*.ini del $(EXEDIR)\*.ini
   if exist $(EXEDIR)\*.ilk del $(EXEDIR)\*.ilk
   if exist $(EXEDIR)\$(NULL) rmdir $(EXEDIR)
   if exist err del err
   if exist z del z
   echo Cleaned.

