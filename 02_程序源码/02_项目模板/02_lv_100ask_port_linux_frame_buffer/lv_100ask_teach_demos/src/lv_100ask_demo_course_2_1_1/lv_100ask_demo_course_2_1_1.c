/**
 ******************************************************************************
 * @file    lv_100ask_demo_course_2_1_1.c
 * @author  百问科技
 * @version V1.0
 * @date    2021-12-02
 * @brief	2_1_1课的课堂代码
 ******************************************************************************
 * Change Logs:
 * Date           Author          Notes
 * 2021-12-02     zhouyuebiao     First version
 ******************************************************************************
 * @attention
 *
 * Copyright (C) 2008-2021 深圳百问网科技有限公司<https://www.100ask.net/>
 * All rights reserved
 *
 ******************************************************************************
 */


/*********************
 *      INCLUDES
 *********************/
#include "../../lv_100ask_teach_demos.h"

#if LV_USE_100ASK_DEMO_COURSE_2_1_1

#include <stdio.h>
#include "lv_100ask_demo_course_2_1_1.h"


/*********************
 *      DEFINES
 *********************/


/**********************
 *  STATIC VARIABLES
 **********************/


void lv_100ask_demo_course_2_1_1(void)
{
    lv_obj_t * obj = lv_obj_create(lv_scr_act());
    lv_obj_set_size(obj, LV_PCT(20), LV_PCT(10));
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * label = lv_label_create(obj);
    lv_label_set_text(label, "Hello, LVGL!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}


#endif /* LV_USE_100ASK_DEMO_COURSE_2_1_1 */
