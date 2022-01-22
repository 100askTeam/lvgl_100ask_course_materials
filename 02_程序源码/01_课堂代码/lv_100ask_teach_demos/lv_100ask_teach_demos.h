/**
 * @file lv_100ask_teach_demos.h
 *
 */

#ifndef LV_100ASK_TECH_DEMOS_H
#define LV_100ASK_TECH_DEMOS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

#if defined(LV_100ASK_DEMO_CONF_PATH)
#define __LV_TO_STR_AUX(x) #x
#define __LV_TO_STR(x) __LV_TO_STR_AUX(x)
#include __LV_TO_STR(LV_100ASK_DEMO_CONF_PATH)
#undef __LV_TO_STR_AUX
#undef __LV_TO_STR
#elif defined(LV_100ASK_DEMO_CONF_INCLUDE_SIMPLE)
#include "lv_100ask_teach_demos_conf.h"
#else
#include "../lv_100ask_teach_demos_conf.h"
#endif

#include "src/lv_100ask_demo_course_2_1_1/lv_100ask_demo_course_2_1_1.h"
#include "src/lv_100ask_demo_course_2_2_2/lv_100ask_demo_course_2_2_2.h"
#include "src/lv_100ask_demo_course_2_2_3/lv_100ask_demo_course_2_2_3.h"
#include "src/lv_100ask_demo_course_2_2_4/lv_100ask_demo_course_2_2_4.h"
#include "src/lv_100ask_demo_course_2_2_5/lv_100ask_demo_course_2_2_5.h"
#include "src/lv_100ask_demo_course_2_2_6/lv_100ask_demo_course_2_2_6.h"

#include "src/lv_100ask_demo_course_3_1_1/lv_100ask_demo_course_3_1_1.h"
#include "src/lv_100ask_demo_course_3_1_2/lv_100ask_demo_course_3_1_2.h"


/*********************
 *      DEFINES
 *********************/
/*Test  lvgl version*/
#if LV_VERSION_CHECK(8, 1, 0) == 0
#error "lv_100ask_teach_demos: Wrong lvgl version"
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_100ASK_TECH_DEMOS_H*/
