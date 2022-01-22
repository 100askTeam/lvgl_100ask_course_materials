/**
 ******************************************************************************
 * @file    lv_100ask_demo_course_3_1_1.c
 * @author  百问科技
 * @version V1.0
 * @date    2022-01-12
 * @brief	3_1_1 课的课堂代码
 ******************************************************************************
 * Change Logs:
 * Date           Author          Notes
 * 2022-01-12     zhouyuebiao     First version
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

#if 1 // LV_USE_100ASK_DEMO_COURSE_3_1_1

#include <stdio.h>
#include "lv_100ask_demo_course_3_1_1.h"


/*********************
 *      DEFINES
 *********************/


/**********************
 *  STATIC VARIABLES
 **********************/
static void label_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);        // 获取触发事件的部件(对象)
    lv_event_code_t code = lv_event_get_code(e);    // 获取当前部件(对象)触发的事件代码

    switch(code){
        case LV_EVENT_PRESSED:
            //lv_label_set_text(label, "LV_EVENT_PRESSED");
            //lv_obj_set_style_bg_color(obj, lv_color_hex(0xc43e1c), 0);  // 通过本地样式(私有样式)设置背景色
            printf("LV_EVENT_PRESSED\n");
            break;
        case LV_EVENT_LONG_PRESSED:
            //lv_label_set_text(label, "LV_EVENT_LONG_PRESSED");
            //lv_obj_set_style_bg_color(obj, lv_color_hex(0x4cbe37), 0);  // 通过本地样式(私有样式)设置背景色
            printf("LV_EVENT_LONG_PRESSED\n");
            break;
        default:
            //printf("NONE\n");
            break;
    }
}

void lv_100ask_demo_course_3_1_1(void)
{
    char * text = "www.100ask.net";   // 要显示的文字

    /* 创建一个基础对象 label */
    lv_obj_t * label = lv_label_create(lv_scr_act());   // 创建一个label部件(对象),他的父对象是活动屏幕对象


    /* 展示文字 */
    /* 设置要显示的文本，函数中会另外开辟和给定的字符串大小的空间存放字符串（常用） */
    //lv_label_set_text(label, text);
    //lv_label_set_text(label, "www.100ask.net"); // 也可以这样使用

    /* 设置要显示的文字，也会另外开辟空间存放字符串，但是可以像 printf 格式化字符串 */
    //lv_label_set_text_fmt(label, "%s: %d", "Value", 15);

    /* 设置要显示的文字，直接使用给定的缓冲区(不常用) */
    //lv_label_set_text_static(label, text);
    //lv_label_set_text_static(label, "www.100ask.net"); // 也可以这样使用


    /* 设置文字字号(内置ASCII字库) */
    /* 使用其他字号的字体，如果不设置默认使用 lv_font_montserrat_14 ，在 lv_conf.h 中 LV_FONT_DEFAULT 定义 */
    //lv_obj_set_style_text_font(label, &lv_font_montserrat_28, 0);  // 为了方便，这里使用本地(私有)样式


    /* 让 label 可以响应输入事件 */
    //lv_obj_add_flag(label, LV_OBJ_FLAG_CLICKABLE);
    //lv_obj_add_event_cb(label, label_event_cb, LV_EVENT_ALL, 0);
}


#endif /* LV_USE_100ASK_DEMO_COURSE_3_1_1 */
