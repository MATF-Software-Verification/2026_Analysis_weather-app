# Seminarski rad iz Verifikacije softvera (2026)

## Autor
- Ime i prezime: Aleksandra Labović 
- Broj indeksa: 1025/24
- Kontakt: labovicaleksandra@gmail.com 

---

## Opis projekta
Projekat koji je analiziran: **Weather App**  
- Link do izvornog koda: https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2023-2024/weather-app.git  
- Grana: `main`
- Commit hash: `d3f0ef7`

### Kratak opis
Weather App je **desktop GUI aplikacija** napisana u C++-u koja prikazuje trenutne vremenske uslove i prognozu za uneti grad. Koristi Qt framework za korisnički interfejs i dve eksterne API usluge:

1. **OpenCage Geocoding API** – pretvara naziv grada u geografske koordinate (lat, lon)  
2. **OpenWeatherMap API** – dohvata stvarne podatke o vremenu (temperatura, vlaga, vetar, padavine, ikone vremena, itd.)

Aplikacija podržava:
- Autocomplete predloge za gradove pri kucanju
- Prikaz trenutnog vremena i kratkoročne prognoze
- Podešavanje jedinica (℃/℉, km/h / m/s / mph, mm / in, itd.)
- Jednostavan, moderan Qt interfejs

**Tehnologije**:
- C++20
- Qt 6.10+ (Widgets modul + Network modul)
- CMake kao build sistem
- QNetworkAccessManager za HTTP zahteve