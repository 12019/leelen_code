 
#ifndef LPCONFIG_H
#define LPCONFIG_H

typedef struct _LpConfig LpConfig;

#ifdef __cplusplus
extern "C" {
#endif

LpConfig * lp_config_new(const char *filename);
const char *lp_config_get_string(LpConfig *lpconfig, const char *section, const char *key, const char *default_string);
int lp_config_get_int(LpConfig *lpconfig,const char *section, const char *key, int default_value);
void lp_config_set_string(LpConfig *lpconfig,const char *section, const char *key, const char *value);
void lp_config_set_int(LpConfig *lpconfig,const char *section, const char *key, int value);
int lp_config_sync(LpConfig *lpconfig);
int lp_config_has_section(LpConfig *lpconfig, const char *section);
void lp_config_clean_section(LpConfig *lpconfig, const char *section);
void lp_config_destroy(LpConfig *cfg);
	
#ifdef __cplusplus
}
#endif

#endif
