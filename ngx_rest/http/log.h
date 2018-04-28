/*
 **************************************************************************************
 *       Filename:  log.h
 *    Description:   header file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 17:31:22
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

int log_init(const char* logfile);
void write_log(const char* fmt, ...);

#ifndef LOG_TAG
#define LOG_TAG "rest"
#endif

#define logd(fmt, ...) write_log("[D/" LOG_TAG "]" fmt "\n", ##__VA_ARGS__)
#define logi(fmt, ...) write_log("[I/" LOG_TAG "]" fmt "\n", ##__VA_ARGS__)
#define logw(fmt, ...) write_log("[W/" LOG_TAG "]" fmt "\n", ##__VA_ARGS__)
#define loge(fmt, ...) write_log("[E/" LOG_TAG "]" fmt "\n", ##__VA_ARGS__)

#endif /*LOG_H_INCLUDED*/

/********************************** END **********************************************/

