#ifndef _COMMAND_H_
#define _COMMAND_H_ 


#include "lpcore.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef int (*lpc_cmd_handler)(LinphoneCore *, char *);
	typedef struct {
		char *name;		/* User printable name of the function. */
		lpc_cmd_handler func;	/* Function to call to do the job. */
	} LPC_COMMAND;

	int lpc_parse_command_line(LinphoneCore *lc, char *cl);

	int lpc_cmd_quit(LinphoneCore *lc, char *args);
	int lpc_cmd_call(LinphoneCore *lc, char *args);
	int lpc_cmd_terminate(LinphoneCore *lc, char *args);
	int lpc_cmd_answer(LinphoneCore *lc, char *args);
	int lpc_cmd_mon(LinphoneCore *lc, char *args);
	int lpc_cmd_open(LinphoneCore *lc, char *args);
	int lpc_cmd_shutmon(LinphoneCore *lc, char *args);
	int lpc_cmd_record(LinphoneCore *lc, char *args);
	int lpc_cmd_video(LinphoneCore *lc, char *args);
	
#ifdef __cplusplus
}
#endif



#endif
