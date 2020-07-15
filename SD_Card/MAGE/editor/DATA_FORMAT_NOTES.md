# Data format notes:
----

I have _no idea_ what I'm going.

----

The `game.dat` file is a binary file generated by a browser-based application which can be started by opening `index.html` in this directory.

----

## Entities

May have a Sprite

Sprites have
- image: path to image file, relative to `scenario_source_files`,
- imageWidth: uint16_t,
- imageHeight: uint16_t,
- spriteWidth: uint16_t,
- spriteHeight: uint16_t,
- **orientationAxis**: 0 if your image holds directions along the X axis, 1 if it holds directions along the Y axis
- **frameAxis**: 1 if your image holds animation frames along the Y axis, 0 if it holds animation frames along the X axis,
- **orientations**: An array of all directions supported in this image.
    - Orientation array items 0 - 3 are expressed clockwise.
        - index 0: ↑
        - index 1: →
        - index 2: ↓
        - index 3: ←
    - The VALUE AT a particular INDEX holds the orientationAxis offset from which to read that orientation's frames
    - If your artwork is pointing `↑→↓←`, your array would be: [0,1,2,3]
    - If your artwork is pointing `←↓↑→`, your array would be: [2,3,1,0]
    - If you download an image from the internet and the artwork is pointing `↑↗→↘↓↙←↖`, some of those directions are unsupported in our engine(`↗↘↙↖`), your array would be: [0,2,4,6]
    - If your artwork only supports `↓`, your array would be: [0,0,0,0]
        - no artwork for ↑ orientation, fall back to orientationAxis offset 0
        - no artwork for → orientation, fall back to orientationAxis offset 0
        - YES artwork for ↓ orientation, read from orientationAxis offset 0
        - no artwork for ← orientation, fall back to orientationAxis offset 0

----

Task list:
- [x] Decode image formats
    - [x] decode multiple formats into 0xRRGGBBAA values
    - [x] convert #RRGGBB value to 2 byte R5 G6 B5 screen format data
    - [x] 0x0020 aka 32 (the least significant green bit) is used as alpha color
- [ ] Build a manual index of data types
    - [ ] Scenario States
        - [ ] offset
        - [ ] how many
            - [ ] name
            - [ ] setup script?
    - [ ] Tilesets
        - [ ] offset
        - [ ] how many
            - [x] char[16] name
            - [x] uint16_t imageIndex
            - [x] uint16_t imageWidth
            - [x] uint16_t imageHeight
            - [x] uint16_t width
            - [x] uint16_t height
            - [ ] data
                - [ ] Tiled file description output is sparse array. Explicitly list all tiles rather than search for properties by id
                - [ ] 1 byte for first char of type
    - [ ] Maps
        - [ ] offset
        - [ ] how many
            - [ ] name
            - 1 byte to specify number of layers
            - 2 bytes to store map tile width
            - 2 bytes to store map tile height
            - layer format
                - https://doc.mapeditor.org/en/stable/reference/tmx-map-format/#tile-flipping
                - 1 byte for mapTilesetIndex index
                - 2 bytes for tile index
                - 1 byte for tile flags
                    ?
                    ?
                    ?
                    ?
                    ?
                    x-mirror
                    y-mirror
                    diagonal-mirror
            - [ ] layers
            - [ ] EntityInstances
                - [ ] offset
                - [ ] how many
                    - [ ] name
                    - [ ] type
                    - [ ] collision
                    - [ ] onHack
                    - [ ] onInteract
                    - [ ] sprite
                        - [ ] file
                        - [ ] orientations
                        - [ ] dimensions
                        - [ ] animations
                    - [ ] position
                    - [ ] state - Is this a byte address?
                    - [ ] direction
                    - [ ] onHack
                    - [ ] onInteract

