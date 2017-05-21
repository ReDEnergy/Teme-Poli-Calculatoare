if (window.File && window.FileReader && window.FileList && window.Blob) {
	// Great it works
}
else {
	alert('The File APIs are not fully supported in this browser.');
}


var ATRIB = [];
var SET = [];
var CLASS;
var D = {};

function Set(labels) {
	var idx;
	var k = 0;
	for (var i in labels) {
		if (labels[i]) {
			this[ATRIB[k]] = labels[i];
			k++;
		}
	}
}

function computeEntropy(S) {
	var len = S.length;
	var Sc = {};
	for (var i in S) {
		// console.log(S[i][CLASS]);
		if (Sc[S[i][CLASS]] === undefined)
			Sc[S[i][CLASS]] = 1;
		else
			Sc[S[i][CLASS]]++;
	}
	// console.log(Sc);
	var ENT = 0;
	for (var i in Sc) {
		ENT += Sc[i] / len * Math.log(Sc[i] / len);		
	}
	// console.log(-ENT);
	return -ENT;
}

function computeGain(S, atrib) {
	// console.log('atrib', atrib);
	var gain = computeEntropy(S);
	Sj = {};
	for (var i in S) {
		if (S[i] === CLASS) break;
		if (Sj[S[i][atrib]] === undefined) Sj[S[i][atrib]] = [];
		Sj[S[i][atrib]].push(S[i]);
	}
	for (var j in Sj) {
		gain -= (Sj[j].length / S.length) * computeEntropy(Sj[j]);
	}
	console.log('GAIN', atrib, gain);
	return gain; 
}

function ID3 (S) {
	var ATR = [];
	var max = 0;
	var amax = 0;
	var g;
	for (var i in ATRIB) {
		if (ATRIB[i] !== CLASS) {
			ATR.push(ATRIB[i]);
			g = computeGain(S, ATRIB[i]);;
			if (g > max)  {
				max = g;
				amax = ATRIB[i];
			}
		}
	}
	
	console.log('max', max, amax);
	var idx = ATR.indexOf(amax);;
	ATR.splice(idx, 1);

	console.log(ATR);

	for (var i in S) {
		if (D[S[i][amax]] === undefined ) 
			D[S[i][amax]] = [];
		D[S[i][amax]].push(S[i]);
	}

	for (var i in D) {
		D[i] = getClass(D[i]);
	}

	console.log(D);
}

function getClass(S) {
	var A = [];
	var K = 0;
	for (var i in S) {
		console.log(A[S[i][CLASS]]);
		if(A[S[i][CLASS]] === undefined) {
			K++;
			A[S[i][CLASS]] = 0;
		} 
	}
	if (K === 1)
		return S[0][CLASS];
	return S;
}

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

			var atr = lines[0].split(' ');
			for (var i in atr) {
				if (atr[i])
					ATRIB.push(atr[i]);
			}
			
			console.log(ATRIB);
			CLASS = ATRIB[ATRIB.length-1];

			for (var i=1; i<len-1; i++) {
				var line = lines[i].split(' ');
				SET.push(new Set(line));
			};
			
			// computeEntropy(SET);
			ID3(SET);
		};

		// Read in the image file as a data URL.
		reader.readAsBinaryString(f);
	}
}

document.getElementById('files').addEventListener('change', handleFileSelect, false);