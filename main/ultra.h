
// funkcie pre ultrazvukovy senzor

#define SOUND_SPEED 0.017

struct UltraVals{
  float fDistEmptyCM;       // merana vzdialenost, ked je schranka prazdna
  
};

extern UltraVals UltraV;


void ultraSetEmpty();
float ultraMeasure();
int ultraCheck();
