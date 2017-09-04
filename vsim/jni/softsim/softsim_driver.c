/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of TGT Limited. (C) 2016
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("TGT SOFTWARE")
*  RECEIVED FROM TGT AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. TGT EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES TGT PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE TGT SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. TGT SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY TGT SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND TGT'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE TGT SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT TGT'S OPTION, TO REVISE OR REPLACE THE TGT SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  TGT FOR SUCH TGT SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename: softsim_driver.c
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-10-28
 ******************************************************************************/

/*****************************************************************************
* Include                                                                     
*****************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h> 
#include "softsim_include.h" 
/*****************************************************************************
* Define                                                                      
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_DRIVER
#define SOFTSIM_TRACE_HEADER			"[DRIVER]" 
/*****************************************************************************
* Typedef                                                                     
*****************************************************************************/

/*****************************************************************************
* Static Local Variable                                                              
*****************************************************************************/
static s32 speed_arr[] = {B200,B300,B600,B1200,B1800,B2400,B4800,B9600,B19200,B38400,B57600,B115200,/*B200000,*/B230400};
static s32 name_arr[] = {200,300,600,1200,1800,2400,4800,9600,19200,38400,57600,115200,/*200000,*/230400};
/*****************************************************************************
* Global Variable                                                       
*****************************************************************************/

/*****************************************************************************
* Extern Global Variable                                                           
*****************************************************************************/

/*****************************************************************************
* Static Local Function                                                              
*****************************************************************************/

/*****************************************************************************
* Global Function                                                      
*****************************************************************************/

/*****************************************************************************
* Extern Global Function                                                     
*****************************************************************************/

/****************************************************************************
 * Function
 ****************************************************************************/
s32 softsim_baudrate_set(s32 fd, s32 baudrate)
{
 	struct termios TermiosTemp;
 	TermiosTemp.c_cflag = 0;
 	TermiosTemp.c_iflag = 0;
 	TermiosTemp.c_oflag = 0;
 	TermiosTemp.c_lflag = 0;
		 
 	TermiosTemp.c_cflag |= PARENB; // Enable parity generation.
 	//TermiosTemp.c_cflag &= ~PARENB; // Disable parity generation.
 	TermiosTemp.c_cflag &= ~PARODD; // Enable even parity type.
				 
 	TermiosTemp.c_cflag |= CLOCAL;
 	TermiosTemp.c_cflag |= CREAD;
 	TermiosTemp.c_cflag &= ~HUPCL;						 
 	TermiosTemp.c_lflag &= ~ICANON; // disable canonical mode, read what you can
 	TermiosTemp.c_cc[VMIN] = 1; 
 	TermiosTemp.c_cc[VTIME] = 0; // non-canonical input settings: wait to receive at least 1 character and read it immediately
										 
 	TermiosTemp.c_cflag &= ~CSIZE;
 	TermiosTemp.c_cflag |= CS8; // Use 8 bits per character.
 	TermiosTemp.c_cflag |= CSTOPB; // Use 2 stop bits.
 	TermiosTemp.c_cflag &= ~CRTSCTS; // Disable hardware flow control.
														 
 	TermiosTemp.c_iflag |= IGNBRK;
 	TermiosTemp.c_iflag &= ~BRKINT;
 	TermiosTemp.c_iflag |= IGNPAR;  
 	TermiosTemp.c_iflag &= ~PARMRK;
 	TermiosTemp.c_iflag |= INPCK;
 	TermiosTemp.c_iflag &= ~IXON; // Disable software flow control.
 	TermiosTemp.c_iflag &= ~IXOFF;
 	TermiosTemp.c_iflag &= ~IGNCR; // don't ignore carriage return \r on input
 	TermiosTemp.c_iflag &= ~INLCR; // don't translate new line \n to carriage return \r on input
 	TermiosTemp.c_iflag &= ~ICRNL; // don't translate \r to \n on input
																								 
 	TermiosTemp.c_oflag &= ~OCRNL; // don't map \r to \n on output
 	TermiosTemp.c_oflag &= ~ONLCR; // don't map \n to \r\n character pair on outputm

 	struct serial_struct ss={0};

 	if((ioctl(fd,TIOCGSERIAL,&ss)) < 0)
 	{
	 	perror("TIOCGSERIAL");
	 	return -1;
 	}

 	ss.flags |= ASYNC_SPD_CUST;  
 	ss.custom_divisor = 1<<31|baudrate;
 
 	if((ioctl(fd, TIOCSSERIAL, &ss)) < 0)
 	{
	 	perror("TIOCSSERIAL");
	 	return -1;
 	}

 	if(tcsetattr (fd, TCSANOW, &TermiosTemp) != 0)
 	{																			  
	 	perror("set tty attr");										   
	 	return -1; 																	   
 	}
	
 	return 0;
}

s32 softsim_baudrate(s32 fd, s32 baudrate)
{
 	struct termios TermiosTemp;
 	TermiosTemp.c_cflag = 0;
 	TermiosTemp.c_iflag = 0;
 	TermiosTemp.c_oflag = 0;
 	TermiosTemp.c_lflag = 0;
		 
 	TermiosTemp.c_cflag |= PARENB; // Enable parity generation.
 	//TermiosTemp.c_cflag &= ~PARENB; // Disable parity generation.
 	TermiosTemp.c_cflag &= ~PARODD; // Enable even parity type.
				 
 	TermiosTemp.c_cflag |= CLOCAL;
 	TermiosTemp.c_cflag |= CREAD;
 	TermiosTemp.c_cflag &= ~HUPCL;
							 
 	TermiosTemp.c_lflag &= ~ICANON; // disable canonical mode, read what you can
 	TermiosTemp.c_cc[VMIN] = 1; 
 	TermiosTemp.c_cc[VTIME] = 0; // non-canonical input settings: wait to receive at least 1 character and read it immediately
										 
 	TermiosTemp.c_cflag &= ~CSIZE;
 	TermiosTemp.c_cflag |= CS8; // Use 8 bits per character.
 	TermiosTemp.c_cflag |= CSTOPB; // Use 2 stop bits.
 	TermiosTemp.c_cflag &= ~CRTSCTS; // Disable hardware flow control.
														 
 	TermiosTemp.c_iflag |= IGNBRK;
 	TermiosTemp.c_iflag &= ~BRKINT;
 	TermiosTemp.c_iflag |= IGNPAR;  
 	TermiosTemp.c_iflag &= ~PARMRK;
 	TermiosTemp.c_iflag |= INPCK;
 	TermiosTemp.c_iflag &= ~IXON; // Disable software flow control.
 	TermiosTemp.c_iflag &= ~IXOFF;
 	TermiosTemp.c_iflag &= ~IGNCR; // don't ignore carriage return \r on input
 	TermiosTemp.c_iflag &= ~INLCR; // don't translate new line \n to carriage return \r on input
 	TermiosTemp.c_iflag &= ~ICRNL; // don't translate \r to \n on input
																								 
 	TermiosTemp.c_oflag &= ~OCRNL; // don't map \r to \n on output
 	TermiosTemp.c_oflag &= ~ONLCR; // don't map \n to \r\n character pair on outputm
 
 	tcflush(fd,TCIOFLUSH);
 	cfsetispeed(&TermiosTemp, B115200);
 	cfsetospeed(&TermiosTemp, B115200);
 	if(tcsetattr(fd,TCSANOW,&TermiosTemp)!=0)
 	{
 		perror("set tty attr");;
 		return -1;
 	}
 	tcflush(fd,TCIOFLUSH);
 
 	return 0;
}

s32 softsim_cat_gpio_value(s32 gpio_number)
{
	char path[64] = {0x00};
	char value_string[9] = {0};
	s32 fd = -1;

	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio_number);
	fd = open(path, O_RDONLY);

	if(fd < 0)
	{
		SOFTSIM_DEBUG("Open GPIO fail!");
		return -1;
	}

	if(read(fd, value_string, 5) < 0)
	{
		SOFTSIM_DEBUG("Read GPIO value fail!");
		return -1;
	}

	close(fd);

	return softsim_atoi(value_string);
}

s32 softsim_echo_value(char *path, s32 value)
{
	char *fd = NULL;
	char buffer[64] = {0};
	s32 len = 0;

	if(NULL == path)
	{
		SOFTSIM_DEBUG("Invalid path!");
		return -1;
	}
	
	fd = fopen(path, "wr");
	
	if(NULL == fd)
	{
		SOFTSIM_DEBUG("Open path fail!");
		return -1;
	}
	
	len = snprintf(buffer, sizeof(buffer), "%d", value);
	
	if(fwrite(buffer, sizeof(buffer), len, fd) < 0) 
	{
		SOFTSIM_DEBUG("Failed to write value!");
		fclose(fd);
		return -1;
	}
		
	fclose(fd);
	return 0;
}

s32 softsim_export(char *path, s32 value)
{
	char *file_p = NULL;
	char buffer[64] = {0};
	s32 len = 0;

	if(NULL == path)
	{
		SOFTSIM_DEBUG("Invalid path!");
		return -1;
	}
	
	file_p = fopen(path, "w");
	
	if(NULL == file_p)
	{
		SOFTSIM_DEBUG("Open path fail!");
		return -1;
	}
	
	len = snprintf(buffer, sizeof(buffer), "%d", value);
	
	if(fwrite(buffer, sizeof(buffer), len, file_p) < 0) 
	{
		SOFTSIM_DEBUG("Failed to export gpio!");
		fclose(file_p);
		return -1;
	}
		
	fclose(file_p);
	return 0;
}
