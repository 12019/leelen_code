#ifndef __SAMSUNG_SYSLSI_APDEV_S3C_LCD_H__
#define __SAMSUNG_SYSLSI_APDEV_S3C_LCD_H__

typedef struct {
	int Bpp;
	int LeftTop_x;
	int LeftTop_y;
	int Width;
	int Height;
} s3c_win_info_t;
typedef struct {
	int bpp;
	int left_x;
	int top_y;
	int width;
	int height;
} s3cfb_win_info_t;
typedef struct {
	int width;
	int height;
	int bpp;
	int offset;
	int v_width;
	int v_height;
}vs_info_t;

struct s3c_fb_dma_info
{
	unsigned int map_dma_f1;
	unsigned int map_dma_f2;
};
typedef struct {
	int direction;
	unsigned int compkey_red;
	unsigned int compkey_green;
	unsigned int compkey_blue;
} s3cfb_color_key_info_t;

typedef struct {
	unsigned int colval_red;
	unsigned int colval_green;
	unsigned int colval_blue;
} s3cfb_color_val_info_t;

#define FBIO_WAITFORVSYNC _IOW('F', 0x20, u_int32_t)

#define SET_VS_START 	_IO('F', 103) 
#define SET_VS_STOP 	_IO('F', 104) 
#define SET_VS_INFO 	_IOW('F', 105, vs_info_t) 
#define SET_VS_MOVE 	_IOW('F', 106, u_int) 

#define SET_OSD_INFO	_IOW('F', 209, s3c_win_info_t)
#define SET_OSD_START	_IO('F', 201)
#define SET_OSD_STOP	_IO('F', 202)

#define GET_FB_INFO		_IOR('F', 307, struct s3c_fb_dma_info)
#define SET_FB_CHANGE_REQ	_IOW('F', 308, int)		
#define SET_VSYNC_INT		_IOW('F', 309, int)



#define S3CFB_OSD_START			_IO  ('F', 201)
#define S3CFB_OSD_STOP			_IO  ('F', 202)
#define S3CFB_OSD_ALPHA_UP		_IO  ('F', 203)
#define S3CFB_OSD_ALPHA_DOWN		_IO  ('F', 204)
#define S3CFB_OSD_MOVE_LEFT		_IO  ('F', 205)
#define S3CFB_OSD_MOVE_RIGHT		_IO  ('F', 206)
#define S3CFB_OSD_MOVE_UP		_IO  ('F', 207)
#define S3CFB_OSD_MOVE_DOWN		_IO  ('F', 208)
#define S3CFB_OSD_SET_INFO		_IOW ('F', 209, s3cfb_win_info_t)
#define S3CFB_OSD_ALPHA0_SET		_IOW ('F', 210, unsigned int)
#define S3CFB_OSD_ALPHA1_SET		_IOW ('F', 211, unsigned int)
#define S3CFB_OSD_ALPHA_MODE		_IOW ('F', 212, unsigned int)

#define S3CFB_COLOR_KEY_START		_IO  ('F', 300)
#define S3CFB_COLOR_KEY_STOP		_IO  ('F', 301)
#define S3CFB_COLOR_KEY_ALPHA_START	_IO  ('F', 302)
#define S3CFB_COLOR_KEY_ALPHA_STOP	_IO  ('F', 303)
#define S3CFB_COLOR_KEY_SET_INFO	_IOW ('F', 304, s3cfb_color_key_info_t)
#define S3CFB_COLOR_KEY_VALUE		_IOW ('F', 305, s3cfb_color_val_info_t)



#define S3CFB_PALETTE_BUFF_CLEAR	(0x80000000)	/* entry is clear/invalid */
#define S3CFB_COLOR_KEY_DIR_BG 		0
#define S3CFB_COLOR_KEY_DIR_FG 		1
#define S3CFB_DEFAULT_BACKLIGHT_LEVEL	2
#define S3CFB_MAX_DISPLAY_OFFSET	200
#define S3CFB_DEFAULT_DISPLAY_OFFSET	100
#define S3CFB_MAX_ALPHA_LEVEL		0xf
#define S3CFB_MAX_BRIGHTNESS		90
#define S3CFB_DEFAULT_BRIGHTNESS	4
#define S3CFB_VS_SET 			12
#define S3CFB_VS_MOVE_LEFT		15
#define S3CFB_VS_MOVE_RIGHT		16
#define S3CFB_VS_MOVE_UP		17
#define S3CFB_VS_MOVE_DOWN		18
#define S3CFB_ALPHA_MODE_PLANE		0
#define S3CFB_ALPHA_MODE_PIXEL		1

#define FB_DEV_NAME	"/dev/fb0"
#define FB_DEV_NAME1	"/dev/fb1"
#define FB_DEV_NAME2	"/dev/fb2"
#define FB_DEV_NAME3	"/dev/fb3"
#define FB_DEV_NAME4	"/dev/fb4"

#define FB_WIDTH 	320
#define FB_HEIGHT 	240
//#define FB_VIRTUAL_WIDTH	800
//#define FB_VIRTUAL_HEIGHT	960
#define FB_VIRTUAL_WIDTH	320
#define FB_VIRTUAL_HEIGHT	240

#define BytesPerPixel			4
#define BitsPerPixel			24

#endif //__SAMSUNG_SYSLSI_APDEV_S3C_LCD_H__
