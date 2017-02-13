if(!window.reversehashd) window.reversehashd = {};

// WebSocket protocol

window.reversehashd.handlerReceivedChatMessage = function(response){
	reversehashd.handlerReceivedChatMessage(response);
};
window.reversehashd.listeners = {}
window.reversehashd.addListener = function(id, d){
	if(!reversehashd.listeners[id]){
		reversehashd.listeners[id] = [];
	}
	reversehashd.listeners[id].push(d);
}
reversehashd.handleCommand = function(response){
	if(reversehashd.listeners[response.rid]){
		for(var d in reversehashd.listeners[response.rid]){
			if(response['error']){
				reversehashd.listeners[response.rid][d].reject(response);
			} else {
				reversehashd.listeners[response.rid][d].resolve(response);
			}
		}
		// clean listeners
		reversehashd.listeners.hello = [];
	}else if(response.cmd == "chat"){
		reversehashd.handlerReceivedChatMessage(response);
	}else{
		console.error("Not found handler for '" + response.cmd + "'");
	}
};

window.reversehashd.WSState = "?";

window.reversehashd.getWSState = function(){
	return reversehashd.WSState;
}

window.reversehashd.setWSState = function(s){
	reversehashd.WSState = s;
	console.log(reversehashd.WSState);
	
	var el = document.getElementById('websocket_state');
	if(el){
		document.getElementById('websocket_state').innerHTML = s;
	}
	if(s == "OK"){
		$('.background').css({'opacity': 0});
		setTimeout(function(){
			$('.background').hide();
			$('.inputform').show();
			$('.inputform').css({'opacity': 1});
		},1000);
	}else{
		$('.background').css({'opacity': 1});
		setTimeout(function(){
			$('.background').show();
			$('.inputform').hide();
			$('.inputform').css({'opacity': 0});
		},1000);
	}
}

window.reversehashd.initWebsocket = function(){
	reversehashd.socket = new WebSocket("ws://" + window.location.hostname + ":43735/");
	// reversehashd.socket = new WebSocket("ws://freehackquest.com:888/api");
	window.reversehashd.socket.onopen = function() {
		console.log('WS Opened');
		reversehashd.setWSState("OK");
		reversehashd.send({'cmd': 'hello'}).done(function(){
			// reversehashd.login();
		});
	};

	window.reversehashd.socket.onclose = function(event) {
		console.log('Closed');
		if (event.wasClean) {
			reversehashd.setWSState("CLOSED");
		} else {
			reversehashd.setWSState("BROKEN");
		}
		setTimeout(function(){
				reversehashd.setWSState("RECONN");
				reversehashd.initWebsocket();
			}, 5000);
		console.log('Code: ' + event.code + ' Reason: ' + event.reason);
	};
	reversehashd.socket.onmessage = function(event) {
		console.log('Received: ' + event.data);
		try{
			var response = JSON.parse(event.data);
			reversehashd.handleCommand(response);
		}catch(e){
			console.error(e);
		}
		
	};
	reversehashd.socket.onerror = function(error) {
		console.log('Error: ' + error.message);
	};
}

window.reversehashd.id = 0;
window.reversehashd.send = function(obj, def){
	reversehashd.id++;
	obj.id = reversehashd.id;
	var d = def || $.Deferred();
	try{
		if(reversehashd.socket.readyState == 0){
			setTimeout(function(){
				reversehashd.send(obj, d);
			}, 1000);
		}else{
			// console.log("ReadyState " + reversehashd.socket.readyState);
			console.log("Send " + JSON.stringify(obj));
			reversehashd.socket.send(JSON.stringify(obj));
		}
	}catch(e){
		console.error(e);
	}
	reversehashd.addListener(obj.id, d);
	return d;
}

window.reversehashd.reverse = function(md5){
	
	return reversehashd.send({
		'cmd': 'reverse',
		'md5': md5
	});
}
