var dataName = "";
var dataID = "";
var dataType = "";
var chatRoom = "";
var verKeeped = "";
var SHA256 = "";
var timestamp = "";
var isdiff = "";
var userHolder = "A, B, C";
var headLineId = "";
var parentId = "";

var Id2append = "";
var currentId = "";
var editbtnElement = "";

var full1 = "";
var full2 = "";
var full3 = "";
var minus1 = "";
var minus2 = "";
var minus3 = "";
var plus1 = "";
var plus2 = "";
var plus3 = "";

var newWay = 0;
var deleteable = 0;
var dataLine = 0;

var socket = io.connect('http://localhost:3333');

socket.on('connect', function() {
	console.log("user conected")
});

socket.on('created', function(data) {
	dataName = data[2];
	dataID = data[3];
	dataType = data[4];
	chatRoom = data[5];
	verKeeped = data[6];
	var type = data[1];
	var appendpoint = "";

	var dataLen = (data.length-8)/12;
	var arr = [7,8,9,10,11,12,13,14,15,16,17,18];

	if (type == "Org") {
		appendpoint = "orgHead";
		dataLine == 0;
	}
	else if (type == "User") {
		appendpoint = "userHead-"+parentId;
	}
	else if (type == "Category") {
		appendpoint = "categoryHead-"+parentId;
	}
	else if (type == "State") {
		appendpoint = "stateHead-"+parentId;
	}
	else if (type == "Task") {
		if (newWay == 1) {appendpoint = "taskHead2-"+parentId; newWay = 0;}
		else {appendpoint = "taskHead-"+parentId;}
		deleteable = 1;
	}
	else if (type == "Subtask") {
		if (newWay == 1) {appendpoint = "subtaskHead2-"+parentId; newWay = 0;}
		else {appendpoint = "subtaskHead-"+parentId;}
		deleteable = 1;
	}    

	if (dataLine == 0) {
		headLineId = data[3];
	};

	console.log("dataID : "+dataID);
	console.log("HeadId : "+headLineId);

  // Create to DataHolder
  $("#"+appendpoint).removeClass("none");
  var parent = $("#"+appendpoint);

  var child = "";
  child += "<li id='data-"+dataID+"'><a id='dname'>"+dataName+"</a>";
  if (deleteable == 1) {
  	child += "<button type='button' class='btn btn-danger btn-xs' onclick=\"deleteData('" + dataID + "')\">delete</button>";
  	deleteable = 0;
  }   
  //child += "<button type='button' class='btn btn-primary btn-xs' onclick=\"clone('" + dataID + "')\">clone</button>";
  child += "<ul id='ul-"+dataID+"'>";
  child += "<li><a class='dataid'>dataID : "+dataID+"</a></li>";
  child += "<li><a>dataType : "+dataType+"</a></li>";
  child += "<li>";
  child += "<a id='chatroom'>Chatroom : "+chatRoom+"</a>";
  //child += "<button type='button' class='btn btn-default btn-xs' data-toggle='modal' data-target='#editData' onclick=\"getdata2modal('" + dataID + '\',\'' + full3 +"')\">edit</button>";
  child += "</li>";
  child += "<li><a>UserHolder : "+userHolder+"</a></li>";
  child += "<li><a>DataHolder</a>";
  child += "<ul id='dataHolder-"+dataID+"'>";
  child += "<li><a>versionKeeped : "+verKeeped+"</a></li>";

  child += "</ul>";
  child += "</li>";
  child += "</ul>";
  child += "</li>";
  child += "</ul>";
  child += "</li>";

  var strVar1 = "";
  var strVar2 = "";
  var strVar3 = "";
  var strVar4 = "";
  var strVar5 = "";
  var strVar6 = "";
  var strVar7 = "";

  strVar1 += "<li>";
  strVar1 += "<a>User</a>";
  strVar1 += "<button type='button' class='btn btn-default btn-xs' onclick='openform(this)'>";
  strVar1 += "<span class='glyphicon glyphicon-plus' aria-hidden='true'></span>";
  strVar1 += "</button>";
  strVar1 += "<!-- form -->";
  strVar1 += "<div class='inline none'>";
  strVar1 += "<input class='form-control inline input-sm custom-size' id='userInput-"+dataID+"' type='text'>";
  strVar1 += "<button type='button' class='btn btn-default btn-xs' onclick='req(2, this)'>add</button>";
  strVar1 += "</div>";
  strVar1 += "<ul id='userHead-"+dataID+"' class='none'>";
  

  strVar2 += "<li>";
  strVar2 += "<a>Category</a>";
  strVar2 += "<button type='button' class='btn btn-default btn-xs' onclick='openform(this)'>";
  strVar2 += "<span class='glyphicon glyphicon-plus' aria-hidden='true'></span>";
  strVar2 += "</button>";
  strVar2 += "<!-- form -->";
  strVar2 += "<div class='inline none'>";
  strVar2 += "<input class='form-control inline input-sm custom-size' id='categoryInput-"+dataID+"' type='text'>";
  strVar2 += "<button type='button' class='btn btn-default btn-xs' onclick='req(3, this)'>add</button>";
  strVar2 += "</div>";
  strVar2 += "<ul id='categoryHead-"+dataID+"' class='none'>";


  strVar3 += "<li>";
  strVar3 += "<a>State</a>";
  strVar3 += "<button type='button' class='btn btn-default btn-xs' onclick='openform(this)'>";
  strVar3 += "<span class='glyphicon glyphicon-plus' aria-hidden='true'></span>";
  strVar3 += "</button>";
  strVar3 += "<!-- form -->";
  strVar3 += "<div class='inline none'>";
  strVar3 += "<input class='form-control inline input-sm custom-size' id='stateInput-"+dataID+"' type='text'>";
  strVar3 += "<button type='button' class='btn btn-default btn-xs' onclick='req(4, this)'>add</button>";
  strVar3 += "</div>";
  strVar3 += "<ul id='stateHead-"+dataID+"' class='none'>";


  strVar4 += "<li>";
  strVar4 += "<a>Task</a>";
  strVar4 += "<button type='button' class='btn btn-default btn-xs' onclick='openform(this)'>";
  strVar4 += "<span class='glyphicon glyphicon-plus' aria-hidden='true'></span>";
  strVar4 += "</button>";
  strVar4 += "<!-- form -->";
  strVar4 += "<div class='inline none'>";
  strVar4 += "<input class='form-control inline input-sm custom-size' id='taskInput-"+dataID+"' type='text'>";
  strVar4 += "<button type='button' class='btn btn-default btn-xs' onclick='req(5, this)'>add</button>";
  strVar4 += "</div>";
  strVar4 += "<ul id='taskHead-"+dataID+"' class='none'>";


  strVar5 += "<li>";
  strVar5 += "<a>Subtask</a>";
  strVar5 += "<button type='button' class='btn btn-default btn-xs' onclick='openform(this)'>";
  strVar5 += "<span class='glyphicon glyphicon-plus' aria-hidden='true'></span>";
  strVar5 += "</button>";
  strVar5 += "<!-- form -->";
  strVar5 += "<div class='inline none'>";
  strVar5 += "<input class='form-control inline input-sm custom-size' id='subtaskInput-"+dataID+"' type='text'>";
  strVar5 += "<button type='button' class='btn btn-default btn-xs' onclick='req(6, this)'>add</button>";
  strVar5 += "</div>";
  strVar5 += "<ul id='subtaskHead-"+dataID+"' class>";

  strVar5 += "</ul>";
  strVar5 += "</li>";


  strVar4 += "</ul>";
  strVar4 += "</li>";


  strVar3 += "</ul>";
  strVar3 += "</li>";


  strVar6 += "<li>";
  strVar6 += "<a>Task</a><button type='button' class='btn btn-default btn-xs' onclick='openform(this)'>";
  strVar6 += "<span class='glyphicon glyphicon-plus' aria-hidden='true'></span>";
  strVar6 += "</button>";
  strVar6 += "<!-- form -->";
  strVar6 += "<div class='inline none'>";
  strVar6 += "<input class='form-control inline input-sm custom-size' id='taskInput2-"+dataID+"' type='text'>";
  strVar6 += "<button type='button' class='btn btn-default btn-xs' onclick='req(7, this)'>add</button>";
  strVar6 += "</div>";
  strVar6 += "<ul id='taskHead2-"+dataID+"' class='none'>";
  

  strVar7 += "<li>";
  strVar7 += "<a>Subtask</a>";
  strVar7 += "<button type='button' class='btn btn-default btn-xs' onclick='openform(this)'>";
  strVar7 += "<span class='glyphicon glyphicon-plus' aria-hidden='true'></span>";
  strVar7 += "</button>";
  strVar7 += "<!-- form -->";
  strVar7 += "<div class='inline none'>";
  strVar7 += "<input class='form-control inline input-sm custom-size' id='subtaskInput2-"+dataID+"' type='text'>";
  strVar7 += "<button type='button' class='btn btn-default btn-xs' onclick='req(8, this)'>add</button>";
  strVar7 += "</div>";
  strVar7 += "<ul id='subtaskHead2-"+dataID+"'>";

  strVar7 += "</ul>";
  strVar7 += "</li>";


  strVar6 += "</ul>";
  strVar6 += "</li>";


  strVar2 += "</ul>";
  strVar2 += "</li>";


  strVar1 += "</ul>";
  strVar1 += "</li>";

  parent.prepend(child);

  if (type == "Org") {
  	$("#ul-"+dataID).append(strVar1);
  	$("#userHead-"+dataID).append(strVar2);
  	$("#categoryHead-"+dataID).append(strVar3);
  	$("#stateHead-"+dataID).append(strVar4);
  	$("#taskHead-"+dataID).append(strVar5);
  	$("#categoryHead-"+dataID).append(strVar6);
  	$("#taskHead2-"+dataID).append(strVar7);
  	dataLine = 1;
  }
  else if (type == "User") {
  	$("#ul-"+dataID).append(strVar2);
  	$("#categoryHead-"+dataID).append(strVar3);
  	$("#stateHead-"+dataID).append(strVar4);
  	$("#taskHead-"+dataID).append(strVar5);
  	$("#categoryHead-"+dataID).append(strVar6);
  	$("#taskHead2-"+dataID).append(strVar7);
  }
  else if (type == "Category") {
  	$("#ul-"+dataID).append(strVar3);
  	$("#stateHead-"+dataID).append(strVar4);
  	$("#taskHead-"+dataID).append(strVar5);
  	$("#ul-"+dataID).append(strVar6);
  	$("#taskHead2-"+dataID).append(strVar7);
  }
  else if (type == "State") {
  	$("#ul-"+dataID).append(strVar4);
  	$("#taskHead-"+dataID).append(strVar5);
  }
  else if (type == "Task") {
  	$("#ul-"+dataID).append(strVar7);
  }  


  //$("#ul-"+dataID).append(strVar1);
  // if (dataLine == 0) {
  //   $("#ul-"+dataID).append(strVar);
  //   dataLine = 1;
  // };
  

  for (var i = 1; i <= dataLen; i++) {
	// Create each DataContent
	if (i == 1) {
		SHA256 = data[arr[0]];
		timestamp = data[arr[1]];
		isdiff = data[arr[2]];
		full1 = data[arr[3]];
		full2 = data[arr[4]];
		full3 = data[arr[5]];
		minus1 = data[arr[6]];
		minus2 = data[arr[7]];
		minus3 = data[arr[8]];
		plus1 = data[arr[9]];
		plus2 = data[arr[10]];
		plus3 = data[arr[11]];
	}
	else{     
		arr[0] = arr[0]+12;
		arr[1] = arr[1]+12;
		arr[2] = arr[2]+12;
		arr[3] = arr[3]+12;
		arr[4] = arr[4]+12;
		arr[5] = arr[5]+12;
		arr[6] = arr[6]+12;
		arr[7] = arr[7]+12;
		arr[8] = arr[8]+12;
		arr[9] = arr[9]+12;
		arr[10] = arr[10]+12;
		arr[11] = arr[11]+12;

		SHA256 = data[arr[0]];
		timestamp = data[arr[1]];
		isdiff = data[arr[2]];
		full1 = data[arr[3]];
		full2 = data[arr[4]];
		full3 = data[arr[5]];
		minus1 = data[arr[6]];
		minus2 = data[arr[7]];
		minus3 = data[arr[8]];
		plus1 = data[arr[9]];
		plus2 = data[arr[10]];
		plus3 = data[arr[11]];
	}

	var parent2 = $("#dataHolder-"+dataID);
	var child2 = "<li><a>DataContent</a>";
	child2 += "<ul>";
	child2 += "<li><a>SHA256 : "+SHA256+"</a></li>";
	child2 += "<li><a>Timestamp : "+timestamp+"</a></li>";
	child2 += "<li><a>isDiff : "+isdiff+"</a></li>";
	child2 += "<li id='fulldata-"+dataID+"'><a>Full</a>";
	child2 += "<ul>";
	child2 += "<li><a>SchemaCode : "+full1+"</a></li>";
	child2 += "<li><a>byteCount : "+full2+"</a></li>";
	if (i == dataLen) {
		child2 += "<li>data : <xmp>"+full3+"</xmp><button type='button' class='btn btn-default btn-xs' data-toggle='modal' data-target='#editData' onclick=\"getdata2modal('" + dataID + "', this)\">edit</button></li>";
	}
	else{
		child2 += "<li>data : <xmp>"+full3+"</xmp></li>";
	}
	child2 += "</ul>";
	child2 += "</li>";
	child2 += "<li id='minusdata-"+dataID+"'><a>Minus</a>";
	child2 += "<ul>";
	child2 += "<li><a>SchemaCode : "+minus1+"</a></li>";
	child2 += "<li><a>byteCount : "+minus2+"</a></li>";
	child2 += "<li>data : <xmp>"+minus3+"</xmp></li>";
	child2 += "</ul>";
	child2 += "</li>";
	child2 += "<li id='plusdata-"+dataID+"'><a>Plus</a>";
	child2 += "<ul>";
	child2 += "<li><a>SchemaCode : "+plus1+"</a></li>";
	child2 += "<li><a>byteCount : "+plus2+"</a></li>";
	child2 += "<li>data : <xmp>"+plus3+"</xmp></li>";
	child2 += "</ul>";
	child2 += "</li>";
	child2 += "</ul>";
	child2 += "</li>";

	parent2.append(child2);
}
		// Create Tree UI
		$("#ul-"+dataID).treemenu({delay:200}).openActive();
	});

socket.on('loginSuccess', function(data) {
	alert(data[1]);
});

socket.on('cloned', function(data) {
	alert(data[1]);
});

socket.on('notification', function(data) {
	alert(data);
});

socket.on('onDatalists', function(data) {
	console.log(data);
	var len = (data.length-3)/2;
	var type = data[1];
	var parent = "";
	var code = "";
	var arr = [2, 3];
	var listID = "";
	var listName = "";
	var appendpoint = "";
	var title = "";

	if (type == "Org") {
		parent = "orgHead-"+Id2append;
		code = "1";
	}
	else if (type == "User") {
		parent = "userHead-";
		appendpoint = "#orgHead-"+Id2append;
		title = "User";
		code = "2";
	}
	else if (type == "Category") {
		parent = "categoryHead-";
		appendpoint = "#userHead-"+Id2append;
		title = "Category";
		code = "3";
	}
	else if (type == "State") {
		parent = "stateHead-";
		appendpoint = "#categoryHead-"+Id2append;
		title = "State";
		code = "4";
	}
	else if (type == "Task") {
		parent = "taskHead-";
		appendpoint = "#stateHead-"+Id2append;
		title = "Task";
		code = "5";
	}
	else if (type == "Subtask") {
		parent = "subtaskHead-";
		appendpoint = "#taskHead-"+Id2append;
		title = "Subtask";
		code = "8";
	}    
	else if (type == "TaskfromCat"){
		parent = "taskHead2-";
		appendpoint = "#categoryHead-"+Id2append;
		title = "Task";
		code = "9";
	}
	else if (type == "SubtaskfromCat"){
		parent = "subtaskHead2-";
		appendpoint = "#taskHead2-"+Id2append;
		title = "Subtask";
		code = "10";
	}
	
	for (var i = 1; i <= len; i++) {
		if (i == 1) {
			listID = data[arr[0]];
			listName = data[arr[1]];
		}
		else{     
			arr[0] = arr[0]+2;
			arr[1] = arr[1]+2;

			listID = data[arr[0]];
			listName = data[arr[1]];
		}

		var child = "";
		var child2 = "";

		if (type == "Subtask") {
			child2 += "<li><a id='a-"+listID+"' onclick=\"query("+code+", '', this)\">"+listName+"</a>";
		}
		else{
			child2 += "<li id='data-"+listID+"'><a id='a-"+listID+"' onclick=\"query(" + code + ',\'' + listID +"', this)\">"+listName+"</a>";
		}
		child2 += "<ul id='"+parent+listID+"'></ul>";
		child2 += "</li>";


		if (type != "Org") {
			if (i == 1) {
				child += "<li><a>"+title+"</a>";	
				child += "<ul id='"+parent+currentId+"'>";
				child += "</ul>";
				child += "</li>";	
			}
			console.log("--> #"+parent+listID);

			$(appendpoint).append(child);
			$("#"+parent+currentId).append(child2);
		}
		else{
			$("#orgHead").append(child2);
		}
	}
	// $(".tree").treemenu({delay:200}).openActive();

});

socket.on('genDatafromDatalists', function(data) {
	console.log(data);
	dataName = data[2];
	dataID = data[3];
	dataType = data[4];
	chatRoom = data[5];
	verKeeped = data[6];
	var type = data[1];
	var appendpoint = "";

	var dataLen = (data.length-8)/12;
	var arr = [7,8,9,10,11,12,13,14,15,16,17,18];

	if (type == "Org") {
		appendpoint = "orgHead-"+currentId;
		dataLine == 0;
	}
	else if (type == "User") {
		appendpoint = "userHead-"+currentId;
	}
	else if (type == "Category") {
		appendpoint = "categoryHead-"+currentId;
	}
	else if (type == "State") {
		appendpoint = "stateHead-"+currentId;
	}
	else if (type == "Task") {
		if (newWay == 1) {
			appendpoint = "taskHead2-"+currentId;
			newWay = 0;
		}
		else{
			appendpoint = "taskHead-"+currentId;
		}
	}
	else if (type == "Subtask") {
		if (newWay == 1) {
			appendpoint = "subtaskHead2-"+currentId;
			newWay = 0;
		}
		else{
			appendpoint = "subtaskHead-"+currentId;
		}
	}   

  // Create to DataHolder
  $("#"+appendpoint).removeClass("none");
  var parent = $("#"+appendpoint);

  var child = "";
  
  //child += "<button type='button' class='btn btn-primary btn-xs' onclick=\"clone('" + dataID + "')\">clone</button>";

  child += "<li><a class='dataid'>dataID : "+dataID+"</a></li>";
  child += "<li><a>dataType : "+dataType+"</a></li>";
  child += "<li>";
  child += "<a id='chatroom'>Chatroom : "+chatRoom+"</a>";
  //child += "<button type='button' class='btn btn-default btn-xs' data-toggle='modal' data-target='#editData' onclick=\"getdata2modal('" + dataID + '\',\'' + full3 +"')\">edit</button>";
  child += "</li>";
  child += "<li><a>UserHolder : "+userHolder+"</a></li>";
  child += "<li><a>DataHolder</a>";
  child += "<ul id='dataHolder-"+dataID+"'>";
  child += "<li><a>versionKeeped : "+verKeeped+"</a></li>";

  child += "</ul>";
  child += "</li>";
  child += "</ul>";
  child += "</li>";

  parent.prepend(child);

  for (var i = 1; i <= dataLen; i++) {
	// Create each DataContent
	if (i == 1) {
		SHA256 = data[arr[0]];
		timestamp = data[arr[1]];
		isdiff = data[arr[2]];
		full1 = data[arr[3]];
		full2 = data[arr[4]];
		full3 = data[arr[5]];
		minus1 = data[arr[6]];
		minus2 = data[arr[7]];
		minus3 = data[arr[8]];
		plus1 = data[arr[9]];
		plus2 = data[arr[10]];
		plus3 = data[arr[11]];
	}
	else{     
		arr[0] = arr[0]+12;
		arr[1] = arr[1]+12;
		arr[2] = arr[2]+12;
		arr[3] = arr[3]+12;
		arr[4] = arr[4]+12;
		arr[5] = arr[5]+12;
		arr[6] = arr[6]+12;
		arr[7] = arr[7]+12;
		arr[8] = arr[8]+12;
		arr[9] = arr[9]+12;
		arr[10] = arr[10]+12;
		arr[11] = arr[11]+12;

		SHA256 = data[arr[0]];
		timestamp = data[arr[1]];
		isdiff = data[arr[2]];
		full1 = data[arr[3]];
		full2 = data[arr[4]];
		full3 = data[arr[5]];
		minus1 = data[arr[6]];
		minus2 = data[arr[7]];
		minus3 = data[arr[8]];
		plus1 = data[arr[9]];
		plus2 = data[arr[10]];
		plus3 = data[arr[11]];
	}

	var parent2 = $("#dataHolder-"+dataID);
	var child2 = "<li><a>DataContent</a>";
	child2 += "<ul>";
	child2 += "<li><a>SHA256 : "+SHA256+"</a></li>";
	child2 += "<li><a>Timestamp : "+timestamp+"</a></li>";
	child2 += "<li><a>isDiff : "+isdiff+"</a></li>";
	child2 += "<li id='fulldata-"+dataID+"'><a>Full</a>";
	child2 += "<ul>";
	child2 += "<li><a>SchemaCode : "+full1+"</a></li>";
	child2 += "<li><a>byteCount : "+full2+"</a></li>";
	if (i == dataLen) {
		child2 += "<li>data : <xmp>"+full3+"</xmp><button type='button' class='btn btn-default btn-xs' data-toggle='modal' data-target='#editData' onclick=\"getdata2modal('" + dataID + "', this)\">edit</button></li>";
	}
	else{
		child2 += "<li>data : <xmp>"+full3+"</xmp></li>";
	}
	child2 += "</ul>";
	child2 += "</li>";
	child2 += "<li id='minusdata-"+dataID+"'><a>Minus</a>";
	child2 += "<ul>";
	child2 += "<li><a>SchemaCode : "+minus1+"</a></li>";
	child2 += "<li><a>byteCount : "+minus2+"</a></li>";
	child2 += "<li>data : <xmp>"+minus3+"</xmp></li>";
	child2 += "</ul>";
	child2 += "</li>";
	child2 += "<li id='plusdata-"+dataID+"'><a>Plus</a>";
	child2 += "<ul>";
	child2 += "<li><a>SchemaCode : "+plus1+"</a></li>";
	child2 += "<li><a>byteCount : "+plus2+"</a></li>";
	child2 += "<li>data : <xmp>"+plus3+"</xmp></li>";
	child2 += "</ul>";
	child2 += "</li>";
	child2 += "</ul>";
	child2 += "</li>";

	parent2.append(child2);
}
	// Create Tree UI
	$("#dataHolder-"+dataID).treemenu({delay:200}).openActive();
});

function create(val){
	var name = $("#dataName").val();
	if ($("#upperId").length) {
		var upperid = $("#upperId").val();
	}
	if (val == 1) {
	 // create Org
	 socket.emit('createData', {msg: "1", name: name});
	}
	else if (val == 2) {
	 // create User
	 socket.emit('createData', {msg: "2", name: name, upperid: upperid});
	}
	else if (val == 3) {
	 // create Category
	 socket.emit('createData', {msg: "3", name: name, upperid: upperid});
	}
	else if (val == 4) {
	 // create state
	 socket.emit('createData', {msg: "4", name: name, upperid: upperid});
	}
	else if (val == 5) {
	 // create Task
	 socket.emit('createData', {msg: "5", name: name, upperid: upperid});
	}
	else if (val == 6) {
	 // create Subtask
	 socket.emit('createData', {msg: "6", name: name, upperid: upperid});
	}
	else if (val == 7) {
	 // create Task2
	 socket.emit('createData', {msg: "7", name: name, upperid: upperid});
	}
	else if (val == 8) {
	 // create Subtask2
	 socket.emit('createData', {msg: "6", name: name, upperid: upperid});
	}
}

function deleteData(){
	var objId = $("#objectId").val();
	var upperId = $("#deleteUpperId").val();
	var userId = $("#userUpperId").val();
	socket.emit('deleteData', {objId: objId, upperId: upperId, userId: userId});
}

function req(val, ele){
	if (val == 1) {
	 // create Org
	 var name = $(ele).prev("input").val();
	 socket.emit('createData', {msg: "1", name: name});
	}
	else if (val == 2) {
	 // create User
	 // parentId = $(ele).parent().parent().parent().children().first().children().next().text();
	 // console.log("parentID: "+parentId);
	 parentId = $(ele).parent().parent().parent().children().first().children().next().text().slice(9);
	 console.log("parentID: "+parentId);
	 var name = $(ele).prev("input").val();
	 socket.emit('createData', {msg: "2", name: name});
	}
	else if (val == 3) {
	 // create Category
	 parentId = $(ele).parent().parent().parent().children().first().children().next().text().slice(9);
	 console.log("parentID: "+parentId);
	 var name = $(ele).prev("input").val();
	 socket.emit('createData', {msg: "3", name: name});
	}
	else if (val == 4) {
	 // create state
	 parentId = $(ele).parent().parent().parent().children().first().children().next().text().slice(9);
	 console.log("parentID: "+parentId);
	 var name = $(ele).prev("input").val();
	 socket.emit('createData', {msg: "4", name: name});
	}
	else if (val == 5) {
	 // create Task
	 parentId = $(ele).parent().parent().parent().children().first().children().next().text().slice(9);
	 console.log("parentID: "+parentId);
	 var name = $(ele).prev("input").val();
	 socket.emit('createData', {msg: "5", name: name});
	}
	else if (val == 6) {
	 // create Subtask
	 parentId = $(ele).parent().parent().parent().children().first().children().next().text().slice(9);
	 console.log("parentID: "+parentId);
	 var name = $(ele).prev("input").val();
	 socket.emit('createData', {msg: "6", name: name});
	}
	else if (val == 7) {
	 // create Task2
	 parentId = $(ele).parent().parent().parent().children().first().children().next().text().slice(9);
	 console.log("parentID: "+parentId);
	 var name = $(ele).prev("input").val();
	 socket.emit('createData', {msg: "7", name: name});
	 newWay = 1;
	}
	else if (val == 8) {
	 // create Subtask2
	 parentId = $(ele).parent().parent().parent().children().first().children().next().text().slice(9);
	 console.log("parentID: "+parentId);
	 var name = $(ele).prev("input").val();
	 socket.emit('createData', {msg: "6", name: name});
	 newWay = 1;
	}
	else if (val == 9) {
	 // saveDatacontent
	 var content = $("#edit-dataContent").val();
	 var dataid = $("#edit-dataId").text();
	 socket.emit('editData', {id: dataid, content: content});
	 $("#data-"+currentId).children().last().empty();
	 $('#editData').modal('hide');
	}
	else if (val == 10) {
	 // queryOrgFromData
	 socket.emit('queryData', "Hello");
	}
	else if (val == 11) {
	 // Login
	 var name = $("#loginName").val();    
	 socket.emit('login', name);
	 $(".cloneBox").removeClass("none");
	 $(".queryBox").removeClass("none");
	 $("#loginName").val("Login as "+name);
	 $("#loginName").attr("disabled", "disabled");
	 $(".loginBox button").attr("disabled", "disabled");
	}
	else if (val == 12) {
	 // Clone Data
	 var dataid = $("#selectId").val();
	 var toUsername = $("#selectUsername").val();
	 socket.emit('cloneData', {id: dataid, user: toUsername});
	}
	else if (val == 13) {
	 // Query Data
	 var dataid = $("#queryId").val();
	 socket.emit('queryData', dataid);
	}
}

function query(val, id, ele){
	if (val == 1) {
		// Get User lists
		if ($(ele).hasClass("open")) {
			$(ele).removeClass("open");
			$(ele).next().empty();
		}
		else{
			$(ele).addClass("open");

			currentId = $(ele).attr('id').slice(-32);
			console.log("currentId: "+currentId);
			Id2append = id;
			socket.emit('getDatalist', {msg: "1", id: id});
			socket.emit('getDatafromDatalist', currentId);
		}
	}
	else if (val == 2) {
	 	// Get Category lists
	 	if ($(ele).hasClass("open")) {
	 		$(ele).removeClass("open");
	 		$(ele).next().empty();
	 	}
	 	else{
	 		$(ele).addClass("open");

	 		currentId = $(ele).attr('id').slice(-32);
	 		console.log("currentId: "+currentId);
	 		Id2append = id;	
	 		socket.emit('getDatalist', {msg: "2", id: id});
	 		socket.emit('getDatafromDatalist', currentId);
	 	}
	 }
	 else if (val == 3) {
	 	// Get State lists
	 	if ($(ele).hasClass("open")) {
	 		$(ele).removeClass("open");
	 		$(ele).next().empty();
	 	}
	 	else{
	 		$(ele).addClass("open");

	 		currentId = $(ele).attr('id').slice(-32);
	 		console.log("currentId: "+currentId);
	 		Id2append = id;
	 		socket.emit('getDatalist', {msg: "3", id: id});
	 		socket.emit('getDatafromDatalist', currentId);
	 		query(6, id, ele);
	 	}
	 }
	 else if (val == 4) {
		// Get Task from State lists
		if ($(ele).hasClass("open")) {
			$(ele).removeClass("open");
			$(ele).next().empty();
		}
		else{
			$(ele).addClass("open");

			currentId = $(ele).attr('id').slice(-32);
			console.log("currentId: "+currentId);
			Id2append = id;
			socket.emit('getDatalist', {msg: "4", id: id});
			socket.emit('getDatafromDatalist', currentId);
		}
	}
	else if (val == 5) {
		// Get Subtask lists
		if ($(ele).hasClass("open")) {
			$(ele).removeClass("open");
			$(ele).next().empty();
		}
		else{
			$(ele).addClass("open");

			currentId = $(ele).attr('id').slice(-32);
			console.log("currentId: "+currentId);
			Id2append = id;
			socket.emit('getDatalist', {msg: "5", id: id});
			socket.emit('getDatafromDatalist', currentId);
		}
	}
	else if (val == 6) {
		// Get Task from Category lists
		currentId = $(ele).attr('id').slice(-32);
		console.log("currentId: "+currentId);
		Id2append = id;
		socket.emit('getDatalist', {msg: "6", id: id});
			//socket.emit('getDatafromDatalist', currentId);

		}
		else if (val == 7) {
		// Get Org lists
		if ($(ele).hasClass("open")) {
			$(ele).removeClass("open");
			$(ele).next().empty();
		}
		else{
			$(ele).addClass("open");
			socket.emit('getDatalist', {msg: "8"});
		}
	}
	else if (val == 8) {
		// Get Subtask data
		if ($(ele).hasClass("open")) {
			$(ele).removeClass("open");
			$(ele).next().empty();
		}
		else{
			$(ele).addClass("open");

			currentId = $(ele).attr('id').slice(-32);
			console.log("currentId: "+currentId);
			Id2append = id;
			socket.emit('getDatafromDatalist', currentId);
		}
	}
	else if (val == 9) {
		// Get Subtasklists from Category lists
		if ($(ele).hasClass("open")) {
			$(ele).removeClass("open");
			$(ele).next().empty();
		}
		else{
			$(ele).addClass("open");

			currentId = $(ele).attr('id').slice(-32);
			console.log("currentId: "+currentId);
			Id2append = id;
			socket.emit('getDatalist', {msg: "7", id: id});
			socket.emit('getDatafromDatalist', currentId);

			newWay = 1;
		}
	}
	else if (val == 10) {
		// Get Subtask data from Category 
		if ($(ele).hasClass("open")) {
			$(ele).removeClass("open");
			$(ele).next().empty();
		}
		else{
			$(ele).addClass("open");

			currentId = $(ele).attr('id').slice(-32);
			console.log("currentId: "+currentId);
			Id2append = id;
			socket.emit('getDatafromDatalist', currentId);

			newWay = 1;
		}
	}
}


function openform(val) {
	if ($(val).next("div").hasClass("none")) {
		$(val).next("div").removeClass("none");
	}
	else{
		$(val).next("div").addClass("none");
	}
}

function getdata2modal(id, ele) {
	editbtnElement = $(ele).parent().parent().parent().parent().parent().parent().parent().parent();
	console.log(editbtnElement);
	var content = $(ele).prev().text();
	$("#edit-dataContent").val(content);
	$("#edit-dataId").html(id);
}

function createUI(ele) {
	var val = $(ele).val();	
	var child = "";
	if (val == "org") {
		child += "<input class='form-control inline input-sm custom-size' id='dataName' type='text'placeholder='DataName'>";
		child += "<button type='button' class='btn btn-default btn-sm' onclick='create(1)'>Create</button>";
	}
	else if (val == "user"){
		child += "<input class='form-control inline input-sm custom-size' id='dataName' type='text'placeholder='DataName'>";
		child += "<input class='form-control inline input-sm custom-size' id='upperId' type='text' placeholder='OrgId'>";
		child += "<button type='button' class='btn btn-default btn-sm' onclick='create(2)'>Create</button>";	
	}
	else if (val == "category"){
		child += "<input class='form-control inline input-sm custom-size' id='dataName' type='text'placeholder='DataName'>";
		child += "<input class='form-control inline input-sm custom-size' id='upperId' type='text' placeholder='UserId'>";
		child += "<button type='button' class='btn btn-default btn-sm' onclick='create(3)'>Create</button>";	
	}
	else if (val == "state"){
		child += "<input class='form-control inline input-sm custom-size' id='dataName' type='text'placeholder='DataName'>";
		child += "<input class='form-control inline input-sm custom-size' id='upperId' type='text' placeholder='CategoryId'>";
		child += "<button type='button' class='btn btn-default btn-sm' onclick='create(4)'>Create</button>";	
	}
	else if (val == "task"){
		child += "<input class='form-control inline input-sm custom-size' id='dataName' type='text'placeholder='DataName'>";
		child += "<input class='form-control inline input-sm custom-size' id='upperId' type='text' placeholder='StateId'>";
		child += "<button type='button' class='btn btn-default btn-sm' onclick='create(5)'>Create</button>";	
	}
	else if (val == "subtask"){
		child += "<input class='form-control inline input-sm custom-size' id='dataName' type='text'placeholder='DataName'>";
		child += "<input class='form-control inline input-sm custom-size' id='upperId' type='text' placeholder='TaskId'>";
		child += "<button type='button' class='btn btn-default btn-sm' onclick='create(6)'>Create</button>";	
	}
	else if (val == "task2"){
		child += "<input class='form-control inline input-sm custom-size' id='dataName' type='text'placeholder='DataName'>";
		child += "<input class='form-control inline input-sm custom-size' id='upperId' type='text' placeholder='CategoryId'>";
		child += "<button type='button' class='btn btn-default btn-sm' onclick='create(7)'>Create</button>";	
	}
	else if (val == "subtask2"){
		child += "<input class='form-control inline input-sm custom-size' id='dataName' type='text'placeholder='DataName'>";
		child += "<input class='form-control inline input-sm custom-size' id='upperId' type='text' placeholder='TaskId'>";
		child += "<button type='button' class='btn btn-default btn-sm' onclick='create(8)'>Create</button>";	
	}

	$(".createForm").empty();
	$(".createForm").append(child);
}
