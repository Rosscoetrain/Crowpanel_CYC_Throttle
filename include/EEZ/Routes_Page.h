/*

 *   By Ross Scanlon iam@rosscoe.com
 *   RosscoeTrain
 *   (c) 2025

*/


#ifndef ROUTES_PAGE_H
#define ROUTES_PAGE_H

void action_routes_button(lv_event_t * e)
 {
  void *user_data = lv_event_get_user_data(e);
  int pressedButton = *((int*)(&user_data));
  Serial.print("Pressed button :");
  Serial.println(pressedButton);
  switch(pressedButton)
  {
    case 30:       //Return to Calling Page
      loadScreen(SCREEN_ID_MAIN);
      break;
    case 31:       //Load routes from exrail
      if (WiFi.status() != WL_CONNECTED)
       {
        lv_label_set_text(objects.lbl_routes, "#FF0000 Not connected to CS");
        return;
       }
      else
       {
        lv_label_set_text(objects.lbl_routes, "");
       }
      Serial.println("Loading routes");
      dccexProtocol.getLists(false,false,true,false);
      break;

    case 29:       //Select route uncheck
      routeMode = NORMAL_MODE;
      break;
    case 32:       //Select route checked
//      Serial.println("Selecting route");
      routeMode = SELECT_MODE;
     break;
  }
 }

static void tbl_route_cb(lv_event_t * e)
 {
  lv_obj_t * obj = lv_event_get_target(e);
  lv_label_set_text(objects.lbl_routes, "");
  uint16_t col;
  uint16_t row;
  lv_table_get_selected_cell(obj, &row, &col);
  Serial.printf("Selected Route Row = %d, Col = %d\n", row, col);

  if(routeMode == SELECT_MODE)
   {
    routeMode = NORMAL_MODE;
    lv_obj_clear_state(objects.btn_select_routes_1, LV_STATE_CHECKED);
    Serial.print("Starting route :");
    const char *id_text = lv_table_get_cell_value(objects.tbl_routes, row, 1);
    const char *type_text = lv_table_get_cell_value(objects.tbl_routes, row, 2);
    Serial.print("Location Route id :");
    Serial.println(id_text);
    Serial.print("Location Route type :");
    Serial.println(type_text);
// send </START id.

    if (strcmp(type_text, "A") == 0)
     {
//      Serial.print("</START ");
//      Serial.print(id_text);
//      Serial.println(">");
      String cmd = ("</START " + String(id_text) + ">");
      Serial.println(cmd);
      client.print(cmd);
     }
    else
     {
      if (activeLocoID != 255)
       {
//        Serial.print("</START ");
//        Serial.print(id_text);
//        Serial.print(" ");
//        Serial.print(locoAddress[activeLocoID]);
//        Serial.println(">");
        String cmd = ("</START " + String(locoAddress[activeLocoID]) + " " + String(id_text) + ">");
        Serial.println(cmd);
        client.print(cmd);
       }
      else
       {
        lv_label_set_text(objects.lbl_routes, "#FF0000 No active locomotive");
       }
     }
   }
 }



void setupExrailRoutes()
 {
  Serial.println("Getting Exrail Routes");

  uint16_t i = 0;
  uint16_t loc_count = 0;
  uint16_t id = 0;
  const char *location = "";
  RouteType type = RouteTypeRoute;
  bool alreadyExists = false;
  uint8_t elementCount = 0;
  uint8_t routeCount = 0;
  char buffer[7];
  char type_buf[2];

  const char* colonPos;

// clear all arrays
  for (int j = 0; j < MAX_ROUTE_LOCATION; j++)
   {
    locationArray[j] = "";
   }
  for (int j = 0; j < MAX_ROUTES; j++)
   {
    strncpy(Routes[j].loc, "", sizeof(Routes[j].loc) - 1);
    strncpy(Routes[j].name, "", sizeof(Routes[j].name) - 1);
    Routes[j].id = 0;
    Routes[j].type = RouteTypeRoute;
   }

  for (Route *route = dccexProtocol.routes->getFirst(); route; route = route->getNext()) {
    id = route->getId();
    snprintf(buffer, sizeof(buffer), "%u", id);
    location = route->getName();
    type = route->getType();
    snprintf(type_buf, sizeof(type_buf), "%c", type);
    alreadyExists = false;

    colonPos = strchr(location, ':');

    if (colonPos != NULL)
     {
// Calculate lengths
      size_t locLength = colonPos - location;
      size_t nameLength = strlen(colonPos + 1);

// Allocate stack buffers (+1 for the null terminator)
      char loc[locLength + 1];
      char name[nameLength + 1];

// Copy the key segment and manually null-terminate
      strncpy(loc, location, locLength);
      loc[locLength] = '\0'; 

// Copy the value segment (standard strcpy is safe here)
      strcpy(name, colonPos + 1);

// Print output variables
//      Serial.printf("loc:   %s|\n", loc);
//      Serial.printf("name: %s\n", name);

// Ensure the search term itself is valid
      if (loc != NULL)
       {
        for (int j = 0; j < MAX_ROUTE_LOCATION; j++)
         {
// Ensure the array element exists before comparing
          if (locationArray[j] != NULL)
           {
            if (locationArray[j] == loc)
             {
              alreadyExists = true;
              break;
             }
           }
         }
       }

      // Add to array only if it is unique and space remains
      if (!alreadyExists && elementCount < MAX_ROUTE_LOCATION)
       {
        locationArray[elementCount] = loc;
//        Serial.print("location added : ");
//        Serial.println(locationArray[elementCount]);
        elementCount++;
        lv_dropdown_add_option(objects.dd_location, loc, LV_DROPDOWN_POS_LAST);
       }

      lv_table_set_cell_value(objects.tbl_routes, i, 0, name);
      lv_table_set_cell_value(objects.tbl_routes, i, 1, buffer);
      lv_table_set_cell_value(objects.tbl_routes, i, 2, type_buf);

      if (routeCount < MAX_ROUTES)
       {
        strncpy(Routes[routeCount].loc, loc, sizeof(Routes[routeCount].loc) - 1);
        strncpy(Routes[routeCount].name, name, sizeof(Routes[routeCount].name) - 1);
        Routes[routeCount].id = id;
        Routes[routeCount].type = type;
       }

      routeCount++;
      alreadyExists = false;
     }
    else
     {

      if (routeCount < MAX_ROUTES)
       {
        strncpy(Routes[routeCount].loc, "", sizeof(Routes[routeCount].loc) - 1);
        strncpy(Routes[routeCount].name, location, sizeof(Routes[routeCount].name) - 1);
        Routes[routeCount].id = id;
        Routes[routeCount].type = type;
       }

      routeCount++;
      lv_table_set_cell_value(objects.tbl_routes, i, 0, location);
      lv_table_set_cell_value(objects.tbl_routes, i, 1, buffer);
      lv_table_set_cell_value(objects.tbl_routes, i, 2, type_buf);
     }

    i++;
   }

  Serial.println("\n");

 }



void updateRoutesTable(char * buf)
 {
  Serial.print("Changing route table to location: ");
  Serial.println(buf);
  lv_table_set_row_cnt(objects.tbl_routes, 0);

  char buffer[7];
  char type_buf[2];

  uint8_t i = 0;     // row counter

  for(int j = 0; j < MAX_ROUTES; j++)
   {
//    Serial.print("Routes.loc : ");
//    Serial.println(Routes[j].loc);

    if ((strcmp(Routes[j].loc, buf) == 0) || (strcmp(buf, "All") == 0))
     {
      if (strcmp(Routes[j].name, "") != 0)
       {
        Serial.print("Routes.name : ");
        Serial.println(Routes[j].name);

        snprintf(buffer, sizeof(buffer), "%u", Routes[j].id);
        snprintf(type_buf, sizeof(type_buf), "%c", Routes[j].type);

        lv_table_set_cell_value(objects.tbl_routes, i, 0, Routes[j].name);
        lv_table_set_cell_value(objects.tbl_routes, i, 1, buffer);
        lv_table_set_cell_value(objects.tbl_routes, i, 2, type_buf);

        i++;

       }
     }
   }
 }


void dd_location_cb(lv_event_t * e)
 {
  lv_event_code_t code = lv_event_get_code(e);
//  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_VALUE_CHANGED) 
   {
    Serial.println("Changed location");

    // Get the dropdown object from the event
    lv_obj_t * dropdown = lv_event_get_target(e);
        
    // Create a buffer to store the selected item's text
    char buf[20];
        
    // Fetch the selected string and copy it into the buffer
    lv_dropdown_get_selected_str(dropdown, buf, sizeof(buf));
        
    // Print the result to the Serial Monitor
    Serial.printf("Selected item: %s\n", buf);

    updateRoutesTable(buf);
    strncpy(currentLocation, buf, sizeof(currentLocation));

    // Optional: Get the numerical index (0 for first item, 1 for second, etc.)
//    uint16_t selected_index = lv_dropdown_get_selected(dropdown);
//    Serial.printf("Selected index: %d\n", selected_index);
   }
 }




#endif
