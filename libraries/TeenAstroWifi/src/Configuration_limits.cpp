#include <TeenAstroLX200io.h>
#include "TeenAstroWifi.h"
// -----------------------------------------------------------------------------------
// configuration_telescope

const char html_configMinAlt[] PROGMEM =
"<div class='bt'> Limits: <br/> </div>"
"<form method='get' action='/configuration_limits.htm'>"
" <input value='%d' type='number' name='hl' min='-30' max='30'>"
"<button type='submit'>Upload</button>"
" (Horizon, in degrees +/- 30)"
"</form>"
"\r\n";
const char html_configMaxAlt[] PROGMEM =
"<form method='get' action='/configuration_limits.htm'>"
" <input value='%d' type='number' name='ol' min='60' max='91'>"
"<button type='submit'>Upload</button>"
" (Overhead, in degrees 60 to 90, set 91 to deactivate)"
"</form>"
"\r\n";
const char html_configPastMerE[] PROGMEM =
"<form method='get' action='/configuration_limits.htm'>"
" <input value='%d' type='number' name='el' min='-45' max='45'>"
"<button type='submit'>Upload</button>"
" (Past Meridian when East of the pier, in degrees +/-45)"
"</form>"
"\r\n";
const char html_configPastMerW[] PROGMEM =
"<form method='get' action='/configuration_limits.htm'>"
" <input value='%d' type='number' name='wl' min='-45' max='45'>"
"<button type='submit'>Upload</button>"
" (Past Meridian when West of the pier, in degrees +/-45)"
"</form>"
"<br />\r\n";

//const char html_reboot_t[] PROGMEM =
//"<br/><form method='get' action='/configuration_mount.htm'>"
//"<b>The main unit will now restart please wait some seconds and then press continue.</b><br/><br/>"
//"<button type='submit'>Continue</button>"
//"</form><br/><br/><br/><br/>"
//"\r\n";
//bool restartRequired_t = false;

void TeenAstroWifi::handleConfigurationLimits()
{
  Ser.setTimeout(WebTimeout);
  sendHtmlStart();
  char temp[320] = "";
  char temp1[50] = "";
  char temp2[50] = "";
  String data;

  processConfigurationLimitsGet();
  preparePage(data, ServerPage::Limits);
  sendHtml(data);
  //if (restartRequired_t)
  //{
  //  data += FPSTR(html_reboot_t);
  //  data += "</div></div></body></html>";
  //  sendHtml(data);
  //  sendHtmlDone(data);
  //  restartRequired_t = false;
  //  delay(1000);
  //  return;
  //}
  //update
  ta_MountStatus.updateMount();

  // Overhead and Horizon Limits
  if (GetLX200(":GXLH#", temp1, sizeof(temp1)) == LX200GETVALUEFAILED) strcpy(temp1, "0"); int minAlt = (int)strtol(&temp1[0], NULL, 10);
  sprintf_P(temp, html_configMinAlt, minAlt);
  data += temp;
  sendHtml(data);
  if (GetLX200(":GXLO#", temp1, sizeof(temp1)) == LX200GETVALUEFAILED) strcpy(temp1, "0"); int maxAlt = (int)strtol(&temp1[0], NULL, 10);
  sprintf_P(temp, html_configMaxAlt, maxAlt);
  data += temp;
  sendHtml(data);
  // Meridian Limits
  if (GetLX200(":GXLE#", temp1, sizeof(temp1)) == LX200VALUEGET && GetLX200(":GXLW#", temp2, sizeof(temp2)) == LX200VALUEGET)
  {
    int degPastMerE = (int)strtol(&temp1[0], NULL, 10);
    degPastMerE = round((degPastMerE*15.0) / 60.0);
    sprintf_P(temp, html_configPastMerE, degPastMerE);
    data += temp;
    int degPastMerW = (int)strtol(&temp2[0], NULL, 10);
    degPastMerW = round((degPastMerW*15.0) / 60.0);
    sprintf_P(temp, html_configPastMerW, degPastMerW);
    data += temp;
  }
  else data += "<br />\r\n";
  strcpy(temp, "</div></body></html>");
  data += temp;
  sendHtml(data);
  sendHtmlDone(data);
}

void TeenAstroWifi::processConfigurationLimitsGet()
{
  String v;
  int i;
  float f;
  char temp[20] = "";

 
  // Overhead and Horizon Limits
  v = server.arg("ol");
  if (v != "")
  {
    if ((atoi2((char*)v.c_str(), &i)) && ((i >= 60) && (i <= 91)))
    {
      sprintf(temp, ":SXLO,%d#", i);
      SetLX200(temp);
    }
  }
  v = server.arg("hl");
  if (v != "")
  {
    if ((atoi2((char*)v.c_str(), &i)) && ((i >= -30) && (i <= 30)))
    {
      sprintf(temp, ":SXLH,%d#", i);
      SetLX200(temp);
    }
  }

  // Meridian Limits
  v = server.arg("el");
  if (v != "")
  {
    if ((atoi2((char*)v.c_str(), &i)) && ((i >= -45) && (i <= 45)))
    {
      i = round((i*60.0) / 15.0);
      sprintf(temp, ":SXLE,%d#", i);
      SetLX200(temp);
    }
  }
  v = server.arg("wl");
  if (v != "")
  {
    if ((atoi2((char*)v.c_str(), &i)) && ((i >= -45) && (i <= 45)))
    {
      i = round((i*60.0) / 15.0);
      sprintf(temp, ":SXLW,%d#", i);
      SetLX200(temp);
    }
  }



  int ut_hrs = -999;
  v = server.arg("u1");
  if (v != "")
  {
    if ((atoi2((char*)v.c_str(), &i)) && ((i >= -13) && (i <= 13)))
    {
      ut_hrs = i;
    }
  }
  v = server.arg("u2");
  if (v != "")
  {
    if ((atoi2((char*)v.c_str(), &i)) && ((i == 00) || (i == 30) || (i == 45)))
    {
      if ((ut_hrs >= -13) && (ut_hrs <= 13))
      {
        sprintf(temp, ":SG%+03d:%02d#", ut_hrs, i);
        SetLX200(temp);
      }
    }
  }
}

