################################################
# Make file for CASIO fx-9860G SDK Addin
#
############################
# Directory defines
TCDIR = C:\Program Files\CASIO\fx-9860G SDK\OS\SH
OSDIR = C:\Program Files\CASIO\fx-9860G SDK\OS
APPDIR = E:\Anderain\fx-9860\BCE-fx
OUTDIR = E:\Anderain\fx-9860\BCE-fx\Debug

################
# Main Defines
SH_EXEDIR=$(TCDIR)\bin

# Hitachi SH C/C++ Compiler02 phase
SHCC02_EXE=shc.exe
SHCC02_DEP="$(OSDIR)\FX\include\fxlib.h" "include\bce.h" "include\blockconf.h" "include\blockmem.h" "include\bool.h" "include\bracket.h" \
      "include\element.h" "include\equation.h" "include\fix.h" "include\fraction.h" "include\kasprintf.h" "include\matrix_io.h" "include\mmdiv.h" \
      "include\molecule.h" "include\polynomial.h" "include\string_ext.h" "include\syntax.h" "dConsole.h"

# Hitachi SH Assembler03 phase
SHASM03_EXE=asmsh.exe

# Hitachi OptLinker04 phase
SHLINK04_EXE=Optlnk.exe
SHLINK04_DEP="$(OSDIR)\FX\lib\fx9860G_library.lib"
SHLINK04_DEP2="$(OSDIR)\FX\lib\setup.obj"

#######################
# Files to build
FILE0=main
FILESRC0="$(APPDIR)\$(FILE0).c"
FILEOBJ0="$(OUTDIR)\$(FILE0).obj"
FILE1=bce
FILESRC1="$(APPDIR)\kernel\$(FILE1).c"
FILEOBJ1="$(OUTDIR)\$(FILE1).obj"
FILE2=equation
FILESRC2="$(APPDIR)\kernel\$(FILE2).c"
FILEOBJ2="$(OUTDIR)\$(FILE2).obj"
FILE3=fix
FILESRC3="$(APPDIR)\kernel\$(FILE3).c"
FILEOBJ3="$(OUTDIR)\$(FILE3).obj"
FILE4=fraction
FILESRC4="$(APPDIR)\kernel\$(FILE4).c"
FILEOBJ4="$(OUTDIR)\$(FILE4).obj"
FILE5=matrix_io
FILESRC5="$(APPDIR)\kernel\$(FILE5).c"
FILEOBJ5="$(OUTDIR)\$(FILE5).obj"
FILE6=mmdiv
FILESRC6="$(APPDIR)\kernel\$(FILE6).c"
FILEOBJ6="$(OUTDIR)\$(FILE6).obj"
FILE7=molecule
FILESRC7="$(APPDIR)\kernel\$(FILE7).c"
FILEOBJ7="$(OUTDIR)\$(FILE7).obj"
FILE8=polynomial
FILESRC8="$(APPDIR)\kernel\$(FILE8).c"
FILEOBJ8="$(OUTDIR)\$(FILE8).obj"
FILE9=syntax
FILESRC9="$(APPDIR)\kernel\$(FILE9).c"
FILEOBJ9="$(OUTDIR)\$(FILE9).obj"
FILE10=blockmem
FILESRC10="$(APPDIR)\lib\$(FILE10).c"
FILEOBJ10="$(OUTDIR)\$(FILE10).obj"
FILE11=kasprintf
FILESRC11="$(APPDIR)\lib\$(FILE11).c"
FILEOBJ11="$(OUTDIR)\$(FILE11).obj"
FILE12=string_ext
FILESRC12="$(APPDIR)\lib\$(FILE12).c"
FILEOBJ12="$(OUTDIR)\$(FILE12).obj"
FILE13=dConsole
FILESRC13="$(APPDIR)\$(FILE13).c"
FILEOBJ13="$(OUTDIR)\$(FILE13).obj"
RFILE=FXADDINror
USERALLOBJ=$(FILEOBJ0) $(FILEOBJ1) $(FILEOBJ2) $(FILEOBJ3) $(FILEOBJ4) $(FILEOBJ5) $(FILEOBJ6) $(FILEOBJ7) $(FILEOBJ8) $(FILEOBJ9) \
      $(FILEOBJ10) $(FILEOBJ11) $(FILEOBJ12) $(FILEOBJ13)

#######################
# nmake "all" statement

ALL:  SH_ENV \
	$(USERALLOBJ) \
	$(OUTDIR)\$(RFILE).bin \

####################
# Description blocks

!MESSAGE %3#C$z`&'0?
!MESSAGE
!MESSAGE Executing Hitachi SH C/C++ Compiler/Assembler phase
!MESSAGE

SH_ENV : 
	set SHC_INC=$(TCDIR)\include
	set PATH=$(TCDIR)\bin
	set SHC_LIB=$(TCDIR)\bin
	set SHC_TMP=$(OUTDIR)

$(FILEOBJ0) : $(FILESRC0) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ0)
-show=source
-listfile="$(OUTDIR)\$(FILE0).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC0)
-lang=c
-nologo
-debug
<<

$(FILEOBJ1) : $(FILESRC1) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ1)
-show=source
-listfile="$(OUTDIR)\$(FILE1).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC1)
-lang=c
-nologo
-debug
<<

$(FILEOBJ2) : $(FILESRC2) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ2)
-show=source
-listfile="$(OUTDIR)\$(FILE2).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC2)
-lang=c
-nologo
-debug
<<

$(FILEOBJ3) : $(FILESRC3) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ3)
-show=source
-listfile="$(OUTDIR)\$(FILE3).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC3)
-lang=c
-nologo
-debug
<<

$(FILEOBJ4) : $(FILESRC4) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ4)
-show=source
-listfile="$(OUTDIR)\$(FILE4).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC4)
-lang=c
-nologo
-debug
<<

$(FILEOBJ5) : $(FILESRC5) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ5)
-show=source
-listfile="$(OUTDIR)\$(FILE5).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC5)
-lang=c
-nologo
-debug
<<

$(FILEOBJ6) : $(FILESRC6) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ6)
-show=source
-listfile="$(OUTDIR)\$(FILE6).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC6)
-lang=c
-nologo
-debug
<<

$(FILEOBJ7) : $(FILESRC7) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ7)
-show=source
-listfile="$(OUTDIR)\$(FILE7).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC7)
-lang=c
-nologo
-debug
<<

$(FILEOBJ8) : $(FILESRC8) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ8)
-show=source
-listfile="$(OUTDIR)\$(FILE8).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC8)
-lang=c
-nologo
-debug
<<

$(FILEOBJ9) : $(FILESRC9) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ9)
-show=source
-listfile="$(OUTDIR)\$(FILE9).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC9)
-lang=c
-nologo
-debug
<<

$(FILEOBJ10) : $(FILESRC10) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ10)
-show=source
-listfile="$(OUTDIR)\$(FILE10).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC10)
-lang=c
-nologo
-debug
<<

$(FILEOBJ11) : $(FILESRC11) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ11)
-show=source
-listfile="$(OUTDIR)\$(FILE11).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC11)
-lang=c
-nologo
-debug
<<

$(FILEOBJ12) : $(FILESRC12) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ12)
-show=source
-listfile="$(OUTDIR)\$(FILE12).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC12)
-lang=c
-nologo
-debug
<<

$(FILEOBJ13) : $(FILESRC13) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ13)
-show=source
-listfile="$(OUTDIR)\$(FILE13).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC13)
-lang=c
-nologo
-debug
<<

!MESSAGE
!MESSAGE Executing Hitachi OptLinker04 phase
!MESSAGE

"$(OUTDIR)\$(RFILE).bin" : $(USERALLOBJ) $(SHLINK04_DEP2) $(SHLINK04_DEP)
	"$(SH_EXEDIR)\$(SHLINK04_EXE)" -subcommand=<<
noprelink
sdebug
rom D=R
nomessage 
list "$(OUTDIR)\$(RFILE).map"
show symbol
nooptimize
start P_TOP,P,C,D,C$VTBL,C$INIT/0300200,B_BR_Size,B,R/08100000
fsymbol P
nologo
input $(USERALLOBJ)
input $(SHLINK04_DEP2)
library $(SHLINK04_DEP)
output "$(OUTDIR)\$(RFILE).abs"
-nomessage=1100
end
input "$(OUTDIR)\$(RFILE).abs"
form binary
output "$(OUTDIR)\$(RFILE).bin"
exit
<<

