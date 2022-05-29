/*
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2017 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

#include "log.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#define LOG_INFO 0
#define LOG_ERR 1
#define LOG_NOTICE 2
#define LOG_DEBUG 3
#define LOG_WARNING 4


// Default values
static const int log_opts = 0; //LOG_CONS | LOG_PERROR;	// print syslog to stderror
static const int log_facility = 0; // LOG_USER;

static uint8_t log_open = 0;

void vsyslog(uint8_t logType, const char *fmt, va_list args)
{
	switch (logType)
	{
	case LOG_INFO:
		printf("INFO ");
		break;
	case LOG_ERR:
		printf("ERROR ");
		break;
	case LOG_NOTICE:
		printf("NOTICE ");
		break;
	case LOG_DEBUG:
		printf("DEBUG ");
		break;
	case LOG_WARNING:
		printf("WARNING ");
		break;
	default:
		break;
	}
	_vfprintf_l(stdout, fmt, NULL, args);
}

void logOpen(int options, int facility)
{
//	openlog(NULL, options, facility);
	log_open = 1;
}

void vlogInfo(const char *fmt, va_list args)
{
	if (!log_open) {
		logOpen(log_opts, log_facility);
	}
	vsyslog(LOG_INFO, fmt, args);
}

void logInfo(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vlogInfo(fmt, args);
	va_end(args);
}

void vlogError(const char *fmt, va_list args)
{
	if (!log_open) {
		logOpen(log_opts, log_facility);
	}
	vsyslog(LOG_ERR, fmt, args);
}

void logError(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vlogError(fmt, args);
	va_end(args);
}

void vlogNotice(const char *fmt, va_list args)
{
	if (!log_open) {
		logOpen(log_opts, log_facility);
	}
	vsyslog(LOG_NOTICE, fmt, args);
}

void logNotice(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vlogNotice(fmt, args);
	va_end(args);
}

void vlogDebug(const char *fmt, va_list args)
{
	if (!log_open) {
		logOpen(log_opts, log_facility);
	}
	vsyslog(LOG_DEBUG, fmt, args);
}

void logDebug(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vlogDebug(fmt, args);
	va_end(args);
}

void vlogWarning(const char *fmt, va_list args)
{
	if (!log_open) {
		logOpen(log_opts, log_facility);
	}
	vsyslog(LOG_WARNING, fmt, args);
}

void logWarning(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vlogWarning(fmt, args);
	va_end(args);
}
