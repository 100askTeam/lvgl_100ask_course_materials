/**
 ******************************************************************************
 * @file    lv_100ask_demo_course_2_2_4.c
 * @author  百问科技
 * @version V1.0
 * @date    2021-12-29
 * @brief	2_2_4课的课堂代码
 ******************************************************************************
 * Change Logs:
 * Date           Author          Notes
 * 2021-12-29     zhouyuebiao     First version
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

#if 1 // LV_USE_100ASK_DEMO_COURSE_2_2_4

#include <stdio.h>
#include "lv_100ask_demo_course_2_2_4.h"


/*********************
 *      DEFINES
 *********************/


/**********************
 *  STATIC VARIABLES
 **********************/


void lv_100ask_demo_course_2_2_4(void)
{
    lv_obj_t * obj1 = lv_obj_create(lv_scr_act());
    lv_obj_align(obj1, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_set_style_border_width(obj1, 10, 0);
    //lv_obj_set_style_outline_width(obj1, 10, 0);

#if 0
    lv_obj_t * obj2 = lv_obj_create(lv_scr_act());
    lv_obj_t * obj_out_top = lv_obj_create(lv_scr_act());
    lv_obj_t * obj_out_bottom = lv_obj_create(lv_scr_act());
    lv_obj_t * obj_out_left = lv_obj_create(lv_scr_act());
    lv_obj_t * obj_out_right = lv_obj_create(lv_scr_act());
    //lv_obj_set_style_outline_width(obj_out_left, 10, 0);

    lv_obj_align(obj1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align_to(obj_out_top, obj1, LV_ALIGN_OUT_TOP_MID, 0, 0);
    lv_obj_align_to(obj_out_bottom, obj1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_align_to(obj_out_left, obj1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_obj_align_to(obj_out_right, obj1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_align_to(obj2, obj_out_right, LV_ALIGN_OUT_TOP_MID, 0, 0);
#endif
}


#endif /* LV_USE_100ASK_DEMO_COURSE_2_2_4 */
