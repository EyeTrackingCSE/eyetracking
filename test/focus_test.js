const eyetracking = require('../index');

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