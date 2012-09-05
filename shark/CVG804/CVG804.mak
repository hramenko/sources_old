# Generated by the VisualDSP++ IDDE

# Note:  Any changes made to this Makefile will be lost the next time the
# matching project file is loaded into the IDDE.  If you wish to preserve
# changes, rename this file and run it externally to the IDDE.

# The syntax of this Makefile is such that GNU Make v3.77 or higher is
# required.

# The current working directory should be the directory in which this
# Makefile resides.

# Supported targets:
#     CVG804_Release
#     CVG804_Release_clean

# Define this variable if you wish to run this Makefile on a host
# other than the host that created it and VisualDSP++ may be installed
# in a different directory.

ADI_DSP=C:\Program Files\Analog Devices\VisualDSP 5.0


# $VDSP is a gmake-friendly version of ADI_DIR

empty:=
space:= $(empty) $(empty)
VDSP_INTERMEDIATE=$(subst \,/,$(ADI_DSP))
VDSP=$(subst $(space),\$(space),$(VDSP_INTERMEDIATE))

RM=cmd /C del /F /Q

#
# Begin "CVG804_Release" configuration
#

ifeq ($(MAKECMDGOALS),CVG804_Release)

CVG804_Release : ./Release/CVG804.ldr 

./Release/init.doj :./KFF.h ./init.asm $(VDSP)/213xx/include/def21363.h $(VDSP)/213xx/include/def21364.h FIR_P_2265_90_100.dat 
	@echo ".\init.asm"
	$(VDSP)/easm21k.exe .\init.asm -proc ADSP-21364 -file-attr ProjectName=CVG804 -I . -o .\Release\init.doj -MM

./Release/CVG804.dxe :./DSPCARD.LDF ./Release/init.doj ./pb804.doj ./pdes804.doj 
	@echo "Linking..."
	$(VDSP)/cc21k.exe .\Release\init.doj .\pb804.doj .\pdes804.doj -T .\DSPCARD.LDF -flags-link -ip -L .\Release -L . -flags-link -e -flags-link -od,.\Release -o .\Release\CVG804.dxe -proc ADSP-21364 -flags-link -MM

./Release/CVG804.ldr :./Release/CVG804.dxe $(VDSP)/213xx/ldr/36x_spi.dxe 
	@echo "Creating loader file..."
	$(VDSP)/elfloader.exe -bspislave -fBinary -HostWidth8 -l $(VDSP)\213xx\ldr\36x_spi.dxe .\Release\CVG804.dxe -o .\Release\CVG804.ldr -proc ADSP-21364 -MM

endif

ifeq ($(MAKECMDGOALS),CVG804_Release_clean)

CVG804_Release_clean:
	-$(RM) ".\Release\init.doj"
	-$(RM) ".\Release\CVG804.dxe"
	-$(RM) ".\Release\CVG804.ldr"
	-$(RM) ".\Release\*.ipa"
	-$(RM) ".\Release\*.opa"
	-$(RM) ".\Release\*.ti"
	-$(RM) ".\Release\*.pgi"
	-$(RM) ".\*.rbld"

endif

