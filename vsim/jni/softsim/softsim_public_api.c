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
 * Filename: softsim_public_api.c
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-08
 ******************************************************************************/

/*****************************************************************************
* Include																		
*****************************************************************************/
#include "softsim_type.h"
#include "softsim_uicc_parameter.h"
#include "softsim_trace.h"
#include "softsim_macro.h"
#include "softsim_public_api.h"
#include <stdlib.h>
/*****************************************************************************
* Define																		
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_API
#define SOFTSIM_TRACE_HEADER			"[softsim publicapi]"
/*****************************************************************************
* Typedef																		
*****************************************************************************/

/*****************************************************************************
* Static Local Variable														   
*****************************************************************************/
	/* Conversion table for ASCII to SMS Default */
static const char ascii_to_gsmdefault_table[] =
{
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ',' ', ' ',
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
' ', '!',0x22, '#', '$', '%', '&',0x27, '(', ')', '*', '+', ',', '-', '.', '/',
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
0x00, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', 0x11,
' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}','~', ' ',
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
' ', 0x40, ' ', 0x01, 0x24, 0x03, ' ', 0x5F, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 0x0B, ' ', ' ', ' ', ' ', ' ', ' ', 0x60,
' ', ' ', ' ', ' ', 0x5B, 0x0E, 0x1C, 0x09, ' ', 0x1F, ' ', ' ', ' ', ' ', ' ', ' ',
' ', 0x5D, ' ', ' ', ' ', ' ', 0x5C, ' ', ' ', ' ', ' ', ' ', 0x5E, ' ', ' ', 0x1E,
0x7F, ' ', ' ', ' ', 0x7B, 0x0F, 0x1D, ' ', 0x04, 0x05, ' ', ' ', 0x07, ' ', ' ', ' ',
' ', 0x7D, 0x08, ' ', ' ', ' ', 0x7C, ' ', 0x0C, 0x06, ' ', ' ', 0x7E, ' ', ' ', ' ',
};
  
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

/*****************************************************************************
 * FUNCTION
 *  ucs2_strlen
 * DESCRIPTION
 *  The function is used for query UCS2 string length in character not in bytes.
  * PARAMETERS
 *  arrOut    [IN]  UCS2 string     
 * RETURNS
 *  Return UCS2 string length in character not in bytes
 *****************************************************************************/
s32 ucs2_strlen(const s8 *arrOut)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    s32 nCount = 0;
    s32 nLength = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Check for NULL character only at the odd no. of bytes 
       assuming forst byte start from zero */
    if (arrOut)
    {
        while (arrOut[nCount] != 0 || arrOut[nCount + 1] != 0)
        {
            ++nLength;
            nCount += 2;
        }
    }
    return nLength; /* One is added to count 0th byte */
}

/*****************************************************************************
 * FUNCTION
 *  ucs2_wcslen
 * DESCRIPTION
 *  Gets the number of characters of the given UCS2 encoded string(wide-character), 
 *  not including the final null wide-characters.
 * PARAMETERS          
 *  string    [IN]  UCS2 encoded string(wide-character) which length is to
 *                  be computed.      
 * RETURNS
 *  The number of characters of a wide-character string.
 *****************************************************************************/
s32 ucs2_wcslen(const u16*string)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    const u16 *str_tmp = string;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	
    while(*str_tmp)
    {
        ++str_tmp;
    }
    
    return (s32) (str_tmp - string);
}

/*****************************************************************************
 * FUNCTION
 *  ucs2_strcmp
 * DESCRIPTION
 *  The function is used for compare UCS2 strings. 
 * PARAMETERS
 *  string1     [IN]   UCS2 string1
 *  string2     [IN]   UCS2 string2
 * RETURNS
 *  <0 - string1 less than string2
 *   0 - string1 identical to string2
 *  >0 - string1 greater than string2
 *****************************************************************************/
s32 ucs2_strcmp(const char*string1, const char*string2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    u16 nStr1;
    u16 nStr2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while ((*string1 == *string2) && (*(string1 + 1) == *(string2 + 1)))
    {

        if ((*string1 == 0) && (*(string1 + 1) == 0))
        {
            return 0;
        }

        string1 += 2;
        string2 += 2;

    }   /* End of while */

    nStr1 = (string1[1] << 8) | (char)string1[0];
    nStr2 = (string2[1] << 8) | (char)string2[0];

    return (s32) (nStr1 - nStr2);

}

/*****************************************************************************
 * FUNCTION
 *  ucs2_strcpy
 * DESCRIPTION
 *  The function is used for copy UCS2 string. 
 * PARAMETERS
 *  strDestination  [OUT]   UCS2 destination string
 *  strSource       [IN]    UCS2 source string
 * RETURNS
 *  Return the destination string.
 *****************************************************************************/
char *ucs2_strcpy(char*strDestination, const char*strSource)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    u16 count = 1;
    char*temp = strDestination;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (strSource == NULL)
    {
        if (strDestination)
        {
            *(strDestination + count - 1) = '\0';
            *(strDestination + count) = '\0';
        }
        return temp;

    }

    if (strDestination == NULL || strSource == NULL)
    {
        return NULL;
    }
	
    while (!((*(strSource + count) == 0) && (*(strSource + count - 1) == 0)))
    {

        *(strDestination + count - 1) = *(strSource + count - 1);
        *(strDestination + count) = *(strSource + count);
        count += 2;
    }

    *(strDestination + count - 1) = '\0';
    *(strDestination + count) = '\0';

    return temp;
}


/*****************************************************************************
 * FUNCTION
 *  ucs2_strncmp
 * DESCRIPTION
 *  The function is used for compare UCS2 characters of two strings. Size is 
 *  the character number not the byte numbers.
 * PARAMETERS
 *  string1     [IN]  UCS2 string1
 *  string2     [IN]  UCS2 string2
 *  size        [IN]  Number in character (not byte)      
 * RETURNS
 *  <0 - string1 substring less than string2 substring
 *   0 - string1 substring identical to string2 substring
 *  >0 - string1 substring greater than string2 substring
 *****************************************************************************/
s32 ucs2_strncmp(const char *string1, const char *string2, u32 size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    u32 count = 0;
    u16 nStr1;
    u16 nStr2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    size = size << 1;   /* User is passing no of charcters not bytes */

    while (count < size)
    {
        nStr1 = (string1[1] << 8) | (u8)string1[0];
        nStr2 = (string2[1] << 8) | (u8)string2[0];
        if (nStr1 == 0 || nStr2 == 0 || nStr1 != nStr2)
        {
            return nStr1 - nStr2;
        }
		
        string1 += 2;
        string2 += 2;
        count += 2;
    }
    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  ucs2_strncpy
 * DESCRIPTION
 *  The function is used for copy UCS2 characters of one string to another.
 *  If size is less than the length of strSource, a null character is still 
 *  appended automatically to strDestination. If size is greater than the 
 *  length of strSource, strDestination is padding with null characters up 
 *  to length size. The function behavior is a little bit different from strncpy.
 * PARAMETERS
 *  strDestination  [OUT]  UCS2 destination string
 *  strSource       [IN]   UCS2 source string
 *  size            [IN]   Size in character (not byte)
 * RETURNS
 *  Return the destination string.
 *****************************************************************************/
char *ucs2_strncpy(char *strDestination, const char *strSource, u32 size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    u16 count = 1;
    u32 count1 = 0;
    char *temp = strDestination;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    size = size * 2;

    /* MMI_ASSERT(!(strDestination == NULL)); */

    while (!((*(strSource + count) == 0) && (*(strSource + count - 1) == 0)) && (count1 < size))
    {

        *(strDestination + count - 1) = *(strSource + count - 1);
        *(strDestination + count) = *(strSource + count);
        count += 2;
        count1 += 2;
    }

    *(strDestination + count - 1) = '\0';
    *(strDestination + count) = '\0';

    return temp;
}


/*****************************************************************************
 * FUNCTION
 *  ucs2_strcat
 * DESCRIPTION
 *  The function is used for append strSource to strDestination and terminate
 *  the resulting string with a null character. 
 * PARAMETERS
 *  strDestination   [OUT]  UCS2 destination string       
 *  strSource        [IN]   UCS2 source string     
 * RETURNS
 *  Return the destination string
 *****************************************************************************/
char *ucs2_strcat(char *strDestination, const char *strSource)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    char *dest = strDestination;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dest = dest + ucs2_strlen(strDestination) * ENCODING_LENGTH;

    ucs2_strcpy(dest, strSource);
    return strDestination;
}

/*****************************************************************************
 * FUNCTION
 *  ucs2_strstr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  a       [IN]        
 *  p       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
u16 *ucs2_strstr(const u16 *a, const u16 *p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    s16 i, m, n;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (p == NULL || a == NULL)
    {
        return NULL;
    }

    m = ucs2_wcslen(a);
    n = ucs2_wcslen(p);

    for (i = 0; i < m; i++)
    {
        int i_tmp = i;
        int j = 0;

        while (*(a + i) == *(p + j))
        {
            i++;
            j++;

            if (j == n)
            {
                return (u16*) (a + i_tmp);
            }
            else if (i >= m)
            {
                return NULL;
            }
        }
        i = i_tmp;
    }
	
    return NULL;
}

void bcd_to_ascii(u8 *bcd, u8 bcd_len, u8 *ascii)
{
	u8 i, lhex = 0, hhex = 0;

	if(NULL == bcd || NULL == ascii)
	{
		return;
	}

	for(i = 0; i < bcd_len; i++)
	{
		lhex = bcd[i] & 0x0f;
		hhex = (bcd[i] >> 4) & 0x0f;
		ascii[2*i] = lhex + '0';
		ascii[2*i + 1] = hhex + '0';
	}
}

s32 softsim_atoi(const char *buf)
{
	u32 result = 0;
	u32 i = 0;
	
	if(NULL == buf)
	{
		return -1;
	}
		
	for (; is_digit(buf[i]); ++i) 
	{
		result *= 10;
		result += buf[i] - '0';
	}
	
	return result;
}

char* softsim_strrev(char * string)
{
	char *start = string;
	char *left = string;
	char ch;

	while (*string++)                 /* find end of string */
		;
	string -= 2;

	while (left < string)
	{
		ch = *left;
		*left++ = *string;
		*string-- = ch;
	}
	
	return(start);
}


void softsim_itoa(u32 val, char* str, u32 radix)
{
	char temp = 0;
	char base[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	while (val >= radix)
	{
	    temp = base[val%radix];
	    softsim_sprintf(str+softsim_strlen(str), "%c", temp);
	    val /= radix;
	}

	if(val >= 0) 
	{
	    temp = base[val%radix];
	    softsim_sprintf(str+softsim_strlen(str), "%c", temp);
	};
	
	softsim_strrev(str);
}

u64 softsim_atoll(char* buf, u8 len)
{
	u64 result = 0;
	int i = 0;
	for (; i < len; ++i) {
		result *= 10;
		result += buf[i] - '0';
	}
	return result;
}

void softsim_lltostr(u64 val, char* str, u64 radix)
{
	char dest[128] = {0};
	char temp = 0;
	char base[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	while (val >= radix)
	{
	    temp = base[val%radix];
	    softsim_sprintf(dest+softsim_strlen(dest), "%c", temp);
	    val /= radix;
	}

	if(val >= 0) 
	{
	    temp = base[val%radix];
	    softsim_sprintf(dest+softsim_strlen(dest), "%c", temp);
	};
	softsim_strrev(dest);
	softsim_strcpy(str, dest);
}


void error_and_exit(char* message)
{
    //printf("error: %s", message);
 	//exit(-1);
}


u32 get_digit_from_symbol(char c)
{
	if ((c >= '0') && (c <= '9')) return (c - '0');
	if ((c >= 'a') && (c <= 'z')) return (c - 'a' + 10);
	if ((c >= 'A') && (c <= 'Z')) return (c - 'A' + 10);
	error_and_exit("HAHA!You have to enter characters within the ranges 0-9, a-z or A-Z");
}

u32 get_number_from_any_base(char* input_char, u32 base)
{   
    u32 x, i, d, oldx;
    char c;
    
    i = 0;
    x = 0;
    while (input_char[i] != 0)
    {
        c = input_char[i];
        oldx = x;
        x = x * base;
        if (x < oldx) 
	 	{
			error_and_exit("overflow, this number is too large");
	 	}
		
        d = get_digit_from_symbol(c);
		
        if (d >= base) 
	 	{
	 		error_and_exit("you cannot use digits greater or equal to the base");
	 	}
		
        oldx = x;
        x = x + d;
		
        if (x < oldx) 
	 	{
	 		error_and_exit("overflow, this number is too large");
	 	}
        i++;
    };
	
    return x;
}

s32 super_atoi(char *str)
{

   s32 len = softsim_strlen(str);
   
   	if(str[0] == '0')
   	{
	     switch(str[1])
	     {
			case 'x' :
	       case 'X' :
	           return get_number_from_any_base(str+2, 16);
			   
	       case 'o' :
	       case 'O' :
	           return get_number_from_any_base(str+2, 8);
			   
	       case 'b' :
	       case 'B' :
	           return get_number_from_any_base(str+2, 2);
			   
	       default :
	           return get_number_from_any_base(str, 10);
	     }
    	}

	return get_number_from_any_base(str, 10);
}


bool softsim_ascii_to_hex(u8 *hex, u8 ascii_h, u8 ascii_l)
{
	u8 hexh = 0;
	u8 hexl = 0;
	
	if(NULL == hex)
	{
		return false;
	}

	if(!is_xdigit(ascii_h) || !is_xdigit(ascii_h))
	{
		return false;
	}

	if(is_num(ascii_h))
	{
		hexh = ascii_h - '0';
	}
	else if(is_hex_lower(ascii_h))
	{
		hexh = ascii_h - 87;
	}
	else if(is_hex_upper(ascii_h))
	{
		hexh = ascii_h - 55;
	}
	else
	{
		return false;
	}

	
	if(is_num(ascii_l))
	{
		hexl = ascii_l - '0';
	}
	else if(is_hex_lower(ascii_l))
	{
		hexl = ascii_l - 87;
	}
	else if(is_hex_upper(ascii_l))
	{
		hexl = ascii_l - 55;
	}
	else
	{
		return false;
	}

	
	*hex = ((hexh<<4)&0xf0) | (hexl&0x0f);

	return true;
}

void softsim_imsi_asc_to_bcd(u8 imsi[SOFTSIM_IMSI_LEN], u8 *imsi_ascii)
{
	u8 index = 0;
	u8 imsi_string_len = softsim_strlen(imsi_ascii);

	if(NULL == imsi_ascii || 0 == imsi_string_len || NULL == imsi)
	{
		return;
	}

	if((17 == imsi_string_len))
	{

	}
	else if(16 == imsi_string_len)
	{

	}
	else if(15 == imsi_string_len)
	{
	
		imsi[0] = 0x08;
		
		softsim_ascii_to_hex(&imsi[1], imsi_ascii[0], '9');
		
		for(index=2; index<9; index++)
		{
			softsim_ascii_to_hex(&imsi[index], imsi_ascii[2*(index - 1)], imsi_ascii[2*(index-1) - 1]);
		}
	}
	else
	{
		;
	}
}

bool softsim_string_to_hex(u8 *hex, u32 hex_size, u8 *string, u32 string_len)
{
	u32 index = 0;
	u8 hexh = 0;
	u8 hexl = 0;
	
	if(NULL == hex || NULL == string || 0 == string_len)
	{
		return false;
	}

	if(hex_size < string_len/2)
	{
		return false;
	}

	for(index=0; index<string_len/2; index++)
	{
		hexh = ascii_to_hex(string[2*index]);
		hexl = ascii_to_hex(string[2*index+1]);
		hex[index] = (hexh<<4)|hexl;
	}

	return true;
}

bool softsim_param_valid(u8 *string)
{
	if(NULL == string || 0 == softsim_strlen(string))
	{
		return false;
	}

	return true;
}

void softsim_asc_to_bcd(u8 *bcd, u8 *asc)
{
	u8 index = 0;
	u8 len = 0;
	u8 bcd_len = 0;
	u8 bcdh = 0;
	u8 bcdl = 0;
	
	if(NULL == bcd || NULL == asc || 0 == softsim_strlen(asc))
	{
		return;
	}

	len = softsim_strlen(asc);

	for(index=0; index<len/2; index++)
	{
		bcdl = ascii_to_hex(asc[2*index]);
		bcdh = ascii_to_hex(asc[2*index+1]);
		bcd[index] = (bcdh<<4)|bcdl; 
	}

	if(1 == len%2)
	{
		bcdh = 0x0F;
		bcdl = ascii_to_hex(asc[2*index]);
		bcd[index] = (bcdh<<4)|bcdl; 
	}
}

void softsim_acc_technology_code(u8 *out, u16 acc_tech)
{
	u8 acc[2] = {0};
	
	if(NULL == out)
	{
		return;
	}

	if(0 != (acc_tech & 0x0001))
	{
		acc[1] = acc[1]|0x80;
	}

	if(0 != (acc_tech & 0x0002))
	{
		acc[1] = acc[1]|0x40;
	}

	if(0 != (acc_tech & 0x0004))
	{
		acc[1] = acc[1]|0x20;
	}

	if(0 != (acc_tech & 0x0008))
	{
		acc[1] = acc[1]|0x10;
	}

	if(0 != (acc_tech & 0x0010))
	{
		acc[0] = acc[0]|0x80;
	}

	if(0 != (acc_tech & 0x0020))
	{
		acc[0] = acc[0]|0x40;
	}

	softsim_memcpy(out, acc, 2);
}

void softsim_oplmnwact_string_to_code(u8 *oplmn, u8 plmn_num, u8 *string, u16 acc_tech)
{
	u8 index = 0;
	u8 len = 0;
	u8 num = 0;
	u8 buffer[150] = {0};
	
	if(NULL == oplmn || NULL == string || 0 == plmn_num)
	{
		return;
	}
	
	softsim_string_to_hex(buffer, 150, string, softsim_strlen(string));
	num = softsim_strlen(string)/2;
	
	num = num/3;

	if(num*5 > plmn_num)
	{
		num = plmn_num/5;
	}
	
	for(index=0; index<num; index++)
	{
		softsim_memcpy(oplmn+index*5, buffer+index*3, 3);
		softsim_acc_technology_code(oplmn+index*5+3, acc_tech);
	}
}

void ASCIIText_ConvertTo_GSMDefault(const u8 *def, u32 len, u8 *gsm_default)
{
  u32  i = 0;
  u32  j = 0;

  if(def == NULL)
  {
    	return;
  }

  if(gsm_default == NULL)
  {
    	return;
  }


  for( i = 0; i < len; i ++ )
  {
    j = def[i];
    gsm_default[i] = ascii_to_gsmdefault_table[j];
  }

} /* ASCIIText_ConvertTo_GSMDefault() */

bool softsim_plmn_list_3byte_conversion(char *plmn_string, char *buffer, u16 buffer_len)
{
	u16 index = 0;
	u16 string_index = 0;
	u16 plmn_num = 0;
	char *p = NULL;
	char *temp_p = NULL;
	char plmn_buffer[7] = {0};
	bool ret = false;
	
	if(NULL == plmn_string || 6 > softsim_strlen(plmn_string))
	{
		return ret;
	}

	p = plmn_string;
	
	for(index=0; index<softsim_strlen(plmn_string); index++)
	{
		if('-' == *p)
		{
			plmn_num++;
		}
		
		p++;
	}

	if(plmn_num*3 > buffer_len)
	{
		return ret;
	}

	p = plmn_string;
	
	for(index=0; index<plmn_num; index++)
	{
		while(0x0D == *p || 0x0A == *p || ' ' == *p)
		{
			p++;
		}

		temp_p = p;

		while(('-' != *temp_p) && (0 != *temp_p))
		{
			temp_p++;
		}
		
		softsim_memset(plmn_buffer, 0x00, sizeof(plmn_buffer));

		if(3 == temp_p - p)
		{
			softsim_memcpy(plmn_buffer,	p, temp_p - p);
		}
		else
		{
			while((',' != *temp_p) && (0 != *temp_p))
			{
				temp_p++;
			}

			if(0 == *temp_p)
			{
				return ret;
			}
			temp_p++;
			p = temp_p;			
			continue;
		}

		temp_p++;	/*skip -*/
		p = temp_p;

		while((',' != *temp_p) && (0 != *temp_p) && (' ' != *temp_p))
		{
			temp_p++;
		}

		if((2 == (temp_p - p)) || (3 == (temp_p - p)))
		{
			softsim_memcpy(plmn_buffer+3, p, temp_p - p);
		}
		else
		{
			while((',' != *temp_p) && (0 != *temp_p))
			{
				temp_p++;
			}
			temp_p++;
			p = temp_p;			

			if(0 == *p)
			{
				break;
			}

			continue;
		}

		while((',' != *temp_p) && (0 != *temp_p))
		{
			temp_p++;
		}

		temp_p++;/*skip ,*/

		p = temp_p;
		
		string_index = softsim_strlen(buffer);

		if(5 == softsim_strlen(plmn_buffer))
		{
			buffer[string_index] = plmn_buffer[1];
			buffer[string_index+1] = plmn_buffer[0];
			buffer[string_index+2] = 'F';			
			buffer[string_index+3] = plmn_buffer[2];
			buffer[string_index+4] = plmn_buffer[4];
			buffer[string_index+5] = plmn_buffer[3];
		}
		else if(6 == softsim_strlen(plmn_buffer))
		{
			buffer[string_index] = plmn_buffer[1];
			buffer[string_index+1] = plmn_buffer[0];
			buffer[string_index+2] = plmn_buffer[5];			
			buffer[string_index+3] = plmn_buffer[2];
			buffer[string_index+4] = plmn_buffer[4];
			buffer[string_index+5] = plmn_buffer[3];
		}
		else
		{
			continue;
		}
		
		ret = true;
	}

	return ret;
}

void softsim_ipstring_to_ip(char *ip_string, u8 ip_string_len, u8 *ip, u8 ip_len)
{
	char *string_p1 = NULL;
	char *string_p2 = NULL;
	u8 string_buffer[10] = {0};
	u8 ip_buffer[5] = {0};
	u8 index = 0;
	
	if((NULL == ip_string) || (softsim_strlen(ip_string) < ip_string_len))
	{
		return;
	}

	if((NULL == ip) || (4 != ip_len))
	{
		return;
	}

	string_p1 = ip_string;

	while(index < (ip_len - 1))
	{
		string_p2 = softsim_strstr(string_p1, ".");

		if((NULL != string_p2) && ((string_p2 - string_p1) < 4))
		{
			softsim_memset(string_buffer, 0x00, sizeof(string_buffer));
			softsim_strncpy(string_buffer, string_p1, string_p2 - string_p1);
			ip_buffer[index++] = softsim_atoi(string_buffer);
			string_p2++;
			string_p1 = string_p2;
		}
		else
		{
			return;
		}
	}

	ip_buffer[index++] = softsim_atoi(string_p1);
	softsim_memcpy(ip, ip_buffer, ip_len);
}

void softism_ip_to_ipstring(u8 *ip, u8 ip_len, char *ip_string, u8 ip_string_len)
{
	u8 index = 0;
	char string_buffer[50] = {0};
	char *string_p = NULL;
	
	if((NULL == ip) || (4 != ip_len) || (NULL == ip_string))
	{
		return;
	}

	string_p = string_buffer;

	while(index < 3)
	{
		softsim_itoa(*(ip+index), string_p, 10);
		softsim_strcat(string_p, ".");
		string_p = string_buffer + softsim_strlen(string_buffer);
		index++;
	}

	softsim_itoa(*(ip+index), string_p, 10);
	softsim_strncpy(ip_string, string_buffer, ip_string_len);
}

bool softsim_ip_check(const char *asci_addr, u8 *ip_addr)
{
	s16 len;
	s16 i,octet_cnt;
	s32 ip_digit;

	SOFTSIM_DEBUG("asci_addr = %s", asci_addr);

	len = softsim_strlen(asci_addr);

	for (i = 0 ; i < len ; i++)
	{
	    if (!is_digit(*(asci_addr+i)) && *(asci_addr+i) != '.' )
	    {
	    	SOFTSIM_DEBUG("asci_addr[%d] = 0x%x", i, *(asci_addr+i));
	        return false;
	    }
	}

	/* Extract the IP adress from character array */
	for (octet_cnt = 0 ; octet_cnt < 4 ; octet_cnt++)
	{
	    if (*asci_addr == '\0') /* in case of "1.2.3." */
	    {
	    	SOFTSIM_DEBUG("");
	        return false;
	    }
	    ip_digit = softsim_atoi(asci_addr);

	    if (ip_digit < 0 || ip_digit > 255)
	    {
	    	SOFTSIM_DEBUG("");
	        return false;
	    }

	    ip_addr[octet_cnt] = (u8)ip_digit;

	    if (octet_cnt == 3)
	        continue;

	    asci_addr = softsim_strstr(asci_addr,".");
	    if (asci_addr)
	        asci_addr++;
	    else
	        break;
	}

	if (octet_cnt != 4)
	{
		SOFTSIM_DEBUG("");
	    return false;
	}

	return true;
}

void softsim_get_rand(u8*out_buffer,u8 size)
{
	u8 index = 0;
	
	if(NULL == out_buffer || 0 == size)
	{
		return;
	}

	for(;index<size;index++)
	{
		out_buffer[index] = (u8)(((u32)(rand()))&0x000000ff);
	}
}
