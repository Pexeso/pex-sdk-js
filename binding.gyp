{
  "targets": [
    {
      "target_name": "pexae",
      "sources": [ "init.c", "pexsearch.c" ],
      "libraries": [
        "<!(pkg-config --libs pexae)"
      ]
    }
  ]
}
