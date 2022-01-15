
// funkcie pre ultrazvukovy senzor


struct UltraVals{
  float fDistEmptyCM1;       // merana vzdialenost, ked je schranka prazdna
  float fDistEmptyCM2;
  float fDistEmptyCM3;
  
};

extern UltraVals UltraV;


void ultraSetEmpty();
float ultraMeasure1();
float ultraMeasure2();
float ultraMeasure3();
bool ultraCheck1();
bool ultraCheck2();
bool ultraCheck3();
