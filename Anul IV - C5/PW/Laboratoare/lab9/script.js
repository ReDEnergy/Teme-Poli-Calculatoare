function validateEmail(email) { 
    var re = /^(([^<>()[\]\\.,;:\s@\"]+(\.[^<>()[\]\\.,;:\s@\"]+)*)|(\".+\"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/;
    return re.test(email);
}

$(document).ready(function() {

	var clk = 0;
	var dbclk = 0;

	$('#btn').click(function() {
		clk++;
		console.log("Clicks", clk);
		$('#clicks').text(clk);
	});

	$('#btn').dblclick(function() {
		clk -=2;
		dbclk++;
		console.log("DB Clicks", dbclk); 
		$('#dblclicks').text(dbclk);
		$('#clicks').text(clk);
		if (clk === dbclk)
			alert('Meeereeeee');
	});
	
	$("#dialog").hide();
	
	console.log($.cookie('abonare'));
	
	if (!$.cookie('abonare')) {
		$('#abonare').show();
	}
	
	setTimeout(function(){
		if ($.cookie('hide') === 'true')
			return;
		$("#dialog").show();
		$("#dialog").dialog();
		$("#hide").click(function(){
			$.cookie('hide', 'true', { expires: 30 });
			$("#dialog").hide();
		});	
		$("#news").click(function(){
			email = $('#email').val();
			console.log('Email', email);
			if (validateEmail(email)) {
				$.cookie('abonare', 'true', { expires: 30 });
			}
			else
				$('#error').text('Eroare ba');
		});	
	}, 2000);
	
	
});

