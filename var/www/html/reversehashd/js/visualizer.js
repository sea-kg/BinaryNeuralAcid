$(document).ready(function(){
	reversehashd.initWebsocket();
	reversehashd.ready(function(){
		loadVisualizer();
	});
});

function loadVisualizer(){
	reversehashd.statistics().done(function(r){
		$('.visualizer-table').html('');
		$('.visualizer-table').append(''
			+ '<div class="visualizer-row">'
			+ '		<div class="visualizer-cell">Name</div>' 
			+ '</div>');

		for(var i = 0; i < r.statistics.length; i++){
			var sbit = r.statistics[i];
			$('.visualizer-table').append(''
			+ '<div class="visualizer-row">'
			+ '		<div class="visualizer-cell name" bitid="' + sbit.name + '">' + sbit.name + ' (' + sbit.lp + '%)</div>' 
			+ '</div>');
		}
		
		$('.visualizer-cell.name').unbind().bind('click',function(){
			showLoading();
			var bitid = $(this).attr('bitid');
			reversehashd.graph(bitid).done(function(graph_r){
				hideLoading();
				drawGraph(bitid, graph_r.graph);
			}).fail(function(r){
				hideLoading();
				$('.visualizer-plot').html('failed');
			})
		});
		
	}).fail(function(r){
		console.error(r);
		$('.content').html('failed');
	});
}


function showLoading(){
	$('.inputform').css({'opacity': 0});
	setTimeout(function(){
		$('.background').show();
		$('.background').css({'opacity': 1});
		$('.inputform').hide();
	},1000);
}

function hideLoading(){
	$('.background').css({'opacity': 0});
	setTimeout(function(){
		$('.background').hide();
		$('.inputform').show();
		$('.inputform').css({'opacity': 1});
	},1000);
}

// overrided
reversehashd.handlerTrainingThreadInfo = function(response){
	
	$('.statistics-cell.name').removeClass("processing");
	$('#' + response.bitid + " .name").addClass("processing");
	$('#' + response.bitid + " .info").html(response.status);
}

window.graph = window.graph || [];

window.graph_width = 1000;
window.graph_height = 1000;

function calculateCoordNodes(){
	// draw in
	var len = graph.length;
	var y_in = 50;
	for(var i = 0; i < len; i++){
		if(graph[i].type == "in"){
			graph[i].x = 10;
			graph[i].y = y_in;
			graph[i].w = 60;
			graph[i].h = 25;
			y_in += 30;
		}
	}
	window.graph_height = y_in;

	var y_m = 100;
	var x_m = 150;
	for(var i = 0; i < len; i++){
		if(graph[i].type == "m"){
			graph[i].x = x_m;
			graph[i].y = y_m;
			graph[i].w = 120;
			graph[i].h = 25;
			y_m += 100;
			if(y_m > window.graph_height){
				y_m = 100;
				x_m += 100;
			}
		}
	}

	// moving nodes
	var movies = true;
	var max_x_out = x_m;
	while(movies){
		movies = false;
		for(var i = 0; i < len; i++){
			var node = graph[i];
			if(node.type == "m"){
				var in1 = findNodeCoordOut(node.in1);
				var in2 = findNodeCoordOut(node.in2);
				var y_min = Math.min(in1.y, in2.y);
				var y_max = Math.max(in1.y, in2.y);
				var x_max = Math.max(in1.x, in2.x);
				var d = (y_max - y_min);
				var new_y = y_min + d/2 - node.h/2;
				if(new_y != graph[i].y){
					graph[i].y = new_y;
					movies = true;
				}
				var new_x = x_max + 100;
				if(new_x != graph[i].x){
					graph[i].x = new_x;
					movies = true;
				}
				// TODO check same coordinates
				
				max_x_out = Math.max(new_x+ 200, max_x_out);
			}
		}
	}
	
	for(var i = 0; i < len; i++){
		if(graph[i].type == "out"){
			graph[i].x = max_x_out;
			graph[i].y = window.graph_height/2;
			graph[i].w = 60;
			graph[i].h = 25;
		}
	}
	
	window.graph_width = max_x_out + 300;
}

function findNodeCoordOut(id){
	var result = {x: 0, y: 0};
	var len = graph.length;
	for(var i = 0; i < len; i++){
		var node = graph[i];
		if(node.id == id){
			result.x = node.x + node.w;
			result.y = node.y + node.h/2;
		}
	}
	return result;
}

function drawLine(context, x1,y1,x2,y2){
	context.beginPath();
	context.moveTo(x1,y1);
	
	var x1_1 = x1 + (x2 - x1) / 3;
	var x1_2 = x1 + ((x2 - x1) / 3)*2;
	
	context.bezierCurveTo(x1_1,y1,x1_2,y2,x2,y2);
	context.stroke();
}

function drawLines(){
	var canvas = document.getElementById('graphid');
	var context = canvas.getContext('2d');
	context.lineWidth = 2;
	context.strokeStyle = '#24aaff';
	
	var len = graph.length;
	for(var i = 0; i < len; i++){
		var node = graph[i];
		if(node.type == 'm' || node.type == 'out'){
			var _x = node.x;
			var _y = node.y + node.h/2;
			
			var in1 = findNodeCoordOut(node.in1);
			drawLine(context, in1.x,in1.y,_x,_y-5);

			var in2 = findNodeCoordOut(node.in2);
			drawLine(context, in2.x,in2.y,_x,_y+5);
		}
	}
}

function drawNodesIn(){
	var canvas = document.getElementById('graphid');
	var context = canvas.getContext('2d');
	context.font = '16pt Calibri';
	
	var len = graph.length;
	for(var i = 0; i < len; i++){
		var node = graph[i];
		if(node.type == "in"){
			context.beginPath();
			context.rect(node.x, node.y, node.w, node.h);
			context.fillStyle = 'black';
			context.fill();
			context.lineWidth = 2;
			context.strokeStyle = '#24aaff';
			context.stroke();
			context.fillStyle = '#24aaff';
			context.fillText(node.id, node.x + 10, node.y + 20);
		}
	}
}

function drawNodesM(){
	var canvas = document.getElementById('graphid');
	var context = canvas.getContext('2d');
	context.font = '16pt Calibri';
	var o = {'|': 'or', '^': 'xor', '&': 'and'}
	var len = graph.length;
	for(var i = 0; i < len; i++){
		var node = graph[i];
		if(node.type == "m"){
			context.beginPath();
			context.rect(node.x, node.y, node.w, node.h);
			
			// context.arc(node.x, node.y, node.r, 0, 2 * Math.PI, false);
			
			if(node.o == '|'){
				context.fillStyle = '#002f4d';
			}else if(node.o == '^'){
				context.fillStyle = '#404040';
			}else if(node.o == '&'){
				context.fillStyle = '#408000';
			}
			context.fill();
			context.lineWidth = 2;
			context.strokeStyle = '#24aaff';
			context.stroke();
			
			context.fillStyle = '#24aaff';
			context.fillText(node.id + ' (' + o[node.o] + ')', node.x + 10, node.y + 18);
		}
	}
}

function drawNodesOut(){
	var canvas = document.getElementById('graphid');
	var context = canvas.getContext('2d');
	context.font = '16pt Calibri';
	
	var len = graph.length;
	for(var i = 0; i < len; i++){
		var node = graph[i];
		if(node.type == "out"){
			context.beginPath();
			context.rect(node.x, node.y, node.w, node.h);
			context.fillStyle = 'black';
			context.fill();
			context.lineWidth = 2;
			context.strokeStyle = '#24aaff';
			context.stroke();
			context.fillStyle = '#24aaff';
			context.fillText(node.id, node.x + 10, node.y + 20);
		}
	}
}


function drawGraph(bitid, graph){
	var width = 1000;
	var height = 1000;
	window.graph = graph;
	calculateCoordNodes();
	
	$('.visualizer-plot').html('<canvas id="graphid" height="' + graph_height + 'px" width="' + graph_width + 'px"></canvas>');
	var canvas = document.getElementById('graphid');
	var context = canvas.getContext('2d');
	
	context.font = '40pt Calibri';
	context.fillStyle = '#24aaff';
	context.fillText(bitid, 10, 40);
	
	drawLines();
	drawNodesIn();
	drawNodesM();
	drawNodesOut();
}

