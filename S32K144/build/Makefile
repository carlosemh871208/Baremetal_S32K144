########################################################################################################################
# $Source: Makefile$
# $Revision: Version 1.0 $
# $Author: Carlos Martinez $
# $Date: 2021-04-28 $
########################################################################################################################
# DESCRIPTION :                                                                                                     
# Makefile
#    Compilation script for entire project
########################################################################################################################

#Get project directory
#Modify this direction according your project direction.
TOPDIR = /home/carlos/Projects/S32K144

RM := rm -rf

#Compiler ARMv7
CC = arm-none-eabi-gcc

#ARM CORTEX M4 processor
MACH = cortex-m4

#Output folders .o & binary
OBJDIR = $(TOPDIR)/build/obj
BINDIR = $(TOPDIR)/build/bin

#Includes folders
INCDIR = $(TOPDIR)/inc

#Source Directory
SRCDIR  = $(TOPDIR)/src

#target place 
TARGET = $(BINDIR)/Project

#objects place
OBJS   = $(OBJDIR)/main.o 

#Compilation flags
CFLAGS = -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -I$(INCDIR) -c -MD

.PHONY : all
#Rule to create target
all: $(TARGET)
$(TARGET) : $(OBJS)
	mkdir -p $(BINDIR) #create bin directory
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

#Compilation stage
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(OBJDIR) #create obj directory
	$(CC) $(CFLAGS) $< -o $@

-include $(OBJDIR)/*.d

#clean project
.PHONY : clean
clean :
	$(RM) $(OBJDIR) $(BINDIR)
	
########################################################################################################################
 #|    ID   | JIRA Ticket |     Date    |                                       Description                            |
 #| CMARTI  |     NA      | 28-Apr-2021 | First version of Makefile                                                    |
########################################################################################################################