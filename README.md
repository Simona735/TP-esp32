# TP-esp32
Program pre hardver schranky (ESP32). Samotny program je v zlozke "main", ostatne su skusobne programy.

### BLE
Nazov: Mailbox
Service: "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

Charakteristika:   "11aa358f-9224-46d9-b0f5-3a7ba1ac651e"

Protokol:
- Appka si musi od ESP najprv vyziadat zvacsenie MTU (aby sa dali prenasat dlhe stringy) na 92 bajtov.
- Ked je v chara. zapisane "0", moze appka zapisovat do charakteristiky, ESP je pripravene. Idealne treba este po zaznamenani "0" pre istotu pockat este cca 50 milisekund pred zapisovanim.
- Prikazy/nastavenie sa zadavaju v tvare
ABCD;XYZ
kde ABCD je kratky kod nastavenia/prikazu (vid. zoznam dole) a XYZ je zadavana hodnota (cislo, string...). Obe casti su oddelene bodkociarkou.
- Priklady:
WP;tajneheslo123
+FRST;0
- Vzdy a vsetko (aj ciselne hodnoty) sa zapisuje ako string.
- Vzdy musia byt vsetky tri casti, nevynechavat bodkociarku a zadavanu hodnotu aj ked sa nic nezadava(treba dat hocico).
- Hned ako to appka zapise, ESP to spracuje a zase to prepise na "0" ked je hotove a pripravene.
- ESP prijima nastavenia/prikazy cez BLE v dvoch situaciach:
1: Reset vyvolany stlacenim tlacitka, po ktorom sa podarilo nacitat pouzitelne predosle nastavenia. V tomto pripade je BLE zapnute na kratku dobu (30s) a appka moze (nemusi) nastavit lubovolne nastavenia (nemusi vsetky).
2: Reset vyvolany stlacenim tlacitka, po ktorom sa nepodarilo nacitat nastavenia (neexistuju, prve zapnutie). V tomto pripade je BLE zapnute dlho (10m), a ak zariadnie nastavenia neobdrzi, permanentne zaspi (do manualneho restartu). Appka musi dodat vsetky(!!!, okrem oznacenych (N)) nastavenia na zozname dole. Nastavovanie treba ukoncit pomocou prikazu +CONF.

Prikazy(BLE):
- "+FRST"		Tovarensky reset. Zmaze vsetko a restartuje zariadenie.
- "+CONF"		Potvrdenie. Da signal ESPcku ze moze predcasne zatvorit BLE a dalej fungovat. Nepovinne

Nastavenia (BLE):
- "WS"		string		SSID WiFi
- "WP"		string		Heslo WiFi
- "FBK"		string(N)		API kluc na Firebase
- "FBM"		string		Mail na Firebase
- "FBP"		string		Heslo na Firebase
- "FBI"		string		ID schranky vo Firebase
- "FBU"		string		ID usera vo Firebase

### Firebase

    [userID]
    ├── mailbox_iter: int
    └── [IDschranky]
         ├── events
         │    ├── NewMail: bool         # Bola zaznamenana nova posta, true = nova posta, false = nie je nova posta
         │    ├── EmptyBox: bool        #
         │    ├── FullBox: bool         #
         │    ├── FatalError: bool      # Vyskytla sa vazna chyba(ale podarilo sa pripojit na Firebase)
         │    └── LastMsgTime: time     # last message time, pri odosielani udalosti zariadenie do nasledujucej premennej vlozi momentalny cas
         └── settings
              ├── UCI: int          # default - 7000000, Interval medzi kontrolami (mikrosekundy)
              ├── UEC: int          # default - 4, Kontroly navyse, ked sa nieco zdetekuje (aby se predislo falosnemu poplachu)
              ├── UECI: int         # default - 500, Interval medzi kontrolami navyse
              ├── UT: float         # default - 0.1, Tolerancia, (vacsia = menej falosnych poplachov, ale tenke listy nezdetekuje, iba ked ich je viac)
              ├── low_power: bool   # default - true, low power mode, false = normal, true = low
              ├── reset: bool       # default - false, esp reset to be done
              └── name: string      # default - name + [mailbox_iter], pomenovanie schranky

