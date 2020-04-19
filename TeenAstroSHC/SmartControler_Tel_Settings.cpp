#include "SmartController.h"
#include "SHC_text.h"


void SmartHandController::menuTelSettings()
{
  buttonPad.setMenuMode();
  current_selection_L1 = 1;
  while (!exitMenu)
  {
    const char *string_list_SettingsL1 = T_HANDCONTROLLER "\n" T_TIME " & " T_SITE "\n" T_SETPARK "\n" T_MOUNT "\n" T_LIMITS "\n" T_MAINUNITINFO "\nWifi";
    current_selection_L1 = display->UserInterfaceSelectionList(&buttonPad, T_TELESCOPESETTINGS, current_selection_L1, string_list_SettingsL1);
    switch (current_selection_L1)
    {
    case 0:
      exitMenu = true;
    case 1:
      menuSHCSettings();
      break;
      //case 2:
      //  menuAlignment();
      //  break;
    case 2:
      menuTimeAndSite();
      break;
    case 3:
      DisplayMessageLX200(SetLX200(":hQ#"), false);
      break;
    case 4:
      menuMount();
      break;
    case 5:
      menuLimits();
      break;
    case 6:
      menuMainUnitInfo();
      break;
    case 7:
      menuWifi();
      break;
    default:
      break;
    }
  }
  buttonPad.setControlerMode();
}
void SmartHandController::menuMainUnitInfo()
{
  const char *string_list = T_SHOWVERSION "\n" T_REBOOT "\n" T_RESETTOFACTORY;
  current_selection_L2 = 1;
  while (current_selection_L2 != 0)
  {
    current_selection_L2 = display->UserInterfaceSelectionList(&buttonPad, T_MAINUNITINFO, 1, string_list);
    switch (current_selection_L2)
    {
    case 0:
      return;
    case 1:
      ta_MountStatus.updateV();
      if (ta_MountStatus.hasInfoV())
      {
        DisplayMessage(ta_MountStatus.getVN(), ta_MountStatus.getVD(), -1);
      }
      char out1[20];
      char out2[20];
      if (DisplayMessageLX200(GetLX200(":GVN#", out1, 20)) && DisplayMessageLX200(GetLX200(":GVD#", out2, 20)))
      {

      }
      break;
    case 2:
      DisplayMessageLX200(SetLX200(":$!#"), false);
      delay(500);
      powerCycleRequired = true;
      return;
    case 3:
      if (display->UserInterfaceMessage(&buttonPad, "Reset", "To", "Factory?", "NO\nYES") == 2)
      {
        DisplayMessageLX200(SetLX200(":$$#"), false);
        delay(500);
        powerCycleRequired = true;
        return;
      }
      break;
    default:
      break;
    }
  }
}
//----------------------------------//
//             LIMITS               //
//----------------------------------//
void SmartHandController::menuLimits()
{
  const char *string_list_LimitsL2 = T_HORIZON "\n" T_OVERHEAD "\n" T_MERIDIANE "\n" T_MERIDIANW "\n" T_UNDERPOLE;
  current_selection_L3 = 1;
  while (current_selection_L3 != 0)
  {
    current_selection_L3 = display->UserInterfaceSelectionList(&buttonPad, T_LIMITS, current_selection_L3, string_list_LimitsL2);
    switch (current_selection_L3)
    {
    case 1:
      menuHorizon();
      break;
    case 2:
      menuOverhead();
      break;
    case 3:
      menuMeridian(true);
      break;
    case 4:
      menuMeridian(false);
      break;
    case 5:
      menuUnderPole();
      break;
    default:
      break;
    }
  }
}
void SmartHandController::menuHorizon()
{
  char out[20];
  if (DisplayMessageLX200(GetLX200(":Gh#", out, sizeof(out))))
  {
    float angle = (float)strtol(&out[0], NULL, 10);
    if (display->UserInterfaceInputValueFloat(&buttonPad, T_HORIZONLIMIT, "", &angle, -10, 20, 2, 0, " " T_DEGREE))
    {
      sprintf(out, ":Sh%+03d#", (int)angle);
      DisplayMessageLX200(SetLX200(out), false);
    }
  }
}
void SmartHandController::menuOverhead()
{
  char out[20];
  if (DisplayMessageLX200(GetLX200(":Go#", out, sizeof(out))))
  {
    float angle = (float)strtol(&out[0], NULL, 10);
    if (display->UserInterfaceInputValueFloat(&buttonPad, T_OVERHEADLIMIT, "", &angle, 60, 91, 2, 0, " " T_DEGREE))
    {
      sprintf(out, ":So%02d#", (int)angle);
      DisplayMessageLX200(SetLX200(out), false);
    }
  }
}
void SmartHandController::menuUnderPole()
{
  char out[20];
  char cmd[15];
  if (DisplayMessageLX200(GetLX200(":GXEB#", out, sizeof(out))))
  {
    float angle = (float)strtol(&out[0], NULL, 10) / 10;
    if (display->UserInterfaceInputValueFloat(&buttonPad, T_MAXHOURANGLE, "+-", &angle, 9, 12, 2, 1, " " T_HOURS))
    {
      sprintf(cmd, ":SXEB_%03d#", (int)(angle * 10));
      DisplayMessageLX200(SetLX200(cmd), false);
    }
  }
}
void SmartHandController::menuMeridian(bool east)
{
  char out[20];
  char cmd[15];
  sprintf(cmd, ":GXEX#");
  cmd[4] = east ? '9' : 'A';

  if (DisplayMessageLX200(GetLX200(cmd, out, sizeof(out))))
  {
    float angle = (float)strtol(&out[0], NULL, 10) / 4.0;
    if (display->UserInterfaceInputValueFloat(&buttonPad, east ? T_MERIDIANLIMITE : T_MERIDIANLIMITW, "", &angle, -45, 45, 2, 0, " " T_DEGREE))
    {
      sprintf(cmd, ":SXEX:%+03d#", (int)(angle*4.0));
      cmd[4] = east ? '9' : 'A';
      DisplayMessageLX200(SetLX200(cmd), false);
    }
  }
}
