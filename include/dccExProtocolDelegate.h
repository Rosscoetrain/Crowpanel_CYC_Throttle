/*
 *
 * @files defines.h
 * 
 *   By Ross Scanlon iam@rosscoe.com
 *
 */

#ifndef DCCEXPROTOCOLDELEGATE_H
#define DCCEXPROTOCOLDELEGATE_H

extern void setupExrailRoster();
extern void setupExrailRoutes();

// Delegate class
class MyDelegate : public DCCEXProtocolDelegate {

public:
  void receivedServerVersion(int major, int minor, int patch) override {
    Serial.print("\n\nReceived version: ");
    Serial.print(major);
    Serial.print(".");
    Serial.print(minor);
    Serial.print(".");
    Serial.println(patch);
  }

  void receivedTrackPower(TrackPower state) override {
    Serial.print("\n\nReceived Track Power: ");
    Serial.println(state);
    Serial.println("\n\n");
  }

  void receivedRosterList() override {
    Serial.println("\n\nReceived Roster");
    setupExrailRoster();
  }

  void receivedTurnoutList() override {
    Serial.print("\n\nReceived Turnouts/Points list");
    Serial.println("\n\n");
  }

  void receivedRouteList() override {
    Serial.println("\n\nReceived Routes List");
    setupExrailRoutes();
  }

  void receivedTurntableList() override {
    Serial.print("\n\nReceived Turntables list");
    Serial.println("\n\n");
  }
};


#endif
