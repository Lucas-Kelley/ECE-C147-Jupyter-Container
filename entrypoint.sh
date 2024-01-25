#!/bin/bash

jupyter notebook --ip 0.0.0.0 --NotebookApp.token="$1" /notebook
