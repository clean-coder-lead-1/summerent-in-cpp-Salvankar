#include "typewise-alert.h"
#include <stdio.h>

                          //lower ,  upper
const int limits[3][2] = {{   0   ,  35   },  //PASSIVE_COOLING
                          {   0   ,  45   },  //HI_ACTIVE_COOLING
                          {   0   ,  40   }}; //MED_ACTIVE_COOLING
const char *message[3] = {"Hi, the temperature is too low\n",  //TOO_LOW
                          "Hi, the temperature is too high\n", //TOO_HIGH
                          ""}                                  //NORMAL

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

BreachType classifyTemperatureBreach(
    CoolingType coolingType, double temperatureInC) {
  int lowerLimit = 0;
  int upperLimit = 0;
  lowerLimit = limits[coolingType][0];
  upperLimit = limits[coolingType][1];
  return inferBreach(temperatureInC, lowerLimit, upperLimit);
}

void checkAndAlert(
    AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {

  BreachType breachType = classifyTemperatureBreach(
    batteryChar.coolingType, temperatureInC
  );

  switch(alertTarget) {
    case TO_CONTROLLER:
      sendToController(breachType);
      break;
    case TO_EMAIL:
      sendToEmail(breachType);
      break;
  }
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  printf("To: %s\n", recepient);
  printf("%s", message[breachType]);
}
