# eyetracking

Eye tracking in Node.js.

This NPM module enables users who own a [Tobii Eye Tracker](https://www.tobii.com/) to use the [Interaction Library](https://developer.tobii.com/product-integration/interaction-library/) from Node.js.

This enables Node.js to detect when the user focuses their gaze in certain regions of the screen space.

## Example

Example using a 1920x1080 screen, and creating 4 500x500 rectangles on each corner of the screen space. The eyetracking module will detect when the user focuses their gaze on these regions, and executes the provided callback function.

```
A screen with 4 "corner" interactors A, B, C and D:
                   1920
  +-------+-------------------+-------+
  |       |                   |       |
  |   A   | 500           500 |   B   |
  |       |                   |       |
  +-------+                   +-------+
  |  500                         500  |
  |                                   | 1080
  |  500                         500  |
  +-------+                   +-------+
  |       |                   |       |
  |   C   | 500          500  |   D   |
  |       |                   |       |
  +-------+------------------ +-------+
```

```javascript
const eyetracking = require('eyetracking');

let width = 1920;
let height = 1080;
const screen = new eyetracking(width, height);

// Make a rectangle of 500x500 on each corner of the screen.
let size = 500;
let array = [
    { id: 0, x: 0, y: 0, width: size, height: size },
    { id: 1, x: screen.GetWidth() - size, y: 0, width: size, height: size },
    { id: 2, x: 0, y: screen.GetHeight() - size, width: size, height: size },
    { id: 3, x: screen.GetWidth() - size, y: screen.GetHeight() - size, width: size, height: size },
];


screen.AddRectangles(array);

screen.Listen((id, hasFocus, timestamp) => {
    console.log(`Rectangle ${id} focused at ${timestamp}`);
});
```