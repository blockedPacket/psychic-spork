#!/bin/bash

python3 -m venv .venv || exit 1
source .venv/bin/activate
pip install -r requirements.txt
