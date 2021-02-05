const Screen = require('../index');


const screen = new Screen(1920.0, 1080.0);


screen.ListenGazePoint((x, y, validity, timestamp) => {
    console.log(`[${x}, ${y}]   validity = ${validity}  timestamp = ${timestamp}`);
});