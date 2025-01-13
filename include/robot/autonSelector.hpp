#ifndef AUTONSELECTOR_HPP
#define AUTONSELECTOR_HPP

#include "pros/apix.h" // Includes the feature set of LVGL

// Declare screens (pages)
extern lv_obj_t *scr_match, *scr_skills, *scr_debug;
extern lv_obj_t *btn_match_autons, *btn_skills_autons, *btn_debug_info;
extern lv_obj_t *label_match, *label_skills, *label_debug;

// Functions to create the pages and set up navigation
void create_auton_selector();
void create_match_autons_screen();
void create_skills_autons_screen();
void create_debug_screen();
void update_debug_info();

#endif // AUTONSELECTOR_HPP
