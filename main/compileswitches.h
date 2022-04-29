// zakomentovanie nasledujuceho riadku zkompiluje program bez debugovacich vypisov na seriovom vystupe pomocou makra serialDBOut("")
#define TPCOMPILEDEBUG

// zakomentovanie nasledujuceho riadku vypne pouzivanie IR senzorov
#define TPENABLEIR





#ifdef TPCOMPILEDEBUG
#define serialDBGOut(a); Serial.println(a);
#define DBG_SERIAL_BEGIN; Serial.begin(115200);
#else
#define serialDBGOut(a);
#define DBG_SERIAL_BEGIN;
#endif


#ifdef TPENABLEIR
#define CHECK_MAIL(); checkMail();
#else
#define CHECK_MAIL(); checkMailNoIR();
#endif
