
// funkcie pre ultrazvukovy senzor


struct UltraVals{
  float fDistEmptyCM;       // merana vzdialenost, ked je schranka prazdna
  
};

extern UltraVals UltraV;


void ultraSetEmpty();
float ultraMeasure();
int ultraCheck();
