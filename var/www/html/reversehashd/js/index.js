$(document).ready(function(){
	reversehashd.initWebsocket();
	$('#input_md5').keypress(function (e) {
		if (e.which == 13) {
			$('#result').html('');
			showLoading();
			reversehashd.reverse($('#input_md5').val()).done(function(r){
				console.log("done");
				console.log("response: ", r);
				setTimeout(function(){
					hideLoading();
					$('#result').html('');
					$('#result').append(compareHashes(r.request_md5, r.result_md5));
					if(r.request_md5 == r.result_md5){
						$('#result').append('Cool!!! I found it:\n');
						$('#result').append('Base64: ' + r.answer_base64 + "\n");
						$('#result').append('HEX: ' + r.answer_hex + "\n");
						$('#result').append('Text: ' + r.answer_text + "\n");
					}else{
						$('#result').append('\nNot found matches.\n\n Response hex is: \n' + prepareHex(r.answer_hex) + "\n");
					}
				},2000);
				hideLoading();
			}).fail(function(r){
				console.log("fail");
				setTimeout(function(){
					hideLoading();
					$('#result').html("Error " + r.code + "): " + r.error);
				},2000);
			});
			return false;
		}
	});
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
	return r1 + '\n' + r2 + '\n';
}

function hexToChar(h1){
	var t = parseInt(h1, 16);
	if((t >= 32 && t <= 126) || (t >= 128 && t <= 254)){
		return String.fromCharCode(t);
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
