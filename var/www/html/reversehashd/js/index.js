
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
	showLoading();
	reversehashd.reverse($('#input_md5').val()).done(function(r){
		console.log("done");
		console.log("response: ", r);
		setTimeout(function(){
			hideLoading();
			$('#reverse_result').html('');
			$('#reverse_result').append(compareHashes(r.request_md5, r.result_md5));
			if(r.request_md5 == r.result_md5){
				$('#reverse_result').append('Cool!!! I found it:\n');
				$('#reverse_result').append('Base64: ' + r.answer_base64 + "\n");
				$('#reverse_result').append('HEX: ' + r.answer_hex + "\n");
				$('#reverse_result').append('Text: ' + r.answer_text + "\n");
			}else{
				$('#reverse_result').append('\nNot found matches.\n\n Response hex is: \n' + prepareHex(r.answer_hex) + "\n");
			}
		},2000);
		hideLoading();
	}).fail(function(r){
		console.log("fail");
		setTimeout(function(){
			hideLoading();
			$('#reverse_result').html("Error " + r.code + "): " + r.error);
		},2000);
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

function loadStatistics(){
	var canvas = document.getElementById("cnv_statistics");
	var ctx = canvas.getContext("2d");
	ctx.fillStyle = "#000000";
	ctx.fillRect(0,0,442,25);
	ctx.fillStyle = "#FFFFFF";
	ctx.fillRect(1,1,440,23);
	
	reversehashd.statistics().done(function(r){
		for(var i = 0; i < r.statistics.length; i++){
			var sbit = r.statistics[i];
			var p = Math.floor((sbit.result / 10000)*255);
			var c = p.toString(16);
			// console.log(p.toString(16));
			ctx.fillStyle = "#" + c + c + c;
			ctx.fillRect(1 + i,1,1,23);
		}
	}).fail(function(r){
		console.error(r);
		$('#statistics_page').html('failed');
	});
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
});


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

var hex2bin_data = {
	'0': '0000',
	'1': '0001',
	'2': '0010',
	'3': '0011',
	'4': '0100',
	'5': '0101',
	'6': '0110',
	'7': '0111',
	'8': '1000',
	'9': '1001',
	'a': '1010',
	'b': '1011',
	'c': '1100',
	'd': '1101',
	'e': '1110',
	'f': '1111'
};

function hex2bin(s){
	var res = '';
	for(var i = 0; i < s.length; i++){
		res += hex2bin_data[s[i]];
	}
	return res;
}

function compareHashes(md5_request, md5_response){
	var r1 = ' Request md5-hash: ';
	var r2 = 'Response md5-hash: ';
	for(var i = 0; i < md5_request.length; i++){
		if(md5_request[i] == md5_response[i]){
			r1 += '<div class="equals-char">' + md5_request[i] + '</div>';
			r2 += '<div class="equals-char">' + md5_response[i] + '</div>';
		}else{
			r1 += md5_request[i];
			r2 += md5_response[i];
		}
	}
	
	md5_request_bin = hex2bin(md5_request);
	md5_response_bin = hex2bin(md5_response);
	
	var r1_bin = ' Request md5-hash (bit): ';
	var r2_bin = 'Response md5-hash (bit): ';
	var sum = 0;
	for(var i = 0; i < md5_request_bin.length; i++){
		if(md5_request_bin[i] == md5_response_bin[i]){
			r1_bin += '<div class="equals-char">' + md5_request_bin[i] + '</div>';
			r2_bin += '<div class="equals-char">' + md5_response_bin[i] + '</div>';
			sum++;
		}else{
			r1_bin += md5_request_bin[i];
			r2_bin += md5_response_bin[i];
		}
	}

	return r1 + '\n' + r2 + '\n' + r1_bin + '\n' + r2_bin + '\n Summary bits: ' + sum + '/' + md5_request_bin.length + '\n';
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
