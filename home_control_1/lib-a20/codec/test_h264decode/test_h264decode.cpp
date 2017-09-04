#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libcedarv.h"
#include "cedarx_hardware.h"
extern "C"
{
#include "cedarv_osal_linux.h"
}
#include "render.h"

static void map32x32_to_yuv_Y(unsigned char* srcY, unsigned char* tarY,unsigned int coded_width,unsigned int coded_height)
{
	unsigned int i,j,l,m,n;
	unsigned int mb_width,mb_height,twomb_line, twomb_width;
	unsigned long offset;
	unsigned char *ptr;
	unsigned char *dst_asm,*src_asm;
    unsigned vdecbuf_width, vdecbuf_height;
    int nWidthMatchFlag;
    int nLeftValidLine;  //in the bottom macroblock(32*32), the valid line is < 32.
	ptr = srcY;

    mb_width = ((coded_width+31)&~31) >>4;
	mb_height = ((coded_height+31)&~31) >>4;
	twomb_line = (mb_height+1)>>1;
    twomb_width = (mb_width+1)>>1;
    if(twomb_line < 1 || twomb_width < 1)
    {
        printf("fatal error! twomb_line=%d, twomb_width=%d", twomb_line, twomb_width);
    }
    vdecbuf_width = twomb_width*32;
    vdecbuf_height = twomb_line*32;
    if(vdecbuf_width > coded_width)
    {
        nWidthMatchFlag = 0;
        if((vdecbuf_width - coded_width) != 16)
        {
            printf("(f:%s, l:%d) fatal error! vdecbuf_width=%d, gpubuf_width=%d,  the program will crash!", __FUNCTION__, __LINE__, vdecbuf_width, coded_width);
        }
        else
        {
            //LOGV("(f:%s, l:%d) Be careful! vdecbuf_width=%d, gpubuf_width=%d", __FUNCTION__, __LINE__, vdecbuf_width, coded_width);
        }
    }
    else if(vdecbuf_width == coded_width)
    {
        nWidthMatchFlag = 1;
    }
    else
    {
        printf("(f:%s, l:%d) fatal error! vdecbuf_width=%d <= gpubuf_width=%d, the program will crash!", __FUNCTION__, __LINE__, vdecbuf_width, coded_width);
        nWidthMatchFlag = 0;
    }
	for(i=0;i<twomb_line-1;i++)   //twomb line number
	{
		for(j=0;j<twomb_width-1;j++)   //macroblock(32*32) number in one line
		{
			for(l=0;l<32;l++)
			{
				//first mb
				m=i*32 + l;     //line num
				n= j*32;        //byte num in one line
				offset = m*coded_width + n;
				//memcpy(tarY+offset,ptr,32);
				dst_asm = tarY+offset;
				src_asm = ptr;
				asm volatile (
				        "vld1.8         {d0 - d3}, [%[src_asm]]              \n\t"
				        "vst1.8         {d0 - d3}, [%[dst_asm]]              \n\t"
				        : [dst_asm] "+r" (dst_asm), [src_asm] "+r" (src_asm)
				        :  //[srcY] "r" (srcY)
				        : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d28", "d29", "d30", "d31"
				        );

				ptr += 32;  //32 byte in one process.
			}
		}
        //process last macroblock of one line, gpu buf must be 16byte align or 32 byte align
        { //last mb of one line
            for(l=0;l<32;l++)
			{
				//first mb
				m=i*32 + l;     //line num
				n= j*32;        //byte num in one line
				offset = m*coded_width + n;
				//memcpy(tarY+offset,ptr,32);
				dst_asm = tarY+offset;
				src_asm = ptr;
                if(nWidthMatchFlag)
                {
    				asm volatile (
    				        "vld1.8         {d0 - d3}, [%[src_asm]]              \n\t"
    				        "vst1.8         {d0 - d3}, [%[dst_asm]]              \n\t"
    				        : [dst_asm] "+r" (dst_asm), [src_asm] "+r" (src_asm)
    				        :  //[srcY] "r" (srcY)
    				        : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d28", "d29", "d30", "d31"
    				        );
                }
                else
                {
                    asm volatile (
    				        "vld1.8         {d0,d1}, [%[src_asm]]              \n\t"
    				        "vst1.8         {d0,d1}, [%[dst_asm]]              \n\t"
    				        : [dst_asm] "+r" (dst_asm), [src_asm] "+r" (src_asm)
    				        :  //[srcY] "r" (srcY)
    				        : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d28", "d29", "d30", "d31"
    				        );
                }

				ptr += 32;  //32 byte in one process.
			}
        }
	}
    //last twomb line, we process it alone
    nLeftValidLine = coded_height - (twomb_line-1)*32;
    if(nLeftValidLine!=32)
    {
        //LOGV("(f:%s, l:%d)hehehaha,gpuBufHeight[%d] is not 32 align", __FUNCTION__, __LINE__, nLeftValidLine);
    }
    for(j=0;j<twomb_width-1;j++)   //macroblock(32*32) number in one line
	{
		for(l=0;l<nLeftValidLine;l++)
		{
			//first mb
			m=i*32 + l;     //line num
			n= j*32;        //byte num in one line
			offset = m*coded_width + n;
			//memcpy(tarY+offset,ptr,32);
			dst_asm = tarY+offset;
			src_asm = ptr;
			asm volatile (
			        "vld1.8         {d0 - d3}, [%[src_asm]]              \n\t"
			        "vst1.8         {d0 - d3}, [%[dst_asm]]              \n\t"
			        : [dst_asm] "+r" (dst_asm), [src_asm] "+r" (src_asm)
			        :  //[srcY] "r" (srcY)
			        : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d28", "d29", "d30", "d31"
			        );

			ptr += 32;  //32 byte in one process.
		}
        ptr += (32-nLeftValidLine)*32;
	}
    //process last macroblock of last line, gpu buf must be 16byte align or 32 byte align
    { //last mb of last line
        for(l=0;l<nLeftValidLine;l++)
		{
			//first mb
			m=i*32 + l;     //line num
			n= j*32;        //byte num in one line
			offset = m*coded_width + n;
			//memcpy(tarY+offset,ptr,32);
			dst_asm = tarY+offset;
			src_asm = ptr;
            if(nWidthMatchFlag)
            {
				asm volatile (
				        "vld1.8         {d0 - d3}, [%[src_asm]]              \n\t"
				        "vst1.8         {d0 - d3}, [%[dst_asm]]              \n\t"
				        : [dst_asm] "+r" (dst_asm), [src_asm] "+r" (src_asm)
				        :  //[srcY] "r" (srcY)
				        : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d28", "d29", "d30", "d31"
				        );
            }
            else
            {
                asm volatile (
				        "vld1.8         {d0,d1}, [%[src_asm]]              \n\t"
				        "vst1.8         {d0,d1}, [%[dst_asm]]              \n\t"
				        : [dst_asm] "+r" (dst_asm), [src_asm] "+r" (src_asm)
				        :  //[srcY] "r" (srcY)
				        : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d28", "d29", "d30", "d31"
				        );
            }

			ptr += 32;  //32 byte in one process.
		}
        ptr += (32-nLeftValidLine)*32;
    }

}

static void map32x32_to_yuv_C(int mode, unsigned char* srcC,unsigned char* tarCb,unsigned char* tarCr,unsigned int coded_width,unsigned int coded_height)
{
	unsigned int i,j,l,m,n,k;
	unsigned int mb_width,mb_height,twomb_line,twomb_width;
	unsigned long offset;
	unsigned char *ptr;
	unsigned char *dst0_asm,*dst1_asm,*src_asm;
    unsigned vdecbuf_width, vdecbuf_height; //unit: pixel
    int         nWidthMatchFlag;
    int         nLeftValidLine;  //in the bottom macroblock(32*32), the valid line is < 32.
	unsigned char line[16];

    int dst_stride = mode==0 ? (coded_width + 15) & (~15) : coded_width;

	ptr = srcC;

//	mb_width = (coded_width+7)>>3;
//	mb_height = (coded_height+7)>>3;
//	fourmb_line = (mb_height+3)>>2;
//	recon_width = (mb_width+1)&0xfffffffe;

    mb_width = ((coded_width+15)&~15)>>4;   //vdec's uvBuf is 32byte align, so uBuf and vBuf is 16byte align!
	mb_height = ((coded_height+31)&~31)>>4;
	twomb_line = (mb_height+1)>>1;
	//recon_width = (mb_width+1)&0xfffffffe;
    twomb_width = mb_width; //vdec mb32 is uv interleave, so uv_32 byte == u_16byte
    if(twomb_line < 1 || twomb_width < 1)
    {
        printf("map32x32_to_yuv_C() fatal error! twomb_line=%d, twomb_width=%d", twomb_line, twomb_width);
    }
    //vdec mb32 uvBuf, one vdec_macro_block, extract u component, u's width and height.
    vdecbuf_width = twomb_width*16;
    vdecbuf_height = twomb_line*32;
    if(vdecbuf_width > coded_width)
    {
        nWidthMatchFlag = 0;
        if((vdecbuf_width - coded_width) != 8)
        {
            printf("(f:%s, l:%d) fatal error! vdec_UVbuf_width=%d, gpu_UVbuf_width=%d,  the program will crash!", __FUNCTION__, __LINE__, vdecbuf_width, coded_width);
        }
        else
        {
            //LOGV("(f:%s, l:%d) vdec_UVbuf_width=%d, gpu_UVbuf_width=%d, not match, gpu_uvBuf is 8byte align?", __FUNCTION__, __LINE__, vdecbuf_width, coded_width);
        }
    }
    else if(vdecbuf_width == coded_width)
    {
        nWidthMatchFlag = 1;
    }
    else
    {
        printf("(f:%s, l:%d) fatal error! vdec_UVbuf_width=%d <= gpu_UVbuf_width=%d, the program will crash!", __FUNCTION__, __LINE__, vdecbuf_width, coded_width);
        nWidthMatchFlag = 0;
    }

	for(i=0;i<twomb_line-1;i++)
	{
		for(j=0;j<twomb_width-1;j++)
		{
			for(l=0;l<32;l++)
			{
				//first mb
				m=i*32 + l; //line num
				n= j*16;    //byte num in dst_one_line
				offset = m*dst_stride + n;

				dst0_asm = tarCb + offset;
				dst1_asm = tarCr+offset;
				src_asm = ptr;
//					for(k=0;k<16;k++)
//					{
//						dst0_asm[k] = src_asm[2*k];
//						dst1_asm[k] = src_asm[2*k+1];
//					}
            asm volatile (
                    "vld2.8         {d0 - d3}, [%[src_asm]]              \n\t"
                    "vst1.8         {d0,d1}, [%[dst0_asm]]              \n\t"
                    "vst1.8         {d2,d3}, [%[dst1_asm]]              \n\t"
                     : [dst0_asm] "+r" (dst0_asm), [dst1_asm] "+r" (dst1_asm), [src_asm] "+r" (src_asm)
                     :  //[srcY] "r" (srcY)
                     : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d28", "d29", "d30", "d31"
                     );

				ptr += 32;
			}
		}
        //process last twomb_macroblock of one line, gpu buf must be 16 byte align or 8 byte align.
        for(l=0;l<32;l++)
		{
			//first mb
			m=i*32 + l; //line num
			n= j*16;    //byte num in dst_one_line
			offset = m*dst_stride + n;

			dst0_asm = tarCb + offset;
			dst1_asm = tarCr+offset;
			src_asm = ptr;
//					for(k=0;k<16;k++)
//					{
//						dst0_asm[k] = src_asm[2*k];
//						dst1_asm[k] = src_asm[2*k+1];
//					}

            if(nWidthMatchFlag)
            {
                asm volatile (
                        "vld2.8         {d0 - d3}, [%[src_asm]]              \n\t"
                        "vst1.8         {d0,d1}, [%[dst0_asm]]              \n\t"
                        "vst1.8         {d2,d3}, [%[dst1_asm]]              \n\t"
                         : [dst0_asm] "+r" (dst0_asm), [dst1_asm] "+r" (dst1_asm), [src_asm] "+r" (src_asm)
                         :  //[srcY] "r" (srcY)
                         : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d28", "d29", "d30", "d31"
                         );
            }
            else
            {
                asm volatile (
                        "vld2.8         {d0,d1}, [%[src_asm]]              \n\t"
                        "vst1.8         {d0}, [%[dst0_asm]]              \n\t"
                        "vst1.8         {d1}, [%[dst1_asm]]              \n\t"
                         : [dst0_asm] "+r" (dst0_asm), [dst1_asm] "+r" (dst1_asm), [src_asm] "+r" (src_asm)
                         :  //[srcY] "r" (srcY)
                         : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d28", "d29", "d30", "d31"
                         );
            }
			ptr += 32;
		}
	}

    //last twomb line, we process it alone
    nLeftValidLine = coded_height - (twomb_line-1)*32;  //uv height can be odd number,must be very careful!
    if(nLeftValidLine!=32)
    {
        //LOGV("(f:%s, l:%d) hehehaha,gpu_UVBuf_extra_Height[%d] is not 32 align, coded_height[%d], twomb_line[%d]",
         //   __FUNCTION__, __LINE__, nLeftValidLine, coded_height, twomb_line);
    }
    for(j=0;j<twomb_width-1;j++)   //macroblock(32*32) number in one line
	{
		for(l=0;l<nLeftValidLine;l++)
		{
			//first mb
			m=i*32 + l;     //line num
			n= j*16;        //byte num in dst_one_line
			offset = m*dst_stride + n;

			dst0_asm = tarCb + offset;
			dst1_asm = tarCr+offset;
			src_asm = ptr;
//              for(k=0;k<16;k++)
//              {
//                  dst0_asm[k] = src_asm[2*k];
//                  dst1_asm[k] = src_asm[2*k+1];
//              }
            asm volatile (
                    "vld2.8         {d0 - d3}, [%[src_asm]]              \n\t"
                    "vst1.8         {d0,d1}, [%[dst0_asm]]              \n\t"
                    "vst1.8         {d2,d3}, [%[dst1_asm]]              \n\t"
                     : [dst0_asm] "+r" (dst0_asm), [dst1_asm] "+r" (dst1_asm), [src_asm] "+r" (src_asm)
                     :  //[srcY] "r" (srcY)
                     : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d28", "d29", "d30", "d31"
                     );
			ptr += 32;  //32 byte in one process.
		}
        ptr += (32-nLeftValidLine)*32;
	}
    //process last macroblock of last line, gpu UVbuf must be 16byte align or 8 byte align
    { //last mb of last line
        for(l=0;l<nLeftValidLine;l++)
		{
			//first mb
			m=i*32 + l;     //line num
			n= j*16;        //byte num in one line
			offset = m*dst_stride + n;

			dst0_asm = tarCb + offset;
			dst1_asm = tarCr+offset;
			src_asm = ptr;
            if(nWidthMatchFlag)
            {
                asm volatile (
                        "vld2.8         {d0 - d3}, [%[src_asm]]              \n\t"
                        "vst1.8         {d0,d1}, [%[dst0_asm]]              \n\t"
                        "vst1.8         {d2,d3}, [%[dst1_asm]]              \n\t"
                         : [dst0_asm] "+r" (dst0_asm), [dst1_asm] "+r" (dst1_asm), [src_asm] "+r" (src_asm)
                         :  //[srcY] "r" (srcY)
                         : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d28", "d29", "d30", "d31"
                         );
            }
            else
            {
                asm volatile (
                        "vld2.8         {d0,d1}, [%[src_asm]]              \n\t"
                        "vst1.8         {d0}, [%[dst0_asm]]              \n\t"
                        "vst1.8         {d1}, [%[dst1_asm]]              \n\t"
                         : [dst0_asm] "+r" (dst0_asm), [dst1_asm] "+r" (dst1_asm), [src_asm] "+r" (src_asm)
                         :  //[srcY] "r" (srcY)
                         : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d28", "d29", "d30", "d31"
                         );
            }

			ptr += 32;  //32 byte in one process.
		}
        ptr += (32-nLeftValidLine)*32;
    }
}

int main()
{
	int ret;
	FILE *fp_in, *fp_out;
    fp_in = fopen("/tmp/out.h264", "rb");
	if (!fp_in)
	{
		printf("Open out.h264 failed\n");
		return -1;
	}

	fp_out = fopen("/tmp/foreman_rec.yuv", "wb");
	if (!fp_out)
	{
		printf("Open foreman_rec.yuv failed\n");
		fclose(fp_in);
		return -1;
	}
	
	cedarx_hardware_init(0);
    printf("decode 0\n");
	render_init();
	
	cedarv_decoder_t* decoder;
	decoder = cedarvDecInit(&ret);
    printf("decode 1\n");
    if (ret < 0)
	{
		printf("can not initialize the decoder library.\n");
		cedarx_hardware_exit(0);	
		fclose(fp_in);
		fclose(fp_out);
		return -1;
	}
	
	cedarv_stream_info_t stream_info;
	stream_info.format 			 = CEDARV_STREAM_FORMAT_H264;
	stream_info.sub_format 		 = CEDARV_SUB_FORMAT_UNKNOW;
	stream_info.container_format = CEDARV_CONTAINER_FORMAT_UNKNOW;
	stream_info.video_width 	 = 352;
	stream_info.video_height	 = 288;
	stream_info.frame_rate		 = 30*1000;
	stream_info.frame_duration	 = 0;
	stream_info.aspect_ratio	 = 1000;
	stream_info.init_data_len	 = 0;
	stream_info.init_data		 = 0;
	decoder->set_vstream_info(decoder, &stream_info);
    printf("decode 3\n");
	ret = decoder->open(decoder);
    printf("decode 4\n");
	if(ret < 0)
	{
		printf("can not open decoder.\n");
		cedarvDecExit(decoder);
		cedarx_hardware_exit(0);	
		fclose(fp_in);
		fclose(fp_out);		

		return -1;
	}
	
	decoder->ioctrl(decoder, CEDARV_COMMAND_PLAY, 0);
	
	int i;
	int len;
	char buf[1024 * 1024];
	cedarv_stream_data_info_t data_info;
	cedarv_picture_t picture;
	unsigned char* buf0;
	unsigned int   bufsize0;
	unsigned char* buf1;
	unsigned int   bufsize1;
	
	i = 0;
	while (1)
	{
		int cnt = fread(&len, 1, 4, fp_in);
		if (cnt < 4) break;
		fread(buf, 1, len, fp_in);
		
		printf("%d\n", i++);
			
		ret = decoder->request_write(decoder, len, &buf0, &bufsize0, &buf1, &bufsize1);
		if (ret < 0)
		{
			printf("request bit stream buffer fail.\n");
			break;
		}
		
		memset(&data_info, 0, sizeof(data_info));
		if (len <= bufsize0)
		{
			memcpy(buf0, buf, len);
			data_info.length = len;
		}
		else
		{
			memcpy(buf0, buf, bufsize0);
			memcpy(buf1, buf + bufsize0, len - bufsize0);
			data_info.length = len;
		}
		
		data_info.flags |= CEDARV_FLAG_FIRST_PART;
		data_info.flags |= CEDARV_FLAG_LAST_PART;
		
		decoder->update_data(decoder, &data_info);
		
		ret = decoder->decode(decoder);
		
		if (ret == CEDARV_RESULT_ERR_NO_MEMORY || ret == CEDARV_RESULT_ERR_UNSUPPORTED)
		{
			printf("bit stream is unsupported.\n");
			break;
		}
		
		ret = decoder->display_request(decoder, &picture);
		if (ret == 0)
		{
            printf("decoded ok\n");
			
			char tmp[352 * 288 * 3/2];
            char *ptr = (char *)cedarv_address_phy2vir((void *)picture.y);
            map32x32_to_yuv_Y((unsigned char *)ptr, (unsigned char *)tmp, 352, 288);
            ptr = (char *)cedarv_address_phy2vir((void *)picture.u);
            map32x32_to_yuv_C(0, (unsigned char *)ptr, (unsigned char *)(tmp + 352 * 288),
                (unsigned char *)(tmp + 352 * 288 * 5/4), 176, 144);
            fwrite(tmp, 1, 352 * 288 * 3/2, fp_out);

			render_render(&picture, picture.id);
			
			decoder->display_release(decoder, picture.id);
		}
		
		usleep(1000000/30);
	}

	decoder->ioctrl(decoder, CEDARV_COMMAND_STOP, 0);
	decoder->close(decoder);
	cedarvDecExit(decoder);

	render_exit();
	cedarx_hardware_exit(0);	

	fclose(fp_in);
	fclose(fp_out);	
}
