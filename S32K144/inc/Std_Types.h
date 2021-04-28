/*********************************************************************************************************************/
/*                                                     BSW GROUP                                                     */
/*********************************************************************************************************************/
/*                                               OBJECT SPECIFICATION                                                */
/*********************************************************************************************************************/
/*!
 * $Source: main.c $
 * $Revision: Version 1.0 $
 * $Author: Carlos Martinez $
 * $Date: 2021-04-28 $
 */
/*********************************************************************************************************************/
/* DESCRIPTION :                                                                                                     */
/** \Std_Types.h
    Here we define all data types required by the project.
 */
/*********************************************************************************************************************/
/* ALL RIGHTS RESERVED                                                                                               */
/*                                                                                                                   */
/* The reproduction, transmission, or use of this document or its content is                                         */
/* not permitted without express written authority. Offenders will be liable                                         */
/* for damages.                                                                                                      */
/*********************************************************************************************************************/
#ifndef INC_TYPES_H_
#define INC_TYPES_H_
/*                                                       Includes                                                    */
/*********************************************************************************************************************/

/*                                                Constants and types                                                */
/*********************************************************************************************************************/
/*Byte types definition*/
typedef unsigned char  u8bit;
typedef signed   char  s8bit;

/*Word types definition*/
typedef unsigned short u16bit;
typedef signed   short s16bit;

/*Long types definition*/
typedef unsigned long  u32bit;
typedef signed   long  s32bit;

/*Boolean type definition*/
typedef u8bit boolean;

#define FALSE (boolean)0u
#define TRUE  (boolean)1u

/*Null type definition*/
#ifndef NULL
    #define NULL (void*) 0u
#endif

/*Exit Program*/
#define EXIT_PROGRAM (boolean) 0u

/*                                                    Exported Variables                                              */
/**********************************************************************************************************************/

/*                                              Exported functions prototypes                                         */
/**********************************************************************************************************************/

#endif /* INC_TYPES_H_ */

/***************************************************Log Projects********************************************************
 *|    ID   | JIRA Ticket |     Date    |                                Description                                   |
 *| CMARTI  |     NA      | 28-Apr-2021 | First version of Std_Types.h                                                 |
***********************************************************************************************************************/
