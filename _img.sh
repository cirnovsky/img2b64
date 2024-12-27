#!/bin/bash

ENCODING_PROGRAM="./img2b64"

IMAGE_EXTENSIONS="jpg|jpeg|png|gif|bmp|tiff|webp"

if [ -z "$1" ]; then
    SEARCH_PATH="."
else
    SEARCH_PATH="$1"
fi

image_files=$(find "$SEARCH_PATH" -type f -regextype posix-extended -iregex ".*\.(($IMAGE_EXTENSIONS))" 2>/dev/null)

if [ -z "$image_files" ]; then
    echo "No image files found in $SEARCH_PATH."
    exit 1
fi

echo "Encoding images from $SEARCH_PATH..."
$ENCODING_PROGRAM $image_files
