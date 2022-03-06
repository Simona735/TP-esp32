// zakomentovanie nasledujuceho riadku zkompiluje program bez debugovacich vypisov na seriovom vystupe pomocou makra serialDBOut("")
#define TPCOMPILEDEBUG

#ifdef TPCOMPILEDEBUG
#define serialDBGOut(a); Serial.println(a);
#define DBG_SERIAL_BEGIN; Serial.begin(115200);
#else
#define serialDBGOut(a);
#define DBG_SERIAL_BEGIN;
#endif