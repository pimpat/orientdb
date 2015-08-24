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
      // create data callback
      console.log(x);
      socket.emit('created', x);
      break;
      case "2":
      // login callback
      socket.emit('loginSuccess', x);
      break;
      case "3":
      // clone callback
      socket.emit('cloned', x);
      break;
      case "4":
      // get datalist
      console.log(x);
      socket.emit('onDatalists', x);
      break;
      case "5":
      // get full data
      console.log(x);
      socket.emit('genDatafromDatalists', x);
      break;
      case "6":
      // delete data callback
      console.log(x);
      socket.emit('deleteCallback', x);
      break;
      case "404":
      // notification to web
      console.log(x);
      socket.emit('notification', x[1]);
      break;
    }
  });

  requester.connect("tcp://localhost:6666");
  console.log("Sending request …");


  //-----------------------------------------------

  socket.on("createData",function(data){
    console.log(data);
    if (data.msg == "1") {
      requester.send("1:"+data.msg+":"+data.name+":org");
    }
    else{
      requester.send("1:"+data.msg+":"+data.name+":"+data.upperid);
    }
  });

  socket.on("editData",function(data){
    requester.send("2:"+data.id+":"+data.content);
  });

  socket.on("deleteData",function(data){
    requester.send("3:"+data.objId+":"+data.upperId+":"+data.userId);
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

  socket.on("getDatalist",function(data){
    if (data.msg == "8") {
      requester.send("7:8:org");
    }
    else{
      requester.send("7:"+data.msg+":"+data.id);
    }
  });

  socket.on("getDatafromDatalist",function(data){
    console.log(data);
    requester.send("8:"+data);
  });

  socket.on("addEdge",function(data){
    requester.send("9:"+data.msg+":"+data.Ldata+":"+data.Rdata);
  }); 

});

http.listen(3333, function(){
  console.log('listening on *:3333');
});
