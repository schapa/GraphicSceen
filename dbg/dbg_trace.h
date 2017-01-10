/*
 * bs_dbg.h
 *
 *  Created on: May 6, 2016
 *      Author: shapa
 */

#ifndef DBG_TRACE_H_
#define DBG_TRACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dbg_base.h"

#undef DBGMSG_L
#undef DBGMSG_M
#undef DBGMSG_H
#undef DBG_ENTRY
#undef DBG_EXIT

#define DBGMSG_L(fmt, ...) \
	dbgmsg(DBGMSG_L_COLOR, "", __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define DBGMSG_M(fmt, ...) \
	dbgmsg(DBGMSG_M_COLOR, "", __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define DBGMSG_H(fmt, ...) \
	dbgmsg(DBGMSG_H_COLOR, "", __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define DBG_ENTRY DBGMSG_L("--> Enter")
#define DBG_EXIT DBGMSG_L("<-- Exit")

#ifdef __cplusplus
}
#endif
#endif /* DBG_TRACE_H_ */
