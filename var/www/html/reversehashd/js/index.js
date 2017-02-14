$(document).ready(function(){
	reversehashd.initWebsocket();
	$('#input_md5').keypress(function (e) {
		if (e.which == 13) {
			$('#result').html('');
			showLoading();
			reversehashd.reverse($('#input_md5').val()).done(function(r){
				console.log("done");
				setTimeout(function(){
					hideLoading();
					$('#result').html('');
					$('#result').append('Your md5-hash: ' + r.request_md5 + "\n");
					$('#result').append('Got md5-hash:  ' + r.result_md5 + "\n");
					if(r.request_md5 == r.result_md5){
						$('#result').append('Cool!!! I found it:\n');
						$('#result').append('Base64: ' + r.answer_base64 + "\n");
						$('#result').append('HEX: ' + r.answer_hex + "\n");
						$('#result').append('Text: ' + r.answer_text + "\n");
					}else{
						$('#result').append('Oops!!! Not match, but result hex is: ' + r.answer_hex + "\n");
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
