# 2026_Analysis_weather-app

## Podaci o autoru i projektu koji je predmet analize

**Autor:** Aleksandra Labović, 1025/24

**Projekat:** Weather App je desktop GUI aplikacija napisana u C++20 uz korišćenje Qt frameworka. Aplikacija prikazuje trenutne vremenske uslove i prognozu za uneti grad, oslanjajući se na OpenCage Geocoding API za geocoding i OpenWeatherMap API za meteorološke podatke. Podržava autocomplete predloge gradova pri pretrazi, prikaz kratkoročne prognoze i podešavanje jedinica merenja.

**Adresa projekta:** [https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2023-2024/weather-app](https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2023-2024/weather-app)

Za analizu projekta korišćeni su sledeći alati:

- **Clang-Tidy**: Alat za statičku analizu i unapređenje C++ koda baziran na Clang infrastrukturi.
  - Instalacija: preuzeti LLVM paket sa [https://releases.llvm.org](https://releases.llvm.org) i uključiti Clang Tools
  - Dokumentacija: [Clang-Tidy](https://clang.llvm.org/extra/clang-tidy/)
  - Folder u projektu: `clang-tidy/`
  - Skript za analizu: `clang-tidy/run_clang_tidy.sh`

- **Cppcheck**: Alat za statičku analizu C/C++ koda fokusiran na logičke greške koje kompajler ne prijavljuje.
  - Instalacija: preuzeti sa [https://cppcheck.sourceforge.io](https://cppcheck.sourceforge.io)
  - Dokumentacija: [Cppcheck](https://cppcheck.sourceforge.io)
  - Folder u projektu: `cppcheck/`
  - Skript za analizu: `cppcheck/run_cppcheck.sh`

- **Doxygen**: Alat za automatsko generisanje dokumentacije iz izvornog koda.
  - Instalacija: preuzeti sa [https://www.doxygen.nl/download.html](https://www.doxygen.nl/download.html)
  - Dokumentacija: [Doxygen](https://www.doxygen.nl/manual/)
  - Folder u projektu: `doxygen/`
  - Konfiguracioni fajl: `doxygen/Doxyfile`

- **Dr. Memory**: Alat za dinamičku analizu memorije na Windows platformi.
  - Instalacija: preuzeti sa [https://drmemory.org](https://drmemory.org)
  - Dokumentacija: [Dr. Memory](https://drmemory.org/docs/)
  - Folder u projektu: `DrMemory/`

- **Very Sleepy Profiler**: Sampling profiler za merenje potrošnje CPU-a i profilisanje performansi na Windows platformi.
  - Instalacija: preuzeti portabilan `VerySleepy.exe` sa [https://github.com/VerySleepy/verysleepy](https://github.com/VerySleepy/verysleepy)
  - Dokumentacija: [Very Sleepy](https://github.com/VerySleepy/verysleepy/wiki)
  - Folder u projektu: `VerySleepyProfiler/`

Za svaki navedeni alat postoji odgovarajući folder.

Detaljan opis izlaza i pokretanja ovih alata nalazi se u izveštaju `ProjectAnalysisReport.md` koji se nalazi u repozitorijumu.