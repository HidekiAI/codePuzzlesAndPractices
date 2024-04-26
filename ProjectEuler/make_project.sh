#!/bin/bash

_TARGET=$1

if [ -z $_TARGET ]; then
    echo "Usage: $0 <project_name>"
    exit 1
fi

if [ -d $_TARGET ]; then
    echo "Project $_TARGET already exists"
    exit 1
fi

mkdir $_TARGET
cd $_TARGET
cargo new $_TARGET
mv $_TARGET rust

mkdir cpp
touch cpp/main.cpp

git add .