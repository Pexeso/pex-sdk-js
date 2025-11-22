#!/bin/sh

node-gyp configure --debug -- -f gyp.generator.compile_commands_json.py
cp build/Debug/compile_commands.json ./
