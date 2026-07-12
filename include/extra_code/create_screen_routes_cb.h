//before //create_screen_routes at about line 4766

static void screen_routes_event_cb(lv_event_t * e)
 {
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_SCREEN_LOADED)
   {
    lv_label_set_text(objects.lbl_routes, ""); // Clears the label's text
   }
 }

