#!/bin/sh

INPUT_FOLDER="${1}"
PSWD="${2}"

if [ ! -d "${INPUT_FOLDER}" ] || [ "$PSWD" == "" ] 
then
	echo "Please specify the source folder and password."
	echo ""
	exit 0
fi

# Create a working copy
TEMP_FOLDER="${INPUT_FOLDER}.tmp"

cp -R "${INPUT_FOLDER}" "${TEMP_FOLDER}"

cd "${TEMP_FOLDER}"

# Find all html file in working folder except 'index.html'!
HTML_FILES=$(find "./" -iname "*.html" ! -path "*index.html")

TEMP_FILE="html.tmp"
# Iterate png files and preprocess alpha bleed.
for HTML_FILE in ${HTML_FILES}
do
#	echo "${HTML_FILE}"
	zip -0 -P "$PSWD" "$TEMP_FILE" "${HTML_FILE}"
	rm "${HTML_FILE}"
	mv "$TEMP_FILE" "${HTML_FILE}"
done

zip -0 -r "../${INPUT_FOLDER}.hpub" *

cd ..

rm -r "${TEMP_FOLDER}"
