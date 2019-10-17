##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug_Win32
ProjectName            :=sys80b-sim-rb
ConfigurationName      :=Debug_Win32
WorkspacePath          :=C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb
ProjectPath            :=C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Fabrz
Date                   :=13/03/2018
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/TDM-GCC-32/bin/g++.exe
SharedObjectLinkerName :=C:/TDM-GCC-32/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).exe
Preprocessors          :=$(PreprocessorSwitch)__HARDWARE_MK20dx256__ $(PreprocessorSwitch)__HARDWARE_MK20DX256__ $(PreprocessorSwitch)__MK20DX256__ $(PreprocessorSwitch)TEENSYDUINO=140 $(PreprocessorSwitch)ARDUINO=10805 $(PreprocessorSwitch)F_CPU=72000000 $(PreprocessorSwitch)USB_SERIAL $(PreprocessorSwitch)LAYOUT_ITALIAN $(PreprocessorSwitch)__cplusplus=201103L $(PreprocessorSwitch)_VMICRO_INTELLISENSE 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="sys80b-sim-rb.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/TDM-GCC-32/bin/windres.exe
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)$(VS_ProjectDir)../sys80b-sim-rb $(IncludeSwitch)"$(VS_ProjectDir)../../../../../../../Program Files (x86)/Arduino/hardware/teensy/avr/libraries/Bounce" $(IncludeSwitch)"$(VS_ProjectDir)../../../../../../../Program Files (x86)/Arduino/hardware/teensy/avr/libraries/LiquidCrystalFast" $(IncludeSwitch)$(VS_ProjectDir)../libraries/MemoryFree-master $(IncludeSwitch)$(VS_ProjectDir)../libraries/SD/src $(IncludeSwitch)$(VS_ProjectDir)../libraries/SD/src/utility $(IncludeSwitch)"$(VS_ProjectDir)../../../../../../../Program Files (x86)/Arduino/hardware/teensy/avr/libraries/SPI" $(IncludeSwitch)"$(VS_ProjectDir)../../../../../../../Program Files (x86)/Arduino/libraries" $(IncludeSwitch)"$(VS_ProjectDir)../../../../../../../Program Files (x86)/Arduino/hardware/teensy/avr/libraries" $(IncludeSwitch)$(VS_ProjectDir)../libraries $(IncludeSwitch)"$(VS_ProjectDir)../../../../../../../Program Files (x86)/Arduino/hardware/teensy/avr/cores/teensy3" $(IncludeSwitch)"$(VS_ProjectDir)../../../../../../../Program Files (x86)/Arduino/hardware/teensy/avr/cores/teensy3/avr" $(IncludeSwitch)"$(VS_ProjectDir)../../../../../../../Program Files (x86)/Arduino/hardware/teensy/avr/cores/teensy3/util" $(IncludeSwitch)"$(VS_ProjectDir)../../../../../../../Program Files (x86)/Arduino/hardware/teensy/avr/cores/teensy3/utils" $(IncludeSwitch)"$(VS_ProjectDir)../../../../../../../Program Files (x86)/Arduino/hardware/teensy/avr/cores/teensy3/avr" $(IncludeSwitch)"$(VS_ProjectDir)../../../../../../../Program Files (x86)/Arduino/hardware/tools/arm/arm-none-eabi/include" $(IncludeSwitch)"$(VS_ProjectDir)../../../../../../../Program Files (x86)/Arduino/hardware/teensy" 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/TDM-GCC-32/bin/ar.exe rcu
CXX      := C:/TDM-GCC-32/bin/g++.exe
CC       := C:/TDM-GCC-32/bin/gcc.exe
CXXFLAGS :=  -g -Wall $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := C:/TDM-GCC-32/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
VS_Configuration:=Debug
VS_IntDir:=./Debug/
VS_OutDir:=./Debug/
VS_Platform:=Win32
VS_ProjectDir:=C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/
VS_ProjectName:=sys80b-sim-rb
VS_SolutionDir:=C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/LightGroup.cpp$(ObjectSuffix) $(IntermediateDirectory)/nvData.cpp$(ObjectSuffix) $(IntermediateDirectory)/BigHouse.cpp$(ObjectSuffix) $(IntermediateDirectory)/LightSet.cpp$(ObjectSuffix) $(IntermediateDirectory)/Player.cpp$(ObjectSuffix) $(IntermediateDirectory)/TimerTask.cpp$(ObjectSuffix) $(IntermediateDirectory)/Game.cpp$(ObjectSuffix) $(IntermediateDirectory)/sdio.cpp$(ObjectSuffix) $(IntermediateDirectory)/BoardIO.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/uiMenu.cpp$(ObjectSuffix) $(IntermediateDirectory)/msgOutput.cpp$(ObjectSuffix) $(IntermediateDirectory)/Sys80b.cpp$(ObjectSuffix) $(IntermediateDirectory)/SwitchGrid.cpp$(ObjectSuffix) $(IntermediateDirectory)/ledGrid.cpp$(ObjectSuffix) $(IntermediateDirectory)/Led8x8.cpp$(ObjectSuffix) $(IntermediateDirectory)/lcdOutput.cpp$(ObjectSuffix) $(IntermediateDirectory)/LampSet.cpp$(ObjectSuffix) $(IntermediateDirectory)/displFX.cpp$(ObjectSuffix) $(IntermediateDirectory)/SoundBuff.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Display.cpp$(ObjectSuffix) $(IntermediateDirectory)/BaseLogic.cpp$(ObjectSuffix) $(IntermediateDirectory)/Light.cpp$(ObjectSuffix) $(IntermediateDirectory)/PIO.cpp$(ObjectSuffix) $(IntermediateDirectory)/TimerSet.cpp$(ObjectSuffix) $(IntermediateDirectory)/SolenoidSet.cpp$(ObjectSuffix) $(IntermediateDirectory)/Solenoid.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/LightGroup.cpp$(ObjectSuffix): LightGroup.cpp $(IntermediateDirectory)/LightGroup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/LightGroup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LightGroup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LightGroup.cpp$(DependSuffix): LightGroup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LightGroup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LightGroup.cpp$(DependSuffix) -MM LightGroup.cpp

$(IntermediateDirectory)/LightGroup.cpp$(PreprocessSuffix): LightGroup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LightGroup.cpp$(PreprocessSuffix) LightGroup.cpp

$(IntermediateDirectory)/nvData.cpp$(ObjectSuffix): nvData.cpp $(IntermediateDirectory)/nvData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/nvData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/nvData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/nvData.cpp$(DependSuffix): nvData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/nvData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/nvData.cpp$(DependSuffix) -MM nvData.cpp

$(IntermediateDirectory)/nvData.cpp$(PreprocessSuffix): nvData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/nvData.cpp$(PreprocessSuffix) nvData.cpp

$(IntermediateDirectory)/BigHouse.cpp$(ObjectSuffix): BigHouse.cpp $(IntermediateDirectory)/BigHouse.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/BigHouse.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BigHouse.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BigHouse.cpp$(DependSuffix): BigHouse.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BigHouse.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BigHouse.cpp$(DependSuffix) -MM BigHouse.cpp

$(IntermediateDirectory)/BigHouse.cpp$(PreprocessSuffix): BigHouse.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BigHouse.cpp$(PreprocessSuffix) BigHouse.cpp

$(IntermediateDirectory)/LightSet.cpp$(ObjectSuffix): LightSet.cpp $(IntermediateDirectory)/LightSet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/LightSet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LightSet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LightSet.cpp$(DependSuffix): LightSet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LightSet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LightSet.cpp$(DependSuffix) -MM LightSet.cpp

$(IntermediateDirectory)/LightSet.cpp$(PreprocessSuffix): LightSet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LightSet.cpp$(PreprocessSuffix) LightSet.cpp

$(IntermediateDirectory)/Player.cpp$(ObjectSuffix): Player.cpp $(IntermediateDirectory)/Player.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/Player.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Player.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Player.cpp$(DependSuffix): Player.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Player.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Player.cpp$(DependSuffix) -MM Player.cpp

$(IntermediateDirectory)/Player.cpp$(PreprocessSuffix): Player.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Player.cpp$(PreprocessSuffix) Player.cpp

$(IntermediateDirectory)/TimerTask.cpp$(ObjectSuffix): TimerTask.cpp $(IntermediateDirectory)/TimerTask.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/TimerTask.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TimerTask.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TimerTask.cpp$(DependSuffix): TimerTask.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TimerTask.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TimerTask.cpp$(DependSuffix) -MM TimerTask.cpp

$(IntermediateDirectory)/TimerTask.cpp$(PreprocessSuffix): TimerTask.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TimerTask.cpp$(PreprocessSuffix) TimerTask.cpp

$(IntermediateDirectory)/Game.cpp$(ObjectSuffix): Game.cpp $(IntermediateDirectory)/Game.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/Game.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Game.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Game.cpp$(DependSuffix): Game.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Game.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Game.cpp$(DependSuffix) -MM Game.cpp

$(IntermediateDirectory)/Game.cpp$(PreprocessSuffix): Game.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Game.cpp$(PreprocessSuffix) Game.cpp

$(IntermediateDirectory)/sdio.cpp$(ObjectSuffix): sdio.cpp $(IntermediateDirectory)/sdio.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/sdio.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdio.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdio.cpp$(DependSuffix): sdio.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sdio.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/sdio.cpp$(DependSuffix) -MM sdio.cpp

$(IntermediateDirectory)/sdio.cpp$(PreprocessSuffix): sdio.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdio.cpp$(PreprocessSuffix) sdio.cpp

$(IntermediateDirectory)/BoardIO.cpp$(ObjectSuffix): BoardIO.cpp $(IntermediateDirectory)/BoardIO.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/BoardIO.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BoardIO.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BoardIO.cpp$(DependSuffix): BoardIO.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BoardIO.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BoardIO.cpp$(DependSuffix) -MM BoardIO.cpp

$(IntermediateDirectory)/BoardIO.cpp$(PreprocessSuffix): BoardIO.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BoardIO.cpp$(PreprocessSuffix) BoardIO.cpp

$(IntermediateDirectory)/uiMenu.cpp$(ObjectSuffix): uiMenu.cpp $(IntermediateDirectory)/uiMenu.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/uiMenu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/uiMenu.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/uiMenu.cpp$(DependSuffix): uiMenu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/uiMenu.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/uiMenu.cpp$(DependSuffix) -MM uiMenu.cpp

$(IntermediateDirectory)/uiMenu.cpp$(PreprocessSuffix): uiMenu.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/uiMenu.cpp$(PreprocessSuffix) uiMenu.cpp

$(IntermediateDirectory)/msgOutput.cpp$(ObjectSuffix): msgOutput.cpp $(IntermediateDirectory)/msgOutput.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/msgOutput.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/msgOutput.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/msgOutput.cpp$(DependSuffix): msgOutput.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/msgOutput.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/msgOutput.cpp$(DependSuffix) -MM msgOutput.cpp

$(IntermediateDirectory)/msgOutput.cpp$(PreprocessSuffix): msgOutput.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/msgOutput.cpp$(PreprocessSuffix) msgOutput.cpp

$(IntermediateDirectory)/Sys80b.cpp$(ObjectSuffix): Sys80b.cpp $(IntermediateDirectory)/Sys80b.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/Sys80b.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Sys80b.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Sys80b.cpp$(DependSuffix): Sys80b.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Sys80b.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Sys80b.cpp$(DependSuffix) -MM Sys80b.cpp

$(IntermediateDirectory)/Sys80b.cpp$(PreprocessSuffix): Sys80b.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Sys80b.cpp$(PreprocessSuffix) Sys80b.cpp

$(IntermediateDirectory)/SwitchGrid.cpp$(ObjectSuffix): SwitchGrid.cpp $(IntermediateDirectory)/SwitchGrid.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/SwitchGrid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SwitchGrid.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SwitchGrid.cpp$(DependSuffix): SwitchGrid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SwitchGrid.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SwitchGrid.cpp$(DependSuffix) -MM SwitchGrid.cpp

$(IntermediateDirectory)/SwitchGrid.cpp$(PreprocessSuffix): SwitchGrid.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SwitchGrid.cpp$(PreprocessSuffix) SwitchGrid.cpp

$(IntermediateDirectory)/ledGrid.cpp$(ObjectSuffix): ledGrid.cpp $(IntermediateDirectory)/ledGrid.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/ledGrid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ledGrid.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ledGrid.cpp$(DependSuffix): ledGrid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ledGrid.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ledGrid.cpp$(DependSuffix) -MM ledGrid.cpp

$(IntermediateDirectory)/ledGrid.cpp$(PreprocessSuffix): ledGrid.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ledGrid.cpp$(PreprocessSuffix) ledGrid.cpp

$(IntermediateDirectory)/Led8x8.cpp$(ObjectSuffix): Led8x8.cpp $(IntermediateDirectory)/Led8x8.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/Led8x8.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Led8x8.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Led8x8.cpp$(DependSuffix): Led8x8.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Led8x8.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Led8x8.cpp$(DependSuffix) -MM Led8x8.cpp

$(IntermediateDirectory)/Led8x8.cpp$(PreprocessSuffix): Led8x8.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Led8x8.cpp$(PreprocessSuffix) Led8x8.cpp

$(IntermediateDirectory)/lcdOutput.cpp$(ObjectSuffix): lcdOutput.cpp $(IntermediateDirectory)/lcdOutput.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/lcdOutput.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcdOutput.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcdOutput.cpp$(DependSuffix): lcdOutput.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcdOutput.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lcdOutput.cpp$(DependSuffix) -MM lcdOutput.cpp

$(IntermediateDirectory)/lcdOutput.cpp$(PreprocessSuffix): lcdOutput.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcdOutput.cpp$(PreprocessSuffix) lcdOutput.cpp

$(IntermediateDirectory)/LampSet.cpp$(ObjectSuffix): LampSet.cpp $(IntermediateDirectory)/LampSet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/LampSet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LampSet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LampSet.cpp$(DependSuffix): LampSet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LampSet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LampSet.cpp$(DependSuffix) -MM LampSet.cpp

$(IntermediateDirectory)/LampSet.cpp$(PreprocessSuffix): LampSet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LampSet.cpp$(PreprocessSuffix) LampSet.cpp

$(IntermediateDirectory)/displFX.cpp$(ObjectSuffix): displFX.cpp $(IntermediateDirectory)/displFX.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/displFX.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/displFX.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/displFX.cpp$(DependSuffix): displFX.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/displFX.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/displFX.cpp$(DependSuffix) -MM displFX.cpp

$(IntermediateDirectory)/displFX.cpp$(PreprocessSuffix): displFX.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/displFX.cpp$(PreprocessSuffix) displFX.cpp

$(IntermediateDirectory)/SoundBuff.cpp$(ObjectSuffix): SoundBuff.cpp $(IntermediateDirectory)/SoundBuff.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/SoundBuff.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SoundBuff.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SoundBuff.cpp$(DependSuffix): SoundBuff.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SoundBuff.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SoundBuff.cpp$(DependSuffix) -MM SoundBuff.cpp

$(IntermediateDirectory)/SoundBuff.cpp$(PreprocessSuffix): SoundBuff.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SoundBuff.cpp$(PreprocessSuffix) SoundBuff.cpp

$(IntermediateDirectory)/Display.cpp$(ObjectSuffix): Display.cpp $(IntermediateDirectory)/Display.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/Display.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Display.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Display.cpp$(DependSuffix): Display.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Display.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Display.cpp$(DependSuffix) -MM Display.cpp

$(IntermediateDirectory)/Display.cpp$(PreprocessSuffix): Display.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Display.cpp$(PreprocessSuffix) Display.cpp

$(IntermediateDirectory)/BaseLogic.cpp$(ObjectSuffix): BaseLogic.cpp $(IntermediateDirectory)/BaseLogic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/BaseLogic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BaseLogic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BaseLogic.cpp$(DependSuffix): BaseLogic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BaseLogic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BaseLogic.cpp$(DependSuffix) -MM BaseLogic.cpp

$(IntermediateDirectory)/BaseLogic.cpp$(PreprocessSuffix): BaseLogic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BaseLogic.cpp$(PreprocessSuffix) BaseLogic.cpp

$(IntermediateDirectory)/Light.cpp$(ObjectSuffix): Light.cpp $(IntermediateDirectory)/Light.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/Light.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Light.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Light.cpp$(DependSuffix): Light.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Light.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Light.cpp$(DependSuffix) -MM Light.cpp

$(IntermediateDirectory)/Light.cpp$(PreprocessSuffix): Light.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Light.cpp$(PreprocessSuffix) Light.cpp

$(IntermediateDirectory)/PIO.cpp$(ObjectSuffix): PIO.cpp $(IntermediateDirectory)/PIO.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/PIO.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PIO.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PIO.cpp$(DependSuffix): PIO.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PIO.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PIO.cpp$(DependSuffix) -MM PIO.cpp

$(IntermediateDirectory)/PIO.cpp$(PreprocessSuffix): PIO.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PIO.cpp$(PreprocessSuffix) PIO.cpp

$(IntermediateDirectory)/TimerSet.cpp$(ObjectSuffix): TimerSet.cpp $(IntermediateDirectory)/TimerSet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/TimerSet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TimerSet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TimerSet.cpp$(DependSuffix): TimerSet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TimerSet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TimerSet.cpp$(DependSuffix) -MM TimerSet.cpp

$(IntermediateDirectory)/TimerSet.cpp$(PreprocessSuffix): TimerSet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TimerSet.cpp$(PreprocessSuffix) TimerSet.cpp

$(IntermediateDirectory)/SolenoidSet.cpp$(ObjectSuffix): SolenoidSet.cpp $(IntermediateDirectory)/SolenoidSet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/SolenoidSet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SolenoidSet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SolenoidSet.cpp$(DependSuffix): SolenoidSet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SolenoidSet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SolenoidSet.cpp$(DependSuffix) -MM SolenoidSet.cpp

$(IntermediateDirectory)/SolenoidSet.cpp$(PreprocessSuffix): SolenoidSet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SolenoidSet.cpp$(PreprocessSuffix) SolenoidSet.cpp

$(IntermediateDirectory)/Solenoid.cpp$(ObjectSuffix): Solenoid.cpp $(IntermediateDirectory)/Solenoid.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Fabrz/Dropbox/Personal/src/GitHub/sys80b-sim-rb/Solenoid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Solenoid.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Solenoid.cpp$(DependSuffix): Solenoid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Solenoid.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Solenoid.cpp$(DependSuffix) -MM Solenoid.cpp

$(IntermediateDirectory)/Solenoid.cpp$(PreprocessSuffix): Solenoid.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Solenoid.cpp$(PreprocessSuffix) Solenoid.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


