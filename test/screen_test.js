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
let size = 500;
screen.AddRectangle(0, 0, 0, size, size);
screen.AddRectangle(1, screen.GetWidth() - size, 0, size, size);
screen.AddRectangle(2, 0, screen.GetHeight() - size, 500, 500);
screen.AddRectangle(3, screen.GetWidth() - size, screen.GetHeight() - size, 500, 500);

screen.Listen();
