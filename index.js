const addon = require('./cpp/build/Release/focus.node');

function printScreen(screen) {
    console.log(`W ${screen.GetWidth()}`);
    console.log(`H ${screen.GetHeight()}`);
}

const screen = new addon.Screen(1920.0, 1080.0);

printScreen(screen);

screen.SetWidth(2560.0);
screen.SetHeight(1440.0);
printScreen(screen);

// console.log(screen.GetHeight());