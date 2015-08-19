var express = require('express');
var app = express();
var http = require('http').Server(app);
var path = require('path');

var io = require('socket.io')(http);

app.use(express.static('/'));
app.use(express.static('views'));
app.use(express.static('public'));

app.get('/', function(req, res){
  res.sendFile('index.html');
});

io.on('connection', function(socket){
	var address = socket.handshake.address;
	console.log(address);
  console.log('New user connected.');

  //-----------------------------------------------

  var zmq = require('zmq');
  console.log("Connecting to ZMQ-node server...");
  var requester = zmq.socket('req');

  requester.on("message", function(reply){
    //console.log(reply.toString());
    var x = reply.toString().split(":");
    switch (x[0]){
      case "1":
      console.log(x);
      socket.emit('created', x);
      break;
      case "2":
      socket.emit('loginSuccess', x);
      break;
      case "3":
      socket.emit('cloned', x);
      break;
    }
  });

requester.connect("tcp://192.168.179.93:6666");
console.log("Sending request …");


  //-----------------------------------------------

  socket.on("createData",function(data){
    console.log(data);
    if (data.msg == "1") {
      requester.send("1:1:"+data.name);
    }
    else if (data.msg == "2") {
      requester.send("1:2:"+data.name);
    }
    else if (data.msg == "3") {
      requester.send("1:3:"+data.name);
    }
    else if (data.msg == "4") {
      requester.send("1:4:"+data.name);
    }
    else if (data.msg == "5") {
      requester.send("1:5:"+data.name);
    }
    else if (data.msg == "6") {
      requester.send("1:6:"+data.name);
    }
    else if (data.msg == "7") {
      requester.send("1:7:"+data.name);
    }
   
 });

  socket.on("editData",function(data){
    requester.send("2:"+data.id+":"+data.content);
  });

  socket.on("deleteData",function(data){
    requester.send("3:"+data);
  });

  socket.on("queryData",function(data){
    console.log(data);
    requester.send("4:"+data);
  });

  socket.on("cloneData",function(data){
    requester.send("5:"+data.id+":"+data.user);
  });

   socket.on("login",function(data){
    requester.send("6:"+data);
  });

  // socket.on("setChatRoom",function(data){
  //   requester.send("4:"+data);
  // });

  // socket.on("getChatRoom",function(data){
  //   requester.send("5:");
  // });


});

http.listen(3333, function(){
  console.log('listening on *:3333');
});
