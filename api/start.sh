#!/bin/bash
DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [ -d "${DIR}/.venv" ]; then
    source "${DIR}/.venv/bin/activate" || {
        echo "[*] error when creating virtual environment"
        exit 1
    }

else 
    echo "[*] .venv not found" 
    exit 1
fi

cd "${DIR}" || exit 1 
gunicorn --bind 0.0.0.0:13000 wsgi:app 

