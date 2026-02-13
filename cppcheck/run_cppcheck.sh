#!/usr/bin/env bash

set -euo pipefail

SOURCE_DIR="../weather-app"           
REPORT_DIR="cppcheck_html"            
XML_FILE="cppcheck_results.xml"       
CPPCHECK_ARGS=(
    --enable=all
    --inconclusive
    --std=c++20                       
    --force
    --xml-version=2                   
    --suppress=missingIncludeSystem  
    --suppress=*:*/Qt*/*              
    --suppress=*:*/usr/include/*      
)

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

CPPCHECK_VERSION=$(cppcheck --version)
echo -e "${GREEN}cppcheck pronadjen: ${CPPCHECK_VERSION}${NC}"

mkdir -p "$REPORT_DIR"

echo -e "${GREEN}Pokrecem cppcheck analizu nad $SOURCE_DIR ...${NC}"

cppcheck "${CPPCHECK_ARGS[@]}" "$SOURCE_DIR" 2> "$XML_FILE"

if [ $? -eq 0 ]; then
    echo -e "${GREEN}cppcheck analiza zavrsena bez fatalnih gresaka.${NC}"
else
    echo -e "${YELLOW}cppcheck je prijavio upozorenja/greske.${NC}"
    echo "Pogledaj fajl: $XML_FILE"
fi

echo -e "${GREEN}Generisem HTML izvestaj...${NC}"

if command -v cppcheck-htmlreport &> /dev/null; then
    cppcheck-htmlreport --title="Weather App - cppcheck" \
                        --source-dir="$SOURCE_DIR" \
                        --report-dir="$REPORT_DIR" \
                        --file="$XML_FILE"
    echo -e "${GREEN}HTML izvestaj generisan u folderu: $REPORT_DIR${NC}"
    echo "Otvorite: $REPORT_DIR/index.html"
else
    echo -e "${YELLOW}cppcheck-htmlreport nije pronadjen – HTML nije generisan.${NC}"
fi

echo ""
echo -e "${GREEN}Sve zavrseno!${NC}"
read -p "Pritisni Enter da zatvoris..."