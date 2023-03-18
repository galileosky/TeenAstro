#include <TeenAstroCoord_EQ.hpp>
#include <TeenAstroCoord_HO.hpp>

int k = 0;


const double Lat[1] = { 0.88660302 };
const double Ha[1] = { -0.69112174 };
const double Dec[1] = { 0.14718022 };
const double RotE[1] = { random(-180, 180)*DEG_TO_RAD };
const double T_Az[1] = { -0.902320657 };
const double T_Alt[1] = { 0.63775167 };

void setup()
{
    Serial.begin(57600);
}

#define DEG2RAD   M_PI/180.
#define RAD2DEG   180./M_PI


void PrintGivenEQ(double Lat, double  Ha, double Dec, double RotE)
{
  char text[200];
  sprintf(text, "Given Lat: %f\n", Lat * RAD2DEG);
  Serial.print(text);
  sprintf(text, "Given Ha: %f\n", Ha * RAD2DEG);
  Serial.print(text);
  sprintf(text, "Given Dec: %f\n", Dec * RAD2DEG);
  Serial.print(text);
  sprintf(text, "Given RotE: %f\n", RotE * RAD2DEG);
  Serial.print(text);
}

void PrintComputedEQ(double Ha, double Dec, double RotE)
{
  char text[200];
  sprintf(text, "Computed Ha: %f\n", Ha * RAD2DEG);
  Serial.print(text);
  sprintf(text, "Computed Dec: %f\n", Dec * RAD2DEG);
  Serial.print(text);
  sprintf(text, "Computed RotE: %f\n", RotE * RAD2DEG);
  Serial.print(text);
}

void PrintGivenHO(double Lat, double  Az, double Alt, double RotH)
{
  char text[200];
  sprintf(text, "Given Lat: %f\n", Lat * RAD2DEG);
  Serial.print(text);
  sprintf(text, "Given Az: %f\n", Az * RAD2DEG);
  Serial.print(text);
  sprintf(text, "Given Alt: %f\n", Alt * RAD2DEG);
  Serial.print(text);
  sprintf(text, "Given RotH: %f\n", RotH * RAD2DEG);
  Serial.print(text);
}

void PrintComputedHO(double Az, double Alt, double RotH)
{
  char text[200];
  sprintf(text, "Computed Az: %f\n", Az * RAD2DEG);
  Serial.print(text);
  sprintf(text, "Computed Alt: %f\n", Alt * RAD2DEG);
  Serial.print(text);
  sprintf(text, "Computed RotH: %f\n", RotH * RAD2DEG);
  Serial.print(text);
}



void HADEC2ALTAZ(const double& Lat, const double& Ha,const double &Dec, const double& RotE,
                 const double& T_Az, const double& T_Alt,
                 double& Az, double& Alt, double& RotH)
{
  char text[200];
  double m2[3][3];

  Serial.println("---------------------------------------");
  Serial.println("-----------SKY TO HORIZONTAL-----------");
  Serial.println("---------------------------------------");
  PrintGivenEQ(Lat, Ha, Dec, RotE);

  LA3::SingleRotation rots[4] = {
    {LA3::RotAxis::ROTAXISX,RotE },
    {LA3::RotAxis::ROTAXISY,Dec },
    {LA3::RotAxis::ROTAXISZ,Ha },
    {LA3::RotAxis::ROTAXISY, (M_PI_2 - Lat) }
  };
  LA3::getMultipleRotationMatrix(m2, rots, 4);
  LA3::getEulerRxRyRz(m2, RotH, Alt, Az);

  PrintComputedHO(Az, Alt, RotH);

  double d_Az = T_Az  - Az;
  double d_Alt = T_Alt - Alt;
 
  sprintf(text, "error Az: %f\n", d_Az * RAD2DEG);
  Serial.print(text);
  sprintf(text, "error Alt: %f\n", d_Alt * RAD2DEG);
  Serial.print(text);
}


void HADEC2ALTAZ2(const double& Lat, const double& Ha, const double& Dec, const double& RotE,
  const double& T_Az, const double& T_Alt,
  double& Az, double& Alt, double& RotH)
{
  char text[200];

  Serial.println("---------------------------------------");
  Serial.println("-----------SKY TO HORIZONTAL-----------");
  Serial.println("---------------------------------------");
  PrintGivenEQ(Lat, Ha, Dec, RotE);

  Coord_EQ EQ1 = Coord_EQ(RotE, Dec, Ha);
  Coord_HO HO1 = EQ1.To_Coord_HO(Lat);

  PrintComputedHO(HO1.Az(), HO1.Alt(), HO1.FrH());

  double d_Az = T_Az - Az;
  double d_Alt = T_Alt - Alt;

  sprintf(text, "error Az: %f\n", d_Az * RAD2DEG);
  Serial.print(text);
  sprintf(text, "error Alt: %f\n", d_Alt * RAD2DEG);
  Serial.print(text);
}

void ALTAZ2HADEC(const double& Lat, const double& Az, const double& Alt, double& RotH,
  const double& T_Ha, const double& T_Dec,
  double& Ha, double& Dec, double& RotE)
{
  char text[200];
  double m2[3][3];

  Serial.println("---------------------------------------");
  Serial.println("-----------HORIZONTAL TO SKY-----------");
  Serial.println("---------------------------------------");
  PrintGivenHO(Lat, Az, Alt, RotH);

  LA3::SingleRotation rotsb[4] = {
{LA3::RotAxis::ROTAXISX, RotH },
{LA3::RotAxis::ROTAXISY, Alt },
{LA3::RotAxis::ROTAXISZ, Az },
{LA3::RotAxis::ROTAXISY, -(M_PI_2 - Lat) }
  };

  LA3::getMultipleRotationMatrix(m2, rotsb, 4);
  LA3::getEulerRxRyRz(m2, RotE, Dec, Ha);

  double d_Ha = T_Ha - Ha;
  double d_Dec = T_Dec - Dec;

  PrintComputedEQ(Ha, Dec, RotE);

  sprintf(text, "error Ha: %f\n", d_Ha * RAD2DEG);
  Serial.print(text);
  sprintf(text, "error Dec: %f\n", d_Dec * RAD2DEG);
  Serial.print(text);

}

void ALTAZ2HADEC2(const double& Lat, const double& Az, const double& Alt, double& RotH,
  const double& T_Ha, const double& T_Dec,
  double& Ha, double& Dec, double& RotE)
{
  char text[200];

  Serial.println("---------------------------------------");
  Serial.println("-----------HORIZONTAL TO SKY-----------");
  Serial.println("---------------------------------------");
  PrintGivenHO(Lat, Az, Alt, RotH);

  Coord_HO HO1 = Coord_HO(RotH, Alt, Az);
  Coord_EQ EQ1 = HO1.To_Coord_EQ(Lat);

  PrintComputedEQ(EQ1.Ha(), EQ1.Dec(), EQ1.FrE());

  double d_Ha = T_Ha - Ha;
  double d_Dec = T_Dec - Dec;

  PrintComputedEQ(Ha, Dec, RotE);

  sprintf(text, "error Ha: %f\n", d_Ha * RAD2DEG);
  Serial.print(text);
  sprintf(text, "error Dec: %f\n", d_Dec * RAD2DEG);
  Serial.print(text);

}


void loop()
{

  char text[100];
 //value in rad
 
  if (k < 1)
  {
    double Az, Alt, RotH;
    double HA_B, Dec_B, RotE_B;


    HADEC2ALTAZ(Lat[k], Ha[k], Dec[k], RotE[k], T_Az[k], T_Alt[k], Az, Alt, RotH);
    ALTAZ2HADEC(Lat[k], Az, Alt, RotH, Ha[k], Dec[k], HA_B, Dec_B, RotE_B);

    HADEC2ALTAZ2(Lat[k], Ha[k], Dec[k], RotE[k], T_Az[k], T_Alt[k], Az, Alt, RotH);
    ALTAZ2HADEC2(Lat[k], Az, Alt, RotH, Ha[k], Dec[k], HA_B, Dec_B, RotE_B);
    k++;
  }
}