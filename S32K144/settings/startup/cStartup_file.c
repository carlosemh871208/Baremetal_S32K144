/*********************************************************************************************************************/
/*                                                     BSW GROUP                                                     */
/*********************************************************************************************************************/
/*                                               OBJECT SPECIFICATION                                                */
/*********************************************************************************************************************/
/*!
 * $Source: cStartup_file.c $
 * $Revision: Version 1.0 $
 * $Author: Carlos Martinez $
 * $Date: 2021-04-28 $
 */
/*********************************************************************************************************************/
/* DESCRIPTION :                                                                                                     */
/** \ cStartup_file.c
    This file contains an interrupt vector for Cortex-M written in C.
    Developer must enable interrupts according application
 */
/*********************************************************************************************************************/
/* ALL RIGHTS RESERVED                                                                                               */
/*                                                                                                                   */
/* The reproduction, transmission, or use of this document or its content is                                         */
/* not permitted without express written authority. Offenders will be liable                                         */
/* for damages.                                                                                                      */
/*********************************************************************************************************************/

/*                                                       Includes                                                    */
/*********************************************************************************************************************/
#include "Std_Types.h"

/*                                                        Macros                                                     */
/*********************************************************************************************************************/
#if !defined (CORE_CM4_H)
#define CORE_CM4_H


#ifdef __cplusplus
extern "C" {
#endif

/** \brief  INT_VECTOR_Reg
 *
 *   Register in which the start of vector table needs to be configured
 */
#define INT_VECTOR_Reg S32_SCB->VTOR

/** \brief  BKPT_ASM
 *
 *   Macro to be used to trigger an debug interrupt
 */
#define BKPT_ASM __asm("BKPT #0\n\t")


/** \brief  Enable FPU
 *
 *   ENABLE_FPU indicates whether SystemInit will enable the Floating point unit (FPU)
 */
#if defined (__GNUC__)
#if defined (__VFP_FP__) && !defined (__SOFTFP__)
#define ENABLE_FPU
#endif

#elif defined (__ICCARM__)
#if defined __ARMVFP__
#define ENABLE_FPU
#endif

#elif defined (__ghs__) || defined (__DCC__)
#if defined (__VFP__)
#define ENABLE_FPU
#endif

#elif defined (__CSMC__)
#if (__CSMC__ & 0x400U) /* bit 10: set if hardware FPU selected (+fpu) */
#define ENABLE_FPU
#endif
#endif /* if defined (__GNUC__) */


/** \brief  Enable interrupts
 */
#if defined (__GNUC__)
#define ENABLE_INTERRUPTS() __asm volatile ("cpsie i" : : : "memory");
#else
#define ENABLE_INTERRUPTS() __asm("cpsie i")
#endif


/** \brief  Disable interrupts
 */
#if defined (__GNUC__)
#define DISABLE_INTERRUPTS() __asm volatile ("cpsid i" : : : "memory");
#else
#define DISABLE_INTERRUPTS() __asm("cpsid i")
#endif


/** \brief  Enter low-power standby state
 *    WFI (Wait For Interrupt) makes the processor suspend execution (Clock is stopped) until an IRQ interrupts.
 */
#if defined (__GNUC__)
#define STANDBY() __asm volatile ("wfi")
#else
#define STANDBY() __asm("wfi")
#endif


/** \brief  Reverse byte order in a word.
 */
#if defined (__GNUC__) || defined (__ICCARM__) || defined (__ghs__)
#define REV_BYTES_32(a, b) __asm volatile ("rev %0, %1" : "=r" (b) : "r" (a))
#else
#define REV_BYTES_32(a, b) (b = ((a & 0xFF000000U) >> 24U) | ((a & 0xFF0000U) >> 8U) \
                                | ((a & 0xFF00U) << 8U) | ((a & 0xFFU) << 24U))
#endif

/** \brief  Reverse byte order in each halfword independently.
 */
#if defined (__GNUC__) || defined (__ICCARM__) || defined (__ghs__)
#define REV_BYTES_16(a, b) __asm volatile ("rev16 %0, %1" : "=r" (b) : "r" (a))
#else
#define REV_BYTES_16(a, b) (b = ((a & 0xFF000000U) >> 8U) | ((a & 0xFF0000U) << 8U) \
                                | ((a & 0xFF00U) >> 8U) | ((a & 0xFFU) << 8U))
#endif

/** \brief  Places a function in RAM.
 */
#if defined ( __GNUC__ )
    #define START_FUNCTION_DECLARATION_RAMSECTION
    #define END_FUNCTION_DECLARATION_RAMSECTION        __attribute__((section (".code_ram")));
#elif defined ( __ghs__ )
    #define START_FUNCTION_DECLARATION_RAMSECTION      _Pragma("ghs callmode=far")
    #define END_FUNCTION_DECLARATION_RAMSECTION        __attribute__((section (".code_ram")));\
                                                       _Pragma("ghs callmode=default")
#elif defined ( __ICCARM__ )
    #define START_FUNCTION_DECLARATION_RAMSECTION      __ramfunc
    #define END_FUNCTION_DECLARATION_RAMSECTION        ;
#elif defined ( __DCC__ )
    #define START_FUNCTION_DECLARATION_RAMSECTION      _Pragma("section CODE \".code_ram\"") \
                                                       _Pragma("use_section CODE")
    #define END_FUNCTION_DECLARATION_RAMSECTION        ; \
                                                       _Pragma("section CODE \".text\"")
#elif defined ( __CSMC__ )
    #define START_FUNCTION_DECLARATION_RAMSECTION      @ext
    #define END_FUNCTION_DECLARATION_RAMSECTION        ;
#else
    /* Keep compatibility with software analysis tools */
    #define START_FUNCTION_DECLARATION_RAMSECTION
    #define END_FUNCTION_DECLARATION_RAMSECTION        ;
#endif

#if defined ( __CSMC__ )
    #define START_FUNCTION_DEFINITION_RAMSECTION       _Pragma("section (code_ram)")
    #define END_FUNCTION_DEFINITION_RAMSECTION         _Pragma("section ()")
#else
    /* For GCC, IAR, GHS and Diab there is no need to specify the section when
    defining a function, it is enough to specify it at the declaration. This
    also enables compatibility with software analysis tools. */
    #define START_FUNCTION_DEFINITION_RAMSECTION
    #define END_FUNCTION_DEFINITION_RAMSECTION
#endif

#if defined (__ICCARM__)
    #define DISABLE_CHECK_RAMSECTION_FUNCTION_CALL     _Pragma("diag_suppress=Ta022")
    #define ENABLE_CHECK_RAMSECTION_FUNCTION_CALL      _Pragma("diag_default=Ta022")
#else
    #define DISABLE_CHECK_RAMSECTION_FUNCTION_CALL
    #define ENABLE_CHECK_RAMSECTION_FUNCTION_CALL
#endif


/** \brief  Data alignment.
 */
#if defined ( __GNUC__ ) || defined ( __ghs__ ) || defined ( __DCC__ )
    #define ALIGNED(x)      __attribute__((aligned(x)))
#elif defined ( __ICCARM__ )
    #define stringify(s) tostring(s)
    #define tostring(s) #s
    #define ALIGNED(x)      _Pragma(stringify(data_alignment=x))
#else
    /* Keep compatibility with software analysis tools */
    #define ALIGNED(x)
#endif

/** \brief  Endianness.
 */
#define CORE_LITTLE_ENDIAN

#ifdef __cplusplus
}
#endif

#endif /* CORE_CM4_H */


#if (defined(__ICCARM__))
    #pragma section = ".data"
    #pragma section = ".data_init"
    #pragma section = ".bss"
#endif

/*                                           External functions prototypes                                           */
/*********************************************************************************************************************/
extern void Reset_Handler      ( void );
extern void NMI_Handler        ( void );
extern void HardFault_Handler  ( void );
extern void MemManage_Handler  ( void );
extern void BusFault_Handler   ( void );
extern void UsageFault_Handler ( void );
extern void SVC_Handler        ( void );
extern void DebugMon_Handler   ( void );
extern void PendSV_Handler     ( void );
extern void SysTick_Handler    ( void );

/************ s32K144 Specific Interrupts Functions *****************/
extern void PORTA_IRQHandler   ( void );
extern void PORTB_IRQHandler   ( void );
extern void PORTC_IRQHandler   ( void );
extern void PORTD_IRQHandler   ( void );
extern void PORTE_IRQHandler   ( void );

/*                                           Internal functions prototypes                                           */
/*********************************************************************************************************************/
static void init_data_bss(void);
static void SystemInit(void);
/*                                                Constants and types                                                */
/*********************************************************************************************************************/
typedef struct
{
	void* __ptr;
}intVectElement;

typedef struct
{
	u32bit backDoorCmpKey1;
	u32bit backDoorCmpKey2;
	u32bit flashProtect;
	u32bit flashSecurity; /*FDPROT:FEPROT:FOPT:FSEC, check FTFC memory map*/

}flashConfiguration;

/*Exceptions table*/
__attribute__ ((section(".__isr_vector")))
const intVecElement __isr_vector[] =
{
    __StackTop,                                      /* Top Stack at 0x00000000 address*/
	Reset_Handler,                                   /* Reset_Handler,*/
	NMI_Handler,                                     /* NMI Handler,*/
	HardFault_Handler,                               /* Hard Fault Handler,*/
	MemManage_Handler,                               /* MPU Fault Handler,*/
	BusFault_Handler,                                /* Bus Fault Handler,*/
	UsageFault_Handler,                              /* Usage Fault Handler,*/
	0,                                               /* Reserved*/
	0,                                               /* Reserved*/
	0,                                               /* Reserved*/
	0,                                               /* Reserved*/
	SVC_Handler,                                     /* SVCall Handler,*/
	DebugMon_Handler,                                /* Debug Monitor Handler,*/
	0,                                               /* Reserved*/
	PendSV_Handler,                                  /* PendSV Handler,*/
	SysTick_Handler,                                 /* SysTick Handler,*/
	/********* s32K144 Specific Interrupt Handlers (BEGIN) ***********/
	0,                                               /* DMA channel 0 transfer complete*/
	0,                                               /* DMA channel 1 transfer complete*/
	0,                                               /* DMA channel 2 transfer complete*/
	0,                                               /* DMA channel 3 transfer complete*/
	0,                                               /* DMA channel 4 transfer complete*/
	0,                                               /* DMA channel 5 transfer complete*/
	0,                                               /* DMA channel 6 transfer complete*/
	0,                                               /* DMA channel 7 transfer complete*/
	0,                                               /* DMA channel 8 transfer complete*/
	0,                                               /* DMA channel 9 transfer complete*/
	0,                                               /* DMA channel 10 transfer complete*/
	0,                                               /* DMA channel 11 transfer complete*/
	0,                                               /* DMA channel 12 transfer complete*/
	0,                                               /* DMA channel 13 transfer complete*/
	0,                                               /* DMA channel 14 transfer complete*/
	0,                                               /* DMA channel 15 transfer complete*/
	0,                                               /* DMA error interrupt channels 0-15*/
	0,                                               /* FPU sources*/
	0,                                               /* FTFC Command complete*/
	0,                                               /* FTFC Read collision*/
	0,                                               /* PMC Low voltage detect interrupt*/
	0,                                               /* FTFC Double bit fault detect*/
	0,                                               /* Single interrupt vector for WDOG and EWM*/
	0,                                               /* RCM Asynchronous Interrupt*/
	0,                                               /* LPI2C0 Master Interrupt*/
	0,                                               /* LPI2C0 Slave Interrupt*/
	0,                                               /* LPSPI0 Interrupt*/
	0,                                               /* LPSPI1 Interrupt*/
	0,                                               /* LPSPI2 Interrupt*/
	0,                                               /* Reserved Interrupt 45*/
	0,                                               /* Reserved Interrupt 46*/
	0,                                               /* LPUART0 Transmit / Receive Interrupt*/
	0,                                               /* Reserved Interrupt 48*/
	0,                                               /* LPUART1 Transmit / Receive  Interrupt*/
	0,                                               /* Reserved Interrupt 50*/
	0,                                               /* LPUART2 Transmit / Receive  Interrupt*/
	0,                                               /* Reserved Interrupt 52*/
	0,                                               /* Reserved Interrupt 53*/
	0,                                               /* Reserved Interrupt 54*/
	0,                                               /* ADC0 interrupt request.*/
	0,                                               /* ADC1 interrupt request.*/
	0,                                               /* CMP0 interrupt request*/
	0,                                               /* Reserved Interrupt 58*/
	0,                                               /* Reserved Interrupt 59*/
	0,                                               /* ERM single bit error correction*/
	0,                                               /* ERM double bit error non-correctable*/
	0,                                               /* RTC alarm interrupt*/
	0,                                               /* RTC seconds interrupt*/
	0,                                               /* LPIT0 channel 0 overflow interrupt*/
	0,                                               /* LPIT0 channel 1 overflow interrupt*/
	0,                                               /* LPIT0 channel 2 overflow interrupt*/
	0,                                               /* LPIT0 channel 3 overflow interrupt*/
	0,                                               /* PDB0 interrupt*/
	0,                                               /* Reserved Interrupt 69*/
	0,                                               /* Reserved Interrupt 70*/
	0,                                               /* Reserved Interrupt 71*/
	0,                                               /* Reserved Interrupt 72*/
	0,                                               /* SCG bus interrupt request*/
	0,                                               /* LPTIMER interrupt request*/
	PORTA_IRQHandler,                                /* Port A pin detect interrupt*/
	PORTB_IRQHandler,                                /* Port B pin detect interrupt*/
	PORTC_IRQHandler,                                /* Port C pin detect interrupt*/
	PORTD_IRQHandler,                                /* Port D pin detect interrupt*/
	PORTE_IRQHandler,                                /* Port E pin detect interrupt*/
	0,                                               /* Software interrupt*/
	0,                                               /* Reserved Interrupt 81*/
	0,                                               /* Reserved Interrupt 82*/
	0,                                               /* Reserved Interrupt 83*/
	0,                                               /* PDB1 interrupt*/
	0,                                               /* FlexIO Interrupt*/
	0,                                               /* Reserved Interrupt 86*/
	0,                                               /* Reserved Interrupt 87*/
	0,                                               /* Reserved Interrupt 88*/
	0,                                               /* Reserved Interrupt 89*/
	0,                                               /* Reserved Interrupt 90*/
	0,                                               /* Reserved Interrupt 91*/
	0,                                               /* Reserved Interrupt 92*/
	0,                                               /* Reserved Interrupt 93*/
	0,                                               /* CAN0 OR'ed [Bus Off OR Transmit Warning OR Receive Warning]*/
	0,                                               /* CAN0 Interrupt indicating that errors were detected on the CAN bus*/
	0,                                               /* CAN0 Interrupt asserted when Pretended Networking operation is enabled, and a valid message matches the selected filter criteria during Low Power mode*/
	0,                                               /* CAN0 OR'ed Message buffer (0-15)*/
	0,                                               /* CAN0 OR'ed Message buffer (16-31)*/
	0,                                               /* Reserved Interrupt 99*/
	0,                                               /* Reserved Interrupt 100*/
	0,                                               /* CAN1 OR'ed [Bus Off OR Transmit Warning OR Receive Warning]*/
	0,                                               /* CAN1 Interrupt indicating that errors were detected on the CAN bus*/
	0,                                               /* Reserved Interrupt 103*/
	0,                                               /* CAN1 OR'ed Interrupt for Message buffer (0-15)*/
	0,                                               /* Reserved Interrupt 105*/
	0,                                               /* Reserved Interrupt 106*/
	0,                                               /* Reserved Interrupt 107*/
	0,                                               /* CAN2 OR'ed [Bus Off OR Transmit Warning OR Receive Warning]*/
	0,                                               /* CAN2 Interrupt indicating that errors were detected on the CAN bus*/
	0,                                               /* Reserved Interrupt 110*/
	0,                                               /* CAN2 OR'ed Message buffer (0-15)*/
	0,                                               /* Reserved Interrupt 112*/
	0,                                               /* Reserved Interrupt 113*/
	0,                                               /* Reserved Interrupt 114*/
	0,                                               /* FTM0 Channel 0 and 1 interrupt*/
	0,                                               /* FTM0 Channel 2 and 3 interrupt*/
	0,                                               /* FTM0 Channel 4 and 5 interrupt*/
	0,                                               /* FTM0 Channel 6 and 7 interrupt*/
	0,                                               /* FTM0 Fault interrupt*/
	0,                                               /* FTM0 Counter overflow and Reload interrupt*/
	0,                                               /* FTM1 Channel 0 and 1 interrupt*/
	0,                                               /* FTM1 Channel 2 and 3 interrupt*/
	0,                                               /* FTM1 Channel 4 and 5 interrupt*/
	0,                                               /* FTM1 Channel 6 and 7 interrupt*/
	0,                                               /* FTM1 Fault interrupt*/
	0,                                               /* FTM1 Counter overflow and Reload interrupt*/
	0,                                               /* FTM2 Channel 0 and 1 interrupt*/
	0,                                               /* FTM2 Channel 2 and 3 interrupt*/
	0,                                               /* FTM2 Channel 4 and 5 interrupt*/
	0,                                               /* FTM2 Channel 6 and 7 interrupt*/
	0,                                               /* FTM2 Fault interrupt*/
	0,                                               /* FTM2 Counter overflow and Reload interrupt*/
	0,                                               /* FTM3 Channel 0 and 1 interrupt*/
	0,                                               /* FTM3 Channel 2 and 3 interrupt*/
	0,                                               /* FTM3 Channel 4 and 5 interrupt*/
	0,                                               /* FTM3 Channel 6 and 7 interrupt*/
	0,                                               /* FTM3 Fault interrupt*/
	0,                                               /* FTM3 Counter overflow and Reload interrupt*/
	0,                                      /* 139*/
	0,                                      /* 140*/
	0,                                      /* 141*/
	0,                                      /* 142*/
	0,                                      /* 143*/
	0,                                      /* 144*/
	0,                                      /* 145*/
	0,                                      /* 146*/
	0,                                      /* 147*/
	0,                                      /* 148*/
	0,                                      /* 149*/
	0,                                      /* 150*/
	0,                                      /* 151*/
	0,                                      /* 152*/
	0,                                      /* 153*/
	0,                                      /* 154*/
	0,                                      /* 155*/
	0,                                      /* 156*/
	0,                                      /* 157*/
	0,                                      /* 158*/
	0,                                      /* 159*/
	0,                                      /* 160*/
	0,                                      /* 161*/
	0,                                      /* 162*/
	0,                                      /* 163*/
	0,                                      /* 164*/
	0,                                      /* 165*/
	0,                                      /* 166*/
	0,                                      /* 167*/
	0,                                      /* 168*/
	0,                                      /* 169*/
	0,                                      /* 170*/
	0,                                      /* 171*/
	0,                                      /* 172*/
	0,                                      /* 173*/
	0,                                      /* 174*/
	0,                                      /* 175*/
	0,                                      /* 176*/
	0,                                      /* 177*/
	0,                                      /* 178*/
	0,                                      /* 179*/
	0,                                      /* 180*/
	0,                                      /* 181*/
	0,                                      /* 182*/
	0,                                      /* 183*/
	0,                                      /* 184*/
	0,                                      /* 185*/
	0,                                      /* 186*/
	0,                                      /* 187*/
	0,                                      /* 188*/
	0,                                      /* 189*/
	0,                                      /* 190*/
	0,                                      /* 191*/
	0,                                      /* 192*/
	0,                                      /* 193*/
	0,                                      /* 194*/
	0,                                      /* 195*/
	0,                                      /* 196*/
	0,                                      /* 197*/
	0,                                      /* 198*/
	0,                                      /* 199*/
	0,                                      /* 200*/
	0,                                      /* 201*/
	0,                                      /* 202*/
	0,                                      /* 203*/
	0,                                      /* 204*/
	0,                                      /* 205*/
	0,                                      /* 206*/
	0,                                      /* 207*/
	0,                                      /* 208*/
	0,                                      /* 209*/
	0,                                      /* 210*/
	0,                                      /* 211*/
	0,                                      /* 212*/
	0,                                      /* 213*/
	0,                                      /* 214*/
	0,                                      /* 215*/
	0,                                      /* 216*/
	0,                                      /* 217*/
	0,                                      /* 218*/
	0,                                      /* 219*/
	0,                                      /* 220*/
	0,                                      /* 221*/
	0,                                      /* 222*/
	0,                                      /* 223*/
	0,                                      /* 224*/
	0,                                      /* 225*/
	0,                                      /* 226*/
	0,                                      /* 227*/
	0,                                      /* 228*/
	0,                                      /* 229*/
	0,                                      /* 230*/
	0,                                      /* 231*/
	0,                                      /* 232*/
	0,                                      /* 233*/
	0,                                      /* 234*/
	0,                                      /* 235*/
	0,                                      /* 236*/
	0,                                      /* 237*/
	0,                                      /* 238*/
	0,                                      /* 239*/
	0,                                      /* 240*/
	0,                                      /* 241*/
	0,                                      /* 242*/
	0,                                      /* 243*/
	0,                                      /* 244*/
	0,                                      /* 245*/
	0,                                      /* 246*/
	0,                                      /* 247*/
	0,                                      /* 248*/
	0,                                      /* 249*/
	0,                                      /* 250*/
	0,                                      /* 251*/
	0,                                      /* 252*/
	0,                                      /* 253*/
	0                                       /* 254*/
};

/*Flash configuration code*/
/*See reference Manual: 34.4.1.7 Program Flash Protection Registers (FPROT0 - FPROT3)*/
__attribute__ ((section(".FlashConfig")))
const flashConfiguration flashConfsetup ={ /*See reference manual 3.3 Flash memory map*/
    0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFF7FFE /*FDPROT:FEPROT:FOPT:FSEC(0xFE = unsecured)*/
};
/*                                                      Variables                                                    */
/*********************************************************************************************************************/

/*                                           External functions implementation                                       */
/*********************************************************************************************************************/

/*********** CORTEX M4 Exceptions *************/
void Reset_Handler      ( void ) /*Reset exception calls main function*/
{
	SystemInit(void);
	init_data_bss(void);
	main(void);
	while(1){}
}

void NMI_Handler        ( void ) { while (1) {} }
void HardFault_Handler  ( void ) { while (1) {} }
void MemManage_Handler  ( void ) { while (1) {} }
void BusFault_Handler   ( void ) { while (1) {} }
void UsageFault_Handler ( void ) { while (1) {} }
void SVC_Handler        ( void ) { while (1) {} }
void DebugMon_Handler   ( void ) { while (1) {} }
void PendSV_Handler     ( void ) { while (1) {} }
void SysTick_Handler    ( void ) { while (1) {} }

/************ s32K144 Specific Interrupts *******************/
void PORTA_IRQHandler   ( void ) { while (1) {} }
void PORTB_IRQHandler   ( void ) { while (1) {} }
void PORTC_IRQHandler   ( void ) { while (1) {} }
void PORTD_IRQHandler   ( void ) { while (1) {} }
void PORTE_IRQHandler   ( void ) { while (1) {} }

/*                                           Internal functions implementation                                       */
/*********************************************************************************************************************/

/*
 * Copy initialized data from ROM to RAM.
 * Clear the zero-initialized data section.
 * Copy the vector table from ROM to RAM.
 */
void init_data_bss(void)
{
	u32bit n;
	/* Declare pointers for various data sections. These pointers
	 * are initialized using values pulled in from the linker file */
	u8bit * data_ram;
	u8bit * code_ram;
	u8bit * bss_start;
	const u8bit * data_rom, * data_rom_end;
	const u8bit * code_rom, * code_rom_end;
	const u8bit * bss_end;

	/* Addresses for VECTOR_TABLE and VECTOR_RAM come from the linker file */
	extern u32bit __RAM_VECTOR_TABLE_SIZE[];
	extern u32bit __VECTOR_TABLE[];
	extern u32bit __VECTOR_RAM[];

	 /* Get section information from linker files */
#if defined(__ICCARM__)
    /* Data */
    data_ram        = __section_begin(".data");
    data_rom        = __section_begin(".data_init");
    data_rom_end    = __section_end(".data_init");

    /* CODE RAM */
	#pragma section = "__CODE_ROM"
	#pragma section = "__CODE_RAM"
	code_ram        = __section_begin("__CODE_RAM");
	code_rom        = __section_begin("__CODE_ROM");
	code_rom_end    = __section_end("__CODE_ROM");

	/* BSS */
	bss_start       = __section_begin(".bss");
	bss_end         = __section_end(".bss");
#else
	extern u32bit __DATA_ROM[];
	extern u32bit __DATA_RAM[];
	extern u32bit __DATA_END[];

    extern u32bit __CODE_RAM[];
    extern u32bit __CODE_ROM[];
    extern u32bit __CODE_END[];

	extern u32bit __BSS_START[];
    extern u32bit __BSS_END[];

    /* Data */
    data_ram        = (ubit8 *)__DATA_RAM;
    data_rom        = (ubit8 *)__DATA_ROM;
    data_rom_end    = (ubit8 *)__DATA_END;
    /* CODE RAM */
    code_ram        = (ubit8 *)__CODE_RAM;
    code_rom        = (ubit8 *)__CODE_ROM;
    code_rom_end    = (ubit8 *)__CODE_END;
    /* BSS */
    bss_start       = (ubit8 *)__BSS_START;
    bss_end         = (ubit8 *)__BSS_END;
#endif

    /* Check if VECTOR_TABLE copy is needed */
    if (__VECTOR_RAM != __VECTOR_TABLE)
    {
        /* Copy the vector table from ROM to RAM */
        for (n = 0; n < (((u32bit)__RAM_VECTOR_TABLE_SIZE)/sizeof(u32bit)); n++)
        {
            __VECTOR_RAM[n] = __VECTOR_TABLE[n];
        }
        /* Point the VTOR to the position of vector table */
            INT_VECTOR_Reg = (u32bit)__VECTOR_RAM;
    }
    else
    {
        /* Point the VTOR to the position of vector table */
        INT_VECTOR_Reg = (u32bit)__VECTOR_TABLE;
    }

    /* Copy initialized data from ROM to RAM */
    while (data_rom_end != data_rom)
    {
        *data_ram = *data_rom;
        data_ram++;
        data_rom++;
    }

    /* Copy functions from ROM to RAM */
    while (code_rom_end != code_rom)
    {
        *code_ram = *code_rom;
        code_ram++;
        code_rom++;
    }

    /* Clear the zero-initialized data section */
    while(bss_end != bss_start)
    {
        *bss_start = 0;
        bss_start++;
    }
}

void SystemInit (void)
{
/**************************************************************************/
	                      /* FPU ENABLE*/
/**************************************************************************/
#ifdef ENABLE_FPU
/* Enable CP10 and CP11 coprocessors */
    S32_SCB->CPACR |= (S32_SCB_CPACR_CP10_MASK | S32_SCB_CPACR_CP11_MASK);
#ifdef  ERRATA_E6940
/* Disable lazy context save of floating point state by clearing LSPEN bit
 * Workaround for errata e6940 */
    S32_SCB->FPCCR &= ~(S32_SCB_FPCCR_LSPEN_MASK);
#endif
#endif /* ENABLE_FPU */

/**************************************************************************/
                      /* WDOG DISABLE*/
/**************************************************************************/

#if (DISABLE_WDOG)
/* Write of the WDOG unlock key to CNT register, must be done in order to allow any modifications*/
    WDOG->CNT = (u32bit) FEATURE_WDOG_UNLOCK_VALUE;
/* The dummy read is used in order to make sure that the WDOG registers will be configured only
 * after the write of the unlock value was completed. */
    (void)WDOG->CNT;

/* Initial write of WDOG configuration register:
 * enables support for 32-bit refresh/unlock command write words,
 * clock select from LPO, update enable, watchdog disabled */
    WDOG->CS  = (u32bit) ((1UL << WDOG_CS_CMD32EN_SHIFT)                      |
	                     (FEATURE_WDOG_CLK_FROM_LPO << WDOG_CS_CLK_SHIFT)     |
	                     (0U << WDOG_CS_EN_SHIFT)                             |
	                     (1U << WDOG_CS_UPDATE_SHIFT)                         );

/* Configure timeout */
    WDOG->TOVAL = (u32bit) 0xFFFF;
#endif /* (DISABLE_WDOG) */

/**************************************************************************/
                      /* Power mode protection */
/**************************************************************************/
#ifdef SYSTEM_SMC_PMPROT_VALUE
/* Power mode protection initialization */
    SMC->PMPROT = SYSTEM_SMC_PMPROT_VALUE;
#endif
}

/*                                                 Inline functions                                                  */
/*********************************************************************************************************************/

/*                                                Private functions                                                  */
/*********************************************************************************************************************/


/***************************************************Log Projects********************************************************
 *|    ID   | JIRA Ticket |     Date    |                                       Description                            |
 *| CMARTI  |     NA      | 28-Apr-2021 | First version of cStartup_file.c                                             |
***********************************************************************************************************************/
