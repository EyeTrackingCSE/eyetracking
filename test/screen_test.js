const addon = require('../cpp/build/Release/focus.node');

function printScreen(screen) {
    console.log(`W ${screen.GetWidth()}`);
    console.log(`H ${screen.GetHeight()}`);
}

console.log("Screen test");

const screen = new addon.Screen(1920.0, 1080.0);

printScreen(screen);

// Make a rectangle of 500x500 on the top left of the screen
let id = 0;
screen.AddRectangle(0, 0, 0, 500, 500);