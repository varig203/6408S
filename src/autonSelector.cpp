/*      Autonomous Selector     */
/*                              */
/* An Autonomous Selector which */
/*  is made in LVGL to make it  */
/*        Pretty looking.       */

#include "robot/autonSelector.hpp"
#include "lvgl.h"
#include "robot/hardware.hpp"
#include <cstddef>

// Declare buttons for navigation
lv_obj_t *btn_match_autons, *btn_skills_autons, *btn_debug_info;
lv_obj_t *label_match, *label_skills, *label_debug;

// Create 3 screens (pages)
lv_obj_t *scr_match, *scr_skills, *scr_debug;

void btn_event_cb(lv_event_t * e) {
    lv_obj_t *btn = lv_event_get_target(e);

    // Switch to Match Autons screen
    if (btn == btn_match_autons) {
        lv_scr_load(scr_match);  // Switch to Match Autons screen
    }
    // Switch to Skills Autons screen
    else if (btn == btn_skills_autons) {
        lv_scr_load(scr_skills);  // Switch to Skills Autons screen
    }
    // Switch to Debug Info screen
    else if (btn == btn_debug_info) {
        lv_scr_load(scr_debug);  // Switch to Debug Info screen
    }
}

// Function to create the buttons on the home screen
void create_buttons() {
    // Create Match Autons button
    btn_match_autons = lv_btn_create(lv_scr_act());
    lv_obj_align(btn_match_autons, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(btn_match_autons, btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_label_create(btn_match_autons);
    lv_label_set_text(lv_obj_get_child(btn_match_autons, NULL), "Match Autons");

    // Create Skills Autons button
    btn_skills_autons = lv_btn_create(lv_scr_act());
    lv_obj_align(btn_skills_autons, LV_ALIGN_TOP_LEFT, 10, 60);
    lv_obj_add_event_cb(btn_skills_autons, btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_label_create(btn_skills_autons);
    lv_label_set_text(lv_obj_get_child(btn_skills_autons, NULL), "Skills Autons");

    // Create Debug Info button
    btn_debug_info = lv_btn_create(lv_scr_act());
    lv_obj_align(btn_debug_info, LV_ALIGN_TOP_LEFT, 10, 110);
    lv_obj_add_event_cb(btn_debug_info, btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_label_create(btn_debug_info);
    lv_label_set_text(lv_obj_get_child(btn_debug_info, NULL), "Debug Info");
}

// Function to create the Match Autons screen
void create_match_autons_screen() {
    scr_match = lv_obj_create(NULL);  // Create a new screen for Match Autons

    // Create a label on the Match Autons screen
    label_match = lv_label_create(scr_match);
    lv_label_set_text(label_match, "Match Autons page");
    lv_obj_align(label_match, LV_ALIGN_CENTER, 0, 0);

    // You can add buttons for individual match autons here, similar to the "Math Autons"
}

// Skills autons
void create_skills_autons_screen() {
    scr_skills = lv_obj_create(NULL); // New screen for Skills

    // label on skills screen
    label_match = lv_label_create(scr_skills);
    lv_label_set_text(label_skills, "Skills Autons page");
    lv_obj_align(label_skills, LV_ALIGN_CENTER,0,0);
}

// debug info
void create_debug_screen() {
    scr_debug = lv_obj_create(NULL);

    // label
    label_match = lv_label_create(scr_debug);
    lv_label_set_text(label_debug, "Debug page");
    lv_obj_align(label_debug, LV_ALIGN_CENTER, 0, 0);
}

// Function to update the Debug Info screen with real-time data
void update_debug_info() {
    char debug_text[128];
    int vEncoder = vertical_encoder.get_position();  // Example: read from a sensor (like an ADI sensor)

    // Create a formatted debug string
    snprintf(debug_text, sizeof(debug_text), "Vertical Encoder: %d", vEncoder);

    // Update the label with new data
    lv_label_set_text(label_debug, debug_text);
}
