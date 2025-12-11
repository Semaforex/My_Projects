Here is the translated content formatted as raw Markdown code for you to copy and paste.

````markdown
# KNR-Keyboard-Detection

## Project Map
Below is a map to help you locate the most important files in the repository.

```python
KNR-Keyboard-Detection
│   README.md
│
└───data_generation
│   │   only_keyboard.blend # environment for generating keyboard and full-keyboard mask
│   │   keys_and_keyboard.blend # environment for generating keyboard and mask highlighting individual keys
│   │
│   └───random_textures # random textures for keyboard surroundings
│   │    │   gray_0.png
│   │    │   gray_1.png
│   │    │   ...
│   │
│   └───data # empty folders for generated data (data size was too large for GitHub, need to figure out sharing)
│
└───code
    │   random_textures.ipynb # generating random textures
    │   mask_operation.ipynb # attempts to find keys based on the full-keyboard mask
    │   finding_keys.ipynb # attempts to find keys based on the full-keyboard mask
    │   just_unet.ipynb # latest solution idea, unfortunately too heavy for my computer
    │   pytorch_knr.yml # lists required packages
````

## How to Generate Data?

1.  Open one of the `.blend` files.
2.  In the **Scripting** tab, the `generating_data_cope.py` script should appear.
3.  Change the value of the `WORK_DIR` variable to the path of the folder containing the `.blend` file.
4.  On the first run of the script in a given Blender session, **uncomment line 220**: `update_file_paths()`. It should be commented out for subsequent runs.
5.  In the **Compositing** tab, inside the **File Output** node, change the path in **Base Path**. The generated data will be saved in this folder.

> [\!NOTE]
> One image may take approx. 10 seconds to render. The engine used puts a significant load on the GPU.
