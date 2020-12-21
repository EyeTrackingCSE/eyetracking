const Screen = require('../index');

function printScreen(screen) {
    console.log(`W ${screen.GetWidth()}`);
    console.log(`H ${screen.GetHeight()}`);
}

console.log("Screen test");

const screen = new Screen(1920.0, 1080.0);

printScreen(screen);

// Make a rectangle of 500x500 on the top left of the screen
let size = 500;
let array = [
    {
        id: 0,
        x: 0,
        y: 0,
        width: screen.GetWidth(),
        height: screen.GetHeight()
    }
]
screen.AddRectangles(array);
screen.Listen();