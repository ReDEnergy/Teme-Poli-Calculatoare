if (window.File && window.FileReader && window.FileList && window.Blob) {
	// Great it works
}
else {
	alert('The File APIs are not fully supported in this browser.');
}


var PT = [];
var PQ = [];

function Point(x) {
	this.id = x[0];
	this.C1 = parseFloat(x[1]);
	this.C2 = parseFloat(x[2]);
	this.C3 = parseFloat(x[3]);
}

Point.prototype.dist = function dist(P) {
	if (P === undefined) {
		console.log('DA');
		return;
	}
	return Math.sqrt(Math.pow((P.C1 - this.C1), 2) +
					Math.pow((P.C2 - this.C2), 2) + 
					Math.pow((P.C3 - this.C3), 2));
};

function Muchie(PT1, PT2) {
	this.dist = PT1.dist(PT2);
	this.PT1 = PT1;
	this.PT2 = PT2;
};

var compare = function compare(M1, M2) {
  return M1.dist < M2.dist;
}

function getDimensions() {
	var len = PT.length;
	// for (var i=0; i<len; i++) {
		// for (var j=i+1; j<len; j++) {
			// PQ.push(new Muchie(PT[i], PT[j]));
		// }
	// }
	
	var min = {};
	min.dist = 1000;
	var change = true;
	var nr = 0;
	while (change) {
		change = false;
		min.dist = 0;
		nr ++;
		for (var i=0; i<len; i++) {
			for (var j=i+1; j<len; j++) {
				if (PT[i].id !== PT[j].id) {
					change = true;
					var x = new Muchie(PT[i], PT[j]);
					if (x.dist < min.dist)
						min = x;
				}
			}
		}
	}
	
	console.log('MIN', min);
	min.PT1.id = min.PT2.id;

	// PQ.sort(compare);
	// console.log('Muchii', PQ);
};


function handleFileSelect(evt) {
    var files = evt.target.files; // FileList object

    // Loop through the FileList and render image files as thumbnails.
    for (var i = 0, f; f = files[i]; i++) {

		var reader = new FileReader();

		reader.onload = function(e)
		{
			var file = e.target;
		    var contents = file.result;
		    var lines = contents.split('\n');
			var len = lines.length;
		
			for (var i=0; i<len-1; i++) {
				if (i<=3) 
					continue;
				var x = lines[i].split('\t');
				PT.push(new Point(x));
			};
		    console.log('Points', PT);
		    getDimensions();
		};

		// Read in the image file as a data URL.
		reader.readAsBinaryString(f);
	}
}

document.getElementById('files').addEventListener('change', handleFileSelect, false);