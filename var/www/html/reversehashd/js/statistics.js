$(document).ready(function(){
	reversehashd.initWebsocket();
	reversehashd.ready(function(){
		loadStatistics();
	});
});

function loadStatistics(){
	reversehashd.statistics().done(function(r){
		// console.log(r);
		$('.content').html('<div class="statistics-table"></div>');
		$('.statistics-table').append(''
			+ '<div class="statistics-row">'
			+ '		<div class="statistics-cell">Modified</div>'
			+ '		<div class="statistics-cell">Name</div>' 
			+ '		<div class="statistics-cell">Last Success Percent</div>'
			+ '		<div class="statistics-cell">Info</div>'
			+ '</div>');
		
		for(var i = 0; i < r.statistics.length; i++){
			var sbit = r.statistics[i];
			
			$('.statistics-table').append(''
			+ '<div class="statistics-row" id="' + sbit.name + '">'
			+ '		<div class="statistics-cell modified">' + sbit.modified + '</div>'
			+ '		<div class="statistics-cell name">' + sbit.name + '</div>' 
			+ '		<div class="statistics-cell lp"></div>'
			+ '		<div class="statistics-cell info"><div class="status"></div>'
			+ '			<div class="bitprogress" style="display: none">'
			+ ' 			<div class="bitprogress-ins" style="width: 0%"></div>'
			+ '			</div>'
			+ '		</div>'
			+ '</div>');

			updatePercents(sbit.name, sbit.lp);
		}
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

function updatePercents(bitid, lp){
	var cl = 'normal';
	if(lp == 100){
		cl = 'success';
	}else if(lp < 20){
		cl = 'low';
	}

	$('#' + bitid + " .lp").removeClass('success');
	$('#' + bitid + " .lp").removeClass('low');
	$('#' + bitid + " .lp").removeClass('normal');
	$('#' + bitid + " .lp").addClass(cl);
	$('#' + bitid + " .name").removeClass('success');
	$('#' + bitid + " .name").removeClass('low');
	$('#' + bitid + " .name").removeClass('normal');
	$('#' + bitid + " .name").addClass(cl);
	$('#' + bitid + " .lp").html(lp + "%");
}

// overrided
reversehashd.handlerTrainingThreadInfo = function(response){
	$('.statistics-cell.name').removeClass("processing");
	$('#' + response.bitid + " .name").addClass("processing");
	var status = response.status;
	if(response.percent){
		status += " (" + response.percent + "%)";
	}
	$('#' + response.bitid + " .info .status").html(status);
	
	if(response.lp){
		updatePercents(response.bitid, response.lp);
	}
	
	$('#' + response.bitid + " .info .bitprogress").show();
	if(response.completed){
		$('#' + response.bitid + " .info .bitprogress-ins").css({'width': response.completed + '%'});
	}
}
