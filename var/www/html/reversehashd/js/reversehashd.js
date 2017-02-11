$(document).ready(function(){
	reversehashd.initWebsocket();
	$('#input_md5').keypress(function (e) {
		if (e.which == 13) {
			$('#result').html('');
			showLoading();
			reversehashd.reverse($('#input_md5').val()).done(function(){
				console.log("done");
				$('#result').html('done');
				hideLoading();
			}).fail(function(){
				console.log("fail");
				setTimeout(function(){
					hideLoading();
					$('#result').html('fail');	
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
