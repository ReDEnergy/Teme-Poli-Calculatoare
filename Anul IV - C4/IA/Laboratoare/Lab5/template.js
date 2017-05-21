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

var Q = [];
var AC = [0, -1, 1];
var Player = function() {
	this.pos = Math.floor(Math.random() * 3);
}

Player.prototype.reset = function reset() {
	this.pos = Math.floor(Math.random() * 3);
} 

Player.prototype.setAction = function setAction(action) {
	this.pos += AC[action];
}

var state = function(posA, posB, action) {
	this.posA = posA;
	this.posB = posA;
	this.action = action;
	this.state = '' + posA + posB + action;
}

var i, j, k, s;

for (i=0; i<3; i++) {
	for (j=0; j<3; j++) {
		for (k=0; k<3; k++) {
			s = new state(i, j, k);
			Q[s.state] = 0;
		}
	}
}

var A = new Player();
var B = new Player();

function getState(A, B, action) {
	return '' + A.pos + B.pos + action;
}

function getBestAction(A, B) {
	var val;
	var action = 0;
	var max = Q[getState(A, B, 0)];
	if (A.pos > 0) {
		action = 1;
		val = Q[getState(A, B, -1)]
		max = val > max ? val : max;		
	}
	if (A.pos < 2) {
		action = 2;
		val = Q[getState(A, B, 1)]
		max = val > max ? val : max;		
	}
	console.log('ACTION: ', A.pos, B.pos, max);
	return {
		max : max,
		action : action,
	}
}

function getReward(A, B, action) {
	A.setAction(action);

	if (A.pos === B.pos) {
		console.log('Game Over');
		return -1000;
	}

	A.pos < B.pos ? B.pos-- : B.pos++;

	if (A.pos === B.pos) {
		console.log('WIN');
		return 1000;
	}

	return 0;	
};

console.log(A, B);

var win = 0;
var gama = 0.5;
var alpha = 0.8;
var action, next;
var state;
var reward = 0;
var timeout = false;

for(k=0; k<50;k++) {
	reward = 0;
	while(!reward) {
		action = getBestAction(A, B).action;
		state = getState(A, B, action);
		reward = getReward(A, B, action);
		
		next = 0;
		if (reward === 0) {
			next = getBestAction(A, B).max;
		}
		else {
			A.reset();
			B.reset();
		}
		
		Q[state] = (1 - alpha) * Q[state] + alpha * (reward + gama * next);
		alpha = alpha * 0.9;
	}
}
console.log(Q);

	
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