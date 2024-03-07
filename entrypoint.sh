#!/bin/bash

if [ ! -d "/notebooks/cifar-10-batches-py" ]; then
    cp -rn /src/notebooks/cifar-10-batches-py /notebooks
fi

if (($HW > 5 || $HW < 1)); then
    echo "[$HW] is not a valid option for HW try [1-5]"
    exit 1
fi

for ((prev_hw=1; prev_hw < $HW; prev_hw++))
do
    if [ ! -d /notebooks/hw${prev_hw} ]
    then
        echo "You should probably do homework ${prev_hw} first!"
        exit 1
    fi
    echo "Ensuring utils from hw${prev_hw} are present"
    if [ -d /src/notebooks/utils/hw${prev_hw} ]
    then
        cp -rn /src/notebooks/utils/hw${prev_hw}/ /notebooks/utils
    fi
done

cp -rn /src/notebooks/hw${HW} /notebooks
jupyter notebook --ip 0.0.0.0 --NotebookApp.token="$PASSWD" /notebooks
