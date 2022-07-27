CFLAGS := PRINT(.\build\Debug\${c}.lst) OMF2 TABS(4) CODE NOCOND SYMBOLS OPTIMIZE(8,SPEED) SMALL ROM(LARGE) INCDIR(.\..\..\..\..\..\..\Program Files\Keil_v5\C51\INC;.\src) DEFINE(__UVISION_VERSION='526')
CXXFLAGS := PRINT(.\build\Debug\${cpp}.lst) OMF2 TABS(4) CODE NOCOND SYMBOLS OPTIMIZE(8,SPEED) SMALL ROM(LARGE) INCDIR(.\..\..\..\..\..\..\Program Files\Keil_v5\C51\INC;.\src) DEFINE(__UVISION_VERSION='526')
ASMFLAGS := PRINT(.\build\Debug\${asm}.lst) SET(SMALL) INCDIR(.\..\..\..\..\..\..\Program Files\Keil_v5\C51\INC;.\src)
LDFLAGS :=  PRINT(.\build\Debug\4-1.map) REMOVEUNUSED
LDLIBS := 
