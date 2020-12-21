const addon = require('./cpp/build/Release/focus.node');

// console.log(addon.hello());

const screen = new addon.Screen(1920.0, 1080.0);

console.log(screen.GetHeight());

screen.SetHeight(20000.0);

console.log(screen.GetHeight());