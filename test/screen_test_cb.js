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
    { id: 0, x: 0, y: 0, width: size, height: size },
    { id: 1, x: screen.GetWidth() - size, y: 0, width: size, height: size },
    { id: 2, x: 0, y: screen.GetHeight() - size, width: size, height: size },
    { id: 3, x: screen.GetWidth() - size, y: screen.GetHeight() - size, width: size, height: size },
]
screen.AddRectangles(array);

screen.Listen((id, hasFocus, timestamp) => {
    console.log(`
        id: ${id},
        hasFocus: ${hasFocus},
        timestamp: ${timestamp}
    `);
});
