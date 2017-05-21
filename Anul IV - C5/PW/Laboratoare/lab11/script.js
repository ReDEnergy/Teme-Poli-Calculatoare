/*
$("span.tooltip").mouseover(function() {
	
	$.ajax({
		type: "post",
		url: "script/wordJSON.php",
		data: {
			jsonData : JSON.stringify({ word : "test" })
		},
		contentType: "application/x-www-form-urlencoded"
	}).done(function(data) {
		$("#tooltip").text(JSON.parse(data)["definition"]);
	});	
});
*/


$("span.tooltip").mouseover(function() {
	
	$.ajax({
		type: "post",
		url: "script/wordXML.php",
		dataType: "XML",
		data: "<content><word>test</word></content>",
		contentType: "application/x-www-form-urlencoded"
	}).done(function(data) {
		console.log(data);
		// var x = $("definintion", data);
		console.log(data.firstChild.firstChild.firstChild.textContent);
	}).error(function(data){
		console.log("ERROR");
	});	
});

var TO = 0;

setInterval(function() {
	$.ajax({
		type: "get",
		url: "script/word.php",
		data: "ceva",
	}).done(function(data) {
		$("#text").val(data);
	});	
}, 2000);

$("#text").keydown(function() {
	value = { ceva : $(this).val()};
	TO = setTimeout(function() {
		$.ajax({
			type: "post",
			url: "script/word.php",
			data: value,
		}).done(function(data) {
		});
	}, 500);
	console.log($(this).val());
});


