const fork = require('child_process').fork;

const node = fork("./screen_test_event2.js");
node.on('message', (msg)=>{

    console.log(typeof msg);
    console.log(msg);

});
