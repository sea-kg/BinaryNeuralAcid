
window.reversehashd.handlerTrainingThreadInfo = function(response){
	console.log(response);
	var status = "Bit (" + response.bitid + "): " + response.status;
	if(response.percent){
		status += " (" + response.percent + "%)";
	}
	$('#training_message').html(status);
	
	$('#' + response.bitid + " .info .status").html(status);
	
	if(response.lp){
		updatePercents(response.bitid, response.lp);
	}

	$('#' + response.bitid + " .info .bitprogress").show();
	if(response.completed){
		$('#' + response.bitid + " .info .bitprogress-ins").css({'width': response.completed + '%'});
	}
}

reversehashd.initWebsocket();


function try_reverse(){
	$('#reverse_result').html('Send request...');
	$('#reverse_result2').html('Send request...');
	clearMD5();
	$('#md5_expected').html('');
	$('#md5_got').html('');
	$('#md5_xor_sum').html('');
	$('#md5_expected2').html('');
	$('#md5_got2').html('');
	$('#md5_xor_sum2').html('');
	
	showLoading();
	reversehashd.reverse($('#input_md5').val()).done(function(r){
		console.log("done");
		console.log("response: ", r);
			
		md5_request_bin = hex2bin(r.request_md5);
		md5_response_bin = hex2bin(r.result_md5);
		drawMD5('cnv_md5_1', md5_request_bin, "#000000");
		drawMD5('cnv_md5_2', md5_response_bin, "#000000");
		
		$('#md5_expected').html(r.request_md5);
		$('#md5_got').html(r.result_md5);
		
		var md5_xor = "";
		var sum = 0;
		for(var i = 0; i < 128; i++){
			if(md5_request_bin[i] == md5_response_bin[i]){
				md5_xor += "1";
				sum++;
			}else{
				md5_xor += "0";
			}
		}
		drawMD5('cnv_md5_3', md5_xor, "#ff0000");
		$('#md5_xor_sum').html(sum + '/' + md5_request_bin.length);
		
		$('#reverse_result').html('');
		// $('#reverse_result').append(compareHashes(r.request_md5, r.result_md5));
		if(r.request_md5 == r.result_md5){
			$('#reverse_result').append('Cool!!! I found it:\n');
			$('#reverse_result').append('Base64: ' + r.answer_base64 + "\n");
			$('#reverse_result').append('HEX: ' + r.answer_hex + "\n");
			$('#reverse_result').append('Text: ' + r.answer_text + "\n");
		}else{
			$('#reverse_result').append('\nNot found matches.\n\n Response hex is: \n' + prepareHex(r.answer_hex) + "\n");
		}
		
		// step2
		md5_response2_bin = hex2bin(r.step2.result_md5);
		drawMD5('cnv_md5_2_1', md5_request_bin, "#000000");
		drawMD5('cnv_md5_2_2', md5_response2_bin, "#000000");
		
		$('#md5_expected2').html(r.request_md5);
		$('#md5_got2').html(r.step2.result_md5);
		
		var md5_xor2 = "";
		sum = 0;
		for(var i = 0; i < 128; i++){
			if(md5_request_bin[i] == md5_response2_bin[i]){
				md5_xor2 += "1";
				sum++;
			}else{
				md5_xor2 += "0";
			}
		}
		drawMD5('cnv_md5_2_3', md5_xor2, "#ff0000");
		$('#md5_xor_sum2').html(sum + '/' + md5_request_bin.length);
		$('#reverse_result2').html('');
		if(r.request_md5 == r.step2.result_md5){
			$('#reverse_result2').append("Cool!!! I found it:\n");
			$('#reverse_result2').append(r.step2.answer_text);
		}else{
			$('#reverse_result2').append("Not equal:\n");
			$('#reverse_result2').append(r.step2.answer_text);
		}
		hideLoading();
		
	}).fail(function(r){
		console.log("Error: ", r);
		hideLoading();
		$('#reverse_result').html("Error " + r.code + "): " + r.error);
	});
}

function updatePercents(bitid, lp){
	var cl = 'alert-info';
	if(lp == 100){
		cl = 'alert-success';
	}else if(lp < 20){
		cl = 'alert-danger';
	}

	$('#bit' + bitid).removeClass('alert-success');
	$('#bit' + bitid).removeClass('alert-info');
	$('#bit' + bitid).removeClass('alert-danger');
	$('#bit' + bitid).addClass(cl);
	$('#bit' + bitid + " .lp").html(lp + "%");
}

function cnv_statistics_more(){
	var el = $('#cnv_statistics_more');
	if(el.is(":visible")){
		$('#cnv_statistics_more').hide();
	} else {
		$('#cnv_statistics_more').show();
	}
}

function loadStatistics(){
	var canvas = document.getElementById("cnv_statistics");
	var ctx = canvas.getContext("2d");
	ctx.fillStyle = "#000000";
	ctx.fillRect(0,0,442,25);
	ctx.fillStyle = "#FFFFFF";
	ctx.fillRect(1,1,440,23);
	var sMore = "";
	reversehashd.statistics().done(function(r){
		for(var i = 0; i < r.statistics.length; i++){
			var sbit = r.statistics[i];
			var p = Math.floor((sbit.result / 10000)*255);
			var c = p.toString(16);
			// console.log(p.toString(16));
			ctx.fillStyle = "#" + c + c + c;
			ctx.fillRect(1 + i,1,1,23);
			sMore += sbit.id + " = " + Math.floor((sbit.result / 10000)*100) + "%<br>";
		}
		$('#cnv_statistics_more').html(sMore);
	}).fail(function(r){
		console.error(r);
		$('#statistics_page').html('failed');
	});
}

function drawMD5(elid, s, color){
	var canvas = document.getElementById(elid);
	var ctx = canvas.getContext("2d");
	ctx.fillStyle = "#000000";
	ctx.fillRect(0,0,258,130);
	ctx.fillStyle = "#FFFFFF";
	ctx.fillRect(1,1,256,128);
	for(var y = 0; y < 16; y++){
		for(var x = 0; x < 16; x++){
			var c = s[y*16 + x];
			if(c == "1"){
				ctx.fillStyle = color;
				ctx.fillRect(1 + x*16,1+y*16,16,16);
			}
		}
	}
}

function clearMD5(){
	var md5_0 = "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

	drawMD5('cnv_md5_1', md5_0, "#000000");
	drawMD5('cnv_md5_2', md5_0, "#000000");
	drawMD5('cnv_md5_3', md5_0, "#ff0000");
	
	drawMD5('cnv_md5_2_1', md5_0, "#000000");
	drawMD5('cnv_md5_2_2', md5_0, "#000000");
	drawMD5('cnv_md5_2_3', md5_0, "#ff0000");
}

$(document).ready(function(){
	$('#try_reverse').unbind().bind('click', try_reverse);
	
	$('#input_md5').keypress(function (e) {
		if (e.which == 13) {
			try_reverse();
			return false;
		}
	});
	loadStatistics();
	clearMD5();
});

function showLoading(){
	$('.background').show();
	$('.background').css({'opacity': 1});
}

function hideLoading(){
	$('.background').css({'opacity': 0});
	setTimeout(function(){
		$('.background').hide();
	}, 200);
}

var hex2bin_data = {
	'0': '0000', '1': '0001', '2': '0010', '3': '0011',
	'4': '0100', '5': '0101', '6': '0110', '7': '0111',
	'8': '1000', '9': '1001', 'a': '1010', 'b': '1011',
	'c': '1100', 'd': '1101', 'e': '1110', 'f': '1111'
};

function hex2bin(s){
	var res = '';
	for(var i = 0; i < s.length; i++){
		res += hex2bin_data[s[i]];
	}
	return res;
}

function hexToChar(h1){
	var t = parseInt(h1, 16);
	if((t >= 32 && t <= 126) || (t >= 128 && t <= 254)){
		return "&#" + t + ";";
	}
	return '.';
}

function prepareHex(hex){
	var r1 = '==================== HEX =======================\n';
	console.log(hex.length);
	console.log(hex);

	var l1 = '' 
	var l2 = '';

	for(var i = 0; i < hex.length; i = i + 2){
		var h1 = hex[i] + '' + hex[i+1];
		
		if(i % 11 == 0){
			if(l1 != '' &&  l2 != ''){
				r1 += l1 + '    ' + l2 + '\n';
			}
			l1 = '';
			l2 = '';
		}
		l1 += ' ' + h1;
		l2 += hexToChar(h1);
	}
	if(l1 != '' &&  l2 != ''){
		r1 += l1 + '    ' + l2 + '\n';
	}
	r1 += '================================================\n';
	return r1;
}
