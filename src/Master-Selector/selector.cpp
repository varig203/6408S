#include "Master-Selector/selector.hpp"
#include "liblvgl/extra/themes/default/lv_theme_default.h"
#include "liblvgl/misc/lv_area.h"

// Global variables
std::vector<ms::Category> auton_categories = {};
std::shared_ptr<ms::Auton> selected_auton(nullptr);
lv_obj_t* tabview = nullptr; // Tab view object
std::vector<lv_obj_t*> btnms = {}; // Button matrices
std::map<lv_obj_t*, ms::Category*> btnm_to_category;

// Set autons
void ms::set_autons(const std::vector<ms::Category>& categories) {
    auton_categories = categories;
    if (!auton_categories.empty() && !auton_categories[0].autons.empty()) {
        auto selected_auton = std::make_shared<ms::Auton>(auton_categories[0].autons[0]);
    }
}

// Call selected auton
void ms::call_selected_auton() {
    if (selected_auton) {
        selected_auton->callback();
    }
}

// Button action callback
void button_action(lv_event_t* e) {
    lv_obj_t* btnm = lv_event_get_target(e); // Get the button matrix that triggered the event
    const char* txt = lv_btnmatrix_get_btn_text(btnm, lv_btnmatrix_get_selected_btn(btnm)); // Get selected button text
    if (!txt) return;

    auto it = btnm_to_category.find(btnm);
    if (it != btnm_to_category.end()) {
        ms::Category* category = it->second;

        for (ms::Auton& auton : category->autons) {
            if (strcmp(auton.name.c_str(), txt) == 0) {
                //auto selected_auton = std::make_shared(new <ms::Auton>(auton));
                auto selected_auton = std::make_shared<ms::Auton>(auton.name, auton.callback);
                break;
            }
        }
    }
}

// Handle tab changes
void handle_tab_change() {
    int previous_tab = lv_tabview_get_tab_act(tabview);

    while (true) {
        int current_tab = lv_tabview_get_tab_act(tabview);

        if (current_tab != previous_tab) {
            auto selected_auton = std::make_shared<ms::Auton>(auton_categories[current_tab].autons[0]);

            lv_obj_t* btnm = btnms[previous_tab];
            const char** btn_map = lv_btnmatrix_get_map(btnm);

            for (uint16_t i = 0; btn_map[i] != NULL; i++) {
                lv_btnmatrix_clear_btn_ctrl(btnm, i, LV_BTNMATRIX_CTRL_CHECKED);
            }
        }

        previous_tab = current_tab;
        pros::delay(20);
    }
}

void ms::initialize(int autons_per_row) {
    btnm_to_category.clear();

    lv_disp_t* disp = lv_disp_get_default();

    // Define theme colors
    lv_color_t primary_color = lv_color_hex(0xFF00F8); // Tab Colour
    lv_color_t secondary_color = lv_color_hex(0x00BCFF);
    bool dark_theme = true;
    const lv_font_t* font = &lv_font_montserrat_16;

    lv_theme_t* theme = lv_theme_default_init(disp, primary_color, secondary_color, dark_theme, font);
    //lv_disp_set_theme(disp, theme);

    // Create the tab view
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 30);

    for (ms::Category& category : auton_categories) {
        lv_obj_t* category_tab = lv_tabview_add_tab(tabview, category.name.c_str());
        lv_obj_t* category_btnm = lv_btnmatrix_create(category_tab);

        size_t num_autons = category.autons.size();
        size_t auton_buttons_size = num_autons + num_autons / autons_per_row + 1;
        const char** auton_buttons = new const char*[auton_buttons_size];

        size_t i = 0, autonIndex = 0;
        for (; i < auton_buttons_size - 1; i++) {
            if (i != 0 && i % autons_per_row == 0) {
                auton_buttons[i] = "\n"; // Line break
            } else {
                auton_buttons[i] = category.autons[autonIndex++].name.c_str();
            }
        }

        auton_buttons[auton_buttons_size - 1] = NULL; // End of map

        lv_btnmatrix_set_map(category_btnm, auton_buttons);
        lv_obj_add_event_cb(category_btnm, button_action, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_set_size(category_btnm, 450, 50);
        lv_obj_align(category_btnm, LV_ALIGN_CENTER, 0, 0);

        btnms.push_back(category_btnm);
        btnm_to_category[category_btnm] = &category;

        delete[] auton_buttons;
    }

    pros::Task handle_tab_change_task(handle_tab_change);
}
