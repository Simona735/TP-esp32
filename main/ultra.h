
// funkcie pre ultrazvukovy senzor


struct UltraVals{
  float fDistEmptyCM1;       // merana vzdialenost, ked je schranka prazdna
  float fDistEmptyCM2;
  float fDistEmptyCM3;
  int iUltraCheckInterval;
  int iUltraExtraChecks;
  int iUltraExtraChecksIntervalMS;
  float fUltraTolerance;
};

extern UltraVals UltraV;


void ultraSetEmpty();
float ultraMeasure1();
float ultraMeasure2();
float ultraMeasure3();
bool ultraCheck1();
bool ultraCheck2();
bool ultraCheck3();
bool ultraCheckAll();
void reloadUltraConfigs();
