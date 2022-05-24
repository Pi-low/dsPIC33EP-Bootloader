#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Bootloader_UART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Bootloader_UART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=01-SRC/01-MAIN/bootloader.c 01-SRC/01-MAIN/main.c 01-SRC/02-FLAH_ROUTINES/flash_routines.c 01-SRC/03-TARGET/target.c 01-SRC/04-CRC/crc.c mcc_generated_files/memory/flash.s mcc_generated_files/memory/flash_demo.c mcc_generated_files/mcc.c mcc_generated_files/system.c mcc_generated_files/traps.c mcc_generated_files/clock.c mcc_generated_files/tmr1.c mcc_generated_files/interrupt_manager.c mcc_generated_files/reset.c mcc_generated_files/adc1.c mcc_generated_files/uart1.c mcc_generated_files/pin_manager.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o ${OBJECTDIR}/01-SRC/01-MAIN/main.o ${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o ${OBJECTDIR}/01-SRC/03-TARGET/target.o ${OBJECTDIR}/01-SRC/04-CRC/crc.o ${OBJECTDIR}/mcc_generated_files/memory/flash.o ${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/reset.o ${OBJECTDIR}/mcc_generated_files/adc1.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o
POSSIBLE_DEPFILES=${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o.d ${OBJECTDIR}/01-SRC/01-MAIN/main.o.d ${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o.d ${OBJECTDIR}/01-SRC/03-TARGET/target.o.d ${OBJECTDIR}/01-SRC/04-CRC/crc.o.d ${OBJECTDIR}/mcc_generated_files/memory/flash.o.d ${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o.d ${OBJECTDIR}/mcc_generated_files/mcc.o.d ${OBJECTDIR}/mcc_generated_files/system.o.d ${OBJECTDIR}/mcc_generated_files/traps.o.d ${OBJECTDIR}/mcc_generated_files/clock.o.d ${OBJECTDIR}/mcc_generated_files/tmr1.o.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d ${OBJECTDIR}/mcc_generated_files/reset.o.d ${OBJECTDIR}/mcc_generated_files/adc1.o.d ${OBJECTDIR}/mcc_generated_files/uart1.o.d ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o ${OBJECTDIR}/01-SRC/01-MAIN/main.o ${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o ${OBJECTDIR}/01-SRC/03-TARGET/target.o ${OBJECTDIR}/01-SRC/04-CRC/crc.o ${OBJECTDIR}/mcc_generated_files/memory/flash.o ${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/reset.o ${OBJECTDIR}/mcc_generated_files/adc1.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o

# Source Files
SOURCEFILES=01-SRC/01-MAIN/bootloader.c 01-SRC/01-MAIN/main.c 01-SRC/02-FLAH_ROUTINES/flash_routines.c 01-SRC/03-TARGET/target.c 01-SRC/04-CRC/crc.c mcc_generated_files/memory/flash.s mcc_generated_files/memory/flash_demo.c mcc_generated_files/mcc.c mcc_generated_files/system.c mcc_generated_files/traps.c mcc_generated_files/clock.c mcc_generated_files/tmr1.c mcc_generated_files/interrupt_manager.c mcc_generated_files/reset.c mcc_generated_files/adc1.c mcc_generated_files/uart1.c mcc_generated_files/pin_manager.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Bootloader_UART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512GM604
MP_LINKER_FILE_OPTION=,--script="01-SRC\00-LINKER\Linker_Boot.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o: 01-SRC/01-MAIN/bootloader.c  .generated_files/flags/default/b6a99e5ebb04ba79a81845b6c43dfc191738ea8f .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/01-SRC/01-MAIN" 
	@${RM} ${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o.d 
	@${RM} ${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  01-SRC/01-MAIN/bootloader.c  -o ${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/01-SRC/01-MAIN/main.o: 01-SRC/01-MAIN/main.c  .generated_files/flags/default/fa66a2b031cc9150739c26b890ba0b653121a5ac .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/01-SRC/01-MAIN" 
	@${RM} ${OBJECTDIR}/01-SRC/01-MAIN/main.o.d 
	@${RM} ${OBJECTDIR}/01-SRC/01-MAIN/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  01-SRC/01-MAIN/main.c  -o ${OBJECTDIR}/01-SRC/01-MAIN/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/01-SRC/01-MAIN/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o: 01-SRC/02-FLAH_ROUTINES/flash_routines.c  .generated_files/flags/default/f0a177d80aeb98888146685fdcd24c588520ada7 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES" 
	@${RM} ${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o.d 
	@${RM} ${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  01-SRC/02-FLAH_ROUTINES/flash_routines.c  -o ${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/01-SRC/03-TARGET/target.o: 01-SRC/03-TARGET/target.c  .generated_files/flags/default/c9beb89bcab79415501105d0ea262865a98e73be .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/01-SRC/03-TARGET" 
	@${RM} ${OBJECTDIR}/01-SRC/03-TARGET/target.o.d 
	@${RM} ${OBJECTDIR}/01-SRC/03-TARGET/target.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  01-SRC/03-TARGET/target.c  -o ${OBJECTDIR}/01-SRC/03-TARGET/target.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/01-SRC/03-TARGET/target.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/01-SRC/04-CRC/crc.o: 01-SRC/04-CRC/crc.c  .generated_files/flags/default/4f1ec764014d92ea25fae8c1a3eda3ade59bd679 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/01-SRC/04-CRC" 
	@${RM} ${OBJECTDIR}/01-SRC/04-CRC/crc.o.d 
	@${RM} ${OBJECTDIR}/01-SRC/04-CRC/crc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  01-SRC/04-CRC/crc.c  -o ${OBJECTDIR}/01-SRC/04-CRC/crc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/01-SRC/04-CRC/crc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o: mcc_generated_files/memory/flash_demo.c  .generated_files/flags/default/c4dd9bfd0376b2a0bae373dc1db71de96fe0efe6 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/memory" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/memory/flash_demo.c  -o ${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/default/d2e4d6d3e2dc2cf1194ab97225a29b8020ce6659 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/default/fe38f79e353c4fe6b24fd19bc1a83f2a34ac4493 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/default/c80b092858841e73a4335567928002a52d4c1778 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/default/60d1d0afece0560a1c2116ea2599275923ec5414 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/default/3e560cabc2b6c064fd65cce1c9d83f9ffa344d9f .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/default/60e8d8c66e515c1374d21d3e731aa4a67d8ea672 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  .generated_files/flags/default/3667d23a14409a9c2811441771a8089d5ab1f101 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  .generated_files/flags/default/dd50077200fbedd10f9a21087992fb51b8e222e0 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/default/d1a526807030b05d2ebfd4b7835efbe15c4f53fa .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/default/a2964f49cac02a909b3e728945f48501f0b63325 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o: 01-SRC/01-MAIN/bootloader.c  .generated_files/flags/default/f32ba854e1527ad53a300616462033966a792ddf .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/01-SRC/01-MAIN" 
	@${RM} ${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o.d 
	@${RM} ${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  01-SRC/01-MAIN/bootloader.c  -o ${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/01-SRC/01-MAIN/bootloader.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/01-SRC/01-MAIN/main.o: 01-SRC/01-MAIN/main.c  .generated_files/flags/default/3a95e1aefe10f89cfd113f4bbddf02059c74a96 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/01-SRC/01-MAIN" 
	@${RM} ${OBJECTDIR}/01-SRC/01-MAIN/main.o.d 
	@${RM} ${OBJECTDIR}/01-SRC/01-MAIN/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  01-SRC/01-MAIN/main.c  -o ${OBJECTDIR}/01-SRC/01-MAIN/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/01-SRC/01-MAIN/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o: 01-SRC/02-FLAH_ROUTINES/flash_routines.c  .generated_files/flags/default/bbf952d17101bf73c1292af80fd1c1efd14dedcf .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES" 
	@${RM} ${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o.d 
	@${RM} ${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  01-SRC/02-FLAH_ROUTINES/flash_routines.c  -o ${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/01-SRC/02-FLAH_ROUTINES/flash_routines.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/01-SRC/03-TARGET/target.o: 01-SRC/03-TARGET/target.c  .generated_files/flags/default/b9de7abe117c44fcd85275f036f81fb0ffed50c7 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/01-SRC/03-TARGET" 
	@${RM} ${OBJECTDIR}/01-SRC/03-TARGET/target.o.d 
	@${RM} ${OBJECTDIR}/01-SRC/03-TARGET/target.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  01-SRC/03-TARGET/target.c  -o ${OBJECTDIR}/01-SRC/03-TARGET/target.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/01-SRC/03-TARGET/target.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/01-SRC/04-CRC/crc.o: 01-SRC/04-CRC/crc.c  .generated_files/flags/default/c0a5e03d49dc73b6ddfc5d540e8d7cea02442fc5 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/01-SRC/04-CRC" 
	@${RM} ${OBJECTDIR}/01-SRC/04-CRC/crc.o.d 
	@${RM} ${OBJECTDIR}/01-SRC/04-CRC/crc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  01-SRC/04-CRC/crc.c  -o ${OBJECTDIR}/01-SRC/04-CRC/crc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/01-SRC/04-CRC/crc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o: mcc_generated_files/memory/flash_demo.c  .generated_files/flags/default/d868776270b5d38b2c3729f9c3fba6344548e13e .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/memory" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/memory/flash_demo.c  -o ${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/memory/flash_demo.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/default/e6d4a7432f96f6b371630d35d59222d13f6f19f3 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/default/475ba2b9a7a1a69b83020b99f694dd8e60763aa2 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/default/9b1ad3682c43c1ecc2e99701c7e7bee8d515b725 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/default/c664dcee8a5f6cb8ce83c7711d01c63c24708408 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/default/cc95dfd2dbf392830b53ef9f7c8ddd0fbaaad745 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/default/36cfa774547957bad5b913e1734eab095861ffc2 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  .generated_files/flags/default/1f0044766a3cb6a3c91ffd4861648e9fb1650443 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  .generated_files/flags/default/f897ddcc797248a4466a99a7c42a18012d95cfd4 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/default/c798e4541726f14eed634fb751961cae6c1eed9b .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/default/2222c13dbb730a30988af0ce16e09fa13c53d892 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -mlarge-code -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/memory/flash.o: mcc_generated_files/memory/flash.s  .generated_files/flags/default/5f220b23caef23d64f5a489bb9e2a3babc407eac .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/memory" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory/flash.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory/flash.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  mcc_generated_files/memory/flash.s  -o ${OBJECTDIR}/mcc_generated_files/memory/flash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -Wa,-MD,"${OBJECTDIR}/mcc_generated_files/memory/flash.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/mcc_generated_files/memory/flash.o: mcc_generated_files/memory/flash.s  .generated_files/flags/default/e7816fcb661140459fc19103c4484eac9c6a42c3 .generated_files/flags/default/21e80fcadfd4ba79b877a88502d75c47a6aec139
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/memory" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory/flash.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory/flash.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  mcc_generated_files/memory/flash.s  -o ${OBJECTDIR}/mcc_generated_files/memory/flash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -Wa,-MD,"${OBJECTDIR}/mcc_generated_files/memory/flash.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Bootloader_UART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    01-SRC/00-LINKER/Linker_Boot.gld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Bootloader_UART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files"     -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-ivt,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/Bootloader_UART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   01-SRC/00-LINKER/Linker_Boot.gld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Bootloader_UART.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"01-SRC/00-LINKER" -I"01-SRC/01-MAIN" -I"01-SRC/02-FLAH_ROUTINES" -I"01-SRC/03-TARGET" -I"01-SRC/04-CRC" -I"mcc_generated_files" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-ivt,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/Bootloader_UART.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
	@echo Normalizing hex file
	@"C:/Program Files/Microchip/MPLABX/v6.00/mplab_platform/platform/../mplab_ide/modules/../../bin/hexmate" --edf="C:/Program Files/Microchip/MPLABX/v6.00/mplab_platform/platform/../mplab_ide/modules/../../dat/en_msgs.txt" ${DISTDIR}/Bootloader_UART.X.${IMAGE_TYPE}.hex -o${DISTDIR}/Bootloader_UART.X.${IMAGE_TYPE}.hex

endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
