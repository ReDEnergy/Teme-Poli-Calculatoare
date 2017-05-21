if (window.File && window.FileReader && window.FileList && window.Blob) {
	// Great it works
}
else {
	alert('The File APIs are not fully supported in this browser.');
}

function createMatrix(matrix, n ,m) {
	for(var i=0; i<n; i++) {
		matrix[i] = new Array(m);
	}
}

function deepCopy(A, B) {
	for(var i=0; i<A.length; i++)
		for(var j=0; j<A[i].length; j++)
			B[i][j] = A[i][j];
}

function printMatrix(M) {
	var i;
	for (i=0; i<M.length; i++) {
		console.log(M[i].join(' '));
	}
}

var dir = [
	[-1, 0],
	[ 0, 1],
	[ 1, 0],
	[ 0, -1]
];

var A = [
	[0, 0, 0, 0, 0, 0],
	[0, 0, 0, 0, 0, 0],
	[-100, 0, 0, -100, 0, 0],
	[0, 0, 0, 0, 100, 0]];

var U = [];
var U2 = [];
var R = [];
var D = [];
createMatrix(U, 4, 6);
createMatrix(U2, 4, 6);
createMatrix(R, 4, 6);
createMatrix(D, 4, 6);
deepCopy(A, U);
deepCopy(A, R);
deepCopy(A, U2);
console.log(A, R, U2);

var i, j, k;	
var max, S, SDif;
var index = -1;
for (k=0; k<5; k++) {
	for (i=0; i<4; i++) {
		for (j=0; j<6; j++) {
		
			deepCopy(U, U2);
			max = -1000;

			// GO UP
			S = 0;
			if (i - 1 >= 0) {	// go UP
				console.log('up');
				if (j - 1 >= 0)		// go LEFT
					S = 0.9 * U[i-1][j] + 0.1 * U[i][j-1];
				else
					S = U[i-1][j];
			}
			else {	// go LEFT
				if (j - 1 >= 0)		// go LEFT
					S = U[i][j-1];
			}
			if (S > max) {
				max = S;
				index = 0;
				console.log('Suma', S);
			}

			// GO RIGHT
			S = 0;
			if (j + 1 < 6) {	// go RIGHT
				console.log('right');
				S = U[i][j+1];
			}
			if (S > max) {
				max = S;
				index = 1;
				console.log('Suma', S);
			}

			// GO DOWN
			S = 0;
			if (i + 1 < 4) {	// go DOWN
				console.log('down');
				if (j + 1 < 6 && j - 1 >= 0)
					S = 0.6 * U[i+1][j] + 0.2 * U[i][j+1] + 0.2 * U[i][j-1];
				else {
					if (j + 1 < 6)
						S = 0.6/0.8 * U[i+1][j] + 0.2/0.8 * U[i][j+1];
					else 
						S = 0.6/0.8 * U[i+1][j] + 0.2/0.8 * U[i][j-1];
				}
			}
			else {
				if (j + 1 < 6) {
					S = U[i][j+1]
					if (j - 1 >= 0) {
						S = 0.5 * S + 0.5 * U[i][j-1];
					}
				}
				else
					S = U[i][j-1]
			}
			if (S > max) {
				max = S;
				index = 2;
				console.log('Suma', S);
			}

			// GO LEFT
			S = 0;
			if (j - 1 >= 0) {	// go LEFT
				console.log('left');
				if (i + 1 < 4)
					S = 0.8 * U[i][j-1] + 0.2 * U[i + 1][j];
				else
					S = U[i][j-1];
			}
			else {
				if (i + 1 < 4)
					S = U[i + 1][j];
			}
			if (S > max) {
				max = S;
				index = 3;
				console.log('Suma', S);
			}

			console.log('Suma MAX', S);

			// final
			U2[i][j] = R[i][j] + 0.5 * max;
			D[i][j] = index;
			
		}
	}

	console.log("U[s]");
	printMatrix(U2);
	console.log("U2[s]");
	printMatrix(U);

	SDif = 0;
	for (i=0; i<4; i++) {
		for (j=0; j<6; j++) {
			SDif += Math.abs(U2[i][j] - U[i][j]);
		}
	}

	console.log('Delta', SDif);
	deepCopy(U2, U);
	
}
	
console.log("Directii");
printMatrix(D);
	
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