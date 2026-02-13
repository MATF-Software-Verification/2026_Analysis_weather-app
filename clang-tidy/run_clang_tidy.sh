#!/usr/bin/env bash

SUBMODULE_DIR="../weather-app"

OUTPUT_FILE="clang_tidy_weather_report.txt"
OUTPUT_HTML="clang_tidy_weather_report.html"

rm -f "$OUTPUT_FILE" "$OUTPUT_HTML"

CHECKS='clang-diagnostic-*,clang-analyzer-*,modernize-*,performance-*,readability-*,bugprone-*,cppcoreguidelines-*'

HEADER_FILTER='.*(Source|Api|Data|Pages|Widgets|Settings|Utils|Tests|Serialization).*'

echo "Pokrecem clang-tidy nad $SUBMODULE_DIR ..."

find "$SUBMODULE_DIR" -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" \) | while read -r file; do
    echo "[Processing] $file"
    clang-tidy "$file" \
        -checks="$CHECKS" \
        -header-filter="$HEADER_FILTER" \
        -- \
        -std=c++20 \
        -I"$SUBMODULE_DIR/Source" \
        -I"$SUBMODULE_DIR" \
        -I'/c/Qt/6.10.2/mingw_64/include' \
        -I'/c/Qt/6.10.2/mingw_64/include/QtCore' \
        -I'/c/Qt/6.10.2/mingw_64/include/QtGui' \
        -I'/c/Qt/6.10.2/mingw_64/include/QtWidgets' \
        -I'/c/Qt/6.10.2/mingw_64/include/QtNetwork' \
        >> "$OUTPUT_FILE" 2>&1
done

echo ""
echo "Filtriram samo upozorenja iz Source foldera za HTML..."
grep -E "Source/.*(warning:|error:|note:)" "$OUTPUT_FILE" > source_filtered.txt

if [ -s source_filtered.txt ]; then
    if command -v pandoc >/dev/null 2>&1; then
        echo "Generisem HTML samo za Source upozorenja..."
        pandoc source_filtered.txt -o source_clang_tidy.html --standalone \
            --css=https://cdn.jsdelivr.net/npm/water.css@2/out/water.css \
            --metadata title="Clang-Tidy Izvestaj - Source Folder"
        echo "HTML sačuvan kao: source_clang_tidy.html"
    else
        echo "Pandoc nije instaliran – preskacem HTML za Source."
    fi
else
    echo "Nema upozorenja u Source folderu – nema sta da se generise."
fi

echo ""
read -p "Pritisni Enter da zatvoris prozor..."