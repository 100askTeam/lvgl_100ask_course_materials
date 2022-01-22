/**
 ******************************************************************************
 * @file    lv_100ask_demo_course_2_2_3.c
 * @author  百问科技
 * @version V1.0
 * @date    2021-12-23
 * @brief	2_2_3课的课堂代码
 ******************************************************************************
 * Change Logs:
 * Date           Author          Notes
 * 2021-12-23     zhouyuebiao     First version
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

#if 1 // LV_USE_100ASK_DEMO_COURSE_2_2_3

#include <stdio.h>
#include "lv_100ask_demo_course_2_2_3.h"


/*********************
 *      DEFINES
 *********************/


/**********************
 *  STATIC VARIABLES
 **********************/


void lv_100ask_demo_course_2_2_3(void)
{
    lv_obj_t * obj = lv_obj_create(lv_scr_act());
    //lv_obj_set_x(obj, 200);
    //lv_obj_set_y(obj, -50);
    lv_obj_set_pos(obj, 1300, 600);
    //lv_obj_set_align(obj, LV_ALIGN_CENTER);
    //lv_obj_align(obj, LV_ALIGN_CENTER, 100, 100);

    //lv_obj_t * label = lv_label_create(lv_scr_act());
    //lv_label_set_text(label, "Hello, LVGL!");
    //lv_obj_align_to(label, obj, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    //printf("w:%d\n", lv_obj_get_x(obj));
    //printf("h:%d\n", lv_obj_get_y(obj));
}


#endif /* LV_USE_100ASK_DEMO_COURSE_2_2_3 */
