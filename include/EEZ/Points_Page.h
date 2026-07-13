/*
 *****************************************************************************************************************************

 *   By Ross Scanlon iam@rosscoe.com
 *   RosscoeTrain
 *   (c) 2025

 *
 *  This program and all its associated modules is free software:
 *  you can redistribute it and/or modify it under the terms of the
 *  GNU General Public License as published by the Free Software
 *  Foundation, either version 3 of the License, or (at your option)
 *  any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses
 *
 *****************************************************************************************************************************
*/

#ifndef POINTS_PAGE_H
#define POINTS_PAGE_H


static void tbl_points_cb(lv_event_t * e)
 {
  lv_obj_t * obj = lv_event_get_target(e);
  uint16_t col;
  uint16_t row;
  lv_table_get_selected_cell(obj, &row, &col);
  Serial.printf("Selected Roster Row = %d, Col = %d\n", row, col);

//  if(pointsMode == SELECT_MODE)
   {
//    pointsMode = NORMAL_MODE;
    lv_obj_clear_state(objects.btn_select_points, LV_STATE_CHECKED);
    Serial.print("Changing point :");
    const char *id_text = lv_table_get_cell_value(objects.tbl_points, row, 1);
    int id = std::stoi(id_text);
    const char *status_text = lv_table_get_cell_value(objects.tbl_points, row, 2);
    Serial.print("Location Point id :");
    Serial.println(id);
    Serial.print("Location Point status :");
    Serial.println(status_text);

    if (strcmp(status_text, "closed") == 0)
     {
      dccexProtocol.throwTurnout(id);
      lv_table_set_cell_value(objects.tbl_points, row, 2, "thrown");
     }
    else
     {
      dccexProtocol.closeTurnout(id);
      lv_table_set_cell_value(objects.tbl_points, row, 2, "closed");
     }
   }
 }

void action_points_button(lv_event_t * e)
{
  void *user_data = lv_event_get_user_data(e);
  int pressedButton = *((int*)(&user_data));
  Serial.print("Pressed button :");
  Serial.println(pressedButton);
  switch(pressedButton)
  {
//    case 28:
//      break;
    case 29:
      pointsMode = NORMAL_MODE;
      break;

    case 30:       //Return to Calling Page
//      loadScreen(SCREEN_ID_MAIN);
      loadScreen(callingPage);
      break;
    case 31:       //Load points from Exrail
      if (WiFi.status() != WL_CONNECTED)
       {
        lv_label_set_text(objects.lbl_points, "#FF0000 Not connected to CS");
        return;
       }
      else
       {
        lv_label_set_text(objects.lbl_points, "");
       }
      Serial.println("Loading points");
      dccexProtocol.getLists(false,true,false,false);
      break;
    case 32:       //Select 
      if (WiFi.status() != WL_CONNECTED)
       {
        lv_label_set_text(objects.lbl_points, "#FF0000 Not connected to CS");
        return;
       }
      else
       {
        lv_label_set_text(objects.lbl_points, "");
       }
      lv_obj_clear_state(objects.btn_select_points, LV_STATE_CHECKED);
      pointsMode = SELECT_MODE;
     break;

    default:
      break;
  }
}


/**********************************************************************************************************
 * Initialize EX-Rail Points list
 **********************************************************************************************************/

void setupExrailPoints()
 {
  Serial.println("Loading command station points");

  uint16_t i = 0;
  uint16_t loc_count = 0;
  uint16_t id = 0;
  const char *location = "";
  bool alreadyExists = false;
  uint8_t elementCount = 0;
  uint8_t pointCount = 0;
  char buffer[7];
//  char type_buf[2];
  bool status;

  const char* colonPos;

// clear all arrays
  for (int j = 0; j < MAX_POINTS_LOCATION; j++)
   {
    pointsLocationArray[j] = "";
   }
  for (int j = 0; j < MAX_POINTS; j++)
   {
    strncpy(Points[j].loc, "", sizeof(Points[j].loc) - 1);
    strncpy(Points[j].name, "", sizeof(Points[j].name) - 1);
    Points[j].id = 0;
    Points[j].status = false;
   }

  for (Turnout *turnout = dccexProtocol.turnouts->getFirst(); turnout; turnout = turnout->getNext()) {
    id = turnout->getId();
    snprintf(buffer, sizeof(buffer), "%u", id);
    location = turnout->getName();
    status = turnout->getThrown();
//    snprintf(type_buf, sizeof(type_buf), "%c", type);
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
        for (int j = 0; j < MAX_POINTS_LOCATION; j++)
         {
// Ensure the array element exists before comparing
          if (pointsLocationArray[j] != NULL)
           {
            if (pointsLocationArray[j] == loc)
             {
              alreadyExists = true;
              break;
             }
           }
         }
       }

      // Add to array only if it is unique and space remains
      if (!alreadyExists && elementCount < MAX_POINTS_LOCATION)
       {
        pointsLocationArray[elementCount] = loc;
//        Serial.print("location added : ");
//        Serial.println(locationArray[elementCount]);
        elementCount++;
        lv_dropdown_add_option(objects.dd_location_points, loc, LV_DROPDOWN_POS_LAST);
       }

      lv_table_set_cell_value(objects.tbl_points, i, 0, name);
      lv_table_set_cell_value(objects.tbl_points, i, 1, buffer);
      lv_table_set_cell_value(objects.tbl_points, i, 2, status?"thrown":"closed");

      if (pointCount < MAX_POINTS)
       {
        strncpy(Points[pointCount].loc, loc, sizeof(Points[pointCount].loc) - 1);
        strncpy(Points[pointCount].name, name, sizeof(Points[pointCount].name) - 1);
        Points[pointCount].id = id;
        Points[pointCount].status = status;
       }

      pointCount++;
      alreadyExists = false;
     }
    else
     {

      if (pointCount < MAX_POINTS)
       {
        strncpy(Points[pointCount].loc, "", sizeof(Points[pointCount].loc) - 1);
        strncpy(Points[pointCount].name, location, sizeof(Points[pointCount].name) - 1);
        Points[pointCount].id = id;
        Points[pointCount].status = status;
       }

      pointCount++;
      lv_table_set_cell_value(objects.tbl_points, i, 0, location);
      lv_table_set_cell_value(objects.tbl_points, i, 1, buffer);
      lv_table_set_cell_value(objects.tbl_points, i, 2, status?"thrown":"closed");
     }

    i++;
   }

  Serial.println("\n");
  pointsMode = NORMAL_MODE;

 }


void updatePointsTable(char * buf)
 {
  Serial.print("Changing points table to location: ");
  Serial.println(buf);
  lv_table_set_row_cnt(objects.tbl_points, 0);

  char buffer[7];
//  char type_buf[2];

  uint8_t i = 0;     // row counter

  for(int j = 0; j < MAX_POINTS; j++)
   {
//    Serial.print("Points.loc : ");
//    Serial.println(Points[j].loc);

    if ((strcmp(Points[j].loc, buf) == 0) || (strcmp(buf, "All") == 0))
     {
      if (strcmp(Points[j].name, "") != 0)
       {
        Serial.print("Points.name : ");
        Serial.println(Points[j].name);

        snprintf(buffer, sizeof(buffer), "%u", Points[j].id);

        lv_table_set_cell_value(objects.tbl_points, i, 0, Points[j].name);
        lv_table_set_cell_value(objects.tbl_points, i, 1, buffer);
        lv_table_set_cell_value(objects.tbl_points, i, 2, Points[j].status?"thrown":"closed");

        i++;

       }
     }
   }
 }


void dd_location_points_cb(lv_event_t * e)
 {
  lv_event_code_t code = lv_event_get_code(e);
//  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_VALUE_CHANGED) 
   {
    Serial.println("Changed points location");

    // Get the dropdown object from the event
    lv_obj_t * dropdown = lv_event_get_target(e);
        
    // Create a buffer to store the selected item's text
    char buf[20];
        
    // Fetch the selected string and copy it into the buffer
    lv_dropdown_get_selected_str(dropdown, buf, sizeof(buf));
        
    // Print the result to the Serial Monitor
    Serial.printf("Selected item: %s\n", buf);

    updatePointsTable(buf);
    strncpy(pointCurrentLocation, buf, sizeof(pointCurrentLocation));

    // Optional: Get the numerical index (0 for first item, 1 for second, etc.)
//    uint16_t selected_index = lv_dropdown_get_selected(dropdown);
//    Serial.printf("Selected index: %d\n", selected_index);
   }
 }


#endif
