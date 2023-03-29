#!/bin/sh

node-gyp configure --debug -- -f gyp.generator.compile_commands_json.py
mv Debug/compile_commands.json ./
rm -rf Release Debug
