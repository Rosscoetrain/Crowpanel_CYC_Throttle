/*
 *
 * @files actions_ui_functions.h
 * 
 * (c)  By Ross Scanlon iam@rosscoe.com
 *
 */

/*
 * add any screeen updates here
 *
 * screens are
 * 
 * 0 main
 * 1 throttle
 * 2 roster
 * 3
 * 4
 * 5
 * 6
 * 7
 * 8
 * 9 config
 * 10
 * 11
 * 12
 * 13
 * 14 routes
 *
 * in EEZ studion add an event for the screen on SCREEN_LOADED.
 *
 */


#ifdef RES4827
#include "EEZ/4827/actions.h"
#endif

#ifdef RES8048
#include "EEZ/8048/actions.h"
#endif

#ifdef RES8048_LS
#include "EEZ/8048_landscape/actions.h"
#endif


void action_screen_load_cb(lv_event_t *e)
 {
  lv_event_code_t code = lv_event_get_code(e);
  void *user_data = lv_event_get_user_data(e);
  int screen = *((int*)(&user_data));

  Serial.print("Screen : ");
  Serial.println(screen);

  if(code == LV_EVENT_SCREEN_LOADED)
   {
    switch (screen)
     {
      case 0:                                               // spare
        break;
      case 1:                                               // 
        break;
      case 2:                                               //
        break;
      case 3:                                               // roster screen 
        lv_table_set_col_width(objects.tbl_roster, 0, 200); // Column 0 width = 200 px
        lv_table_set_col_width(objects.tbl_roster, 1, 58); // Column 1 width = 58 px
        break;
      case 4:                                               // 
        break;
      case 5:                                               // program screen
        lv_obj_add_flag(objects.kbd_program, LV_OBJ_FLAG_HIDDEN);
        break;
      case 6:                                               // 
        break;
      case 7:                                               // 
        break;
      case 8:                                                // wifi
        lv_label_set_recolor(objects.lbl_wifi_status, true); // allows colouring of text
        break;
      case 9:                                               // 
        break;
      case 10:                                              // config
        lv_label_set_text(objects.lbl_config, "");          // Clears the label's text
        lv_label_set_recolor(objects.lbl_config, true);     // allows colouring of text
        break;
      case 11:                                              // 
        break;
      case 12:                                              // 
        break;
      case 13:                                              // 
        break;
      case 14:                                              // routes screen
        lv_label_set_text(objects.lbl_routes, "");          // Clears the label's text
        lv_label_set_recolor(objects.lbl_routes, true);     // allows colouring of text
#ifdef RES4827
        lv_table_set_col_width(objects.tbl_routes, 0, 170); // Column 0 width = 170 px
        lv_table_set_col_width(objects.tbl_routes, 1, 58);  // Column 1 width = 58 px
        lv_table_set_col_width(objects.tbl_routes, 2, 30);  // Column 2 width = 30 px
#endif
#ifdef RES8048
        lv_table_set_col_width(objects.tbl_routes, 0, 350); // Column 0 width = 170 px
        lv_table_set_col_width(objects.tbl_routes, 1, 59);  // Column 1 width = 58 px
        lv_table_set_col_width(objects.tbl_routes, 2, 50);  // Column 2 width = 30 px
#endif
#ifdef RES8048_LS                                           // total width 624 px
        lv_table_set_col_width(objects.tbl_routes, 0, 490); // Column 0 width = 490 px
        lv_table_set_col_width(objects.tbl_routes, 1, 64);  // Column 1 width = 64 px
        lv_table_set_col_width(objects.tbl_routes, 2, 70);  // Column 2 width = 70 px
#endif
        break;
      case 15:                                              // points screen
        lv_label_set_text(objects.lbl_points, "");          // Clears the label's text
        lv_label_set_recolor(objects.lbl_points, true);     // allows colouring of text
#ifdef RES4827
        lv_table_set_col_width(objects.tbl_points, 0, 138); // Column 0 width = 170 px
        lv_table_set_col_width(objects.tbl_points, 1, 50);  // Column 1 width = 28 px
        lv_table_set_col_width(objects.tbl_points, 2, 70);  // Column 2 width = 60 px
#endif
#ifdef RES8048                                              // total width 
        lv_table_set_col_width(objects.tbl_points, 0, 330); // Column 0 width = 330 px
        lv_table_set_col_width(objects.tbl_points, 1, 59);  // Column 1 width = 59 px
        lv_table_set_col_width(objects.tbl_points, 2, 70);  // Column 2 width = 70 px
#endif
#ifdef RES8048_LS                                           // total width 624 px
        lv_table_set_col_width(objects.tbl_points, 0, 490); // Column 0 width = 490 px
        lv_table_set_col_width(objects.tbl_points, 1, 64);  // Column 1 width = 64 px
        lv_table_set_col_width(objects.tbl_points, 2, 70);  // Column 2 width = 70 px
#endif
        break;
      default:
        break;
     }
   }
 }

