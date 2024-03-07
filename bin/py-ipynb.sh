#!/bin/bash

CONTAINER_ROOT=$(find  $HOME -type d -name "ECE-C147-Jupyter-Container" -print -quit)
TARGET_DIR="$CONTAINER_ROOT/notebooks/$1"
BASE_NAME="${2:-"py_files"}"
TARGET_NAME="$BASE_NAME"
TARGET_FILE="$TARGET_DIR/$TARGET_NAME.ipynb"

cd "$TARGET_DIR" 2> /dev/null

if [ $? -gt 0 ] || [ "$TARGET_DIR" = "$CONTAINER_ROOT/notebooks/" ]; then
    echo "$TARGET_DIR either does not exist or is not one of hw{1, 2, 3, 4, 5}."
    echo "Usage: ./py-pdf.sh [hw{1, 2, 3, 4, 5}] [target_name]"
    echo "Exiting..."
    exit 1
fi

readarray PY_FILES < <(find "$TARGET_DIR" -type f -name "*.py" -exec grep -l "YOUR CODE" {} +)

echo "Found ${#PY_FILES[@]} files:"
for PY_FILE in ${PY_FILES[@]}; do
    PY_FILE_NAME=$(basename $PY_FILE)
    echo "$PY_FILE_NAME"
done

if [ ${#PY_FILES[@]} -eq 0 ]; then
    echo "Exiting..."
    exit 0
fi

if test -f $TARGET_FILE; then
    read -p "$TARGET_FILE exists! would you like to overwrite it? [y/N] " yn

    counter=1
    while [[ -e "$TARGET_NAME.ipynb" ]]; do
        TARGET_NAME="${BASE_NAME}-${counter}"
        ((counter++))
    done

    TARGET_FILE="$TARGET_DIR/$TARGET_NAME.ipynb"

    case $yn in
        [Yy] ) echo "You might regret it. I also don't trust your judgement, so I moved the file to $TARGET_FILE instead";;
        [Nn] ) echo "Moving the file to $TARGET_FILE";;
    esac
fi

if ! test -f "$CONTAINER_ROOT/ipynb-generator"; then
    $(cd "$CONTAINER_ROOT" && make)
fi

$CONTAINER_ROOT/bin/ipynb-generator "$TARGET_FILE" ${#PY_FILES[@]}

for PY_FILE in ${PY_FILES[@]}; do
    PY_FILE_NAME=$(basename $PY_FILE)
    TMP_FILE=$(mktemp)

    sed -e 's/\t/     /g' -e 's/"/\\"/g' -e 's/$/\\n/' -e 's/.*/"&"/' -e '$!s/.*/&,/' $PY_FILE > $TMP_FILE
    sed -i "0,/FILE-NAME-HERE/s|FILE-NAME-HERE|\"# $PY_FILE_NAME\"|" "$TARGET_FILE"
    perl -i -pe 'BEGIN{undef $/; $content = `cat '$TMP_FILE'`} s/PYTHON-CODE-HERE/$content/e if $. == 1' "$TARGET_FILE"

    echo "added $PY_FILE_NAME"
done

# TODO: I want to add the nbconvert command here, but I don't really know how you set up the
# container, so I guess we can figure it out together. For now, I'm just running it through a simple
# JSON validator in python.
python -mjson.tool $TARGET_FILE > /dev/null
if [ $? -ne 0 ]; then
    echo "Failed to create PDF. Cleaning up and exiting..."
    rm $TARGET_FILE
    exit 1
fi

echo "Successfully created $TARGET_NAME.pdf. It can be found here:"
echo "$TARGET_FILE"

exit 0
