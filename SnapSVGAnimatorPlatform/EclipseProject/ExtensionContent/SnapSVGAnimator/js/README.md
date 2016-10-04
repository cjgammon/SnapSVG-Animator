
### footer.js/banner.js  
  - boilerplate for packaging library. banner.js goes at top, footer.js goes at bottom

### component.js  
  - `SVGAnim`, The base animation object for instantiating your timeline animation.

### commands.js  
  - Commands for placing removing or updating objects to the stage, defined in the timeline JSON.
* `PlaceObjectCommand`
* `MoveObjectCommand`
* `UpdateObjectCommand`
* `RemoveObjectCommand`
* `UpdateVisibilityCommand`
* `UpdateMaskCommand`
* `ApplyMaskCommand`
* `UpdateColorTransformCommand`

### bitmap.js
  - Bitmap class for drawing bitmap assets as snap.svg image.

### shape.js  
  - Shape class for drawing geometry in snap.svg.

### movieclip.js  
  - MovieClip class displaying nested content with a timeline.

### text.js  
  - Text class displaying text in snap.svg.

### garbagePool.js    
  - helper class for managing and purging empty or unused elements in the DOM.

### resourceManager.js  
  - model class for resources generated from JSON.
  * `getShape()`
  * `getMovieClip()`
  * `getBitmap()`
  * `getText()`
