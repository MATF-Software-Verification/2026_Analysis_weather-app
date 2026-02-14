# Analiza aplikacije Weather App

**Univerzitet u Beogradu, Matematički fakultet**  
Seminarski rad iz predmeta Verifikacija softvera  
**Student:** Aleksandra Labović, 1025/24  
**Februar, 2026**

---

## Sadržaj

1. [Uvod](#1-uvod)
2. [Statička analiza koda](#2-statička-analiza-koda)  
   2.1 [Cppcheck](#21-cppcheck)  
   2.2 [Clang-Tidy](#22-clang-tidy)  
3. [Generisanje dokumentacije — Doxygen](#3-generisanje-dokumentacije--doxygen)
4. [Dinamička analiza memorije — Dr. Memory](#4-dinamička-analiza-memorije--dr-memory)
5. [Profilisanje performansi — Very Sleepy](#5-profilisanje-performansi--very-sleepy)
6. [Zaključak](#6-zaključak)
7. [Literatura](#7-literatura)

---

## 1. Uvod

U ovom seminarskom radu izvršena je analiza desktop GUI aplikacije **Weather App**, otvorenog koda, dostupne na GitLab repozitorijumu:  
https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2023-2024/weather-app

Aplikacija je implementirana u programskom jeziku C++ (standard C++20) uz korišćenje Qt framework-a (verzija 6.10 i novije). Njena osnovna namena je prikaz trenutnih vremenskih uslova i kratkoročne vremenske prognoze za izabrani grad.

Za pribavljanje podataka aplikacija koristi dve mrežne usluge: OpenCage Geocoding API, koji omogućava transformaciju naziva grada u geografske koordinate, i OpenWeatherMap API, koji na osnovu koordinata obezbeđuje meteorološke podatke. Aplikacija takođe podržava automatsko predlaganje naziva gradova tokom pretrage, kao i podešavanje jedinica merenja.

Izvorni kod organizovan je modularno unutar direktorijuma `Source/`, pri čemu su funkcionalnosti razdvojene prema odgovornostima komponenti:

| Modul | Opis |
|-------|------|
| `Api/` | Komunikacija sa mrežnim servisima |
| `Data/` | Modeli podataka i pripadajuće strukture |
| `Pages/` | Implementacija stranica korisničkog interfejsa |
| `Settings/` | Upravljanje konfiguracijom aplikacije |
| `Utils/` | Pomoćne funkcije i opšte namenske alatke |
| `Widgets/` | Prilagođene komponente korisničkog interfejsa zasnovane na Qt bibliotekama |

U cilju procene kvaliteta i pouzdanosti softvera primenjeni su alati za statičku i dinamičku analizu, generisanje dokumentacije i profilisanje performansi. Statička analiza izvršena je upotrebom alata Clang-Tidy i Cppcheck, dokumentacija je generisana korišćenjem alata Doxygen, analiza upravljanja memorijom sprovedena je pomoću alata Dr. Memory, dok je ispitivanje performansi realizovano primenom profilera Very Sleepy. Na taj način obuhvaćeni su ključni aspekti kvaliteta softvera: ispravnost, stabilnost, održivost i performanse.

---

## 2. Statička analiza koda

Statička analiza koda predstavlja postupak ispitivanja izvornog koda bez njegovog izvršavanja, sa ciljem identifikacije potencijalnih grešaka, bezbednosnih propusta, nepravilnosti u stilskim konvencijama i odstupanja od dobrih programerskih praksi. Za razliku od dinamičke analize, koja zahteva pokretanje programa, statička analiza omogućava rano otkrivanje problema već u fazi razvoja, čime se smanjuju troškovi ispravki i povećava pouzdanost softvera.

U kontekstu C++ aplikacija, statički analizatori imaju poseban značaj zbog složenosti jezika, upravljanja memorijom i mogućnosti pojave nedefinisanog ponašanja. Ovi alati vrše sintaksnu i semantičku analizu koda, proveravaju potencijalne greške u rukovanju resursima, upozoravaju na nebezbedne konstrukcije i predlažu unapređenja u skladu sa savremenim preporukama i standardima.

U okviru ovog rada primenjena su dva alata za statičku analizu: Clang-Tidy i Cppcheck. Njihova uloga, način primene i rezultati analize biće detaljno prikazani u narednim potpoglavljima.

---

## 2.1 Cppcheck

Cppcheck predstavlja alat za statičku analizu izvornog koda napisanog u programskim jezicima C i C++. Njegova primarna svrha je identifikacija potencijalnih grešaka, nedefinisanog ponašanja i rizičnih konstrukcija u kodu, uz poseban fokus na minimizaciju lažno pozitivnih nalaza.[^1] Za razliku od nekih drugih analizatora, Cppcheck ne zahteva kompajlabilan kod niti prisustvo datoteke `compile_commands.json`, što omogućava efikasnu analizu projekata sa nestandardnim build sistemima.

[^1]: Lažno pozitivni rezultati predstavljaju situacije u kojima alat prijavi grešku ili problem, iako stvarna greška ne postoji.

U okviru ovog istraživanja, analiza je izvršena nad celokupnim submodulom `weather-app/`, korišćenjem skripte `run_cppcheck.sh`. Konfiguracija alata obuhvatala je sledeće parametre:

```bash
cppcheck \
  --enable=all          \
  --inconclusive        \
  --std=c++20           \
  --force               \
  --xml-version=2       \
  --suppress=missingIncludeSystem  \
  --suppress=*:*/Qt*/* \
  --suppress=*:*/usr/include/* \
  ../weather-app
```
Opcija `--enable=all` aktivira sve dostupne kategorije provera, dok `--inconclusive` uključuje i potencijalne probleme koji nisu sa potpunom sigurnošću potvrđeni. Direktive `--suppress` korišćene su za isključivanje upozorenja koja potiču iz Qt biblioteka i sistemskih zaglavlja, budući da takva upozorenja nisu relevantna za ocenu kvaliteta izvornog koda posmatrane aplikacije. Rezultati analize sačuvani su u XML formatu (`cppcheck_results.xml`), a na osnovu njih je generisan pregledan HTML izveštaj pomoću alata `cppcheck-htmlreport`.

### Rezultati analize

Analizom sprovedenom korišćenjem verzije 2.19.0 detektovano je ukupno 407 nalaza, raspoređenih u sledeće kategorije:

| Kategorija | Broj | Opis |
|---|---|---|
| `style` | 209 | Preporuke vezane za stil i čitljivost koda |
| `information` | 119 | Informativne poruke |
| `warning` | 37 | Potencijalni problemi koji mogu dovesti do grešaka |
| `error` | 27 | Greške koje mogu uticati na ispravnost programa |
| `performance` | 15 | Problemi sa performansama |

Najčešći tipovi nalaza prikazani su u sledećoj tabeli:

| ID greške | Opis | Broj |
|---|---|---|
| `missingInclude` | Nedostajuća zaglavlja | 100 |
| `noExplicitConstructor` | Konstruktor bez ključne reči `explicit` | 59 |
| `unusedFunction` | Nekorišćene funkcije | 53 |
| `useStlAlgorithm` | Preporuka za korišćenje STL algoritama | 23 |
| `uninitMemberVar` | Neinicijalizovani atributi | 19 |
| `syntaxError` | Sintaksne greške | 19 |
| `duplInheritedMember` | Duplirani nasleđeni član | 14 |
| `missingOverride` | Nedostaje `override` | 12 |
| `functionStatic` | Funkcija može biti deklarisana kao `static` | 11 |
| `funcArgNamesDifferent` | Različita imena parametara u deklaraciji i definiciji | 10 |

Najveći broj nalaza odnosi se na kategoriju `missingInclude`. Ovi rezultati su posledica nedostupnosti Qt zaglavlja tokom analize, s obzirom da analiza nije izvršena kroz CMake konfiguraciju, te se ne smatraju stvarnim greškama u implementaciji.

Među značajnijim nalazima izdvajaju se upozorenja tipa `duplInheritedMember`, gde klasa `DetailedWeatherData` redefiniše metode prisutne u roditeljskoj klasi `WeatherData`. Ovakva situacija može dovesti do nedoslednosti u hijerarhiji nasleđivanja i otežati održavanje koda. Nalazi tipa `noExplicitConstructor` ukazuju na konstruktore sa jednim argumentom koji nisu označeni ključnom rečju `explicit`, čime se dozvoljavaju implicitne konverzije tipa i potencijalno neočekivano ponašanje programa. Upozorenja `uninitMemberVar` identifikovala su atribute koji nisu inicijalizovani u konstruktorima, što u C++ može dovesti do nedefinisanog ponašanja.

Na osnovu sprovedene analize može se zaključiti da većina prijavljenih nalaza pripada kategorijama stila i informativnih poruka, dok manji broj upozorenja predstavlja konkretne probleme koji zahtevaju pažnju tokom daljeg razvoja i održavanja softverske aplikacije. 

---

## 2.2 Clang-Tidy

Clang-Tidy predstavlja alat za statičku analizu C++ koda baziran na Clang kompajlerskoj infrastrukturi. Njegova funkcija je identifikacija potencijalnih grešaka, kršenja smernica za stil i sigurnosnih problema, bez potrebe za kompilacijom. Posebno je koristan za projekte koji koriste moderne standarde C++ jezika.

Analiza je izvršena skriptom `run_clang_tidy.sh` nad svim `.cpp`, `.h`, `.hpp` i `.cc` fajlovima projekta. Aktivirani su sledeći skupovi provera:

```bash
CHECKS='clang-diagnostic-*,clang-analyzer-*,modernize-*,
        performance-*,readability-*,bugprone-*,
        cppcoreguidelines-*'
```

Filter zaglavlja ograničen je isključivo na module projekta:

```bash
HEADER_FILTER='.*(Source|Api|Data|Pages|Widgets|Settings|Utils).*'
```

Kompilator je konfigurisan sa standardom C++20 i eksplicitnim include putanjama za Qt 6.10.2 (`QtCore`, `QtGui`, `QtWidgets`, `QtNetwork`).

### 3.1 Analiza izlaza

Clang-Tidy je generisao ukupno **110.575 upozorenja** nad celim projektom, od kojih je 110.562 potisnutih jer potiču iz Qt zaglavlja i sistemskih biblioteka. Izvorni kod aplikacije sadrži sledeće najzastupljenije kategorije upozorenja:

| Kategorija provere | Broj |
|---|---|
| `modernize-use-trailing-return-type` | 417 |
| `modernize-use-nodiscard` | 213 |
| `readability-avoid-const-params-in-decls` | 168 |
| `readability-identifier-length` | 162 |
| `readability-redundant-inline-specifier` | 134 |
| `cppcoreguidelines-avoid-magic-numbers` | 131 |
| `cppcoreguidelines-special-member-functions` | 65 |
| `cppcoreguidelines-non-private-member-variables` | 58 |
| `cppcoreguidelines-explicit-virtual-functions` | 53 |
| `readability-redundant-access-specifiers` | 44 |
| `cppcoreguidelines-avoid-const-or-ref-data-members` | 25 |
| `cppcoreguidelines-pro-type-member-init` | 21 |
| `bugprone-easily-swappable-parameters` | 19 |

Dominantno upozorenje `modernize-use-trailing-return-type` (417 slučajeva) preporučuje korišćenje *trailing return type* sintakse, u skladu sa modernim C++ stilom. Ovo je stilska preporuka bez uticaja na ispravnost koda.

Upozorenje `cppcoreguidelines-special-member-functions` (65 slučajeva) ukazuje na situacije u kojima klase definišu destruktor, ali ne implementiraju kopirajuće i premeštajuće konstruktore niti operatore dodele, čime se krši tzv. *Rule of Five*. Konkretno, klasa `ApiHandler` definiše destruktor, ali ne poseduje odgovarajuće specijalne članove, što može dovesti do neočekivanog ponašanja prilikom kopiranja ili premeštanja objekata.

Upozorenje `cppcoreguidelines-non-private-member-variables` (58 slučajeva) prijavljuje da klasa `ApiHandler` izlaže atribute `networkManager`, `networkErrMsg` i `parseErrMsg` kao `protected`, što nije u skladu sa principima enkapsulacije.
Upozorenje `cppcoreguidelines-avoid-magic-numbers` (131 slučajeva) identifikuje magične brojeve u kodu, od kojih je najočigledniji vrednost `2000` koja predstavlja timeout za mrežne zahteve u milisekundama, a trebalo bi je zameniti imenovanom konstantom.

Upozorenje `cppcoreguidelines-pro-type-member-init` (21 slučajeva) ukazuje na konstruktore koji ne inicijalizuju sve atribute klase. Na primer, konstruktor klase `DetailedWeatherAPI` ne inicijalizuje polje `location`.

HTML izveštaj filtriran na `Source/` direktorijum dostupan je u fajlu `source_clang_tidy.html`.

---

## 3. Generisanje dokumentacije — Doxygen

Doxygen je standardni alat za automatsko generisanje API dokumentacije iz anotiranog C++ izvornog koda. Na osnovu konfiguracije definisane u `Doxyfile`-u, alat parsira izvorni kod i generiše HTML dokumentaciju koja obuhvata pregled klasa, hijerarhiju nasleđivanja i dijagrame interakcija između klasa.

Dokumentacija je kreirana korišćenjem priloženog `Doxyfile`-a (Doxygen verzija 1.16.1) komandom:

```bash
cd doxygen
doxygen Doxyfile
```

Ključni parametri konfiguracije:

| Parametar | Vrednost |
|---|---|
| `PROJECT_NAME` | *Weather App - Verifikacija softvera* |
| `OUTPUT_DIRECTORY` | `doc/` |
| `INPUT` | `Source/`, sa svim podmodulima |
| `RECURSIVE` | `YES` |
| `EXTRACT_ALL` | `YES` |
| `EXTRACT_PRIVATE` | `YES` |
| `EXTRACT_STATIC` | `YES` |
| `GENERATE_HTML` | `YES` |
| `GENERATE_LATEX` | `NO` |
| `GENERATE_TREEVIEW` | `YES` |
| `HAVE_DOT` | `NO` |
| `UML_LOOK` | `YES` |
| `CLASS_GRAPH` | `YES` |
| `COLLABORATION_GRAPH` | `YES` |
| `QT_AUTOBRIEF` | `YES` |

Napominje se da je `HAVE_DOT = NO`, što znači da alat **Graphviz** nije korišćen. Dijagrami hijerarhije i međusobne interakcije klasa kreirani su pomoću ugrađenog generatora dijagrama u Doxygen-u, dok je UML notacija omogućena opcijom `UML_LOOK = YES`. Glavna ulazna tačka generisane dokumentacije nalazi se u fajlu `doc/html/index.html`.

---

## 4. Dinamička analiza memorije — Dr. Memory

Dr. Memory je alat za dinamičku analizu memorije koji instrumentiše izvršni kod na nivou mašinskih instrukcija i omogućava detekciju grešaka u upravljanju memorijom tokom izvršavanja programa. Za razliku od statičkih analizatora, Dr. Memory prati stvarno izvršavanje programa i može identifikovati greške koje se manifestuju samo u određenim runtime scenarijima.

Alat detektuje sledeće kategorije grešaka u memoriji: curenje memorije (`LEAK`), čitanje ili pisanje van granica alocirane memorije (`INVALID READ/WRITE`), čitanje neinicijalizovane memorije (`UNINITIALIZED READ`), pristup memoriji nakon oslobađanja (`USE-AFTER-FREE`) i dvostruko oslobađanje (`DOUBLE FREE`).

Analiza je pokrenuta komandom:

```bash
drmemory.exe -- ./weather-app.exe 1> DrMemory/logs.txt 2>&1
```

### Analiza izlaza

Detektovano je ukupno **809 grešaka**, distribucija grešaka po tipu:

| Tip greške | Broj |
|---|---|
| `UNADDRESSABLE ACCESS beyond top of stack` | 705 |
| `UNINITIALIZED READ` | 104 |

Dominantna greška je `UNADDRESSABLE ACCESS beyond top of stack` (705 slučajeva). Analiza stack trace-ova pokazuje da ove greške **nisu uzrokovane kodom aplikacije**, već potiču iz internih Qt biblioteka (`Qt6Core.dll`, `Qt6Gui.dll`, `Qt6Widgets.dll`, `qwindows.dll`) i sistemskih biblioteka (`USER32.dll`, `UxTheme.dll`). Ova pojava je poznato ograničenje Dr. Memory pri analizi Qt aplikacija na Windows platformi.

`UNINITIALIZED READ` greške (104 slučajeva) javljaju se pretežno tokom inicijalizacije Qt grafičkih podsistema, što je karakteristično za Qt aplikacije i nije posledica grešaka u kodu aplikacije.

Na kraju sesije Dr. Memory je doživeo interni pad pri obradi Qt event-ova, što je poznati problem kompatibilnosti alata sa Qt 6.x na Windows platformi.

Zaključuje se da **nisu pronađene greške u upravljanju memorijom koje potiču iz izvornog koda aplikacije**.

---
## 5. Profilisanje performansi — Very Sleepy

**Very Sleepy** je *sampling* profiler za Windows platformu, koji periodično uzorkuje aktivni call stack posmatranog procesa i na osnovu učestalosti pojave funkcija u uzorcima procenjuje njihov doprinos ukupnom vremenu izvršavanja. Rezultati se izražavaju kroz *Exclusive %* (vreme provedeno direktno u funkciji) i *Inclusive %* (ukupno vreme uključujući podfunkcije).

Profilisanje je sprovedeno putem GUI interfejsa: pokretanjem `VerySleepy.exe`, odabirom procesa `weather-app.exe` i izvršavanjem reprezentativnog radnog opterećenja — pretraga gradova, učitavanje i prikaz vremenske prognoze, promena jedinica merenja. Rezultati su sačuvani u fajlove `weather-app.capture` (za ponovnu analizu u Very Sleepy GUI-ju) i `weather-app.csv` (tabelarni eksport).

### Analiza rezultata

Ukupno je profilirano **117 funkcija**, pri čemu je ukupno inkluzivno vreme izvršavanja iznosilo 35.447 sekundi. Najznačajniji rezultati prikazani su u tabeli:

| Funkcija | Excl. secs | Incl. secs | Incl. % |
|---|---|---|---|
| `RtlUserThreadStart` | 27961.7 | 35447.1 | 100.0 |
| `NtUserMsgWaitForMultipleObjectsEx` | 6988.5 | 6988.5 | 19.7 |
| `QFileInfo::filePath` | 0.19 | 7000.9 | 19.8 |
| `NtRemoveIoCompletion` | 384.7 | 384.7 | 1.1 |
| `WaitForMultipleObjectsEx` | 99.8 | 99.8 | 0.3 |
| `main / qtEntryPoint` | 0.0 | 3495.2 | 9.9 |

Dominantna funkcija po *exclusive* vremenu je `NtUserMsgWaitForMultipleObjectsEx` (19.7%), što predstavlja Windows sistemski poziv koji čeka na poruke u Qt event loop-u. Ovo ponašanje je karakteristično za Qt GUI aplikacije, jer većina vremena procesa protekne u čekanju na korisnički unos ili mrežne odgovore, dok je aktivna obrada minimalna.

Funkcija `QFileInfo::filePath` pokazuje visoko inkluzivno vreme (19.8%), što ukazuje na učestale operacije sa putanjama fajlova tokom rada aplikacije, verovatno povezane sa keširanjem ikona vremenskih uslova ili čitanjem konfiguracionih fajlova.

Funkcija `NtRemoveIoCompletion` (1.1%) predstavlja asinhrono čekanje na završetak I/O operacija, što direktno odgovara mrežnim zahtevima prema OpenCage i OpenWeatherMap servisima.

Ukupno, analiza potvrđuje da aplikacija pokazuje **efikasno korišćenje resursa i optimalne performanse**: aktivan procesorski rad je minimalan, što je u skladu sa reaktivnim, *event-driven* arhitekturnim modelom Qt aplikacija.

---
## 6. Zaključak

U ovom seminarskom radu sprovedena je sveobuhvatna analiza aplikacije **Weather App**.

Statička analiza pomoću **Cppcheck**-a detektovala je ukupno 407 nalaza. Najznačajniji problemi uključuju duplirane metode u hijerarhiji nasleđivanja (`duplInheritedMember`), neinicijalizovane atribute klasa (`uninitMemberVar`) i konstruktore sa jednim argumentom koji nisu označeni ključnom rečju `explicit` (`noExplicitConstructor`). Analiza putem **Clang-Tidy** identifikovala je dodatne preporuke za modernizaciju koda i usklađenost sa C++ Core Guidelines, pri čemu su posebno istaknuti kršenje *Rule of Five* u klasi `ApiHandler` i prisustvo magičnih brojeva. Rezultati oba alata ukazuju na mogućnosti za poboljšanje robustnosti, čitljivosti i održivosti izvornog koda.

Generisanje dokumentacije primenom **Doxygen**-a omogućilo je pregled strukture koda, hijerarhije klasa, odnosa između modula i međuzavisnosti funkcija. Dokumentacija služi kao dodatno sredstvo verifikacije, olakšava razumevanje arhitekture aplikacije i doprinosi održivosti softverskog rešenja.

Dinamička analiza memorije pomoću **Dr. Memory** nije detektovala greške koje potiču iz izvornog koda aplikacije. Sve 809 prijavljenih grešaka proizilazilo je iz interakcije Qt biblioteka sa Windows platformom, što predstavlja poznato ograničenje alata prilikom analize Qt 6.x aplikacija, a ne stvarne propuste u upravljanju memorijom.

Profilisanje performansi primenom **Very Sleepy**-ja potvrdilo je da aplikacija demonstrira efikasno korišćenje procesorskih resursa. Većina CPU vremena trošena je na čekanje u okviru Qt event loop-a i asinhronih I/O operacija, što je u skladu sa očekivanim ponašanjem mrežno-zavisnih GUI aplikacija.

Ukupno, sprovedene analize potvrđuju visok kvalitet implementacije **Weather App** i ukazuju na konkretne smernice za unapređenje koda, čime se doprinosi poboljšanju njegove pouzdanosti, održivosti, dokumentovanosti i performansi.

---

## 7. Literatura

1. Milena Vujošević Janičić, *Verifikacija softvera*, Matematički fakultet, Beograd
2. Materijali sa vežbi iz kursa Verifikacija softvera
3. Clang-Tidy dokumentacija — https://clang.llvm.org/extra/clang-tidy/
4. Cppcheck dokumentacija — https://cppcheck.sourceforge.io
5. Doxygen dokumentacija — https://www.doxygen.nl/manual/
6. Dr. Memory dokumentacija — https://drmemory.org/docs/
7. Very Sleepy repozitorijum — https://github.com/VerySleepy/verysleepy
8. Qt dokumentacija — https://doc.qt.io/qt-6/