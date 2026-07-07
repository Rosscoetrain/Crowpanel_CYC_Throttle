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

#ifndef CONFIG_PAGE_H
#define CONFIG_PAGE_H

#include "functions.h"

void action_config_button(lv_event_t * e)
{
  void *user_data = lv_event_get_user_data(e);
  int pressedButton = *((int*)(&user_data));
  switch(pressedButton)
  {

//#ifndef ESP32DIS06043H
    case (27):              //Retreive EX-Rail Roster 
      if (WiFi.status() != WL_CONNECTED)
       {
        lv_label_set_text(objects.lbl_roster, "Not connected to CS");
        return;
       }
      //First Clear the Roster
      for(int i = 0; i < NUM_LOCOS; i++)
      {
        lv_table_set_cell_value(objects.tbl_roster, i, 0, "");
        lv_table_set_cell_value(objects.tbl_roster, i, 1, "");
        locoName[i][0] = '\0';
        locoAddress[i][0] = '\0';
        //Now clear the Functions
        for(uint8_t f = 0; f < NUM_FUNC_SLOTS; f++) 
        {
          strcpy(funcNumber[i][f], "255");
          strcpy(funcName[i][f], " ");
        }
      }
      lv_label_set_text(objects.lbl_roster,"Sending List Request");
//      dccexProtocol.getLists(true,false,false,false);
//      delay(1000);
      setupExrailRoster();
      lv_label_set_text(objects.lbl_roster,"Roster updated");
      break;

    case (28):              //Reload Local Roster
      lv_label_set_text(objects.lbl_roster, "Restoring Local Roster");
      setupLocalRoster();
      lv_label_set_text(objects.lbl_roster, "Local Roster Restored");
      break;

    case (29):              //Save EX-Rail Roster to local
      if (WiFi.status() != WL_CONNECTED)
       {
        lv_label_set_text(objects.lbl_roster, "Not connected to CS");
        return;
       }
      locosDirty = 1;
      functionsDirty = 1;
      saveLittleFS();
      break;

    case (30):       //WiFi
    {
      enum ScreensEnum callingPage = SCREEN_ID_CONFIG;
      loadScreen(SCREEN_ID_WI_FI);
      break;
    }
    case (31):       //Update
    {
      brightness = atoi(lv_textarea_get_text(objects.ta_tft_backlight));
      setBacklight(brightness);
      timeout = atoi(lv_textarea_get_text(objects.ta_wifi_timeout));
      break;
    }
    case (32):       //Done
    {
      loadScreen(SCREEN_ID_MAIN);
      break;
    }
    default:
      break;
  }
}

#endif
