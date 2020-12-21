{
  "targets": [
    {
      "target_name": "focus",
      "sources": [
        "main.cc",
        "screen.cc"
      ],
      "conditions": [
        [
          "OS==\"win\"",
          {
            "libraries": [
              "<(module_root_dir)/tobii/lib/x64/tobii_interaction_lib.lib",
              "<(module_root_dir)/tobii/lib/x64/tobii_stream_engine.lib"
            ],
            "include_dirs": [
              "<(module_root_dir)/tobii/include"
            ],
            "copies": [
              {
                "destination": "<(module_root_dir)/build/Release/",
                "files": [
                  "<(module_root_dir)/tobii/lib/x64/tobii_interaction_lib.dll",
                  "<(module_root_dir)/tobii/lib/x64/tobii_stream_engine.dll"

                ]
              }
            ]
          }
        ]
      ]
    }
  ]
}