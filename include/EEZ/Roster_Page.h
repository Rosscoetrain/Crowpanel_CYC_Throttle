/*
 *****************************************************************************************************************************
 *  COPYRIGHT (c) 2025 Norman Halland (NormHal@gmail.com) unless otherwise noted.
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
/*

 *   By Ross Scanlon iam@rosscoe.com
 *   RosscoeTrain
 *   (c) 2025

*/

#ifndef ROSTER_PAGE_H
#define ROSTER_PAGE_H


static void tbl_roster_cb(lv_event_t * e)
{
  lv_obj_t * obj = lv_event_get_target(e);
  uint16_t col;
  uint16_t row;
  lv_table_get_selected_cell(obj, &row, &col);
//  Serial.printf("Selected Roster Row = %d, Col = %d\n", row, col);
  if(rosterMode == EDIT_MODE)
  {
    editingID = row;
    lv_textarea_set_text(objects.ta_name, locoName[editingID]);
    lv_textarea_set_text(objects.ta_address, locoAddress[editingID]);

    //Now set the functions
    setupFuncEditSlots();
    rosterMode = NORMAL_MODE;
    callingPage = SCREEN_ID_ROSTER;
    lv_obj_clear_state(objects.btn_edit_loco, LV_STATE_CHECKED);
    loadScreen(SCREEN_ID_EDIT_LOCO);
  }
  if(rosterMode == SELECT_MODE)
  {
    selectedIDs[activeThrottle][activeSlot[activeThrottle]] = row;
    populateThrottle();
    rosterMode = NORMAL_MODE;
    lv_obj_clear_state(objects.btn_select_loco, LV_STATE_CHECKED);
    loadScreen(SCREEN_ID_THROTTLE);
  }
}

void action_roster_button(lv_event_t * e)
{
  void *user_data = lv_event_get_user_data(e);
  int pressedButton = *((int*)(&user_data));
  switch(pressedButton)
  {
    case 28:
      rosterMode = NORMAL_MODE;
      break;
    case 29:
      rosterMode = NORMAL_MODE;
      break;
    case 30:       //Return to Calling Page
      loadScreen(SCREEN_ID_MAIN);
      break;
    case 31:       //Edit Loco
      callingPage = SCREEN_ID_ROSTER;
      rosterMode = EDIT_MODE;
      lv_obj_clear_state(objects.btn_select_loco, LV_STATE_CHECKED);
//      loadScreen(SCREEN_ID_EDIT_LOCO);
      break;
    case 32:       //Select Loco
      rosterMode = SELECT_MODE;
      lv_obj_clear_state(objects.btn_edit_loco, LV_STATE_CHECKED);
     break;
    default:
      break;
  }
}


/*
 **********************************************************************************************************
 * Initialize Local Roster
 **********************************************************************************************************
*/
void setupLocalRoster()
{
  Serial.println("Now populating Locos...");
  populateLocoArray("/locos.txt");

  // Set all function numbers to 255
  for(int i = 0; i < NUM_LOCOS; i++)
  {
    for(int j = 0; j < NUM_FUNC_SLOTS; j++) strcpy(funcNumber[i][j], "255");
  }

  Serial.println("Now populating Functions");
  populateLocoFunctions("/functions.txt");

  Serial.println("Now Populating the Roster...");
  for(int i = 0; i < locoCount; i++)                    //Using the count from LittleFS
  {
    lv_table_set_cell_value(objects.tbl_roster, i, 0, locoName[i]);
    lv_table_set_cell_value(objects.tbl_roster, i, 1, locoAddress[i]);
  }
  Serial.println("Local Roster Populated");
}
/*
 **********************************************************************************************************
 * Initialize EX-Rail Roster
 **********************************************************************************************************
*/
void setupExrailRoster()
{
//  dccexProtocol.getLists(true,false,false,false);
  delay(1000);
  Serial.println("Replacing Roster with EX-Rail list");
  uint16_t id = 0;
  uint8_t slot = 0;
  std::string lAddr;
  std::string funcNum;
//  delay(500);

  for (Loco *loco = dccexProtocol.roster->getFirst(); loco; loco = loco->getNext())
  {
    int ad = loco->getAddress();
    const char *name = loco->getName();
    Serial.println();
    Serial.printf("Loco ID: %d Address: ", id);
    Serial.print(ad);
    Serial.print(" Name: ");
    Serial.print(name);
    Serial.println();


    lv_table_set_cell_value(objects.tbl_roster, id, 0, name);
    lAddr = std::to_string(ad);
    lv_table_set_cell_value(objects.tbl_roster, id, 1, lAddr.c_str());

    strcpy(locoName[id], name);
    strcpy(locoAddress[id], lAddr.c_str());
    locoSpeed[id] = 0;
    locoDir[id] = 1;       //Default to Forward

    slot = 0;

    for (int i = 0; i < 32; i++)
    {
      const char *fName = loco->getFunctionName(i);
      funcNum = std::to_string(i);
      if ((fName != nullptr) && (fName[0] != '\0'))
      {
        Serial.printf("    Function Number: %d |", i);
        Serial.print(fName);
        Serial.print("|");
        if (loco->isFunctionMomentary(i)) {
          Serial.print(" - Momentary");
        }
        strcpy(funcNumber[id][slot], funcNum.c_str());
        strcpy(funcName[id][slot], fName);
        funcOption[id][slot] = loco->isFunctionMomentary(i);                                             //0= Function | Momentary, 1=image
        Serial.printf(" slot; %d", slot);

        slot++;
        Serial.println();
      }
    }

    id++;
  }

  rosterMode = NORMAL_MODE;

}


#endif
