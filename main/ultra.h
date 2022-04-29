
// funkcie pre ultrazvukovy a infracerveny senzor


struct UltraVals{
  float fDistEmptyCM1;       // merana vzdialenost, ked je schranka prazdna
  float fDistEmptyCM2;
  float fDistEmptyCM3;
  float fDistLastCM1;
  float fDistLastCM2;
  float fDistLastCM3;
  int iUltraCheckInterval;
  int iUltraExtraChecks;
  int iUltraExtraChecksIntervalMS;
  float fUltraTolerance;
  bool bMailEmpty;
};

extern UltraVals UltraV;


void ultraSetEmpty();
void ultraSetLast();
float ultraMeasure1();
float ultraMeasure2();
float ultraMeasure3();
bool ultraCheck1Empty();
bool ultraCheck2Empty();
bool ultraCheck3Empty();
bool ultraCheckAllEmpty();
bool ultraCheck1NotEmpty();
bool ultraCheck2NotEmpty();
bool ultraCheck3NotEmpty();
bool ultraCheckAllNotEmpty();
bool IRCheck3();
int checkMail();
int checkMailNoIR();
void setMailEmpty(bool in);
void reloadUltraConfigs();
