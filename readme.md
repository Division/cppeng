Engine info
=============

This is a not-for-production rendering engine. I work on during my free time to improve my programming skills.
Here's how it works.

Lighting
============= 

Basic tiled forward shading is implemented. Lights are assigned on CPU by projection light AABB on the grid.
There is a buffer (texture buffer) to store screen grid cells, every cell contains offset in the light data buffer and count of point/spot/projectors/decals encoded into G and B channels.
The second texture buffer contains indices of the corresponding lights/decals/projectors.
The light / projector data is stored in UBO array.

Shadows
=============
Regular shadow mapping by rendering depth maps into the atlas (all lights are the same size in the atlas). Point light shadows are rendered in the same way as spot - in a single direction (no cubemaps, for simplicity)
 
Decals and texture projection light sources
=============

Decals and projectors are implemented in the same way as lights. The only difference is that decal substitutes the diffuse color (normals can be also done easily) and contain the projection matrix and atlas offset in their UBO structure.

Render Pipeline
=============
Visible objects add themselves into the appropriate render queue (Opaque/Transparent etc)
At the frame start, all the object and light data are uploaded into corresponding UBO/texture buffers.  
Depth only pass goes first. Then depth maps for visible light sources are rendered into the atlas.  
Then the scene is drawn into the offscreen framebuffer. 
Finally the full screen quad is drawn (post effect to be done)
 
Buffers
=============
The engine has high-level wrappers for OpenGL buffers that also helps to work with limited size uniform buffers (usually 64kb).
`MultiVertexBufferObject` automatically switches to the next VBO for writing data if it's current buffer doesn't have enough space. 

Skinning
=============

Matrices (with blending and interpolation) are calculated on CPU. Then they are stored in the UBO array of matrices. When rendering a skinned mesh object, buffer range is bound by calling `glBindBufferRange`       
There is support for having geometry/skinning data and animations in the separate files.

Model loading
=============

There is an export script `collada -> custom binary format`.
The binary format structure:
1) `[uint32]` size of the JSON header in bytes
2) `[string]` JSON header that describes included components (e.g. geometry, scene hierarchy, animations, skinning etc)
3) binary data as described in JSON

Export script allows to skip geometry and keep `Maya` reference node IDs during export - to use `Maya` as a basic level editor.
Also it's possible to export skinning mesh model as `mesh + joint data only` or `joint animations only`.

Supported Platforms
=============
The engine doesn't contain platform specific code.
Currently supported platforms is Mac OS and partial support for WebGL2 via emscripten (no shadows support in emscripten build).  
Windows support is easy to add, need setup cmake for it.

TODO
=============

* Layers for objects, lights/projectors and cameras
* scene spatial partitioning
* improve light tiles assignment (with compute shaders or at least split into multiple CPU threads)
* Decouple material and object params (e.g. skinning matrices)
* Flare billboard for lights
* sort render queues
* OpenGL state cache
* Bloom