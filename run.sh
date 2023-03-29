#!/bin/bash

export AE_SDK_LOG_LEVEL="debug"

node-gyp build
exec node
