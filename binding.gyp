{
  "targets": [
    {
      "target_name": "pex",
      "sources": [
        "baseworker.cc",
        "connectworker.cc",
        "defer.cc",
        "fingerprint.cc",
        "fingerprinter.cc",
        "fingerprintworker.cc",
        "init.cc",
        "pexsearch.cc",
        "archiveworker.cc",
        "ingestworker.cc",
        "privatesearch.cc",
        "searchworker.cc",
        "error.cc",
        "lister.cc",
        "listworker.cc",
        "getworker.cc"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include.replace(' ', '\\\\\\ ')\")",
      ],
      "libraries": [
        "-Wl,-rpath,/usr/local/lib",
        "-Wl,-rpath,/usr/lib",
        "<!(pkg-config --libs pexsdk)"
      ],
      "defines": [
        "NAPI_DISABLE_CPP_EXCEPTIONS",
        "NAPI_ADDON_API_ENABLE_MAYBE",
      ]
    }
  ]
}
