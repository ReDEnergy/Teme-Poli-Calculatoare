var TS1 = 3;
var TS2 = 341;

var TEMP = 30;
var TD = 0;

var state = 0;

function SRL(X) {
    return X%2;
}

if (SRL(TS2) == 1)
    state = 2;
    
TS2 = Math.floor(TS2 / 2);

while (TEMP) {
   
   
    console.log("TS1 " + TS1);
    console.log("TS2 " + TS2);
    console.log("TD " + TD);
    console.log("========NEXT=========");
    console.log("TEMP " + TEMP);
    console.log("State " + state);
    
    if (state == 0) {
        TEMP--;
        TS1 *=2;

        state = 0;
        if (SRL(TS2) == 1)
            state = 2;

        TS2 = Math.floor(TS2 / 2);
        continue;
    }
        
    if (state == 2) {
        TEMP--;
        TD -= TS1;
        TS1 *=2;

        state = 1;
        if (SRL(TS2) == 1)
            state = 3;

        TS2 = Math.floor(TS2 / 2);
        continue;
    }
    
    if (state == 1) {
        TEMP--;
        TD += TS1;
        TS1 *=2;
        
        state = 0;
        if (SRL(TS2) == 1)
            state = 2;
        
        TS2 = Math.floor(TS2 / 2);
        continue;
    }
    
    if (state == 3) {
        TEMP--;
        TS1 *=2;
        
        state = 1;
        if (SRL(TS2) == 1)
            state = 3;
        
        TS2 = Math.floor(TS2 / 2);
        continue;
    }
}

console.log("Rezultat: " + TD);
