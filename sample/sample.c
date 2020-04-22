/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2020-04-15    xiangxistu      the first version
 */
 
#include <cmux.h>
#include <rtthread.h>


#define CMUX_PPP_NAME "cmux_ppp"
#define  CMUX_AT_NAME "cmux_at"

#define CMUX_PPP_PORT 1
#define  CMUX_AT_PORT 2

#define DBG_TAG    "cmux.sample"

#ifdef CMUX_DEBUG
#define DBG_LVL   DBG_LOG
#else
#define DBG_LVL   DBG_INFO
#endif

#include <rtdbg.h>

struct cmux *sample = RT_NULL;

int cmux_sample_start(void)
{
    rt_err_t result;
    sample = cmux_object_find(CMUX_DEPEND_NAME);
    if(sample == RT_NULL)
    {
        result = -RT_ERROR;
        LOG_E("Can't find %s", CMUX_DEPEND_NAME);
        goto end;
    }
    result =cmux_start(sample);
    if(result != RT_EOK)
    {
        LOG_E("cmux sample start error. Can't find %s", CMUX_DEPEND_NAME);
        goto end;
    }
    LOG_I("cmux sample (%s) start successful.", CMUX_DEPEND_NAME);
#ifdef CMUX_AT_NAME
    result = cmux_attach(sample, CMUX_AT_PORT,  CMUX_AT_NAME, RT_DEVICE_FLAG_DMA_RX, RT_NULL);
    if(result != RT_EOK)
    {
        LOG_E("cmux attach (%s) failed.", CMUX_AT_NAME);
        goto end;
    }
    LOG_I("cmux object channel (%s) attach successful.", CMUX_AT_NAME);
#endif
#ifdef CMUX_PPP_NAME
    result = cmux_attach(sample, CMUX_PPP_PORT, CMUX_PPP_NAME, RT_DEVICE_FLAG_DMA_RX, RT_NULL);
    if(result != RT_EOK)
    {
        LOG_E("cmux attach %s failed.", CMUX_PPP_NAME);
        goto end;
    }
    LOG_I("cmux object channel (%s) attach successful.", CMUX_PPP_NAME);
#endif

end:
    return RT_EOK;
}
INIT_APP_EXPORT(cmux_sample_start);
MSH_CMD_EXPORT_ALIAS(cmux_sample_start, cmux_start, a sample of cmux function);

int cmux_sample_stop(void)
{
    rt_err_t result;
#ifdef CMUX_AT_NAME
    result = cmux_detach(sample, CMUX_AT_NAME);
    if(result != RT_EOK)
    {
        LOG_E("cmux object (%s) detach failed.", CMUX_AT_NAME);
        goto end;
    }
#endif
#ifdef CMUX_PPP_NAME
    result = cmux_detach(sample, CMUX_PPP_NAME);
    if(result != RT_EOK)
    {
        LOG_E("cmux object (%s) detach failed.", CMUX_PPP_NAME);
        goto end;
    }
#endif
    result = cmux_stop(sample);
    if(result != RT_EOK)
    {
        LOG_E("cmux sample stop error.");
        goto end;
    }

end:
    return RT_EOK;
}
MSH_CMD_EXPORT_ALIAS(cmux_sample_stop, cmux_stop, a sample of cmux function);
