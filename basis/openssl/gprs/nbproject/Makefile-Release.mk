#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=arm-brcm-linux-gnueabi-gcc
CCC=arm-brcm-linux-gnueabi-g++
CXX=arm-brcm-linux-gnueabi-g++
FC=g77
AS=as

# Macros
CND_PLATFORM=Gnueabi-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include NbMakefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/appmain.o


# C Compiler Flags
CFLAGS="-I${SDKV3INC}" -fsigned-char -Wundef -Wstrict-prototypes -Wno-trigraphs -Wimplicit -Wformat 

# CC Compiler Flags
CCFLAGS="-I${SDKV3INC}" -fsigned-char -Wundef -Wno-trigraphs -Wimplicit -Wformat 
CXXFLAGS="-I${SDKV3INC}" -fsigned-char -Wundef -Wno-trigraphs -Wimplicit -Wformat 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lcaethernet -lcafont -lcafs -lcakms -lcalcd -lcamodem -lcapmodem -lcaprt -lcartc -lcauart -lcauldpm -lcausbh -lcagsm -lcabarcode -lpthread -ldl -lcatls -lctosapi -lcrypto -lcurl -lssl -lz -lfreetype -lcrypto

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk dist/V3/App/gprs.exe

dist/V3/App/gprs.exe: ${OBJECTFILES}
	${MKDIR} -p dist/V3/App
	arm-brcm-linux-gnueabi-g++ -o dist/V3/App/gprs ${OBJECTFILES} ${LDLIBSOPTIONS} -L . "-L${SDKV3LIB}" "-L${SDKV3LIBN}"

${OBJECTDIR}/appmain.o: appmain.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O2 -I/cygdrive/C/Program\ Files/Castles/VEGA3000/include -I/cygdrive/C/Program\ Files\ \(x86\)/Castles/VEGA3000/include -o ${OBJECTDIR}/appmain.o appmain.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
